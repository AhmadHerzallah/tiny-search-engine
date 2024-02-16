<!-- Add a file IMPLEMENTATION.md to provide the implementation spec and testing plan for querier. Your IMPLEMENTATION.md file need not repeat the information provided in the assignment or other specs; instead, it should describe implementation details specific to your implementation. -->

# CS50 TSE Querier Implementation

## Overview

This specification details the implementation of the TSE Querier, which reads the index file produced by the TSE Indexer and page files produced by the TSE Querier, answering search queries submitted via stdin. The Querier supports complex queries with logical operators "and" and "or".

## Data Structures 

1. **Index**: Utilizes a hashtable to store the index loaded from a file. Each entry in the hashtable corresponds to a unique word found in the documents, mapping to a counters structure that tracks the occurrence of that word in various document IDs.

2. **Query**: An array of strings (tokens) representing the parsed user query. This array alternates between words and operators, facilitating the evaluation process.

3. **Document Scores**: A counters structure that keeps track of the cumulative score for each document ID as queries are evaluated. The score represents the relevance of a document to the query.

## Control Flow and Pseudo Code

### Main Function
1. **Argument Parsing and Validation**: Check the correctness of command-line arguments, ensuring the presence of a valid page directory and index filename.
2. **Index Loading**: Read the index from the specified file into an in-memory hashtable structure.
3. **Query Processing Loop**:
    - Prompt the user for a query.
    - Read the query from stdin.
    - If the query is empty or invalid, display an error and prompt again.
    - Otherwise, process the query against the loaded index.

### Query Processing
1. **Tokenization**: Break down the query into individual words and operators. Normalize words to lowercase.
2. **Validation**: Ensure the query follows logical syntax rules (e.g., no starting or ending with operators, no consecutive operators).
3. **Evaluation**: For each token in the query:
    - If it's a word, retrieve its document counters from the index.
    - Apply logical operators to combine counters:
        - "and" results in the intersection of document sets.
        - "or" results in the union of document sets.
4. **Scoring and Ranking**: Calculate scores based on the frequency of query terms in documents. Sort documents by their scores in descending order.
5. **Output**: For each document in the sorted list, print the document ID, score, and URL to stdout.

## Functional Decomposition into Modules

### Functions

- **updateCount**: Updates the count for non-zero key-value pairs.
- **countItemsInCounters**: Counts the number of items in a counters structure.
- **initializeOrUpdateIntersectionCount**: Initializes or updates counters for intersection operation.
- **updateIntersectionCounters**: Updates counters for intersection operation.
- **computeCountersIntersection**: Computes the intersection of two counters.
- **updateUnion**: Updates counters for union operation.
- **tokQuery**: Tokenizes the query into words.
- **unionCounters**: Performs the union of two counters.
- **normalizeQuery**: Normalizes the query and performs intersection and union operations.
- **grabURL**: Retrieves the URL for a given docID.
- **updateMaxNodeValue**: Updates the maximum node value.
- **printSearchResults**: Prints search results.

## Error Handling Strategies

- Invalid command-line arguments result in an error message and termination of the program.
- Failure to load the index from the specified file results in an error message and termination.
- Queries that do not conform to the expected syntax are rejected with an error message, prompting the user for another query.
- Memory allocation failures are checked and handled gracefully, with appropriate cleanup and error messages.

## Testing and Validation

- **Syntax Validation Tests**: Verify that the Query Processor correctly handles a range of valid and invalid queries.
- **Evaluation Logic Tests**: Test the Search Evaluator with various queries to ensure accurate document matching and correct application of "and" and "or" logic.
- **Ranking Tests**: Confirm that the Result Ranker correctly scores and orders documents according to the frequency of query terms.
- **End-to-End Tests**: Execute comprehensive tests using real index files to validate the entire process from query input to result output.
- **Memory Leak Detection**: Utilize tools like Valgrind to identify and resolve any memory leaks to ensure efficient resource management.
