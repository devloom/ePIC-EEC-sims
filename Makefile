# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 `root-config --cflags`
LDFLAGS = `root-config --ldflags --libs`

# Absolute paths to directories
SRCDIR = $(shell pwd)/src
INCDIR = $(shell pwd)/include
OBJDIR = $(shell pwd)/obj
LIBDIR = $(shell pwd)/lib

# Library name
TARGET_LIB = $(LIBDIR)/libEnergyCorrelator.so

# Source files and objects
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES)) $(DICTOBJ)

# Dictionary files
DICT = $(OBJDIR)/EnergyCorrelatorDict.cxx
DICTOBJ = $(OBJDIR)/EnergyCorrelatorDict.o

# Rule for building shared library
all: $(TARGET_LIB)

# Generate the shared library
$(TARGET_LIB): $(OBJECTS)
	@mkdir -p $(LIBDIR)
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

# Compile object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -fPIC -c $< -o $@

# Generate ROOT dictionary with correct paths
$(DICT): $(INCDIR)/EnergyCorrelatorLinkDef.h $(INCDIR)/Particle.h $(INCDIR)/Jet.h $(INCDIR)/EnergyCorrelator.h $(INCDIR)/HistoManager.h
	@mkdir -p $(OBJDIR)
	@mkdir -p $(LIBDIR)
	rootcling -f $(DICT) -c -I$(INCDIR) -s $(LIBDIR)/EnergyCorrelatorDict \
	    HistoManager.h Particle.h Jet.h EnergyCorrelator.h EnergyCorrelatorLinkDef.h

# Compile ROOT dictionary object
$(DICTOBJ): $(DICT)
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -fPIC -c $< -o $@

# Clean up build artifacts
clean:
	rm -rf $(OBJDIR) $(LIBDIR) $(DICT) $(DICT:.cxx=.pcm)

.PHONY: all clean