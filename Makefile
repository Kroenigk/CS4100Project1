CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

cmos: cmos.cpp
	$(CXX) $(CXXFLAGS) cmos.cpp -o cmos

clean:
	rm -f cmos

.PHONY: clean
