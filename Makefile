# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 \
            -I/usr/local/opt/openblas/include \
            -I/usr/local/opt/jpeg-turbo/include \
            -I/usr/local/Cellar/opencv/4.10.0_12/include/opencv4
LDFLAGS = -L/usr/local/opt/openblas/lib -lopenblas \
           -L/usr/local/opt/jpeg-turbo/lib -ljpeg \
           -L/usr/local/Cellar/opencv/4.10.0_12/lib -lopencv_core -lopencv_imgcodecs -lopencv_highgui

# Directories
SRC_DIR = src
EXE_DIR = .
TARGET = $(EXE_DIR)/squishy

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(SRC_DIR)/%.o)

# Default target
all: $(TARGET)
	./$(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(EXE_DIR)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)
