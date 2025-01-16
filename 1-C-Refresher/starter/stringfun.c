#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
int  reverse_str(char *, int, int);
int  word_print(char *, int, int);
int  search_replace(char *, char *, char *, int, int);

//add additional prototypes here
int  isspace(int);

// format and add the supplied string to the buffer
int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int whitespace = 1;
    int user_length = 0;

    // fill buffer with '.' characters
    memset(buff, '.', len*sizeof(char));

    // copy the string into the buffer
    for (char *cur_user = user_str, *cur_buf = buff; *cur_user != '\0'; cur_user++, cur_buf++){
        // returns -1 if the length of the user supplied string
        // exceeds the length of the buffer
        if (++user_length > len)
            return -1;
        
        // remove excess whitespace from string
        if (isspace(*cur_user)){
            if (!whitespace){
                *cur_buf = ' ';
            }
            else{
                // undo previous length and buffer pointer increments when removing whitespace
                cur_buf--; 
                user_length--;
            }
            whitespace = 1;
        }
        else{
            *cur_buf = *cur_user;
            whitespace = 0;
        }
    }

    // remove excess whitespace at the end of the string
    for (char *end_buf = buff + user_length - 1; isspace(*end_buf); end_buf--){
        *end_buf = '.';
        user_length--;
    }

    return user_length;
}

