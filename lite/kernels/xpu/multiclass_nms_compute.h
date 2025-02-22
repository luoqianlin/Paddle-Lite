// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
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

#include <vector>
#include "lite/core/kernel.h"

namespace paddle {
namespace lite {
namespace kernels {
namespace xpu {

class MulticlassNmsCompute
    : public KernelLite<TARGET(kXPU), PRECISION(kFloat)> {
 public:
  using param_t = operators::MulticlassNmsParam;

  virtual void Run();

  void PrepareForRun() override;

  virtual ~MulticlassNmsCompute() = default;

 private:
  std::vector<float> outs_vec_;
  std::vector<int> out_index_vec_;
  float custom_config_score_threshod_{0.0};
};

}  // namespace xpu
}  // namespace kernels
}  // namespace lite
}  // namespace paddle
