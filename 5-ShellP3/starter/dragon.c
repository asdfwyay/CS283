#include <stdio.h>
#include "dragon.h"

// EXTRA CREDIT - print the drexel dragon from the readme.md
void print_dragon(){
    FILE *stream = fopen("dragon", "rb");
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
