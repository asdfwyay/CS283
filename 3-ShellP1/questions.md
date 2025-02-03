1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  `fgets()` reads and stores a specified number of characters from a file stream.
    > Since we need to obtain user input, we need to read from STDIN (which is itself a file), so
    > `fgets()` would be applicable here.

2. You needed to use `malloc()` to allocate memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  _start here_


3. In `dshlib.c`, the function `build_cmd_list()` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**: When processing commands and their arguments, spaces are used to determine the
    > boundaries between commands/arguments. Therefore, if leading and trailing spaces exist, the
    > actual commands and arguments the shell interprets would be incorrect.
    >
    > An example is provided below:
    > - The command `"cat a b"` is interpreted by the shell as follows:
    >   - `exe = "cat"`
    >   - `args[0] = "a"`
    >   - `args[1] = "b"`
    > - However, with a leading and trailing space added to the input (`" cat a b "`), the command
    > is instead interpreted as follows:
    >   - `exe = ""`
    >   - `args[0] = "cat"`
    >   - `args[1] = "a"`
    >   - `args[2] = "b"`
    >   - `args[3] = ""`

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:
    > 1. **Redirect Output** (`cmd > file.txt`): The output of a command is redirected from STDOUT
    > to the specified filestream.
    > 1. **Redirect Input** (`cmd < file.txt`): The input to a command is redirected from STDIN
    > to the specified filestream.
    > 1. **Redirect & Append Output** (`cmd >> file.txt`): The output of a command is redirected
    > from STDOUT and appended to the specified filestream.


- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**: Redirection is used for interactions between commands and filestreams; for example,
    > reading input from a file into a command or storing the output of a command in a file.
    >
    > Piping, on the other hand, is used for interactions between commands. For example, `cmd1 | cmd2`
    > will take the output of `cmd1` and pass it to `cmd2` without the need for an intermediate file.
    >
    > Another advantage of piping over redirection when transferring data between commands is that
    > the commands can run in parallel, thus allowing for a more streamlined performance.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**: The output of a command may contain data that needs to be redirected to a file or
    > piped to another command. Error messages, however, are typically either printed to a screen or
    > redirected to a separate log file.
    >
    > Because they serve different purposes, outputs and error messages should be written to separate
    > filestreams. If they were mixed together, 

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  _start here_