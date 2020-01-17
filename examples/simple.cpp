#define PROFILER_IMPL
#define PROFILER_PRETTY_FUNCTION
#define PROFILER_FILE
#define PROFILER
#include <usque/usque.hpp>

#include <iostream>

float foo(int i) {
  PROFILE_FUNC()
  float avg = 0.0f;
  for (int j = 0; j < i; ++j) {
    avg += rand() / static_cast<float>(i);
  }
  return avg;
}

int main(int argc, char const *argv[]) {
  PROFILE_FUNC()

  for (int i = 0; i < 10; ++i) {
    std::cout << foo(i) << '\n';
  }
  return 0;
}
