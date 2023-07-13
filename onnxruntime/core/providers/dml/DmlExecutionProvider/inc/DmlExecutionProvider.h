// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once
interface IMLOperatorRegistry;

#include "core/common/status.h"
#include "core/framework/data_transfer.h"
#include "IWinmlExecutionProvider.h"
#include "core/providers/dml/DmlExecutionProvider/src/DmlBufferRegion.h"

namespace onnxruntime
{
    class IExecutionProvider;
    class IAllocator;
    class CustomRegistry;
    class InferenceSession;
    class KernelRegistry;
}

enum class AllocatorRoundingMode
{
    Disabled = 0,
    Enabled = 1,
};

namespace Dml
{
    std::unique_ptr<onnxruntime::IExecutionProvider> CreateExecutionProvider(
        IDMLDevice* dmlDevice,
        ID3D12CommandQueue* commandQueue,
        bool enableMetacommands = true);

    D3D12BufferRegion GetD3D12ResourceRegionFromAllocation(onnxruntime::IAllocator* allocator, void* opaquePointer, uint64_t size_in_bytes);
    void FlushContext(onnxruntime::IExecutionProvider* provider);
    void SetDefaultRoundingMode(onnxruntime::IExecutionProvider* provider, AllocatorRoundingMode roundingMode);
    void ReleaseCompletedReferences(onnxruntime::IExecutionProvider* provider);
    void DisableBfcAllocator(onnxruntime::IExecutionProvider* provider);

    onnxruntime::common::Status CopyTensor(
        onnxruntime::IExecutionProvider* provider,
        const onnxruntime::Tensor& src, onnxruntime::Tensor& dst
    );

    void* CreateGPUAllocationFromD3DResource(ID3D12Resource* pResource);
    void FreeGPUAllocation(void* ptr);

    void RegisterDmlOperators(IMLOperatorRegistry* registry);
    void RegisterCpuOperatorsAsDml(onnxruntime::KernelRegistry* registry);

} // namespace Dml
