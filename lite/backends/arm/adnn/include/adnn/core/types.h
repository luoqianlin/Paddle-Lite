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

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <sys/cdefs.h>
#include <string>
#include "adnn/core/bfloat16.h"
#include "adnn/core/float16.h"
#include "adnn/core/macros.h"

namespace adnn {

/**
 * Result code.
 */
typedef enum {
  SUCCESS = 0,
  OUT_OF_MEMORY,
  INVALID_PARAMETER,
  FEATURE_NOT_SUPPORTED
} Status;

/**
 * Power mode.
 */
typedef enum {
  /**
   * Bind the worker threads to big clusters, use the max cores if the number of
   * the requested worker threads exceed the number of cores.
   */
  HIGH = 0,
  /**
   * Bind the worker threads to little clusters, use the max cores if the number
   * of the requested worker threads exceed the number of cores.
   */
  LOW,
  /**
   * Bind the worker threads to big and little clusters, use the max cores if
   * the number of the requested worker threads exceed the number of cores.
   */
  FULL,
  /**
   * No bind depends on the scheduling policy of the system.
   */
  NO_BIND
} PowerMode;

/**
 * CPU arch.
 */
typedef enum {
  UNKOWN = -1,
  /* ARM Cortex-35. */
  CORTEX_A35,
  /* ARM Cortex-53. */
  CORTEX_A53,
  /* ARM Cortex-55. */
  CORTEX_A55,
  /* ARM Cortex-57. */
  CORTEX_A57,
  /* ARM Cortex-72. */
  CORTEX_A72,
  /* ARM Cortex-73. */
  CORTEX_A73,
  /* ARM Cortex-75. */
  CORTEX_A75,
  /* ARM Cortex-76. */
  CORTEX_A76,
  /* ARM Cortex-77. */
  CORTEX_A77,
  /* ARM Cortex-78. */
  CORTEX_A78,
  /* ARM Cortex-X1. */
  CORTEX_X1,
  /* ARM Cortex-X2. */
  CORTEX_X2,
  /* ARM Cortex-A510. */
  CORTEX_A510,
  /* ARM Cortex-A710. */
  CORTEX_A710,
  /* Qualcomm Kryo 485 Gold Prime. */
  KRYO_485_GOLD_PRIME,
  /* Qualcomm Kryo 485 Gold. */
  KRYO_485_GOLD,
  /* Qualcomm Kryo 485 Silver. */
  KRYO_485_SILVER,
  /* Apple A-series processors. */
  APPLE,
} CPUArch;

/**
 * The key-value pair in device_setparam(...) and context_setparam(...) is used
 * for updating the parameters of the device and context.
 */
typedef enum {
  /**
   * Max number of threads of thread pool, dtype of value is int32_t.
   */
  DEVICE_MAX_THREAD_NUM = 0,
  /**
   * The power mode, dtype of value is int32_t.
   */
  DEVICE_POWER_MODE,
  /**
   * The cpu uarch, dtype of value is int32_t.
   */
  DEVICE_ARCH,
  /**
   * Does support Arm float16 instruction set on the current platform, dtype of
   * value is bool, readonly.
   */
  DEVICE_SUPPORT_ARM_FP16,
  /**
 * Does support Arm bfloat16 instruction set on the current platform, dtype of
 * value is bool, readonly.
 */
  DEVICE_SUPPORT_ARM_BF16,
  /**
   * Does Support Arm dotprod instruction set on the current platform, dtype of
   * value is bool, readonly.
   */
  DEVICE_SUPPORT_ARM_DOTPROD,
  /**
   * Does support Arm sve2 instruction set on on the current platform, dtype of
   * value is bool, readonly.
   */
  DEVICE_SUPPORT_ARM_SVE2,
  /**
   * Does support Arm sve2+i8mm instruction set on on the current platform,
   * dtype of
   * value is bool, readonly.
   */
  DEVICE_SUPPORT_ARM_SVE2_I8MM,
  /**
 * Does support Arm sve2+f32mm instruction set on on the current platform, dtype
 * of
 * value is bool, readonly.
 */
  DEVICE_SUPPORT_ARM_SVE2_F32MM,
  /**
   * The number of threads used in the current context, dtype of value is
   * int32_t.
   */
  CONTEXT_WORK_THREAD_NUM,
  /**
   * Enable/disable using Arm float16 instruction set, dtype of value is bool.
   */
  CONTEXT_ENABLE_ARM_FP16,
  /**
   * Enable/disable using Arm bfloat16 instruction set, dtype of value is bool.
   */
  CONTEXT_ENABLE_ARM_BF16,
  /**
   * Enable/disable using Arm dotprod instruction set, dtype of value is bool.
   */
  CONTEXT_ENABLE_ARM_DOTPROD,
  /**
   * Enable/disable using Arm sve2 instruction set, dtype of value is bool.
   */
  CONTEXT_ENABLE_ARM_SVE2,
  /**
   * Enable/disable using Arm sve2+i8mm instruction set, dtype of value is bool.
   */
  CONTEXT_ENABLE_ARM_SVE2_I8MM,
  /**
   * Enable/disable using Arm sve2+f32mm instruction set, dtype of value is
   * bool.
   */
  CONTEXT_ENABLE_ARM_SVE2_F32MM,
  /**
   * The pointer of workspace, dtype of value is void*.
   */
  CONTEXT_WORK_PACE_DATA,
  /**
   * The bytes of workspace, dtype of value is size_t.
   */
  CONTEXT_WORK_PACE_SIZE,
} ParamKey;

typedef union {
  bool b;
  int32_t i32;
  int64_t i64;
  float f32;
  double f64;
  size_t szt;
  void* ptr;
} ParamValue;

/**
 * Custom callback functions.
 */
typedef void* (*DEVICE_OPEN_CALLBACK_FUNC)();
typedef void (*DEVICE_CLOSE_CALLBACK_FUNC)(void* device);
typedef Status (*DEVICE_SETPARAM_CALLBACK_FUNC)(void* device,
                                                ParamKey key,
                                                ParamValue value);
typedef Status (*DEVICE_GETPARAM_CALLBACK_FUNC)(void* device,
                                                ParamKey key,
                                                ParamValue* value);
typedef void* (*CONTEXT_CREATE_CALLBACK_FUNC)(void* device);
typedef void (*CONTEXT_DESTROY_CALLBACK_FUNC)(void* context);
typedef Status (*CONTEXT_SETPRARM_CALLBACK_FUNC)(void* device,
                                                 ParamKey key,
                                                 ParamValue value);
typedef Status (*CONTEXT_GETPRARM_CALLBACK_FUNC)(void* device,
                                                 ParamKey key,
                                                 ParamValue* value);
typedef void* (*MEMORY_ALLOC_CALLBACK_FUNC)(void* context, size_t size);
typedef void (*MEMORY_FREE_CALLBACK_FUNC)(void* context, void* ptr);
typedef void* (*MEMORY_ALIGNED_ALLOC_CALLBACK_FUNC)(void* context,
                                                    size_t size,
                                                    size_t alignment);
typedef void (*MEMORY_ALIGNED_FREE_CALLBACK_FUNC)(void* context, void* ptr);
typedef struct {
  DEVICE_OPEN_CALLBACK_FUNC device_open;
  DEVICE_CLOSE_CALLBACK_FUNC device_close;
  DEVICE_SETPARAM_CALLBACK_FUNC device_setparam;
  DEVICE_GETPARAM_CALLBACK_FUNC device_getparam;
  CONTEXT_CREATE_CALLBACK_FUNC context_create;
  CONTEXT_DESTROY_CALLBACK_FUNC context_destroy;
  CONTEXT_SETPRARM_CALLBACK_FUNC context_setparam;
  CONTEXT_GETPRARM_CALLBACK_FUNC context_getparam;
  MEMORY_ALLOC_CALLBACK_FUNC memory_alloc;
  MEMORY_FREE_CALLBACK_FUNC memory_free;
  MEMORY_ALIGNED_ALLOC_CALLBACK_FUNC memory_aligned_alloc;
  MEMORY_ALIGNED_FREE_CALLBACK_FUNC memory_aligned_free;
} Callback;

/**
 * An opaque type for Device.
 */
typedef struct Device Device;

/**
 * An opaque type for Context.
 */
typedef struct Context Context;

/* Convert types to readable string */
std::string status_to_string(Status type);
std::string power_mode_to_string(PowerMode type);
std::string cpu_arch_to_string(CPUArch type);
std::string param_key_to_string(ParamKey type);

}  // namespace adnn
