CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
SRCDIR = src
BINDIR = bin
DATADIR = data
TESTDIR = tests
RESULTSDIR = results

MATCHING = $(BINDIR)/matching_engine
VERIFIER = $(BINDIR)/verifier
GENERATOR = $(BINDIR)/generator

.PHONY: all clean test directories

all: directories $(MATCHING) $(VERIFIER) $(GENERATOR)

directories:
	@mkdir -p $(BINDIR) $(DATADIR) $(RESULTSDIR)

$(MATCHING): $(SRCDIR)/matching.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(VERIFIER): $(SRCDIR)/verifier.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

$(GENERATOR): $(SRCDIR)/generator.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

test: all
	@echo "Running example test..."
	@$(MATCHING) $(DATADIR)/example.in $(RESULTSDIR)/example.out
	@echo "Verifying example output..."
	@$(VERIFIER) $(TESTDIR)/example.in $(RESULTSDIR)/example.out

clean:
	rm -rf $(BINDIR) $(DATADIR)/*.txt $(RESULTSDIR)/*.out $(RESULTSDIR)/*.csv $(RESULTSDIR)/*.png