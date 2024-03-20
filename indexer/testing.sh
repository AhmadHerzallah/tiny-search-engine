#!/bin/bash

# Compile the indexer and indextest programs
make indexer
make indextest

# Function to handle test results
handle_test_result() {
    if [ -s "$1" ]; then
        echo "❌ Failure: $2. Check $2 for details."
    else
        echo "✅ $2 passed."
    fi
}

echo "Starting Indexer Tests..."

# Testing indexer with provided datasets and comparing outputs
datasets=("letters-0" "letters-1" "letters-2" "letters-3" "letters-22" "toscrape-0" "toscrape-1" "toscrape-2" "toscrape-3" "wikipedia_1" "wikipedia_2")
for dataset in "${datasets[@]}"; do
    echo "Processing $dataset..."
    ./indexer ../data/$dataset/ ../data/$dataset/.index
    ./indextest ../data/$dataset/.index ../data/$dataset/.index2
    ~/cs50-dev/shared/tse/indexcmp ../data/$dataset/.index ../data/$dataset/.index2 &>../data/output/cs-$dataset.out
    handle_test_result "../data/output/cs-$dataset.out" "$dataset comparison"
done

# Memory leak checks with Valgrind on a subset
echo "Performing memory leak checks with Valgrind..."
valgrind --leak-check=full ./indexer ../data/wikipedia_1/ ../data/wikipedia_1/.index -s
valgrind --leak-check=full ./indexer ../data/letters-22/ ../data/letters-22/.index

# Testing with invalid arguments
echo "Testing with invalid arguments..."
./indexer
./indexer ../data/letters-0/
./indexer not_a_directory ../data/letters-0/.index

# Testing with missing arguments
echo "Testing indexer with missing arguments..."
./indexer
./indexer only_one_argument

# Testing with too many arguments
echo "Testing indexer with too many arguments..."
./indexer arg1 arg2 arg3

# Testing with invalid directory
echo "Testing indexer with invalid directory..."
./indexer ./nonexistent_directory outputfile.index

# Testing with invalid file permissions
echo "Testing indexer with a directory as output file..."
./indexer ../data/letters-0/ .

# Testing with a read-only file as output
touch readonly.index
chmod 444 readonly.index
echo "Testing indexer with a read-only file as output..."
./indexer ../data/letters-0/ readonly.index
chmod 644 readonly.index # Reset permissions after test

# Testing with non-existent input file
echo "Testing indextest with non-existent input file..."
./indextest nonexistent.index output.index

# Testing with non-writable output file
touch unwritable.index
chmod 444 unwritable.index
echo "Testing indextest with non-writable output file..."
./indextest ../data/letters-0/.index unwritable.index
chmod 644 unwritable.index # Reset permissions after test

# Clean up
make clean
