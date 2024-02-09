# IMPLEMENTATION
## Methods

### main
- Checks if the correct amount of arguments are entered.
- Passes the argument to indexer
### Indexer
- Validates the arguments
- Passes them to indexBuild
### indexBuild
- Checks if file exists
- Creates an empty Index
- loops through documentID and loads the page with the documentID that been iterated.
- If the webpage exists, it passes it to indexPage.
### indexPage
- Goes through each word in the document.
- If letters in the word are > 3 it normalizes the word.
- Adds its word count into the index if it doesn't exist.
## Testing
### Indexer Tests

1. **Comparison Tests**:
   - The script tests the indexer program by comparing the outputs of the indexer and indextest programs using provided datasets.
   - Datasets include "letters-0", "letters-1", "letters-2", "letters-3", "letters-22", "toscrape-0", "toscrape-1", "toscrape-2", "toscrape-3", "wikipedia_1", and "wikipedia_2".

2. **Memory Leak Checks**:
   - Valgrind is used to perform memory leak checks on a subset of datasets, including "wikipedia_1" and "letters-22".

### Error and Exception Handling Tests

3. **Invalid Argument Tests**:
   - Tests are conducted to check the behavior of the indexer program when provided with invalid arguments.
   - Tests include running the indexer program with no arguments, with only one argument, with a non-existent directory as an argument, and with too many arguments.

4. **Invalid Directory Tests**:
   - Tests whether the indexer program handles invalid directory inputs appropriately.

5. **Invalid File Permission Tests**:
   - Checks how the indexer program behaves when given a directory as the output file.
   - Tests the behavior when a read-only file is used as the output file.

6. **Missing Input File Tests**:
   - Tests the indextest program with a non-existent input file.

7. **Non-writable Output File Tests**:
   - Checks the behavior of the indextest program when the output file is non-writable.

### Cleanup
- After running the tests, the script performs cleanup by executing `make clean` to remove any generated files or artifacts.