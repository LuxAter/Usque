#define PROFILER_IMPL
#define PROFILE_PRETTY_FUNCTION
#define PROFILER
#define PROFILER_FILE
#include <usque/usque.hpp>

#include <thread>
#include <vector>

int bar() {
  PROFILE_FUNC();
  return rand();
}

void foo(const int &c) {
  PROFILE_FUNC();
  float avg = 0;
  for (int i = 0; i < c; ++i) {
    avg += (bar() / static_cast<float>(c));
  }
}

int main(int argc, char const *argv[]) {
  PROFILE_FUNC();
  std::vector<std::thread> threads;
  PROFILE_BEGIN("Emplace");
  for (int i = 0; i < 100; i += 10) {
    threads.emplace_back(foo, i);
  }
  PROFILE_END()
  PROFILE_BEGIN("Join")
  for (auto &t : threads) {
    t.join();
  }
  PROFILE_END()
  PROFILE_STOP()
  return 0;
}
