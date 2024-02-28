// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <unordered_map>

#include "core/common/common.h"
#include "core/common/optional.h"
#include "core/platform/ort_mutex.h"
#include "core/providers/cuda/cuda_pch.h"

namespace onnxruntime {

using CudaGraphAnnotation_t = int;
using CudaGraphAnnotationOptional_t = optional<CudaGraphAnnotation_t>;

constexpr CudaGraphAnnotation_t kCudaGraphAnnotationSkip = -1;

struct CUDAGraph {
  CUDAGraph(){};
  CUDAGraph(cudaStream_t stream);
  ~CUDAGraph();

  void SetStream(cudaStream_t stream);
  void SetGraphAnnotationId(CudaGraphAnnotationOptional_t cuda_graph_annotation_id);

  void CaptureBegin();
  void CaptureEnd();
  Status Replay();

  void Reset();

  bool IsAdditionalGraphCaptured(CudaGraphAnnotation_t cuda_graph_annotation_id) const;
  bool IsGraphCaptureAllowedOnRun() const;

 private:
  cudaGraph_t graph_ = NULL;
  cudaGraphExec_t graph_exec_ = NULL;

  bool has_graph_ = false;
  bool has_graph_exec_ = false;

  cudaGraph_t additional_graph_ = NULL;
  std::unordered_map<CudaGraphAnnotation_t, cudaGraphExec_t> graph_exec_map_;
  CudaGraphAnnotationOptional_t cuda_graph_annotation_id_;
  bool has_additional_graph_ = false;

  cudaStream_t stream_ = nullptr;  // Does not own the stream
};

}  // namespace onnxruntime
