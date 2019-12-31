#define PROFILE_PRETTY_FUNCTION
#define PROFILER
#include <usque/usque.hpp>

float foo(int i) {
  PROFILE_FUNC();
  float avg = 0.0f;
  for (int j = 0; j < i; ++j) {
    avg += rand() / (float)(i);
  }
  return avg;
}

int main(int argc, char const *argv[]) {
  PROFILE_FILE("speed_check.txt");
  PROFILE_FUNC();
  for (int i = 0; i < 100000; i += 10) {
    PROFILE_BLOCK("frame");
    foo(i);
  }
  PROFILE_SAVE()
  return 0;
}