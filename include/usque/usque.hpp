#ifndef USQUE_USQUE_HPP_
#define USQUE_USQUE_HPP_

#ifdef PROFILER
#include <chrono>
#include <cstdio>
#include <string>
#include <thread>

#define PROFILER_CONCAT_RAW4(a, b, c, d) a##b##c##d
#define PROFILER_CONCAT4(a, b, c, d) PROFILER_CONCAT_RAW4(a, b, c, d)
#define PROFILER_STRINGIFY_RAW(x) #x
#define PROFILER_STRINGIFY(x) PROFILER_STRINGIFY_RAW(x)

#define PROFILER_FUNCTION_NAME __PRETTY_FUNCTION__
#define PROFILER_NAME PROFILER_CONCAT4(__PROFILER_, __COUNTER__, _, __LINE__)

#define PROF_BLOCK(name)                                                       \
  Profiler::Timer PROFILER_NAME = Profiler::Timer(name, "block");
#define PROF_FUNC()                                                            \
  Profiler::Timer PROFILER_NAME =                                              \
      Profiler::Timer(PROFILER_FUNCTION_NAME, "func");

class Profiler {
public:
  struct Event {
    std::string name;
    std::string cat;
    char ph;
    std::chrono::high_resolution_clock::time_point ts;
    std::thread::id tid;
  };
  class Timer {
  public:
    Timer(const std::string name, const std::string cat);
    ~Timer();

  protected:
    const std::string name, cat;
  };

  ~Profiler();

  static inline Profiler *get() { return &instance; }

  void push_event(const Event &event);

private:
  Profiler();

  bool load_file();
  bool close_file();

  static Profiler instance;
  std::string file_path;
  std::size_t event_count;
  bool first;
  FILE *out;
};
#ifdef PROFILER_IMPL
Profiler Profiler::instance;

Profiler::~Profiler() { this->close_file(); }

void Profiler::push_event(const Event &event) {
  if (!out && !this->load_file())
    return;
  if (first) {
    std::fprintf(out,
                 "{\"name\":\"%s\",\"cat\":\"%s\",\"ph\":\"%c\",\"ts\":%lu,"
                 "\"pid\":1,\"tid\":%lu}",
                 event.name.c_str(), event.cat.c_str(), event.ph,
                 std::chrono::duration_cast<std::chrono::microseconds>(
                     event.ts.time_since_epoch())
                     .count(),
                 std::hash<std::thread::id>{}(event.tid));
    this->first = false;
  } else {
    std::fprintf(out,
                 ",{\"name\":\"%s\",\"cat\":\"%s\",\"ph\":\"%c\",\"ts\":%lu,"
                 "\"pid\":1,\"tid\":%lu}",
                 event.name.c_str(), event.cat.c_str(), event.ph,
                 std::chrono::duration_cast<std::chrono::microseconds>(
                     event.ts.time_since_epoch())
                     .count(),
                 std::hash<std::thread::id>{}(event.tid));
  }
  event_count++;
  if (event_count >= 1000) {
    std::fflush(out);
    event_count = 0;
  }
}

Profiler::Profiler() : file_path("profile.json"), first(true), out(nullptr) {}
bool Profiler::load_file() {
  out = std::fopen(file_path.c_str(), "w");
  if (!out) {
    std::fprintf(stderr, "Failed to open profiling file %s to write to",
                 file_path.c_str());
    return false;
  } else {
    std::fprintf(out, "[");
  }
  return true;
}
bool Profiler::close_file() {
  if (out) {
    std::fprintf(out, "]");
    std::fclose(out);
  }
  return true;
}

Profiler::Timer::Timer(const std::string name, const std::string cat)
    : name(name), cat(cat) {
  Profiler::get()->push_event({name, cat, 'B',
                               std::chrono::high_resolution_clock::now(),
                               std::this_thread::get_id()});
}
Profiler::Timer::~Timer() {
  Profiler::get()->push_event({name, cat, 'E',
                               std::chrono::high_resolution_clock::now(),
                               std::this_thread::get_id()});
}

#endif // PROFILER_IMPL
#else
#define PROF_BLOCK(name)
#define PROF_FUNC()
#endif // PROFILER

#endif // USQUE_USQUE_HPP_
