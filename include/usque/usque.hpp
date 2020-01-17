#ifndef USQUE_USQUE_HPP_
#define USQUE_USQUE_HPP_

#ifdef PROFILER
#include <chrono>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

#define USQUE_CONCAT_RAW6(a, b, c, d, e, f) a##b##c##d##e##f
#define USQUE_CONCAT6(a, b, c, d, e, f) USQUE_CONCAT_RAW6(a, b, c, d, e, f)
#define USQUE_STRINGIFY_RAW(x) #x
#define USQUE_STRINGIFY(x) USQUE_STRINGIFY_RAW(x)

#define USQUE_REFERENCE_STR __FILE__ USQUE_STRINGIFY(__LINE__)
#define USQUE_PROFILER_NAME                                                    \
  USQUE_CONCAT6(__, __FUNCTION__, _, __LINE__, _, __COUNTER__)
#ifdef PROFILER_PRETTY_FUNCTION
#define USQUE_FUNCTION_NAME __PRETTY_FUNCTION__
#else
#define USQUE_FUNCTION_NAME __FUNCTION__
#endif

#define PROFILE_FUNC()                                                         \
  usque::ScopedProfiler USQUE_PROFILER_NAME =                                  \
      usque::Profiler::get()->push(USQUE_FUNCTION_NAME, USQUE_REFERENCE_STR);
#define PROFILE_BLOCK(name)                                                    \
  usque::ScopedProfiler USQUE_PROFILER_NAME =                                  \
      usque::Profiler::get()->push(name, USQUE_REFERENCE_STR);
#define PROFILE_BEGIN(name)                                                    \
  usque::ScopedProfiler USQUE_PROFILER_NAME =                                  \
      usque::Profiler::get()->push(name, USQUE_REFERENCE_STR);
#define PROFILE_END() usque::Profiler::get()->pop();
#define PROFILE_STOP() usque::Profiler::get()->pop_all();
#define PROFILE_FREE() usque::Profiler::get()->free();

namespace usque {

typedef std::chrono::steady_clock clock;

enum ProfileState { START, STOP };

struct ProfileEntry {
  clock::time_point time;
  ProfileState state;
  std::size_t ref;
};

class ScopedProfiler;

class Profiler {
public:
  ~Profiler() {
#ifdef PROFILER_FILE
    if (output_file != nullptr)
      std::fclose(output_file);
#endif
  }

  static inline Profiler *get() { return &instance; }

  inline void push_identifier(const std::size_t &ref, const std::string &id);

  inline ScopedProfiler push(const std::string &name, const std::string &ref,
                             std::uint32_t mask = 0);
  inline void push(const std::size_t &ref);
  inline void pop(const std::thread::id id = std::this_thread::get_id());
  inline void pop(const std::size_t &ref);
  inline void pop_all();

  inline std::string get_name(const std::size_t &ref) const;
  inline void clear();

  inline std::vector<ProfileEntry>
  get_thread_data(const std::thread::id id = std::this_thread::get_id()) const;
  inline std::unordered_map<std::size_t, std::string> get_identifiers() const;

private:
  Profiler()
      : buffer_count(0), data_mem_limit(100000), identifiers(), data(),
        active_scope(), reference_hash() {
#ifdef PROFILER_FILE
    file_flush_limit = 500;
    output_file = std::fopen("profile.txt", "w");
#endif
  }

  static Profiler instance;
  std::size_t buffer_count;
  std::size_t data_mem_limit;
  std::unordered_map<std::size_t, std::string> identifiers;
  std::unordered_map<std::thread::id, std::vector<ProfileEntry>> data;
  std::unordered_map<std::thread::id, ScopedProfiler *> active_scope;
  std::hash<std::string> reference_hash;
#ifdef PROFILER_FILE
  std::size_t file_flush_limit;
  FILE *output_file;
#endif
};

class ScopedProfiler {
public:
  ScopedProfiler(const std::size_t &ref, const bool &active,
                 ScopedProfiler *parent, Profiler *root_ptr);
  ~ScopedProfiler();

  inline void push();
  inline void pop();

