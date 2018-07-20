//
//  File: DKUtils.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKArray.h"
#include "DKMap.h"
#include "DKString.h"


namespace DKFoundation
{
	DKGL_API uint32_t DKRandom(void); ///< generate 32bit random number
	DKGL_API DKString DKTemporaryDirectory(void); ///< temporary directory provided by OS
	DKGL_API DKArray<DKString> DKProcessArguments(void); ///< process arguments
	DKGL_API DKMap<DKString, DKString> DKProcessEnvironments(void); ///< process environment variables
	DKGL_API uint32_t DKNumberOfProcessors(void); ///< return number of logical processors
	DKGL_API uint32_t DKNumberOfCpuCores(void);	///< return number of physical CPU cores
}
