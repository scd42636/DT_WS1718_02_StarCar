
CXX=g++

CXXFLAGS=-std=c++14


IBCTest: IBC.o IBC.test.o
	$(LINK.cpp) $^ -o $@

SerialTest:Serial.o Serial.test.o
	$(LINK.cpp) $^ -o $@

%.o:%.cpp
	$(COMPILE.cpp) $< -o $@

clean:
	-rm *.o
	-rm SerialTest
	-rm IBCTest

.PHONY:clean
