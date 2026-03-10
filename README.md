# CS4100Project1
- This code is written by Kylie Roenigk and Angie Bragg
## Set Up
```bash
sudo apt update
sudo apt install flex bison gcc make
```

# Testing the Lexer
```bash
flex -o lex.yy.cpp cmos.l
g++ -std=c++17 lex.yy.cpp -o scanner
./scanner < test.txt
```

## Project Report
# Tokens and Regular Expressions Identifies
- The Flex framework currently identifies 20 different tokens, including one unmatched token. It will ignore all whitespace and comments using regular expressions that match white space and comments format. We ignore multiple line comments by using a comment state that only examples when the end characters appear due to issues of malformed comments and source code. Due to the format of this project, when a token is correctly identifies it will print out the token ID to scanner_out.text with a space between each token. This format will help later during the winnowing implementation. The tokens are roughly split up between a few main categories: keywords, literals, and punctuation. Furthermore, since we have a lot of keywords, we made a keyword stucture that will store the keyword itself and its associated ID for clarity in the code. All the keywords are stored in a Keywords array to be used like a switch statement to indentify which token the keyword is as all keywords and variables share the same regular expression for indentification. The full list of tokens the we identify include: punctuation, pointer, variable, if, while, for, return, else, do, void, literals (char, string, int, bool, floats), int, bool, char, string, operator, and unmatched. There are some important things to note. All operators are treated the same, including single and double character operators as are all punctionation.
# Winnowing Implementation
- Using the output from lex, we reading in each line and place all of the tokens into a vector. Since we just have the token ID's as ints, this makes it easier to turn them into k-mers and hash them. After we get the vector of tokens, we pass them into our hash function which uses a base hash of 31. This function will take create all of the overlapping k-mers and hash them. This the gets us a vector of all of our k-mers hash values which we can then use to winnow. We then go into the winnow function, whcih takes the vector of hashes and finds the unique fingerprints of all of the overlapping windows. We decided to use a k and w value of 4 at the moment. Note that it will only store unique fingerprints, which means that it will only store the smallest hash to fringerprints if the position of the hash is new, this prevents unnecessary duplicates from mucking up our similarity scores. Once the winnowing is done, we repeat the steps for all files that we have tokenizes. We used a custom structure called FileData that stores all of the associated vectos of a certain file, including tokens, hashes, and fingerprints. Once we have all of our files ready, we then make a N * N table with N being the number of files we have. Then each file has its fingerprint compared with the other files to produce a similarity score, which is then stored in our similarityTable. Once the similarity table is complete, the results are printed in a table format. 
# Results of Analysis
- 
