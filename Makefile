# Makefile for olcPixelGameEngineST

LIBCMINI=/opt/libcmini
CXX = m68k-atari-mint-g++-8
CXXFLAGS =  -Wall -O2 -std=c++17 -fomit-frame-pointer -fno-exceptions -D_DEBUG_ -D__ATARI_ST__
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

LDFLAGS = -lgem -lpng -lz

sge.prg: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) sge.prg
