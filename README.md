# CS4100Project1
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
