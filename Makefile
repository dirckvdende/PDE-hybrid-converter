
cc = g++
cargs = -Isrc -std=c++17 -Wall -Wextra -Wpedantic -g
cfiles = $(shell find src -name "*.cc")
hfiles = $(cfiles:.cc=.h)
ofiles = $(subst src,build/src,$(cfiles:.cc=.o))
objfiles = $(filter-out build/src/main.o, $(ofiles))
dfiles = $(ofiles:.o=.d)

subfolders = $(subst src,,$(subst src/,,$(shell find src -type d)))
buildfolders = $(addprefix build/src/,$(subfolders)) build build/tests tmp

testfiles = $(shell find tests -name "*.cc")
tests = $(subst .cc,,$(addprefix build/,$(testfiles)))

.PHONY: all
all: build/main $(tests)

clean:
	rm -r -f build/*
	rm -r -f tmp/*

# Create build folders
$(buildfolders):
	mkdir -p $@

# Main executable
build/main: build/src/main
	cp build/src/main build/main
build/src/main: $(ofiles) | $(buildfolders)
	$(cc) $(cargs) -o $@ $(ofiles)
# Testing files
build/tests/%: tests/%.cc $(objfiles) | $(buildfolders)
	$(cc) $(cargs) -o $@ $< $(objfiles)

# Dependency files
-include $(dfiles)

build/src/%.o: src/%.cc | $(buildfolders)
	$(cc) $(cargs) -MMD -MP -c $< -o $@
