#define PROFILE_PRETTY_FUNCTION
// #define PROFILER
#include <usque/usque.hpp>

int foo() {
  PROFILE_FUNC();
  return 0.0;
}

int main(int argc, char const *argv[]) {
  for (std::size_t i = 0; i < 500; i++) {
    printf("Frame: %i\n", i);
    PROFILE_FUNC();
    if (true) {
      PROFILE_BLOCK("if block");
      foo();
    }
    for (int j = 0; j < 100000; j++) {
      foo();
    }
  }
  printf("Saving file\n");
  PROFILE_SAVE("simple.json")
  return 0;
}
