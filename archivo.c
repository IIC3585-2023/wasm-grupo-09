#include <stdio.h>
#include <emscripten.h>

EMSCRIPTEN_KEEPALIVE
int square(int num) {
  return num * num;
}
