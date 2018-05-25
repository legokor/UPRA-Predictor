BINARY = predictor
OBJECTS = balloonproperties.o coords.o flight.o vecs.o main.o telemetrypacket.o weatherdata.o
HEADERS = balloonproperties.h coords.h dataset.hpp flight.h units.hpp vecs.h telemetrypacket.h weatherdata.h

CXX = g++
CXXFLAGS = -std=c++17 -O0 -Wall -Wdeprecated -pedantic -ggdb
LDFLAGS = -ggdb -fno-omit-frame-pointer

.PHONY: all clean install run

all: $(BINARY)

clean:
	rm -f $(BINARY) $(OBJECTS)

$(BINARY): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.hpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

install:
	echo "Installing is not supported"

run:
	./predictor
