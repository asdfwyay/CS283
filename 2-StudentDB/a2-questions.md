## Assignment 2 Questions

#### Directions
Please answer the following questions and submit in your repo for the second assignment.  Please keep the answers as short and concise as possible.

1. In this assignment I asked you provide an implementation for the `get_student(...)` function because I think it improves the overall design of the database application.   After you implemented your solution do you agree that externalizing `get_student(...)` into it's own function is a good design strategy?  Briefly describe why or why not.

    > **Answer**:  Externalizing `get_student(...)` into it's own function is useful
    as the following operations the program can perform all benefit from its utilization.
    > 1. The `add_student(...)` function can use it to check if a student already exists in the database so as to not add a duplicate entry.
    > 2. On the contrary, the `del_student(...)` function can also use it to check
    if a student already exists in the database so that his/her entry can be deleted.
    > 3. The `print_db(...)` function can use it to obtain information about students before displaying them to the user. 

2. Another interesting aspect of the `get_student(...)` function is how its function prototype requires the caller to provide the storage for the `student_t` structure:

    ```c
    int get_student(int fd, int id, student_t *s);
    ```

    Notice that the last parameter is a pointer to storage **provided by the caller** to be used by this function to populate information about the desired student that is queried from the database file. This is a common convention (called pass-by-reference) in the `C` programming language. 

    In other programming languages an approach like the one shown below would be more idiomatic for creating a function like `get_student()` (specifically the storage is provided by the `get_student(...)` function itself):

    ```c
    //Lookup student from the database
    // IF FOUND: return pointer to student data
    // IF NOT FOUND: return NULL
    student_t *get_student(int fd, int id){
        student_t student;
        bool student_found = false;
        
        //code that looks for the student and if
        //found populates the student structure
        //The found_student variable will be set
        //to true if the student is in the database
        //or false otherwise.

        if (student_found)
            return &student;
        else
            return NULL;
    }
    ```
    Can you think of any reason why the above implementation would be a **very bad idea** using the C programming language?  Specifically, address why the above code introduces a subtle bug that could be hard to identify at runtime? 

    > **ANSWER:** After returning, the `student` variable is freed since it is a local variable stored on the stack (rather than the heap using `malloc()`). Therefore, the pointer returned will point to data that does not exist anymore, which is undefined behavior.

3. Another way the `get_student(...)` function could be implemented is as follows:

    ```c
    //Lookup student from the database
    // IF FOUND: return pointer to student data
    // IF NOT FOUND or memory allocation error: return NULL
    student_t *get_student(int fd, int id){
        student_t *pstudent;
        bool student_found = false;

        pstudent = malloc(sizeof(student_t));
        if (pstudent == NULL)
            return NULL;
        
        //code that looks for the student and if
        //found populates the student structure
        //The found_student variable will be set
        //to true if the student is in the database
        //or false otherwise.

        if (student_found){
            return pstudent;
        }
        else {
            free(pstudent);
            return NULL;
        }
    }
    ```
    In this implementation the storage for the student record is allocated on the heap using `malloc()` and passed back to the caller when the function returns. What do you think about this alternative implementation of `get_student(...)`?  Address in your answer why it work work, but also think about any potential problems it could cause.  
    
    > **ANSWER:** Unlike in the previous example, the data stored in the `pstudent` buffer will persist after returning from the function as it is allocated on the heap. However, the main disadvantage of this implementation is that a new data buffer is allocated instead of using an existing data buffer when passing by reference, reducing memory efficiency. Furthermore, the buffer will need to be freed at a later point, as failing to free the buffer could introduce a memory leak if the function is called repeatedly.


4. Lets take a look at how storage is managed for our simple database. Recall that all student records are stored on disk using the layout of the `student_t` structure (which has a size of 64 bytes).  Lets start with a fresh database by deleting the `student.db` file using the command `rm ./student.db`.  Now that we have an empty database lets add a few students and see what is happening under the covers.  Consider the following sequence of commands:

    ```bash
    > ./sdbsc -a 1 john doe 345
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 128 Jan 17 10:01 ./student.db
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 3 jane doe 390
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 256 Jan 17 10:02 ./student.db
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 63 jim doe 285 
    > du -h ./student.db
        4.0K    ./student.db
    > ./sdbsc -a 64 janet doe 310
    > du -h ./student.db
        8.0K    ./student.db
    > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 4160 Jan 17 10:03 ./student.db
    ```

    For this question I am asking you to perform some online research to investigate why there is a difference between the size of the file reported by the `ls` command and the actual storage used on the disk reported by the `du` command.  Understanding why this happens by design is important since all good systems programmers need to understand things like how linux creates sparse files, and how linux physically stores data on disk using fixed block sizes.  Some good google searches to get you started: _"lseek syscall holes and sparse files"_, and _"linux file system blocks"_.  After you do some research please answer the following:

    - Please explain why the file size reported by the `ls` command was 128 bytes after adding student with ID=1, 256 after adding student with ID=3, and 4160 after adding the student with ID=64? 

        > **ANSWER:**
        > 
        > Each entry given an `id`:
        > - Takes up **64 bytes** of space
        > - Starts at byte `id*sizeof(student_t)` of the file
        >
        > The `ls` command reports the **logical file size**, which is the number of bytes between the start-of-file and end-of-file pointers.
        >
        > Therefore, the formula for the file size returned by `ls` is `64*(max_id + 1)`, where `max_id` is the largest student id present in the database.
        >
        > - `max_id = 1 -> ls file size = 64*(1 + 1) = 128`
        > - `max_id = 3 -> ls file size = 64*(3 + 1) = 256`
        > - `max_id = 64 -> ls file size = 64*(64 + 1) = 4160`

    -   Why did the total storage used on the disk remain unchanged when we added the student with ID=1, ID=3, and ID=63, but increased from 4K to 8K when we added the student with ID=64? 

        > **ANSWER:** When creating the database and first adding the student with ID=1, one filesystem block is allocated to store the file (which happens to be 4096 bytes in length, the typical block size for x86 systems).
        >
        > Writing data for students with ID=3 and ID=63 does not change the disk usage of the file, size the data is being written within the previously allocated block. After writing data for the student with ID=64, however, the logical file size exceeds 4.0K and thus another filesystem block must be allocated, raising the total disk usage to `2 * 4.0K = 8.0K`.

    - Now lets add one more student with a large student ID number  and see what happens:

        ```bash
        > ./sdbsc -a 99999 big dude 205 
        > ls -l ./student.db
        -rw-r----- 1 bsm23 bsm23 6400000 Jan 17 10:28 ./student.db
        > du -h ./student.db
        12K     ./student.db
        ```
        We see from above adding a student with a very large student ID (ID=99999) increased the file size to 6400000 as shown by `ls` but the raw storage only increased to 12K as reported by `du`.  Can provide some insight into why this happened?

        > **ANSWER:**  From the formula for logical file size of the student database, the file size as reported by `ls` will be `64*(99999+1) = 6400000` after adding the student with ID=99999. However, only one additional filesystem block is allocated with this addition to the database; thus, the total disk usage as reported by `du` will be `3 * 4.0K = 12K`.