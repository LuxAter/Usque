#ifndef USQUE_USQUE_HPP_
#define USQUE_USQUE_HPP_

#include <chrono>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#define USQUE_CONCAT_RAW(a, b, c, d, e, f) a##b##c##d##e##f
#define USQUE_CONCAT(a, b, c, d, e, f) USQUE_CONCAT_RAW(a, b, c, d, e, f)
#define USQUE_STRINGIFY_RAW(x) #x
#define USQUE_STRINGIFY(x) USQUE_STRINGIFY_RAW(x)

#define USQUE_FILENAME                                                         \
  (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1     \
                                    : __FILE__)
#define USQUE_REFERENCE_STR __FILE__ USQUE_STRINGIFY(__LINE__)
#define UNIQUE_PROFILER_NAME                                                   \
  USQUE_CONCAT(__, __FUNCTION__, _, __LINE__, _, __COUNTER__)
#ifdef PROFILE_PRETTY_FUNCTION
#define USQUE_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define USQUE_FUNCTION_NAME __FUNCTION__
#endif
#define USQUE_GET_ARG2(arg1, arg2, ...) arg2

#ifdef PROFILER
#define PROFILE_FUNC()                                                         \
  usque::Profiler UNIQUE_PROFILER_NAME =                                       \
      usque::Usque::get()->get_profiler()->scope(USQUE_FUNCTION_NAME,          \
                                                 USQUE_REFERENCE_STR);
#define PROFILE_BLOCK(name)                                                    \
  usque::Profiler UNIQUE_PROFILER_NAME =                                       \
      usque::Usque::get()->get_profiler()->scope(name, USQUE_REFERENCE_STR);
#define PROFILE_BEGIN(name)                                                    \
  usque::Profiler UNIQUE_PROFILER_NAME =                                       \
      usque::Usque::get()->get_profiler()->scope(name, USQUE_REFERENCE_STR);
#define PROFILE_END() usque::Usque::get()->get_profiler()->pop();
#define PROFILE_FILE(name) usque::Usque::get()->set_file(name)
#define PROFILE_SAVE() usque::Usque::get()->free();
#else
#define PROFILE_FUNC()
#define PROFILE_BLOCK(name)
#define PROFILE_BEGIN(name)
#define PROFILE_END()
#define PROFILE_FILE(name)
#define PROFILE_SAVE()
#endif

