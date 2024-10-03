CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lcurl

# Define include directories and source/object locations
INCLUDES = -Iinclude -Iexternal/nlohmann
SRC_DIR = src
OBJ_DIR = obj
SOURCES = $(SRC_DIR)/BinanceAPI.cpp $(SRC_DIR)/TradeParser.cpp $(SRC_DIR)/PerformanceTimer.cpp $(SRC_DIR)/main.cpp
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = binance_api_test

# Target to build the executable
all: $(EXECUTABLE)

# Build the executable from object files
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files into object files inside obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)
