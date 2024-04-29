# Compiler settings
CC = g++
CFLAGS = -Wall -I./lib
LDFLAGS = -lpigpio -lrt -pthread
LIBRARY_FLAGS = -fPIC -shared

# Directory for binaries (keep it separate from source files)
BINDIR = ./bin

# Target executables and sources
EXEC1 = $(BINDIR)/hourAvg
SRC1 = src/hourAvg.cpp

EXEC2 = $(BINDIR)/mainFetch
SRC2 = src/mainFetch.cpp

# Library source and output
LIBRARY_SRC = lib/SHT3.cpp
LIBRARY_OUT = lib/libSHT3.so

# Default target
all: $(EXEC1) $(EXEC2)

# Rule for creating the shared library
$(LIBRARY_OUT): $(LIBRARY_SRC)
	$(CC) $(CFLAGS) $(LIBRARY_FLAGS) $(LIBRARY_SRC) -o $@ $(LDFLAGS)

# Rule for creating the executable hourAvg
$(EXEC1): $(SRC1)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(SRC1) -o $@ $(LDFLAGS)

# Rule for creating the executable mainFetch
$(EXEC2): $(LIBRARY_OUT) $(SRC2)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(SRC2) -o $@ -L./lib -lSHT3 $(LDFLAGS) -Wl,-rpath,$(PWD)/lib

# Clean rule
clean:
	rm -f $(BINDIR)/* $(LIBRARY_OUT)

# Phony targets
.PHONY: all clean