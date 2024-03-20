#!/bin/bash

# Stop on error
set -e

# Compile the program
echo "Compiling querier..."
make all

# Define directories and files
pageDirectory="../data/toscrape-1"
indexFile="${pageDirectory}/.index"

# Function to run a query test
run_query_test() {
    query=$1
    echo "Query: $query"
    echo "$query" | ./querier $pageDirectory $indexFile
}

# Function to run parseArgs test
run_parseargs_test() {
    echo "Testing invalid parseArgs: $1 $2"
    # Temporarily disable 'exit on error'
    set +e
    ./querier "$1" "$2"
    # Check if the last command (querier) failed
    if [ $? -ne 0 ]; then
        echo "Correctly failed. Moving on..."
    fi
    # Re-enable 'exit on error'
    set -e
}

# Test cases
echo "Running query tests..."
# Single word query
run_query_test "applicants" # existing word
run_query_test "computer" # word that doesn't exist

# Logical AND queries
run_query_test "day and moments" 
run_query_test "war and fall" 
run_query_test "dresser and male" 

# Logical OR queries
run_query_test "moments or prada" 
run_query_test "fall or tough" 
run_query_test "male or critiquer" 

# Mixed queries
run_query_test "moments or prada and war" 
run_query_test "day and fall or tough" 
run_query_test "dresser or male and critique" 

# Queries with parentheses
run_query_test "(day and moments) or prada" 
run_query_test "day and (fall or tough)" 
run_query_test "(dresser or male) and critique" 

# Failed cases - multiple ands in a row
run_query_test "day and moments and prada" 
run_query_test "fall and tough" 
run_query_test "dresser and male and critique" 

echo "Running invalid parseArgs tests..."
run_parseargs_test "" "" # No arguments
run_parseargs_test $pageDirectory # Only one argument
run_parseargs_test "nonexistentDir" "nonexistentIndex" # Nonexistent directory and index
run_parseargs_test $pageDirectory "nonexistentIndex" # Existing directory, nonexistent index
run_parseargs_test "nonexistentDir" $indexFile # Nonexistent directory, existing index

echo "All tests completed successfully."
