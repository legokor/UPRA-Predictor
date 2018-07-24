BINARY = predictor
OBJECTS = balloonproperties.o coords.o flight.o vecs.o main.o telemetrypacket.o weatherdata.o util.o

SRCDIR = ./src
OBJDIR = ./obj
INCLDIR = ./include

CXX = g++
CXXFLAGS = -std=c++17 -O0 -Wall -Wdeprecated -pedantic -ggdb -DDEBUG
LDFLAGS = -ggdb -fno-omit-frame-pointer

.PHONY: all clean install run

all: $(BINARY)

clean:
	rm -f $(BINARY) $(OBJDIR)/*

$(BINARY): $(addprefix $(OBJDIR)/, $(OBJECTS))
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -I$(SRCDIR) -I$(INCLDIR) $(CXXFLAGS) -c $< -o $@

install:
	echo "Installing is not supported"

run:
	./predictor
