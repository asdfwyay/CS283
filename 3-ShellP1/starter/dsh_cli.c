#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dshlib.h"

typedef enum decode_state {
    START = 0x00,
    ONE = 0x01,
    ONE_ZERO = 0x02,
    END_SPACE = 0x20,
    END_PERCENT = 0x25,
    END_ATSIGN = 0x40,
    END_NEWLINE = 0x0A
} DECODE_STATE;

/*
 * Prints the Drexel dragon to standard output. The name of the file containing
 * the Drexel dragon ASCII art compressed using Huffman encoding should be passed
 * as input to the function.
 * 
 *      HUFFMAN CODE TABLE
 * -----------+-----------+------
 *  Character | Frequency | Code
 * -----------+-----------+------
 *    space   |    1937   |  0
 *      %     |    1818   |  11
 *      @     |    37     |  101
 *   newline  |    37     |  110
 * 
 * Note: the values for the static huffman code above were calculated externally.
 */
void print_dragon(const char *filename) {
    FILE *stream = fopen(filename, "rb");
    DECODE_STATE state = START;
    int c;
    int d;

    // perform huffman decoding and print decoded characters to stdout
    while ((c = fgetc(stream)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            d = c >> i;

            switch (state) {
                case START:
                    if (d & 1)
                        state = ONE;
                    else
                        state = END_SPACE;
                    break;
                case ONE:
                    if (d & 1)
                        state = END_PERCENT;
                    else
                        state = ONE_ZERO;
                    break;
                case ONE_ZERO:
                    if (d & 1)
                        state = END_ATSIGN;
                    else
                        state = END_NEWLINE;
                    break;
                default:
            }

            //printf("%d, ", state);
            switch (state) {
                case END_SPACE:
                case END_PERCENT:
                case END_ATSIGN:
                case END_NEWLINE:
                    putchar(state);
                    state = START;
                    break;
                default:
            }
        }
    }

    putchar('\n');
    fclose(stream);
}

/*
 * Implement your main function by building a loop that prompts the
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.  Since we want fgets to also handle
 * end of file so we can run this headless for testing we need to check
 * the return code of fgets.  I have provided an example below of how
 * to do this assuming you are storing user input inside of the cmd_buff
 * variable.
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
 *
 *   Expected output:
 *
 *      CMD_OK_HEADER      if the command parses properly. You will
 *                         follow this by the command details
 *
 *      CMD_WARN_NO_CMD    if the user entered a blank command
 *      CMD_ERR_PIPE_LIMIT if the user entered too many commands using
 *                         the pipe feature, e.g., cmd1 | cmd2 | ... |
 *
 *  See the provided test cases for output expectations.
 */
int main()
{
    char *cmd_buff = (char *)malloc(ARG_MAX*sizeof(char));
    int rc = 0;
    command_list_t clist;

    while(1) {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL) {
            printf("\n");
            break;
        }

        cmd_buff[strcspn(cmd_buff, "\n")] = '\0';

        if (strcmp(cmd_buff, "") == 0) {
            printf(CMD_WARN_NO_CMD);
        } else if (strcmp(cmd_buff, EXIT_CMD) == 0) {
            free(cmd_buff);
            exit(OK);
        } else if (strcmp(cmd_buff, "dragon") == 0) {
            print_dragon("dragon");
        } else {
            rc = build_cmd_list(cmd_buff, &clist);

            switch (rc) {
                case OK:
                    printf(CMD_OK_HEADER, clist.num);
                    for (int i = 0; i < clist.num; i++) {
                        printf("<%d> %s", i + 1, clist.commands[i].exe);
                        if (strcmp(clist.commands[i].args, "")) {
                            printf(" [%s]", clist.commands[i].args);
                        }
                        printf("\n");
                    }
                    break;
                case ERR_TOO_MANY_COMMANDS:
                    printf(CMD_ERR_PIPE_LIMIT, CMD_MAX);
            }
        }
    }
}