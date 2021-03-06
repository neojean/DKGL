//
//  File: DKShaderBindingSet.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"
#include "DKGpuBuffer.h"
#include "DKTexture.h"
#include "DKSampler.h"
#include "DKShader.h"

namespace DKFramework
{
    struct DKShaderBinding
    {
        uint32_t binding;
        DKShader::DescriptorType type;
        uint32_t arrayLength; // array size or bytes of inline buffer
        DKSamplerState* immutableSamplers;
    };
    struct DKShaderBindingSetLayout
    {
        using Binding = DKShaderBinding;
        DKArray<Binding> bindings;
    };
    /**
    @brief Shader Resource Binding Unit, binding descriptors for single set
    */
    class DKShaderBindingSet
    {
    public:
        virtual ~DKShaderBindingSet() {}

        struct BufferInfo
        {
            DKGpuBuffer* buffer;
            uint64_t offset;
            uint64_t length;
        };

        // bind buffers
        virtual void SetBuffer(uint32_t binding, DKGpuBuffer*, uint64_t offset, uint64_t length) = 0;
        virtual void SetBufferArray(uint32_t binding, uint32_t numBuffers, BufferInfo*) = 0;

        // bind textures
        virtual void SetTexture(uint32_t binding, DKTexture*) = 0;
        virtual void SetTextureArray(uint32_t binding, uint32_t numTextures, DKTexture**) = 0;

        // bind samplers
        virtual void SetSamplerState(uint32_t binding, DKSamplerState*) = 0;
        virtual void SetSamplerStateArray(uint32_t binding, uint32_t numSamplers, DKSamplerState**) = 0;
    };
}
