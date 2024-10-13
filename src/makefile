# Define the compiler
CXX = g++

# Define the flags for compilation
CXXFLAGS = -Wall -Wextra -std=c++11

# Define the target executable name
TARGET = connect4

# Define the source files
SRCS = main.cpp Board.cpp

# Define the object files (derived from the source files)
OBJS = $(SRCS:.cpp=.o)

# Default target to build the executable
all: $(TARGET)

# Rule to build the target (executable)
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile each source file into an object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up the project (remove object files and the executable)
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: all
	./$(TARGET)