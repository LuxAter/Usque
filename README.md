# Usque

> Lightweight extendable profiling library

Usque is a single header file that provides easy to use profiling tools, for C++ projects. Usage of Usque is very simple, and does not greatly impact the preformance of the code.

```c++
#define PROFILE_PRETTY_FUNCTION
#define PROFILER
#include <usque/usque.hpp>

float foo(int i) {
  PROFILE_FUNC();
  float avg = 0.0f;
  for (int j = 0; j < i; ++j) {
    avg += rand() / (float)(i);
  }
  return avg;
}

int main(int argc, char const *argv[]) {
  PROFILE_FILE("simple.txt");
  PROFILE_FUNC();
  for (int i = 0; i < 1000; i += 10) {
    PROFILE_BLOCK("frame");
    foo(i);
  }
  PROFILE_SAVE()
  return 0;
}
```

To allow for beter preformance in release builds, all profiling functions are disabled if the preprocessor ``PROFILER`` has not been defined. To enable profiling simply define this somewhere prior to including the header file, or in the compile commands.

Usque has been constructed to use minimal memory and cause as little impact on preformance as possible. This design strategy means that without additional define flags, Usque does not retain any the profiling data, everything is dumped to the output file at regular intervals.

Durring profiling data is available with the following structure:

```json
{
  "totalData": [NUMBER, NUMBER],
  "totalDuration": NUMBER,
  "threads": {
    12345: {
      "threadId": "0xabc",
      "rawData": [[NUMBER, NUMBER]],
      "durations": [NUMBER],
      "averageDuration": NUMBER,
      "maxDuration": NUMBER,
      "minDuration": NUMBER,
      "startTimePoint": NUMBER,
      "stopTimePoint": NUMBER,
      "instances": NUMBER,
      "blocks": {
        54321: {
          "name": "STRING",
          "rawData": [[NUMBER, NUMBER]],
          "durations": [NUMBER],
          "averageDuration": NUMBER,
          "maxDuration": NUMBER,
          "minDuration": NUMBER,
          "startTimePoint": NUMBER,
          "stopTimePoint": NUMBER,
          "instances": NUMBER,
          "blocks": { ...}
        }
      }
    }
  }
}
```