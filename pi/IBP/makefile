
CXX=g++

CXXFLAGS=-std=c++14
LDLIBS+=-lpthread

default: IBC

IBC: IBC.o IBC_Inbox.o IBC_Packet.o IBC_Rule.o IBC_Transceiver.o Serial.o IBC.test.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@ 

IBCEmu: CXXFLAGS += -D IBC_EMULATED
IBCEmu: IBC.o IBC_Inbox.o IBC_Packet.o IBC_Rule.o IBC_Transceiver.o Serial_emulated.o IBC.test.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@ 
IBCRuleTest: IBC_Rule.o IBC_Rule.test.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@ 

PacketTest : IBC_Packet.o IBC_Packet.test.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@ 


SerialEmulatedTest: Serial_emulated.o Serial_emulated.test.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@ 

SerialTest:Serial.o Serial.test.o
	$(LINK.cpp) $^ $(LDLIBS) -o $@ 

%.o:%.cpp
	$(COMPILE.cpp) $< -o $@

dox: dox.cfg
	doxygen dox.cfg

clean:
	-rm *.o
	-rm SerialTest
	-rm SerialEmulatedTest
	-rm IBCRuleTest
	-rm IBCEmu
	-rm PacketTest
	-rm -r ../../dokumentation/grr/doc/*
	-rm IB
	-rm IBC

.PHONY:clean IBCEmu IBCRuleTest PacketTest SerialEmulatedTest SerialTest dox IBC
