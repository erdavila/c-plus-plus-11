MAIN_TARGET := curry
EXE := $(MAIN_TARGET)


.PHONY: all
all: $(EXE)

.PHONY: test
test: $(EXE)
	./$(EXE)
	@echo OK

.PHONY: clean
clean:
	$(RM) $(EXE)

$(EXE): $(MAIN_TARGET).hpp

$(EXE): $(MAIN_TARGET).cpp
	$(CXX) -Wall -std=c++1y $< -o $@
