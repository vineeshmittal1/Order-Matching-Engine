CXX = g++
CXXFLAGS = -std=c++17 -pthread -O2

SRC = src/main.cpp \
      src/MatchingEngine.cpp

TARGET = engine

all:
	$(CXX) $(CXXFLAGS) \
	$(SRC) \
	-o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f $(TARGET)