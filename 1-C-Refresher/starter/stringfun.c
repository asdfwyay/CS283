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
void reverse_str(char *, int, int);
int  word_print(char *, int, int);
int  search_replace(char *, char *, char *, int, int);

//add additional prototypes here
int  isspace(int);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int whitespace = 0;
    int user_length = 0;

    memset(buff, '.', len*sizeof(char));

    for (char *cur_user = user_str, *cur_buf = buff; *cur_user != '\0'; cur_user++, cur_buf++){
        if (++user_length > len)
            return -1;
        
        if (isspace(*cur_user)){
            if (!whitespace){
                *cur_buf = ' ';
            }
            else{
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

    return user_length; //for now just so the code compiles. 
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

int count_words(char *buff, int len, int str_len){
    //YOU MUST IMPLEMENT
    int num_words = 1;
    int c = 0;

    if (str_len == 0 || len == 0 || (str_len == 1 && *buff == ' '))
        return 0;

    for (char *cur = buff; c < str_len; cur++, c++)
        if (*cur == ' ' && (c != 0 && c != str_len-1)) num_words++;

    return num_words;
}

void reverse_str(char *buff, int len, int str_len){
    // stack (data structure) variables
    char *base = (char *)malloc(len*sizeof(char));
    int top = -1;
    int c = 0;

    if (base == NULL){
        printf("error: could not allocate space for temporary buffer");
        exit(3);
    }

    // push all characters in buffer to stack
    for (char *cur = buff; c < str_len; cur++, c++){
        if (top >= len){
            printf("error: Stack overflow in reverse string function\n");
            exit(3);
        }

        *(base + ++top) = *cur;
    }

    // pop all characters from stack back to buffer
    c = 0;
    for (char *cur = buff; c < str_len; cur++, c++){
        if (top < 0){
            printf("error: Stack underflow in reverse string function\n");
            exit(3);
        }

        *cur = *(base + top--);
    }

    free(base);
}

int word_print(char *buff, int len, int str_len){
    int num_words = 1;
    int c = 0;
    int nl = 1;
    int length = 0;

    printf("Word Print\n");
    printf("----------\n");

    if (str_len == 0 || len == 0 || (str_len == 1 && *buff == ' '))
        return 0;

    for (char *cur = buff; c < str_len; cur++, c++){
        if (nl) printf("%d. ", num_words);
        nl = (*cur == ' ' && (c != 0 && c != str_len-1));

        if (nl){
            printf(" (%d)\n", length);
            num_words++;
            length = 0;
        }
        else{
            putchar(*cur);
            length++;
        }
    }
    printf(" (%d)\n", length);

    return num_words;
}

int search_replace(char *buff, char *find, char *replace, int len, int str_len){
    int c = 0;
    int d = 0;

    int flen = 0;
    int rlen = 0;

    char *bcur = NULL;
    char *fcur = NULL;
    char *word = NULL;

    char *base = (char *)malloc(len*sizeof(char));

    if (base == NULL) {
        printf("error: could not allocate space for temporary buffer");
        return -2;
    }

    // Effectively strlen(find)
    for (char *f = find; *f++ != '\0'; flen++); 

    // Effectively strlen(replace))
    for (char *r = replace; *r++ != '\0'; rlen++);

    printf(" \b");

    // Effectively strstr(buff, find)
    for (char *start = buff; c < str_len; start++, c++){
        for (bcur = start, fcur = find, d = c; *fcur != '\0' && *bcur == *fcur && d < str_len; bcur++, fcur++, d++);

        if (*fcur == '\0'){
            word = start;
            break;
        }
    }

    if (word != NULL){
        if (str_len - flen + rlen > len){
            printf("error: the buffer is not long enough to search and replace\n");
            return -3;
        }

        memcpy(base, buff, c); // copy characters before found word to temp buff
        memcpy(base + c, replace, rlen); // append replaced word to temp buff
        memcpy(base + c + rlen, buff + c + flen, len - c - flen); // append remaining characters to temp buff
        memcpy(buff, base, len); // replace original buffer with temp buff
    }

    free(base);
    return 0;
}

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

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

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
            reverse_str(buff, BUFFER_SZ, user_str_len);
            printf("Reversed String: ");
            for (int i = 0; i < user_str_len; i++){
                putchar(*(buff+i));
            }
            putchar('\n');
            break;
        
        case 'w':
            word_print(buff, BUFFER_SZ, user_str_len);
            break;

        case 'x':
            if (argc != 5){
                printf("usage: %s -x \"string\" find replace\n", argv[0]);
                free(buff);
                exit(1);
            }

            search_replace(buff, argv[3], argv[4], BUFFER_SZ, user_str_len);
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
//          PLACE YOUR ANSWER HERE