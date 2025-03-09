#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include "dshlib.h"
#include "dragon.h"

int grc = 0;

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    char *cmd_buff;
    int rc = 0;
    command_list_t cmds;

    char *cmd;
    char *tok;
    int stridx = 0;

    // allocate space for command buffer
    cmd_buff = (char *)malloc(SH_CMD_MAX*sizeof(char));
    if (cmd_buff == NULL) {
        return ERR_MEMORY;
    }

    cmd = (char *)malloc(SH_CMD_MAX*sizeof(char));
    if (cmd == NULL) {
        return ERR_MEMORY;
    }

    while (1) {
        printf("%s", SH_PROMPT);

        // read line into cmd_buff
        if (fgets(cmd_buff, SH_CMD_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }
        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        // format command
        tok = strtok(cmd_buff, PIPE_STRING);
        stridx = 0;
        while (tok != NULL) {
            // format token
            strcpy(cmd,tok);
            cmd = fmt_cmd(cmd);

            // add token to command buffer
            strcpy(cmd_buff + stridx,cmd);

            // add pipe character and update string pointer
            *(cmd_buff + stridx + strlen(cmd)) = PIPE_CHAR;
            stridx += strlen(cmd) + 1;

            // move to next token
            tok = strtok(NULL, PIPE_STRING);
        }
        *(cmd_buff + stridx) = '\0';

        // check if there are commands in user input
        if (cmd_buff[0] == '\0' || (cmd_buff[0] == '|' && cmd_buff[1] == '\0')) {
            printf(CMD_WARN_NO_CMD);
            continue;
        }

        // build command list
        if ((rc = build_cmd_list(cmd_buff, &cmds)) < 0)
            return rc;

        // exit shell if exit command was executed
        if ((grc = execute_pipeline(&cmds)) == 256 + OK_EXIT)
            return OK_EXIT;
    }

    return OK;
}

char *fmt_cmd(char *str)
{
    char *end = strchr(str, '\0') - 1;
    char *c;
    char *d;
    bool is_space = false;
    bool in_quotes = false; 

    // remove trailing spaces
    while (end >= str && *end == SPACE_CHAR) {
        end--;
    }
    *(end + 1) = '\0';

    // remove leading spaces
    while (*str == SPACE_CHAR) {
        str++;
    }

    // remove duplicate spaces (excluding inside quotes)
    c = str;
    d = str;
    while (*c != '\0') {
        switch (*c) {
            case QUOTE_CHAR:
                in_quotes = !in_quotes;
                is_space = false;
                *d++ = *c++;
                break;
            case SPACE_CHAR:
                if (is_space && !in_quotes) {
                    c++;
                } else {
                    is_space = true;
                    *d++ = *c++;
                }
                break;   
            default:
                is_space = false;
                *d++ = *c++;
                break;
        }
    }
    *d = '\0';

    return str;
}

int alloc_cmd_buff(cmd_buff_t *cmd_buff) {
    // allocate command buffer structure
    cmd_buff = (cmd_buff_t *)malloc(sizeof(cmd_buff_t));

    // allocate command buffer inside structure
    cmd_buff->_cmd_buffer = (char *)malloc(SH_CMD_MAX*sizeof(char));
    if (cmd_buff->_cmd_buffer == NULL) {
        return ERR_MEMORY;
    }

    return OK;
}

int free_cmd_buff(cmd_buff_t *cmd_buff) {
    // free all malloced arrays within command buffer structure
    clear_cmd_buff(cmd_buff);
    free(cmd_buff->_cmd_buffer);
    free(cmd_buff);

    return OK;
}

