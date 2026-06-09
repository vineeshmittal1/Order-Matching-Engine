CXX = g++

CXXFLAGS = -std=c++17 \
            -pthread \
            -O3 \
            -march=native

SRC = src/main.cpp \
      src/MatchingEngine.cpp

TEST_SRC = tests/stress_test.cpp \
           src/MatchingEngine.cpp

TARGET = engine
TEST_TARGET = stress_test

all:
	$(CXX) \
	$(CXXFLAGS) \
	$(SRC) \
	-o $(TARGET)

test:
	$(CXX) \
	$(CXXFLAGS) \
	$(TEST_SRC) \
	-o $(TEST_TARGET)

run:
	./$(TARGET)

stress:
	./$(TEST_TARGET)

clean:
	rm -f \
	$(TARGET) \
	$(TEST_TARGET) \
	trades.log