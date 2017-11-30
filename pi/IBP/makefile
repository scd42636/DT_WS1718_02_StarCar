
CXX=g++

CXXFLAGS=-std=c++14


IBCEmu: IBC.o IBC_Inbox.o IBC_Packet.o IBC_Rule.o IBC_Transceiver.o Serial_emulated.o IBC.test.o
	$(LINK.cpp) -D IBC_EMULATED $^ -o $@ 
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
