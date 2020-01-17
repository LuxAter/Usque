// #define PROFILER_IMPL
// #define PROFILE_PRETTY_FUNCTION
// #define PROFILER
// #define PROFILER_FILE
#include <usque/usque.hpp>

#include <thread>
#include <vector>

int bar() {
  PROF_FUNC();
  return rand();
}

void foo(const int &c) {
  PROF_FUNC();
  float avg = 0;
  for (int i = 0; i < c; ++i) {
    avg += (bar() / static_cast<float>(c));
  }
}

int main(int argc, char const *argv[]) {
  PROF_FUNC();
  // PROFILE_BEGIN("Multi Thread")
  std::vector<std::thread> threads;
  // PROFILE_BEGIN("Emplace");
  for (int i = 0; i < 100; i += 10) {
    threads.emplace_back(foo, i);
  }
  // PROFILE_END()
  // PROFILE_BEGIN("Join")
  for (auto &t : threads) {
    t.join();
  }
  // PROFILE_END()
  // PROFILE_END()
  // PROFILE_BEGIN("Single Thread")
  for (int i = 0; i < 100; i += 10) {
    foo(i);
  }
  // PROFILE_STOP()
  return 0;
}
