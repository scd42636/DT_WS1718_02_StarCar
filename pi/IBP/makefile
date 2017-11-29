
CXX=g++

CXXFLAGS=-std=c++14




IBCRuleTest: IBC_Rule.o IBC_Rule.test.o
	$(LINK.cpp)  $^ -o $@

SerialEmulatedTest: Serial_emulated.o Serial_emulated.test.o
	$(LINK.cpp) -g3  $^ -o $@

SerialTest:Serial.o Serial.test.o
	$(LINK.cpp) $^ -o $@

%.o:%.cpp
	$(COMPILE.cpp) $< -o $@

clean:
	-rm *.o
	-rm SerialTest
	-rm SerialEmulatedTest
	-rm IBCRuleTest

.PHONY:clean
