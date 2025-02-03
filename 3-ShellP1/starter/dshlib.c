#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

int build_cmd_list(char *cmd_line, command_list_t *clist);
char *strip(char *str);

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    command_t cmd = {
        .exe = "",
        .args = ""
    };
    char *pch;
    char *token = strtok(cmd_line, PIPE_STRING);
    char *argv = (char *)malloc(SH_CMD_MAX*sizeof(char));
    char *argv_start = argv;
    int ncmds = 0;

    if (argv == NULL) {
        return ERR_CMD_OR_ARGS_TOO_BIG;
    }

    clist->num = 0;

    while (token != NULL) {
        strcpy(argv, token);
        argv = strip(argv);

        pch = strchr(argv, SPACE_CHAR);

        if (pch == NULL) {
            if (strlen(argv) > EXE_MAX) {
                free(argv_start);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            strcpy(cmd.exe, argv);
            memset(cmd.args, '\0', ARG_MAX);
        } else {
            if (pch - argv + 1 > EXE_MAX) {
                free(argv_start);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }
            if (strlen(pch + 1) > ARG_MAX) {
                free(argv_start);
                return ERR_CMD_OR_ARGS_TOO_BIG;
            }

            strncpy(cmd.exe, argv, pch - argv);
            strcpy(cmd.args, pch + 1);
        }

        if (ncmds + 1 > CMD_MAX) {
            free(argv_start);
            return ERR_TOO_MANY_COMMANDS;
        }
        memcpy(&clist->commands[ncmds++], &cmd, sizeof(cmd));

        token = strtok(NULL, PIPE_STRING);
    }

    clist->num = ncmds;
    free(argv_start);
    return OK;
}

char *strip(char *str)
{
    char *end = strchr(str, '\0') - 1;

    while (end >= str && *end == SPACE_CHAR) {
        end--;
    }
    *(end + 1) = '\0';

    while (*str == SPACE_CHAR) {
        str++;
    }

    return str;
}