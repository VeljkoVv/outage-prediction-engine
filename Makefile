CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g
LDLIBS   := -lsqlite3

SRC      := $(wildcard src/*.cpp)
OBJ      := $(patsubst src/%.cpp,build/%.o,$(SRC))
TARGET   := outage_predictor

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDLIBS)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build $(TARGET)

-include $(OBJ:.o=.d)

.PHONY: all clean

test: $(filter-out build/main.o,$(OBJ))
	$(CXX) $(CXXFLAGS) $^ tests/*.cpp -o build/run_tests $(LDLIBS) -lgtest_main -lgtest -pthread
	./build/run_tests