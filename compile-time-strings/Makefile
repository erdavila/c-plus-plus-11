PRJ := compile-time-strings
HPP := $(PRJ).hpp
CPP := test-$(PRJ).cpp
EXE := test-$(PRJ)

.PHONY: all
all: $(EXE)

.PHONY: clean
clean:
	rm -f $(EXE)

$(EXE): $(HPP)

$(EXE): $(CPP)
	g++ -Wall -std=c++1y $< -o $@
