1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

My shell implementation keeps track of all the process ids for each child process. During execution of a pipeline, the parent processes call `waitpid()` for each child process to ensure they complete before allowing the shell to continue processing. If `waitpid()` is not called on all child processes, memory or file descriptior leaks could arise as a malicious actor could use the shell to continuously execute commands, spawning more and more child processes. Furthermore, if `waitpid()` was not used, the shell program would not be able to obtain the status codes of each command executed.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

If you forget to close the file descriptors at each stage in the pipeline, a resource leak could occur. Furthermore, blocking may occur when the child process uses the `read()` system call or the parent process uses the `write()` system call if the pipe ends remain open.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

`cd` is implemented as a built-in command since if it were implemented as an external program, it would need to be forked and execed. In this case, `cd` would only change the directory in the child process while leaving the parent process directory unchanged.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

Changing from a fixed number to an arbitrary number of commands can be done by dynamically allocating the `commands` attribute of the `command_list_t` data structure according to the number of commands read by the parser and freeing the data after all the commands in the input have finished execution. Overall, implementing it this way would lengthen the execution time and introduce potential points of failure during memory allocation (especially with exceedingly large command inputs).
