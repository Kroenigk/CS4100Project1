# CS4100Project1
- This code is written by Kylie Roenigk and Angie Bragg
## Set Up
```bash
sudo apt update
sudo apt install flex bison gcc make
```

## Testing the Lexer
```bash
flex -o lex.yy.cpp cmos.l
g++ -std=c++17 lex.yy.cpp -o scanner
./scanner < test.txt
```

## Running the Project
```bash
make clean
make
chmod +x PlagerismDetector.sh
./PlagerismDetector Examples
```

# Project Report
## Tokens and Regular Expressions Identifies
 The Flex framework currently identifies 20 different tokens, including one unmatched token. It will ignore all whitespace and comments using regular expressions that match white space and comments format. We ignore multiline comments by using a comment state that only examples when the end characters appear due to issues of malformed comments and source code. Due to the format of this project, when a token is correctly identifies it will print out the token ID to scanner_out.text with a space between each token. This format will help later during the winnowing implementation. The tokens are roughly split up between a few main categories: keywords, literals, and punctuation. Furthermore, since we have a lot of keywords, we made a keyword structure that will store the keyword itself and its associated ID for clarity in the code. All the keywords are stored in a Keywords array to be used like a switch statement to identify which token the keyword is as all keywords and variables share the same regular expression for identification. The full list of tokens the we identify include: punctuation, pointer, variable, if, while, for, return, else, do, void, literal values (char, string, int, bool, floats), type keywords (int, bool, char, string, operator), and unmatched. Some important things to note are that all operators are given equivalent tokens, including single and double character operators. All punctuation are also given the same token value.
## Winnowing Implementation
 Using the output from lex, we read in each line and place all of the tokens into a vector. Since we just have the token ID's as ints, this makes it easier to turn them into k-mers and hash them. After we get the vector of tokens, we pass them into our hash function which uses a base hash of 31, we chose a prime number to reduce collisions without initalizing too large of a space. This function will take create all of the overlapping k-mers and hash them. This then gets us a vector of all of our k-mers hash values which we can then use to winnow. We then go into the winnow function, which takes the vector of hashes and finds the unique fingerprints of all of the overlapping windows. We decided to use a k and w value of 4 at the moment. Note that it will only store unique fingerprints, which means that it will only store the smallest hash to fingerprints if the position of the hash is new, this prevents unnecessary duplicates from mucking up our similarity scores. Once the winnowing is done, we repeat the steps for all files that we have tokenized. We used a custom structure called FileData that stores all of the associated vectos of a certain file, including tokens, hashes, and fingerprints. Once we have all of our files ready, we then make a N * N table with N being the number of files we compare. Then each file has its fingerprint compared with the other files to produce a similarity score, which is then stored in our similarityTable. Once the similarity table is complete, the results are printed into a table format. 
## Results of Analysis
 From the output file (PlagarismReport.txt) all percentages are similarity are printed out in the table. Afterwards, a list of notable pairs are listed out, filtered by results over eighty percent. To change this, you can change the global double FILTER The pairs are ordered by the first file then the second file, then list the similarity score. No pair is listed more than once. After all notable pairs are listed, the average score of all the pairings is printed, to give a perspective on what values to be considered when looking at the similarity scores for plagarism.
 Using the Notable Pairs, we believe that files 3, 9, 53, and 54 are all completely plagarized due to them all having a 100% similarity score between all of them. There is also high suspicion regarding 5 and 18; 10 and 30; 15 and 22; 21, 38, and 43; 26 and 37; 35 and 39; and 47 and 49. When double-checking manually, the files with remarkably high similarity scores read almost identically, confirming the likelihood of plagarism within the files. We believe that eighty percent was the best threshold to find files most likely to contain plagarism for this dataset, giving us about 40 pairs out of over 2500.
 When comparing files with average or lower similarity percentages, we found that there were major differences in the code implementation or syntax mistakes, in one notable case, the file was just a terminal output. We also included the average similarity due to the fact that the programs were all solving the same issue or following the same prompt, which meant that many coders would follow similar structures and solutions to the given problem. We used this value, forty-eight percent, to place our suspicion of plagarism at eighty percent and near certainty of plagarism at ninety percent.