// print the entire buffer
void print_buff(char *buff, int len){
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

// displays how to properly use the program
void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

// counts the number of words in the buffer
int count_words(char *buff, int len, int str_len){
    int num_words = 1;
    int c = 0;

    if (str_len > len)
        return -1;

    // If string is empty or contains only whitespace, return 0 words
    if (str_len == 0 || len == 0 || (str_len == 1 && *buff == ' '))
        return 0;

    // count words by counting the number of spaces in the string
    // (except at beginning or end)
    for (char *cur = buff; c < str_len; cur++, c++)
        if (*cur == ' ' && (c != 0 && c != str_len-1)) num_words++;

    return num_words;
}

// reverses the string (in place) contained in the buffer
int reverse_str(char *buff, int len, int str_len){
    int end_idx = str_len - 1;        //should be length of string - 1
    int start_idx = 0;
    char tmp_char;

    if (str_len > len)
        return -1;

    while (end_idx > start_idx){
        tmp_char = *(buff + end_idx);
        *(buff + end_idx) = *(buff + start_idx);
        *(buff + start_idx) = tmp_char;

        start_idx++;
        end_idx--;
    }

    return 0;
}

// print all the words contained in the buffer and the length of each word
int word_print(char *buff, int len, int str_len){
    int num_words = 1;
    int c = 0;
    int nl = 1; // semaphore used to determine when to print new word
    int length = 0;

    if (str_len > len)
        return -1;

    printf("Word Print\n");
    printf("----------\n");

    // If string is empty or contains only whitespace, return 0 words
    if (str_len == 0 || len == 0 || (str_len == 1 && *buff == ' '))
        return 0;

    // go through the string, counting and printing words
    for (char *cur = buff; c < str_len; cur++, c++){
        if (nl) printf("%d. ", num_words);

        // a new word begins when a whitespace is encountered
        nl = (*cur == ' ' && (c != 0 && c != str_len-1));

        // if the end of a word is encountered, print the number of
        // characters in that word
        if (nl){
            printf("(%d)\n", length);
            num_words++;
            length = 0;
        }
        // otherwise, print the character currently being iterated on
        else{
            putchar(*cur);
            length++;
        }
    }
    printf("(%d)\n", length);
    printf("\nNumber of words returned: %d\n", num_words);

    return num_words;
}

// locate "find" word in string and replace with "replace" word
int search_replace(char *buff, char *find, char *replace, int len, int str_len){
    // iterator variables
    int c = 0;
    int d = 0;

    // boolean for start of word
    int sw = 1;

    // lengths of find and replace words
    int flen = 0;
    int rlen = 0;

    // pointers for buffer and find words
    char *bcur = NULL;
    char *fcur = NULL;
    char *word = NULL;

    if (str_len > len)
        return -1;

    // Effectively strlen(find)
    for (char *f = find; *f++ != '\0'; flen++);

    // Effectively strlen(replace)
    for (char *r = replace; *r++ != '\0'; rlen++);

    // Effectively strstr(buff, find) but only checking after whitespace characters (and at beginning of string)
    for (char *start = buff; c < str_len; start++, c++){
        if (sw){
            for (bcur = start, fcur = find, d = c; *fcur != '\0' && *bcur == *fcur && d < str_len; bcur++, fcur++, d++);

            if (*fcur == '\0'){
                word = start;
                break;
            }

            sw = 0;
        }

        if (*start == ' ')
            sw = 1;
    }

    if (word == NULL){
        printf("error: word not found in buffer\n");
        return -2; // return code for when word is not found in string
    }
    else{
        // Raise buffer overflow error if replacing the total string length after replacement
        // is longer than the buffer length.
        if (str_len - flen + rlen > len){
            printf("error: the buffer is not long enough to search and replace\n");
            return -3; // return code for when search and replace would extend string beyond buffer length
        }

        memcpy(buff + c + rlen, buff + c + flen, len - c - flen); // shift characters after found word
        memcpy(buff + c, replace, rlen);  // replace found word with replaced word

        // add trailing periods to buffer (only necessary if string length is smaller)
        for (int end_idx = str_len - flen + rlen; end_idx < len; end_idx++)
            *(buff + end_idx) = '.';
    }

    return 0;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

// additional helper function to determine if a character is whitespace
int isspace(int c){
    if (c == 0x09 ||
        c == 0x0a ||
        c == 0x0b ||
        c == 0x0c ||
        c == 0x0d ||
        c == 0x20)
        return c;
    
    return 0;
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if argv[1] does not exist?
    //      In the condition below, the first condition (argc < 2) is
    //      checked first. If the check succeeds, the second condition
    //      (*argv[1] != '-') is not executed. Therefore, if
    //      argv[1] does not exist, then argc < 2 and the condition
    //      statement passes without needing to check the second
    //      condition.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      All commands that provide functionality in our
    //      utility (except -h, which was handed in the
    //      previous if statement) require the following 3
    //      arguments:
    //          1. The name of the tool (stringfun)
    //          2. The command to execute (e.g. -c)
    //          3. The sample string (e.g. "hello world")
    //      
    //      If the user input contained less than the required
    //      3 arguments (argc < 3), then the user likely does
    //      not know how to use the program. Therefore, the
    //      synopsis is displayed and the program is exited
    //      with status 1, indicating that there was a command
    //      line problem.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ*sizeof(char));

    // if malloc() failed (buffer was not allocated), exit
    // with status 99
    if (buff == NULL){
        printf("malloc() failed to allocate buffer\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        free(buff);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d\n", rc);
                free(buff);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            rc = reverse_str(buff, BUFFER_SZ, user_str_len);
            if (rc < 0){
                printf("Error reversing string, rc = %d\n", rc);
                free(buff);
                exit(2);
            }

            //printf("Reversed String: ");
            //for (int i = 0; i < user_str_len; i++){
            //    putchar(*(buff+i));
            //}
            //putchar('\n');
            break;
        
        case 'w':
            rc = word_print(buff, BUFFER_SZ, user_str_len);
            if (rc < 0) {
                printf("Error , rc = %d\n", rc);
                free(buff);
                exit(3);
            }
            break;

        case 'x':
            if (argc != 5){
                printf("usage: %s -x \"string\" find replace\n", argv[0]);
                free(buff);
                exit(1);
            }

            rc = search_replace(buff, argv[3], argv[4], BUFFER_SZ, user_str_len);
            if (rc < 0) {
                printf("Error performing search and replace, rc = %d\n", rc);
                free(buff);
                exit(3);
            }
            break;

        default:
            usage(argv[0]);
            free(buff);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          If we hardcoded the buffer length to 50
//          bytes, you would need to painstakingly update
//          each of the helper functions if you wanted to
//          change that length. Including the buffer length
//          along with the buffer pointer as arguments to
//          each of the helper functions is beneficial
//          because it allows those functions to take in
//          buffers of various sizes as input.