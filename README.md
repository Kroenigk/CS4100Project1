# CS4100Project1
## Set Up
```bash
sudo apt update
sudo apt install flex bison gcc make
```
flex -o lex.yy.cpp cmos.l
g++ -std=c++17 lex.yy.cpp -o scanner
./scanner < test.txt
#Testing the Lexer
```bash

```
