CXX = g++
TARGET = MenuConcierge
SRCS = MenuConcierge.cpp
LIBS = -lspc -lPocoFoundation -lPocoNet -lxml2
OBJS := $(SRCS:.cpp=.o)
INC_PATH = -I. `pkg-config --cflags libxml-2.0`
CFLAGS = -Wall -c -Wno-deprecated
LFLAGS = -Wl

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INC_PATH) -MMD -MP -MF"$(@:%.o=%.d)" -o"$@" "$<"

clean:
	rm -f $(TARGET) $(OBJS)
