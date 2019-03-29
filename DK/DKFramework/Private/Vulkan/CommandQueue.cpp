//
//  File: CommandQueue.cpp
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2016-2019 Hongtae Kim. All rights reserved.
//

#include "../GraphicsAPI.h"
#if DKGL_ENABLE_VULKAN
#include "CommandQueue.h"
#include "CommandBuffer.h"
#include "GraphicsDevice.h"
#include "SwapChain.h"

using namespace DKFramework;
using namespace DKFramework::Private::Vulkan;

CommandQueue::CommandQueue(DKGraphicsDevice* d, QueueFamily* f, VkQueue q)
	: device(d)
	, family(f)
	, queue(q)	
{
}

CommandQueue::~CommandQueue()
{
	vkQueueWaitIdle(queue);
	family->RecycleQueue(queue);
}

DKObject<DKCommandBuffer> CommandQueue::CreateCommandBuffer()
{
	GraphicsDevice* dev = (GraphicsDevice*)DKGraphicsDeviceInterface::Instance(device);

	VkCommandPoolCreateInfo cmdPoolCreateInfo = { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
	cmdPoolCreateInfo.queueFamilyIndex = this->family->FamilyIndex();
	cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	VkCommandPool commandPool = VK_NULL_HANDLE;
	VkResult err = vkCreateCommandPool(dev->device, &cmdPoolCreateInfo, dev->allocationCallbacks, &commandPool);
	if (err != VK_SUCCESS)
	{
		DKLogE("ERROR: vkCreateCommandPool failed: %s", VkResultCStr(err));
		return NULL;
	}

	DKObject<CommandBuffer> buffer = DKOBJECT_NEW CommandBuffer(commandPool, this);
	return buffer.SafeCast<DKCommandBuffer>();
}

DKObject<DKSwapChain> CommandQueue::CreateSwapChain(DKWindow* window)
{
	DKObject<SwapChain> swapChain = DKOBJECT_NEW SwapChain(this, window);
	if (swapChain->Setup())
	{
		if (!this->family->IsSupportPresentation())
		{
			GraphicsDevice* dev = (GraphicsDevice*)DKGraphicsDeviceInterface::Instance(device);
			VkPhysicalDevice physicalDevice = dev->physicalDevice;

			VkBool32 supported = VK_FALSE;
			VkResult err = dev->iproc.vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, this->family->FamilyIndex(), swapChain->surface, &supported);
			if (err != VK_SUCCESS)
			{
				DKLogE("ERROR: vkGetPhysicalDeviceSurfaceSupportKHR failed: %s", VkResultCStr(err));
				return NULL;
			}
			if (!supported)
			{
				DKLogE("ERROR: Vulkan WSI not supported with this queue family. Try to use other queue family!");
				return NULL;
			}
		}
		return swapChain.SafeCast<DKSwapChain>();
	}
	return NULL;
}

bool CommandQueue::Submit(const VkSubmitInfo* submits, uint32_t submitCount, DKOperation* callback)
{
	GraphicsDevice* dev = (GraphicsDevice*)DKGraphicsDeviceInterface::Instance(device);

	VkFence fence = VK_NULL_HANDLE;
	if (callback)
		fence = dev->GetFence();

	VkResult err = vkQueueSubmit(queue, submitCount, submits, fence);
	if (err != VK_SUCCESS)
	{
		DKLogE("ERROR: vkQueueSubmit failed: %s", VkResultCStr(err));
		DKASSERT(err == VK_SUCCESS);
	}
	if (fence)
		dev->AddFenceCompletionHandler(fence, callback);

	return err == VK_SUCCESS;
}

bool CommandQueue::WaitIdle()
{
	return vkQueueWaitIdle(queue) == VK_SUCCESS;
}

uint32_t CommandQueue::Type() const
{
	VkQueueFlags flags = family->properties.queueFlags;
	uint32_t type = 0;
	if (flags & VK_QUEUE_GRAPHICS_BIT)
		type = type | Graphics;
	if (flags & VK_QUEUE_COMPUTE_BIT)
		type = type | Compute;
	return type;
}

#endif //#if DKGL_ENABLE_VULKAN