//
//  File: DKStringUE.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include <stdarg.h>		// for va_list
#include "../DKInclude.h"

namespace DKFoundation
{
	/// DKString encoder / decoder list
	enum class DKStringEncoding
	{
		UTF8,		///< UTF-8 without BOM
		UTF16,		///< UTF-16 with system byteorder without BOM
		UTF32,		///< UTF-32 with system byteorder without BOM
		UTF16BE,	///< UTF-16 BigEndian without BOM
		UTF16LE,	///< UTF-16 LittleEndian without BOM
		UTF32BE,	///< UTF-32 BigEndian without BOM
		UTF32LE,	///< UTF-32 LittleEndian without BOM
	};

#if defined(DKUNICODE_NATIVE)
	typedef char			DKUniChar8;
	typedef char16_t		DKUniChar16;
	typedef char32_t		DKUniChar32;
#elif defined(DKUNICHAR_UNSIGNED)
	typedef unsigned char	DKUniChar8;
	typedef unsigned short	DKUniChar16;
	typedef unsigned int	DKUniChar32;
#else
	typedef char			DKUniChar8;
	typedef short			DKUniChar16;
	typedef int				DKUniChar32;
#endif
	typedef wchar_t			DKUniCharW;
	
	static_assert(sizeof(char) != sizeof(wchar_t), "wchar_t must be greater than char");

	class DKBuffer;
	class DKStringU8;
	class DKStringW;

	DKGL_API DKStringEncoding DKStringWEncoding();
	DKGL_API DKStringEncoding DKStringU8Encoding();	///< always return DKStringEncodingUTF8
	DKGL_API const char* DKStringEncodingCanonicalName(DKStringEncoding e);

	DKGL_API void DKStringEncode(DKBuffer* output, const DKStringU8& input, DKStringEncoding e);
	DKGL_API void DKStringEncode(DKBuffer* output, const DKStringW& input, DKStringEncoding e);

	DKGL_API bool DKStringSetValue(DKStringU8& strOut, const void* p, size_t bytes, DKStringEncoding e);
	DKGL_API bool DKStringSetValue(DKStringW& strOut, const void* p, size_t bytes, DKStringEncoding e);

	DKGL_API void DKStringFormatV(DKStringU8& strOut, const DKUniChar8* fmt, va_list v);
	DKGL_API void DKStringFormatV(DKStringU8& strOut, const DKUniCharW* fmt, va_list v);
	DKGL_API void DKStringFormatV(DKStringW& strOut, const DKUniChar8* fmt, va_list v);
	DKGL_API void DKStringFormatV(DKStringW& strOut, const DKUniCharW* fmt, va_list v);

	DKGL_API bool DKStringSetValue(DKStringU8& strOut, const DKStringW& strIn);
	DKGL_API bool DKStringSetValue(DKStringW& strOut, const DKStringU8& strIn);

	DKGL_API bool DKStringSetValue(DKStringU8& strOut, const DKUniChar8* strIn, size_t len);
	DKGL_API bool DKStringSetValue(DKStringU8& strOut, const DKUniChar16* strIn, size_t len);
	DKGL_API bool DKStringSetValue(DKStringU8& strOut, const DKUniChar32* strIn, size_t len);
	DKGL_API bool DKStringSetValue(DKStringU8& strOut, const DKUniCharW* strIn, size_t len);
	DKGL_API bool DKStringSetValue(DKStringW& strOut, const DKUniChar8* strIn, size_t len);
	DKGL_API bool DKStringSetValue(DKStringW& strOut, const DKUniChar16* strIn, size_t len);
	DKGL_API bool DKStringSetValue(DKStringW& strOut, const DKUniChar32* strIn, size_t len);
	DKGL_API bool DKStringSetValue(DKStringW& strOut, const DKUniCharW* strIn, size_t len);
}
