appname := pabx

CXX := g++
CXXFLAGS := -Wall -O3
LDFLAGS := -lwiringPi -lpaho-mqtt3a

srcfiles := $(shell find . -name "*.cpp")
objects  := $(patsubst %.cpp, %.o, $(srcfiles))

all: $(appname)

$(appname): $(objects)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(appname) $(objects) $(LDLIBS)

depend: .depend

.depend: $(srcfiles)
	rm -f ./.depend
	$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

clean:
	rm -rf $(objects) $(appname)

dist-clean: clean
	rm -f *~ .depend

include .depend