  inline bool is_active() const { return active; }
  inline ScopedProfiler *get_parent() { return parent; }
  inline ScopedProfiler *get_this() { return this; }

private:
  bool active;
  const std::size_t ref;
  ScopedProfiler *parent;

  Profiler *root;
};

} // namespace usque

#ifdef PROFILER_IMPL
usque::Profiler usque::Profiler::instance;

std::string usque::Profiler::get_name(const std::size_t &ref) const {
  return identifiers.at(ref);
}

void usque::Profiler::push_identifier(const std::size_t &ref,
                                      const std::string &id) {
  if (identifiers.find(ref) == identifiers.end()) {
    identifiers[ref] = id;
#ifdef PROFILER_FILE
    fprintf(output_file, "IDENTIFIER %lx %s\n", ref, id.c_str());
    buffer_count++;
    if (buffer_count > file_flush_limit) {
      fflush(output_file);
      buffer_count = 0;
    }
#endif
  }
}

usque::ScopedProfiler usque::Profiler::push(const std::string &name,
                                            const std::string &ref,
                                            std::uint32_t mask) {
  const std::thread::id id = std::this_thread::get_id();
  this->push_identifier(reference_hash(ref), name);
  ScopedProfiler *parent = active_scope[id];
  ScopedProfiler new_scope(reference_hash(ref), true, parent, this);
  active_scope[id] = new_scope.get_this();
  return new_scope;
}
void usque::Profiler::push(const std::size_t &ref) {
  const std::thread::id id = std::this_thread::get_id();
  data[id].push_back({clock::now(), ProfileState::START, ref});
  if(data[id].size() > data_mem_limit){
    data[id].erase(data[id].begin());
  }
#ifdef PROFILER_FILE
  fprintf(output_file, "START %lx %lx %lu\n", id, ref, data[id].back().time);
  buffer_count++;
  if (buffer_count > file_flush_limit) {
    fflush(output_file);
    buffer_count = 0;
  }
#endif
}
void usque::Profiler::pop(const std::thread::id id) {
  std::unordered_map<std::thread::id, ScopedProfiler *>::iterator it;
  if ((it = active_scope.find(id)) != active_scope.end()) {
    it->second->pop();
  }
}
void usque::Profiler::pop(const std::size_t &ref) {
  const std::thread::id id = std::this_thread::get_id();
  data[id].push_back({clock::now(), ProfileState::STOP, ref});
  if(data[id].size() > data_mem_limit){
    data[id].erase(data[id].begin());
  }
#ifdef PROFILER_FILE
  fprintf(output_file, "STOP %lx %lx %lu\n", id, ref, data[id].back().time);
  buffer_count++;
  if (buffer_count > file_flush_limit) {
    fflush(output_file);
    buffer_count = 0;
  }
#endif
  if (this->active_scope[id] != nullptr)
    this->active_scope[id] = this->active_scope[id]->get_parent();
}
void usque::Profiler::pop_all() {
  for (auto &thread : active_scope) {
    while (thread.second != nullptr) {
      thread.second->pop();
    }
  }
}

void usque::Profiler::clear() {
  active_scope.clear();
  identifiers.clear();
  data.clear();
}

std::vector<usque::ProfileEntry>
usque::Profiler::get_thread_data(const std::thread::id id) const {
  return data.at(id);
}
std::unordered_map<std::size_t, std::string>
usque::Profiler::get_identifiers() const {
  return identifiers;
}

usque::ScopedProfiler::ScopedProfiler(const std::size_t &ref,
                                      const bool &active,
                                      ScopedProfiler *parent,
                                      Profiler *root_ptr)
    : active(active), ref(ref), parent(parent), root(root_ptr) {
  this->push();
}
usque::ScopedProfiler::~ScopedProfiler() { this->pop(); }
void usque::ScopedProfiler::push() {
  if (this->active)
    this->root->push(ref);
}
void usque::ScopedProfiler::pop() {
  if (this->active)
    this->root->pop(ref);
  this->active = false;
}
#endif
#else
#define PROFILE_FUNC()
#define PROFILE_BLOCK(name)
#define PROFILE_BEGIN(name)
#define PROFILE_END()
#define PROFILE_STOP()
#define PROFILE_FREE()
#endif

#endif // USQUE_USQUE_HPP_
