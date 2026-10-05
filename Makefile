CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC_DIR = src
OBJ_DIR = obj

# Find all .cpp files automatically
SRCS = main.cpp $(wildcard src/*.cpp)
# Translate those into .o (object) files for the build folder
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

BIN = simulator

# The default command when you type 'make'
all: $(BIN)

# Link all the .o files together into the final executable
$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each individual .cpp file into a .o file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# A shortcut to build and run the program in one step
run: all
	./$(BIN)

# A shortcut to delete the compiled files and start fresh
clean:
	rm -rf $(OBJ_DIR) $(BIN)