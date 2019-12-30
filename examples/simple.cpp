// #define PROFILE_PRETTY_FUNCTION
#include <usque/usque.hpp>

int foo() {
  PROFILE_FUNC();
  return 0.0;
}

int main(int argc, char const *argv[]) {
  for (std::size_t i = 0; i < 2; i++) {
    PROFILE_FUNC();
    if (true) {
      PROFILE_BLOCK("if block");
      foo();
    }
    for (int j = 0; j < 100; j++) {
      PROFILE_BLOCK("frame");
      foo();
    }
  }
  usque::Profiler::save("usque.json");
  usque::Profiler::free();
  return 0;
}
