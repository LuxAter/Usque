#define PROFILER_IMPL
#define PROFILER
#include <cmath>
#include <usque/usque.hpp>

float foo(int i) {
  PROF_FUNC();
  float avg = 0.0f;
  for (int j = 0; j < i; ++j) {
    avg += rand() / (float)(i);
  }
  return avg;
}

int main(int argc, char const *argv[]) {
  PROF_FUNC();
  for (int i = 0; i < 100000; i += 10) {
    PROF_BLOCK("frame");
    foo(i);
  }
  return 0;
}