namespace usque {

typedef std::chrono::high_resolution_clock clock;
typedef std::chrono::microseconds duration;

class Profiler;
class ThreadProfiler;
class Usque;

class Profiler {
public:
  Profiler(const std::string &, const std::size_t &, ThreadProfiler *);
  ~Profiler();
  inline Profiler *get_this() { return this; }
  std::string name;
  std::size_t ref;
  Profiler *parent;
  ThreadProfiler *thread_profiler;
  usque::clock::time_point start_time;
};

class ThreadProfiler {
public:
  ThreadProfiler();
  ThreadProfiler(const std::thread::id &);
  ~ThreadProfiler();
  inline Profiler scope(const std::string &, const std::string &);
  inline void push(Profiler *);
  inline void pop();
  inline void push_start(const std::size_t &, const std::string &,
                         const usque::clock::time_point &);
  inline void push_stop(const std::size_t &, const usque::clock::time_point &);

private:
  std::thread::id id;
  Profiler *active_scope;
  std::hash<std::string> ref_hash;
};

class Usque {
public:
  static inline Usque *get() {
    static Usque *instance = nullptr;
    if (instance == nullptr) {
      instance = new Usque();
    }
    return instance;
  }
  static inline void free() {
    Usque *instance = usque::Usque::get();
    for (auto &prof : instance->profilers) {
      delete prof.second;
    }
    instance->close_file();
    delete instance;
  }
  inline ThreadProfiler *get_profiler();
  inline void set_file(const std::string &);
  inline void close_file();
  inline void push_start(const std::thread::id &, const std::size_t &,
                         const std::string &, const usque::clock::time_point &);
  inline void push_stop(const std::thread::id &, const std::size_t &,
                        const usque::clock::time_point &);

private:
  Usque();
  std::string file_path;
  FILE *file_ptr;
  std::uint32_t count, flush_count;
  std::unordered_map<std::thread::id, ThreadProfiler *> profilers;
  std::unordered_set<std::size_t> profiler_ids;
};

Profiler::Profiler(const std::string &name, const std::size_t &ref,
                   ThreadProfiler *thread)
    : name(name), ref(ref), parent(nullptr), thread_profiler(thread),
      start_time(usque::clock::now()) {}
Profiler::~Profiler() {
  if (thread_profiler) {
    thread_profiler->pop();
  }
}

ThreadProfiler::ThreadProfiler() : id(), active_scope(nullptr), ref_hash() {}
ThreadProfiler::ThreadProfiler(const std::thread::id &id)
    : id(id), active_scope(nullptr), ref_hash() {}
ThreadProfiler::~ThreadProfiler() {
  while (active_scope != nullptr) {
    this->pop();
  }
}

void ThreadProfiler::push_start(const std::size_t &ref, const std::string &name,
                                const usque::clock::time_point &start) {
  usque::Usque::get()->push_start(id, ref, name, start);
}
void ThreadProfiler::push_stop(const std::size_t &ref,
                               const usque::clock::time_point &stop) {
  usque::Usque::get()->push_stop(id, ref, stop);
}

Profiler ThreadProfiler::scope(const std::string &name,
                               const std::string &ref) {
  Profiler scoped(name, ref_hash(ref), this);
  this->push(scoped.get_this());
  return scoped;
}

void ThreadProfiler::push(Profiler *scoped) {
  push_start(scoped->ref, scoped->name, scoped->start_time);
  if (active_scope == nullptr) {
    active_scope = scoped;
  } else {
    scoped->parent = active_scope;
    active_scope = scoped;
  }
}
void ThreadProfiler::pop() {
  if (active_scope == nullptr) {
    return;
  } else {
    Profiler *pop_scope = active_scope;
    active_scope = pop_scope->parent;
    push_stop(pop_scope->ref, usque::clock::now());
    pop_scope->thread_profiler = nullptr;
  }
}

ThreadProfiler *Usque::get_profiler() {
  const std::thread::id thread_id = std::this_thread::get_id();
  std::unordered_map<std::thread::id, ThreadProfiler *>::iterator it;
  if ((it = profilers.find(thread_id)) != profilers.end()) {
    return it->second;
  } else {
    ThreadProfiler *thread_profiler = new ThreadProfiler(thread_id);
    profilers[thread_id] = thread_profiler;
    return thread_profiler;
  }
}
void Usque::set_file(const std::string &file) { file_path = file; }
void Usque::close_file() {
  if (file_ptr != nullptr) {
    fflush(file_ptr);
    fclose(file_ptr);
  }
}
void Usque::push_start(const std::thread::id &thread, const std::size_t &ref,
                       const std::string &name,
                       const usque::clock::time_point &start) {
  if (file_ptr == nullptr) {
    file_ptr = fopen(file_path.c_str(), "w");
  }
  if (profiler_ids.find(ref) == profiler_ids.end()) {
    profiler_ids.insert(ref);
    fprintf(file_ptr, "ID 0x%lX %s\n", ref, name.c_str());
  }
  fprintf(file_ptr, "BEGIN 0x%lX 0x%lX %lu\n", thread, ref,
          std::chrono::duration_cast<usque::duration>(start.time_since_epoch())
              .count());
  count++;
  if (count >= flush_count) {
    fflush(file_ptr);
  }
}
void Usque::push_stop(const std::thread::id &thread, const std::size_t &ref,
                      const usque::clock::time_point &stop) {
  if (file_ptr == nullptr) {
    file_ptr = fopen("profile.txt", "w");
  }
  fprintf(file_ptr, "END 0x%lX 0x%lX %lu\n", thread, ref,
          std::chrono::duration_cast<usque::duration>(stop.time_since_epoch())
              .count());
  count++;
  if (count >= flush_count) {
    fflush(file_ptr);
  }
}

Usque::Usque()
    : file_path("profile.txt"), file_ptr(nullptr), count(0), flush_count(1000),
      profilers(), profiler_ids() {}

} // namespace usque

#endif // USQUE_USQUE_HPP_
