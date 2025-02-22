// Copyright (c) 2023 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * This file implements an lightweight alternative for glog, which is from
 * PaddleLite
 */

#pragma once

#include <assert.h>
#include <time.h>
#include "adnn/core/macros.h"
#if ADNN_OS_WINDOWS
#define NOMINMAX  // msvc max/min macro conflict with std::min/max
#include <windows.h>
#undef min
#undef max
extern struct timeval;
static int gettimeofday(struct timeval* tp, void* tzp) {
  LARGE_INTEGER now, freq;
  QueryPerformanceCounter(&now);
  QueryPerformanceFrequency(&freq);
  tp->tv_sec = now.QuadPart / freq.QuadPart;
  tp->tv_usec = (now.QuadPart % freq.QuadPart) * 1000000 / freq.QuadPart;
  return (0);
}
#else
#include <sys/time.h>
#include <sys/types.h>
#endif

#include <cstdlib>
#include <cstring>
#include <sstream>
#include <string>

#if ADNN_OS_ANDROID
#include <android/log.h>
// Android log macors
#define ANDROID_LOG_TAG "ADNN"
#define ANDROID_LOG_I(msg) \
  __android_log_print(ANDROID_LOG_INFO, ANDROID_LOG_TAG, "%s", msg)
#define ANDROID_LOG_W(msg) \
  __android_log_print(ANDROID_LOG_WARN, ANDROID_LOG_TAG, "%s", msg)
#define ANDROID_LOG_F(msg) \
  __android_log_print(ANDROID_LOG_FATAL, ANDROID_LOG_TAG, "%s", msg)
#endif

// ADNN_LOG()
#define ADNN_LOG(status) ADNN_LOG_##status.stream()
#define ADNN_LOG_INFO adnn::LogMessage(__FILE__, __FUNCTION__, __LINE__, "I")
#define ADNN_LOG_ERROR ADNN_LOG_INFO
#define ADNN_LOG_WARNING adnn::LogMessage(__FILE__, __FUNCTION__, __LINE__, "W")
#define ADNN_LOG_FATAL adnn::LogMessageFatal(__FILE__, __FUNCTION__, __LINE__)

// ADNN_VLOG()
#define ADNN_VLOG(level) \
  adnn::VLogMessage(__FILE__, __FUNCTION__, __LINE__, level).stream()

// ADNN_CHECK()
// clang-format off
#define ADNN_CHECK(x) if (!(x)) adnn::LogMessageFatal(__FILE__, __FUNCTION__, __LINE__).stream() << "Check failed: " #x << ": " // NOLINT(*)
#define _ADNN_CHECK_BINARY(x, cmp, y) ADNN_CHECK((x cmp y)) << (x) << "!" #cmp << (y) << " " // NOLINT(*)

// clang-format on
#define ADNN_CHECK_EQ(x, y) _ADNN_CHECK_BINARY(x, ==, y)
#define ADNN_CHECK_NE(x, y) _ADNN_CHECK_BINARY(x, !=, y)
#define ADNN_CHECK_LT(x, y) _ADNN_CHECK_BINARY(x, <, y)
#define ADNN_CHECK_LE(x, y) _ADNN_CHECK_BINARY(x, <=, y)
#define ADNN_CHECK_GT(x, y) _ADNN_CHECK_BINARY(x, >, y)
#define ADNN_CHECK_GE(x, y) _ADNN_CHECK_BINARY(x, >=, y)

namespace adnn {

struct Exception : public std::exception {
  const std::string exception_prefix = "ADNN C++ Exception: \n";
  std::string message;
  explicit Exception(const char* detail) {
    message = exception_prefix + std::string(detail);
  }
  const char* what() const noexcept { return message.c_str(); }
};

class LogMessage {
 public:
  LogMessage(const char* file,
             const char* func,
             int lineno,
             const char* level = "I");
  ~LogMessage();

  std::ostream& stream() { return log_stream_; }

 protected:
  std::stringstream log_stream_;
  std::string level_;

  LogMessage(const LogMessage&) = delete;
  void operator=(const LogMessage&) = delete;
};

class LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file,
                  const char* func,
                  int lineno,
                  const char* level = "F")
      : LogMessage(file, func, lineno, level) {}
  ~LogMessageFatal() noexcept(false);
};

class VLogMessage {
 public:
  VLogMessage(const char* file,
              const char* func,
              int lineno,
              const int32_t level_int = 0);
  ~VLogMessage();

  std::ostream& stream() { return log_stream_; }

 protected:
  std::stringstream log_stream_;
  int32_t GLOG_v_int;
  int32_t level_int;

  VLogMessage(const VLogMessage&) = delete;
  void operator=(const VLogMessage&) = delete;
};

}  // namespace adnn
