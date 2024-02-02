# Testing with wrong number of arguments
echo "Testing with wrong number of arguments\n"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 5 bruh
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"
# testing with out of range depth value
echo "testing with out of range depth value\n"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 69
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

# Testing with a non-existent link
echo "Testing with a non-existent link\n"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.htmlbruhbruhbruhbruh ../data/letters 5
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

# Valgrind should report no memory errors or memory leaks, when crawler exits normally
echo "Valgrind should report no memory errors or memory leaks, when crawler exits normally\n"
valgrind ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 1
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

# Testing with a non-existent directory
echo -e "testing with improper directory\n"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../notarealfile/letters 5
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

echo "Crawling letters, depth = 5"
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 5

echo -e "Sample of two files in letters\n"
head -3 ../data/letters/1
head -3 ../data/letters/2
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

echo "Crawling toscrape, depth = 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape 1
echo -e "Sample a file in toscrape\n"
head -3 ../data/toscrape/1
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"

echo "Crawling wikipedia, depth = 1"
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia 1
echo -e "Sample a file in wikipedia\n"
head -3 ../data/wikipedia/1
echo "-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-"
