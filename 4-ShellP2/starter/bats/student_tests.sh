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

@test "Display file contents" {
    current=$(pwd)

    cd /tmp
    echo content > test1

    run ${current}/dsh <<EOF
cat test1
EOF

    # Output Expectations
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="contentdsh2>dsh2>cmdloopreturned0"

    # Debug Info
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Run bash script" {
    current=$(pwd)

    cd /tmp
    cat << EOF > basic.sh
#!/bin/bash
echo "Hello World!"
exit 0
EOF

    run ${current}/dsh <<EOF
sh /tmp/basic.sh
EOF

    # Output Expectations
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="HelloWorld!dsh2>dsh2>cmdloopreturned0"

    # Debug Info
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Run nonexistent command" {
    current=$(pwd)

    run ${current}/dsh <<EOF
not_exists
rc
EOF

    # Output Expectations
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="dsh2>dsh2>CommandnotfoundinPATHdsh2>2dsh2>cmdloopreturned0"

    # Debug Info
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}

@test "Attempt to display contents of file without permission" {
    current=$(pwd)

    cd /tmp
    echo content > test2
    chmod a-r test2

    run ${current}/dsh <<EOF
cat test2
rc
EOF

    # Output Expectations
    stripped_output=$(echo "$output" | tr -d '[:space:]')
    expected_output="cat:test2:Permissiondenieddsh2>dsh2>1dsh2>cmdloopreturned0"

    # Debug Info
    echo "Captured stdout:"
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Assertions
    [ "$stripped_output" = "$expected_output" ]
    [ "$status" -eq 0 ]
}