ifeq ("$(shell which pkg-config  2> /dev/null)","")
$(error 'pkg-config' NOT FOUND)
endif

ifeq ("$(shell pkg-config --libs libvlc 2> /dev/null)","")
$(error 'libvlc' NOT FOUND)
endif

ifeq ("$(shell which fltk-config  2> /dev/null)","")
$(error 'fltk-config' NOT FOUND)
endif

APP := flvlc
CPP = g++
CXXLIBS := $(shell pkg-config --libs libvlc) $(shell fltk-config --ldflags) -lpthread
CXXFLAGS := $(shell pkg-config --cflags libvlc) -Wno-write-strings -std=c++11

OBJ := $(subst .cxx,.o,$(wildcard *.cxx))

release: CXXFLAGS+= -O3 -DNDEBUG
release: CXXLIBS+= -Wl,-s
release: $(APP)

debug: CXXFLAGS+= -Wall -O0 -ggdb -DDEBUG
debug: $(APP)

$(APP): $(OBJ)
	$(CPP) $(OBJ) $(CXXLIBS) -o $(APP)

%.o: %.cxx
	$(CPP) $(CXXFLAGS) -c $< -o $@

.PHONY: release

clean:
	rm  *.o $(APP)
