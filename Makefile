CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O2
TARGET = othello
SOURCES = main.cpp board.cpp game.cpp player.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = board.hpp game.hpp player.hpp

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
