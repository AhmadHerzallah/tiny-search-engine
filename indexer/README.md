# README.md file for indexer
## Assume that docID filenames in crawler-produced directory are less than 100 characters
## No implementation differences from two Specs
#### Note: According to myvalgrind, there's no memory leaks
#### Note: for testing script to work, must have data directory parallel to indexer directory. Data directory must contain (empty) directories: indextesting, letters, readonlydirectory (must have chmod -rwx), toscrape, wikipedia, and readonlyfile.txt (must have chmod -rwx)
