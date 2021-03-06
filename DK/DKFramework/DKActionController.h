//
//  File: DKActionController.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKFoundation.h"
#include "DKModel.h"

namespace DKFramework
{
	/// @brief Action control node class inherited from DKModel.
	/// @details 
	/// Useful for dynamics control of DKDynamicsScene. (called by DKDynamicsScene)
	/// @note
	///    Use DKAnimationController for individual node animation.
	class DKActionController : public DKModel
	{
	public:
		DKActionController() : DKModel(TypeAction) {}
		virtual ~DKActionController() {}
		virtual void Update(double timeDelta, DKTimeTick tick) = 0;
	};
}
