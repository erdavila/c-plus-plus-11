CXXFLAGS =	-O2 -g -Wall -fmessage-length=0 -std=c++1y

OBJS =		perfect-forwarding.o

LIBS =

TARGET =	perfect-forwarding

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
