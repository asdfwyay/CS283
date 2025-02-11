1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  Using `execvp` without `fork` would overwrite the shell process when executing the program. Using `fork` creates a separate child process for the program to run in before returning back to the parent shell process upon completion.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If the fork system call fails (which could happen if too many processes are running), the shell will return a code indicating a failure to execute their specified program.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  `execvp` search directiories stored in the PATH variable to look for programs to execute. 

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  We call `wait` so that the shell prompt doesn't return until the program in the child process has finished executing.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  `WEXITSTATUS` extracts the status code from the process termination variable that is produced when the child process finishes executing. It is useful for determining whether a program executed successfully or not. In the case of failure, the status code is also useful to determine why the program failed.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**: My implementation of `build_cmd_buff` performs a single passthrough of the command line string, marking locations where the string needs to be split to create arguments (spaces outside quotes). When a quote is encountered, the semaphore for whether or not the current pointer is inside quotes is toggled. This semaphore is used whenever a space is encountered to determine whether to mark the location or not.
    >
    > This is necessary since the purpose of quotes in our command line string is to allow our shell to accept argument that may include spaces (such as strings).

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The parsing logic described above did not change much functionally. However, the new parsing logic utilizes the `cmd_buff_t` structure for better organization.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  _start here_

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  _start here_

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  _start here_