int clear_cmd_buff(cmd_buff_t *cmd_buff) {
    // free all malloced string arrays in cmd_buff->argv 
    for (int i = 0; i < cmd_buff->argc; i++)
        free(cmd_buff->argv[i]);
    cmd_buff->argc = 0;

    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *cmd_buff) {
    long argv_pos[CMD_ARGV_MAX];
    int argc;
    size_t argv_size;
    bool in_quotes;

    // extract indices of arguments from command line string
    argv_pos[0] = -1;
    argc = 1;
    in_quotes = false;
    for (char *cur = cmd_line; *cur != '\0'; cur++) {
        switch (*cur) {
            case QUOTE_CHAR:
                in_quotes = !in_quotes;
                break;
            case SPACE_CHAR:
                if (!in_quotes) {
                    if (argc >= CMD_MAX)
                        return ERR_CMD_OR_ARGS_TOO_BIG;
                    argv_pos[argc++] = cur - cmd_line;
                }
                break;
            default:
                break;
        }
    }
    argv_pos[argc] = strlen(cmd_line);

    // allocate argv and populate with arguments
    for (int i = 0; i < argc; i++) {
        // calculate buffer size
        argv_size = argv_pos[i+1] - argv_pos[i];

        // allocate and zero argv buffer
        cmd_buff->argv[i] = (char *)calloc(argv_size,sizeof(char));
        if (cmd_buff->argv[i] == NULL)
            return ERR_MEMORY;

        // copy string to argv (without quotes)
        if (*(cmd_line + argv_pos[i] + 1) == '"' && argv_size > 3)
            strncpy(cmd_buff->argv[i], cmd_line + argv_pos[i] + 2, argv_size - 3);
        else
            strncpy(cmd_buff->argv[i], cmd_line + argv_pos[i] + 1, argv_size - 1);
    }
    cmd_buff->argv[argc] = NULL;

    // set argc
    cmd_buff->argc = argc;

    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *clist) {
    char *tok = strtok(cmd_line, PIPE_STRING);
    int cmds = 0;

    // add commands to command list
    while (tok != NULL) {
        if (cmds >= CMD_MAX) {
            printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            return ERR_TOO_MANY_COMMANDS;
        }

        alloc_cmd_buff(&clist->commands[cmds]);
        build_cmd_buff(tok,&clist->commands[cmds]);

        //for (int j = 0; j < clist->commands[cmds].argc; j++)
        //    printf("argv[%d]: %s\n", j, clist->commands[cmds].argv[j]);

        cmds++;
        tok = strtok(NULL, PIPE_STRING);
    }
    clist->num = cmds;

    return OK;
}

int free_cmd_list(command_list_t *cmd_lst) {
    for (int i = 0; i < cmd_lst->num; i++)
        free_cmd_buff(&cmd_lst->commands[i]);
    cmd_lst->num = 0;
    return OK;
}

Built_In_Cmds match_command(const char *input) {
    if (!strcmp(input, EXIT_CMD))
        return BI_CMD_EXIT;
    if (!strcmp(input, "dragon"))
        return BI_CMD_DRAGON;
    if (!strcmp(input, "cd"))
        return BI_CMD_CD;
    if (!strcmp(input, "rc"))
        return BI_CMD_RC;
    return BI_NOT_BI;
}

int exec_built_in_cmd(cmd_buff_t *cmd) {
    Built_In_Cmds cmd_enum = match_command(cmd->argv[0]);
    int rc;

    switch (cmd_enum) {
        case BI_CMD_EXIT:
            return OK_EXIT;
        case BI_CMD_CD:
            if (cmd->argc >= 2) {
                chdir(cmd->argv[1]);
            }
            return OK;
        case BI_CMD_DRAGON:
            print_dragon();
            return OK;
        case BI_CMD_RC:
            printf("%d\n", grc);
            return OK;
        default:
            rc = execvp(cmd->argv[0], cmd->argv);
            switch (rc) {
                case ENOENT:
                    printf("Command not found in PATH\n");
                    break;
                case EACCES:
                    printf("Permission denied\n");
                    printf("%d\n",rc);
                    break;
                default:
            }
            return rc;
    }
}

int exec_cmd(cmd_buff_t *cmd) {
    int pid;
    int crc;

    pid = fork();
    if (pid < 0) {
        return ERR_EXEC_CMD;
    }

    if (pid == 0) {
        int rc;
        rc = execvp(cmd->argv[0], cmd->argv);
        if (rc < 0) {
            exit(errno);
        }
    } else {
        wait(&crc);
        if (WEXITSTATUS(crc) != 0)
            return WEXITSTATUS(crc);
    }

    return OK;
}

int execute_pipeline(command_list_t *clist) {
    pid_t pids[clist->num];
    int fds[2*clist->num];
    int crc;

    // create pipeline
    for (int i = 0; i < clist->num; i++)
        if (pipe(fds + 2*i) != 0) return ERR_EXEC_CMD;

    pids[0] = fork();
    crc = exec_pipe(clist, fds, pids, 0);

    return crc;
}

int exec_pipe(command_list_t *clist, int *fds, pid_t *pids, int i) {
    int crc;

    if (pids[i] == 0) {
        int rc;

        // duplicate file descriptors
        if (i > 0)
            dup2(fds[2*(i-1)], STDIN_FILENO);
        if (i < clist->num - 1)
            dup2(fds[2*i+1], STDOUT_FILENO);

        // close all file descriptors
        for (int j = 0; j < 2*clist->num; j++)
            close(fds[j]);

        // execute command
        rc = exec_built_in_cmd(&clist->commands[i]);
        if (rc == OK_EXIT)
            exit(rc);
        if (rc < 0)
            exit(errno);
    } else {
        if (i < clist->num - 1) {
            pids[i+1] = fork();
            exec_pipe(clist, fds, pids, i+1);
        }

        for (int j = 0; j < 2*clist->num; j++)
            close(fds[j]);

        waitpid(pids[i], &crc, 0);
        if (WEXITSTATUS(crc) != 0)
            return WEXITSTATUS(crc);
    }

    return OK;
}