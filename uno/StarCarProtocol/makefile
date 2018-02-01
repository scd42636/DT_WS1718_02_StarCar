
CXX=g++
CXXFLAGS=-std=c++14

test: IBC.o IBC.test.o
	$(LINK.cpp) $^ -o $@

%.o:%.cpp
	$(COMPILE.cpp) $< -o $@

clean:
	-rm test
	-rm *.o

.PHONY: test clean
