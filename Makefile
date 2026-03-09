CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

cmos: cmos.cpp lex
	$(CXX) $(CXXFLAGS) cmos.cpp -o cmos

lex: cmos.l
	flex -o lex.yy.cpp cmos.l
	g++ -std=c++17 lex.yy.cpp -o scanner

clean:
	rm -f cmos scanner *.txt

.PHONY: clean
