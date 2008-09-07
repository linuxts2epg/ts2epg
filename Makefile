# Makefile

CC	 = gcc
CXX	 = g++

INCDIR = 

CXXFLAGS = -g -Wall -O0 ${INCDIR}
CFLAGS	 = -g -Wall -O0 ${INCDIR}

MV = mv
RM = rm -f

LDFLAGS	 = 

OBJS	 = main.o TSPacket.o TSParser.o SectionFormat.o ExtendedSectionFormat.o EventInformation.o EventInformationTable.o Descriptor.o TSPacketDemuxer.o DescriptorFactory.o ShortEventDescriptor.o ExtendedEventDescriptor.o ARIBStringConverter.o

TARGET	 = ts2epg

.c.o:
	${CC} -c ${CFLAGS} $<

.cpp.o:
	${CXX} -c ${CXXFLAGS} $<

all: ${TARGET}

${TARGET}: ${OBJS}
	$(RM) $@
	${CXX} -o $@ $^ ${LDFLAGS}

clean:
	$(RM) $(OBJS) $(TARGET)	*~
