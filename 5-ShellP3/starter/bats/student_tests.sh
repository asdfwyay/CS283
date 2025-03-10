#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

# Warning should be printed when fed empty command
@test "Check empty command" {
    run ./dsh <<EOF                
 
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>warning:nocommandsprovideddsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Test functionality of multiple commands
@test "Check multiple pipes" {
    run ./dsh <<EOF
ls -l | grep dragon | wc -l
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="3dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Should fail when fed more than 8 commands in a pipeline
@test "Check pipe limit" {
    run ./dsh <<EOF
ls | ls | ls | ls | ls | ls | ls | ls | ls
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>error:pipinglimitedto8commandscmdloopreturned-2"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Should print nothing when running first nonexistent command then print 2 when checking return code
@test "Check rc built-in command" {
    run ./dsh <<EOF
h
rc
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>dsh3>dsh3>2dsh3>cmdloopreturned0dsh3>dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Second command should run then shell should exit
@test "Exit in pipe (first command)" {
    run ./dsh <<EOF
exit | echo "hi"
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="hidsh3>cmdloopreturned-7"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# No output since stdout of first command is passed to "exit," which itself fails as it is executed in a child process.
@test "Exit in pipe (second command)" {
    run ./dsh <<EOF
echo "hi" | exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh3>dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Test normal output file redirection
@test "Output redirection" {
    run ./dsh <<EOF
echo "hello, class" > out.txt
cat out.txt
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    file_output=$(cat out.txt | tr -d '[:space:]')

    rm -rf out.txt

    # Expected output with all whitespace removed for easier matching
    expected_output="hello,classdsh3>dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    echo "${file_output} -> hello,class"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Check exact match
    [ "$file_output" = "hello,class" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Test normal input file redirection
@test "Input Redirection" {
echo "hello, class" > out.txt
    run ./dsh <<EOF
cat < out.txt
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    file_output=$(cat out.txt | tr -d '[:space:]')

    rm -rf out.txt

    # Expected output with all whitespace removed for easier matching
    expected_output="hello,classdsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    echo "${file_output} -> hello,class"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Check exact match
    [ "$file_output" = "hello,class" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Output redirection with appending
@test "Output redirection (append)" {
    run ./dsh <<EOF
echo "hello, class" > out.txt
cat out.txt
echo "this is line 2" >> out.txt
cat out.txt
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    file_output=$(cat out.txt | tr -d '[:space:]')

    rm -rf out.txt

    # Expected output with all whitespace removed for easier matching
    expected_output="hello,classhello,classthisisline2dsh3>dsh3>dsh3>dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    echo "${file_output} -> hello,classthisisline2"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Check exact match
    [ "$file_output" = "hello,classthisisline2" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Test output and input file redirection in one command
@test "Output and Input Redirection" {
echo "the quick brown fox jumped over the lazy dogs" > out.txt
    run ./dsh <<EOF
wc -w > wc.txt < out.txt
cat wc.txt
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    file_output=$(cat wc.txt | tr -d '[:space:]')

    rm -rf out.txt
    rm -rf wc.txt

    # Expected output with all whitespace removed for easier matching
    expected_output="9dsh3>dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    echo "${file_output} -> 9"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Check exact match
    [ "$file_output" = "9" ]

    # Assertions
    [ "$status" -eq 0 ]
}

# Test file redirection with piping
@test "Redirection w/ Piping" {
    run ./dsh <<EOF
ls | grep "dsh_cli.c" | wc -c > wc.txt
cat wc.txt
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    file_output=$(cat wc.txt | tr -d '[:space:]')

    rm -rf wc.txt

    # Expected output with all whitespace removed for easier matching
    expected_output="10dsh3>dsh3>dsh3>cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"
    echo "${file_output} -> 10"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Check exact match
    [ "$file_output" = "10" ]

    # Assertions
    [ "$status" -eq 0 ]
}