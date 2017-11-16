
CXX=g++

CXXFLAGS=-std=c++14 -lpthread


SerialTest:Serial.o Serial.test.o
	$(LINK.cpp) $^ -o $@

%.o:%.cpp
	$(COMPILE.cpp) $< -o $@

clean:
	-rm *.o
	-rm SerialTest

.PHONY:clean
