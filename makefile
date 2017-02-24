appname := Matrix
CXX := g++
CXXFLAGS := -std=c++11

srcfiles := $(shell find . -name "*.c++")
objects := $(patsubst %.c++, %.o, $(srcfiles))

all: $(appname)

$(appname): $(srcfiles)
	$(CXX) $(CXXFLAGS) -o $(appname) $(srcfiles)

run: $(appname)
	./Matrix


