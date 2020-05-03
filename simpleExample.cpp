#include "simpleGameEngine.h"
#include <cstdio>
#include <cstdlib>
#include <cstdint>

#if defined(__ATARI_ST__)

#endif

int main()
{
  simpleGameEngine ge = simpleGameEngine(0,0,320,200);
  while (1) {}
  return 0;
}
