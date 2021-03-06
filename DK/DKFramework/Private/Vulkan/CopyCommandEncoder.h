//
//  File: CopyCommandEncoder.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2016-2019 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../GraphicsAPI.h"
#if DKGL_ENABLE_VULKAN
#include <vulkan/vulkan.h>

#include "../../DKCopyCommandEncoder.h"
#include "CommandBuffer.h"

namespace DKFramework::Private::Vulkan
{
    class CopyCommandEncoder : public DKCopyCommandEncoder
    {
        class Encoder;
        struct EncodingState
        {
            Encoder* encoder;
        };
        using EncoderCommand = DKFunctionSignature<void(VkCommandBuffer, EncodingState&)>;
        class Encoder : public CommandEncoder
        {
        public:
            Encoder(class CommandBuffer*);
            ~Encoder();

            bool Encode(VkCommandBuffer) override;

            // Retain ownership of all encoded objects
            DKArray<DKObject<DKGpuBuffer>> buffers;
            DKArray<DKObject<DKTexture>> textures;
            DKArray<DKObject<DKGpuEvent>> events;
            DKArray<DKObject<DKGpuSemaphore>> semaphores;

            class CommandBuffer* commandBuffer;
            DKArray<DKObject<EncoderCommand>> commands;
            DKArray<DKObject<EncoderCommand>> setupCommands;
            DKArray<DKObject<EncoderCommand>> cleanupCommands;
        };
        DKObject<Encoder> encoder;

    public:
        CopyCommandEncoder(class CommandBuffer*);

        void EndEncoding() override;
        bool IsCompleted() const override { return encoder == nullptr; }
        DKCommandBuffer* CommandBuffer() override { return commandBuffer; }

        void WaitEvent(const DKGpuEvent*) override;
        void SignalEvent(const DKGpuEvent*) override;
        void WaitSemaphoreValue(const DKGpuSemaphore*, uint64_t) override;
        void SignalSemaphoreValue(const DKGpuSemaphore*, uint64_t) override;


        void CopyFromBufferToBuffer(const DKGpuBuffer* src,
                                    size_t srcOffset,
                                    const DKGpuBuffer* dst,
                                    size_t dstOffset,
                                    size_t size) override;

        void CopyFromBufferToTexture(const DKGpuBuffer* src,
                                     const BufferImageOrigin& srcOffset,
                                     const DKTexture* dst,
                                     const TextureOrigin& dstOffset,
                                     const Size& size) override;
        void CopyFromTextureToBuffer(const DKTexture* src,
                                     const TextureOrigin& srcOffset,
                                     const DKGpuBuffer* dst, 
                                     const BufferImageOrigin& dstOffset,
                                     const Size& size) override;

        void CopyFromTextureToTexture(const DKTexture* src,
                                      const TextureOrigin& srcOffset,
                                      const DKTexture* dst,
                                      const TextureOrigin& dstOffset,
                                      const Size& size) override;

        void FillBuffer(const DKGpuBuffer* buffer,
                        size_t offset,
                        size_t length,
                        uint8_t value) override;

        DKObject<class CommandBuffer> commandBuffer;

    private:
        static void SetupSubresource(const TextureOrigin& origin,
                                     uint32_t layerCount,
                                     DKPixelFormat pixelFormat,
                                     VkImageSubresourceLayers& subresource);
        static void SetupSubresource(const TextureOrigin& origin,
                                     uint32_t layerCount,
                                     uint32_t levelCount,
                                     DKPixelFormat pixelFormat,
                                     VkImageSubresourceRange& subresource);
    };
}
#endif //#if DKGL_ENABLE_VULKAN
