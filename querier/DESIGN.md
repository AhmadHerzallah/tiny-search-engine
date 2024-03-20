<!-- Add a file DESIGN.md to provide the Design Spec for querier. Your DESIGN.md file should not repeat the information provided in the assignment or in the Requirements Spec; instead, it should describe the abstract data structures and pseudo code for your querier. -->

# Design Specification for Querier

## Overview

The `querier` program reads an index file produced by the TSE Indexer and page files produced by the TSE Querier. It answers search queries submitted via standard input, retrieving relevant documents based on the queries. The main goal is to process user queries efficiently and return matching documents along with their relevance scores.

## Abstract Data Structures

### Index Data Structure

The index stores information about words encountered during indexing, including the documents (URLs) where those words occur and their frequency in each document.

- **Index Entry**: Represents a word in the index with:
  - The word itself.
  - A set of document IDs where the word occurs.
  - The frequency of the word in each document.

### Counters Data Structure

Counters associate integer keys with integer values. In `querier`, counters maintain word frequencies and relevance scores during query processing.

- **Counters**: Associates integer keys with integer values. Used to:
  - Store word frequencies for each document.
  - Compute and store relevance scores for documents.

## Pseudo Code Overview

1. **Command Line Argument Validation**:
   - Validate command-line arguments.

2. **Main Functionality**:
   - Read and process user queries until end of input.

3. **Query Processing**:
   - Tokenize query into words.
   - Normalize query by removing non-alphabetic characters and converting words to lowercase.
   - Handle edge cases such as empty queries, invalid characters, and adjacent logical operators.

4. **Index Retrieval**:
   - Retrieve index information for each word in query.
   - Perform union and intersection operations based on logical operators (AND, OR).

5. **Document Retrieval**:
   - Compute relevance scores for documents based on query terms and frequencies.
   - Retrieve document URLs corresponding to relevant documents.
   - Print search results with document IDs, relevance scores, and URLs.

6. **Error Handling**:
   - Handle errors related to file I/O, memory allocation, and invalid input gracefully.

## Conclusion

The `querier` program implements a robust querying mechanism, efficiently processing user queries and retrieving relevant documents based on the TSE Indexer-generated index. Through careful design and utilization of appropriate data structures, it provides a reliable and user-friendly interface for information retrieval.
