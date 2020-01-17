#define PROFILER_IMPL
#define PROFILER
#include <usque/usque.hpp>

#include <iostream>

float foo(int i) {
  PROF_FUNC()
  float avg = 0.0f;
  for (int j = 0; j < i; ++j) {
    avg += rand() / static_cast<float>(i);
  }
  return avg;
}

int main(int argc, char const *argv[]) {
  PROF_FUNC()
  for (int i = 0; i < 10000; ++i) {
    std::cout << foo(i) << '\n';
  }
  return 0;
}
