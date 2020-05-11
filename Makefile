# Makefile for olcPixelGameEngineST

LIBCMINI=/opt/libcmini
CXX = m68k-atari-mint-g++-8
CXXFLAGS = -O2 -Wall -std=c++17 -fno-exceptions -D__WITH_STDIO__ -D__ATARI_ST__ 
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)

LDFLAGS = -lgem -lpng -lz

sge.prg: $(obj)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) sge.prg
