#ifndef USQUE_USQUE_HPP_
#define USQUE_USQUE_HPP_

#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <vector>

#include "compiler.hpp"

#if USQUE_COMPILER_CXX_FUNC_IDENTIFIER == 1
#ifdef PROFILE_PRETTY_FUNCTION
#if USQUE_COMPILER_IS_MSVC != 0
#define USQUE_FUNCTION_NAME __FUNCSIG__
#else
#define USQUE_FUNCTION_NAME __PRETTY_FUNCTION__
#endif
#else
#define USQUE_FUNCTION_NAME __func__
#endif
#else
#define USQUE_FUNCTION_NAME __FUNCTION__
#endif

#define USQUE_STRINGIFY(x) #x
#define USQUE_TOSTRING(x) USQUE_STRINGIFY(x)
#define USQUE_REFERENCE __FILE__ ":" USQUE_TOSTRING(__LINE__)

#define PROFILE_BLOCK(name)                                                    \
  usque::Block __FUNCTION__##__LINE__ = usque::Profiler::get()->block(         \
      name, USQUE_REFERENCE, usque::BlockType::NONE);

#define PROFILE_FUNC()                                                         \
  usque::Block __FUNCTION__##__LINE__ = usque::Profiler::get()->block(         \
      USQUE_FUNCTION_NAME, USQUE_REFERENCE, usque::BlockType::FUNCTION);

namespace usque {
enum BlockType {
  NONE,
  FUNCTION,
};
struct Block;
struct BlockData;

class Profiler {
public:
  static inline Profiler *get() {
    static Profiler *instance = new Profiler();
    if (instance == nullptr)
      instance = new Profiler();
    return instance;
  }
  static inline void free() {
    Profiler *instance = Profiler::get();
    delete instance;
  }
  static inline bool save(const std::string &file) {
    return Profiler::get()->dump(file);
  }
  inline Block block(const std::string &name, const std::string &ref,
                     const BlockType &type);
  inline void pop_block();

  bool dump(const std::string &file) const;
  std::string dumps() const;

private:
  Profiler() {}

  std::stringstream &serialize_thread_data(
      std::stringstream &ser,
      const std::map<std::string, BlockData> &thread_data) const;
  std::stringstream &
  serialize_block_data(std::stringstream &ser,
                       const std::pair<std::string, BlockData> &data) const;

  std::map<std::thread::id, std::map<std::string, BlockData>> data;
  std::map<std::thread::id, std::stack<Block *>> latest_block;
};

struct BlockData {
  BlockData() : name("null"), type(usque::BlockType::NONE) {}
  BlockData(const std::string &name, const BlockType &type)
      : name(name), type(type) {}

  inline BlockData &at(const std::string &ref) { return children.at(ref); }
  inline const BlockData &at(const std::string &ref) const {
    return children.at(ref);
  }

  std::string name;
  BlockType type;
  std::map<std::string, BlockData> children;
  std::vector<std::array<std::chrono::steady_clock::time_point, 2>> time_points;
};

class Block {
public:
  Block(BlockData *data_ref)
      : data(data_ref), start_time(std::chrono::steady_clock::now()) {}
  ~Block() {
    data->time_points.push_back({start_time, std::chrono::steady_clock::now()});
    Profiler::get()->pop_block();
  }

  inline BlockData *get_data() { return data; }
  inline Block *get_this() { return this; }

protected:
  BlockData *data;
  std::chrono::steady_clock::time_point start_time;
};

Block Profiler::block(const std::string &name, const std::string &ref,
                      const BlockType &type) {
  const std::thread::id thread_id = std::this_thread::get_id();
  BlockData *data_ref = nullptr;
  std::map<std::thread::id, std::stack<Block *>>::iterator it;
  if ((it = latest_block.find(thread_id)) != latest_block.end() &&
      it->second.size() != 0) {
    if (it->second.top()->get_data()->children.find(ref) ==
        it->second.top()->get_data()->children.end()) {
      it->second.top()->get_data()->children.insert(
          {ref, BlockData(name, type)});
    }
    data_ref = &(it->second.top()->get_data()->at(ref));
  } else {
    if (data[thread_id].find(ref) == data[thread_id].end()) {
      data[thread_id].insert({ref, BlockData(name, type)});
    }
    data_ref = &(data[thread_id][ref]);
  }
  Block new_block(data_ref);
  latest_block[thread_id].push(new_block.get_this());
  return new_block;
}
void Profiler::pop_block() {
  const std::thread::id thread_id = std::this_thread::get_id();
  std::map<std::thread::id, std::stack<Block *>>::iterator it;
  if ((it = latest_block.find(thread_id)) != latest_block.end()) {
    it->second.pop();
  }
}

bool Profiler::dump(const std::string &file) const {
  FILE *out_file = fopen(file.c_str(), "w");
  if (out_file) {
    fprintf(out_file, "%s", dumps().c_str());
    fclose(out_file);
    return true;
  } else {
    fprintf(stderr, "Failed to open file \"%s\"", file.c_str());
    return false;
  }
}

std::string Profiler::dumps() const {
  std::stringstream ser;
  ser << "{";
  for (auto &it : data) {
    ser << "\"" << it.first << "\":";
    serialize_thread_data(ser, it.second);
  }
  ser << "}";
  std::string json = ser.str();
  std::string::size_type it;
  while ((it = json.find("],]")) != std::string::npos) {
    json.replace(it, 3, "]]");
  }
  while ((it = json.find("},}")) != std::string::npos) {
    json.replace(it, 3, "}}");
  }
  return json;
}
std::stringstream &Profiler::serialize_thread_data(
    std::stringstream &ser,
    const std::map<std::string, BlockData> &thread_data) const {
  ser << "{";
  for (auto &it : thread_data) {
    serialize_block_data(ser, it);
  }
  ser << "},";
  return ser;
}
std::stringstream &Profiler::serialize_block_data(
    std::stringstream &ser,
    const std::pair<std::string, BlockData> &data) const {
  ser << "\"" << data.second.name << "\":{";
  switch (data.second.type) {
  case usque::BlockType::FUNCTION:
    ser << "\"type\":\"FUNCTION\"";
    break;
  default:
    ser << "\"type\":null";
    break;
  }
  if (data.second.time_points.size() != 0) {
    ser << ",\"data\":[";
    for (auto &tp : data.second.time_points) {
      ser << "["
          << (std::chrono::duration_cast<std::chrono::microseconds>(
                  tp[0].time_since_epoch()))
                 .count()
          << ","
          << (std::chrono::duration_cast<std::chrono::microseconds>(
                  tp[1].time_since_epoch()))
                 .count()
          << ","
          << (std::chrono::duration_cast<std::chrono::microseconds>(tp[1] -
                                                                    tp[0]))
                 .count()
          << "],";
    }
    ser << "]";
  }
  if (data.second.children.size() != 0) {
    ser << ",\"blocks\":{";
    for (auto &sub : data.second.children) {
      serialize_block_data(ser, sub);
    }
    ser << "}";
  }
  ser << "},";
  return ser;
}

} // namespace usque

#endif // USQUE_USQUE_HPP_