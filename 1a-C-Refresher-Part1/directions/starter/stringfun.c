#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define SPACE_CHAR ' '

//prototypes for functions to handle required functionality

// TODO: #1 What is the purpose of providing prototypes for
//          the functions in this code module
//
//       The purpose of including the following function
//       prototypes is to inform the compiler about the
//       functions utilized in the program, their return
//       types, and info about their parameters.
void  usage(char *);
int   count_words(char *);
void  reverse_string(char *);
void  word_print(char *);


void usage(char *exename){
    printf("usage: %s [-h|c|r|w] \"string\" \n", exename);
    printf("\texample: %s -w \"hello class\" \n", exename);
}

// counts the number of words in a string
int count_words(char *str){
    int wc = 0;

    for (char *word = strtok(str, " "); word != NULL; word = strtok(NULL, " ")) wc++;

    return wc;
}

//reverse_string() algorithm
//  1.  Initialize the start and end index variables
//      a.  end_idx is the length of str - 1.  We want to remove one
//          becuase at index str[len(str)] is the '\0' that we want
//          to preserve because we are using C strings.  That makes
//          the last real character in str as str[len(str)-1]
//      b.  start_idx is 0, thus str[0] is the first character in the
//          string.
//
//  2.  Loop while end_idx > start_idx
//      2a. swap the characters in str[start_idx] and str[end_idx]
//      2b. increment start_idx by 1
//      2c. decrement end_indx by 1
//
//  3. When the loop above terminates, the string should be reversed in place
void  reverse_string(char *str){
    int end_idx = strlen(str) - 1;        //should be length of string - 1
    int start_idx = 0;
    char tmp_char;

    while (end_idx > start_idx){
        tmp_char = str[end_idx];
        str[end_idx] = str[start_idx];
        str[start_idx] = tmp_char;

        start_idx++;
        end_idx--;
    }

    return;
}

//word_print()
// counts the number of words and characters in each word and prints them out
//
// EXAMPLE OUTPUT
// ==============
// ./stringfun -w "C programming is fun"
// Word Print
// ----------
// 1. C (1)
// 2. programming (11)
// 3. is (2)
// 4. fun (3)
void  word_print(char *str){
    int wc = 0;         //counts words

    for (char *word = strtok(str, " "); word != NULL; word = strtok(NULL, " "))
        printf("%d. %s (%lu)\n", ++wc, word, strlen(word));
}


int main(int argc, char *argv[]){
    char *input_string;     //holds the string provided by the user on cmd line
    char *opt_string;       //holds the option string in argv[1]
    char opt;               //used to capture user option from cmd line

    //THIS BLOCK OF CODE HANDLES PROCESSING COMMAND LINE ARGS
    if (argc < 2){
        usage(argv[0]);
        exit(1);
    }
    opt_string = argv[1];

    //note arv[2] should be -h -r -w or -c, thus the option is
    //the second character and a - is the first char
    if((opt_string[0] != '-') && (strlen(opt_string) != 2)){
        usage(argv[0]);
        exit(1);
    }

    opt = opt_string[1];   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //Finally the input string must be in argv[2]
    if (argc != 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2];
    //ALL ARGS PROCESSED - The string you are working with is
    //is the third arg or in arv[2]
    
    switch (opt){
        case 'c':
            int wc = 0;         //variable for the word count

            //TODO: #2. Call count_words, return of the result
            //          should go into the wc variable
            wc = count_words(input_string);
            printf("Word Count: %d\n", wc);
            break;
        case 'r':
            //TODO: #3. Call reverse string using input_string
            //          input string should be reversed
            reverse_string(input_string);
            printf("Reversed string: %s\n", input_string);

            //TODO:  #4.  The algorithm provided in the directions 
            //            state we simply return after swapping all 
            //            characters because the string is reversed 
            //            in place.  Briefly explain why the string 
            //            is reversed in place - place in a comment
            //            
            //            Operating in place is beneficial since
            //            not doing so would require an allocation
            //            for a new string, which would need to be
            //            freed at a later point.
            break;
        case 'w':
            printf("Word Print\n----------\n");

            //TODO: #5. Call word_print, output should be
            //          printed by that function
            word_print(input_string);
            break;

        //TODO: #6. What is the purpose of the default option here?
        //          Please describe replacing this TODO comment with
        //          your thoughts.
        //          
        //          If the user passes an argument other than the
        //          ones specified above, the default branch in the
        //          switch statement will catch those cases and
        //          provide an info message.
        default:
            usage(argv[0]);
            printf("Invalid option %c provided, exiting!\n", opt);
            exit(1);
    }
    //TODO: #7. Why did we place a break statement on each case
    //          option, and did not place one on default.  What
    //          would happen if we forgot the break statement?
    //
    //          Excluding a break statement on a case results
    //          in a case fallthrough, where the code in the
    //          following case is executed. This is not
    //          intended behavior; therefore, the break
    //          statements need to be included. The default
    //          branch does not need a break statement since
    //          it is the last case in the switch statement
    //          (additionally, it ends with a call to exit(),
    //          which will terminate the program anyways).
}
