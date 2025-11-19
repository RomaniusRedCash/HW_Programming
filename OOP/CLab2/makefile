CXX = g++
CXXFLAGS = -std=c++17  -Wall -Wextra -O3
TARGET = CLab2
SRCS = Main.cpp Dater.cpp XmlParcer.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
