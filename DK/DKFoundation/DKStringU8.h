//
//  File: DKStringU8.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include <stdarg.h>		// for va_list
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKSet.h"
#include "DKArray.h"
#include "DKStringUE.h"

namespace DKFoundation
{
	class DKData;
	/// a string class with UTF-8 encoded character string.
	class DKGL_API DKStringU8
	{
	public:
		using CharT = DKUniChar8;

		using CharacterSet = DKSet<CharT>;
		using IntegerArray = DKArray<int64_t>;
		using UnsignedIntegerArray = DKArray<uint64_t>;
		using RealNumberArray = DKArray<double>;
		using StringArray = DKArray<DKStringU8>;

		static const DKStringU8 empty;
		/// SystemEncoding returns 'DKStringEncoding::UTF8' always!
		static DKStringEncoding SystemEncoding();

		DKStringU8();
		DKStringU8(DKStringU8&& str);
		DKStringU8(const DKStringU8& str);
		DKStringU8(const DKUniChar8* str, size_t len = (size_t)-1);
		DKStringU8(const DKUniCharW* str, size_t len = (size_t)-1);
		explicit DKStringU8(const void* str, size_t bytes, DKStringEncoding e);
		explicit DKStringU8(DKUniCharW c);
		explicit DKStringU8(DKUniChar8 c);
		~DKStringU8();

		static DKStringU8 Format(const DKUniChar8* fmt, ...);
		static DKStringU8 Format(const DKUniCharW* fmt, ...);
		static DKStringU8 FormatV(const DKUniChar8* fmt, va_list v);
		static DKStringU8 FormatV(const DKUniCharW* fmt, va_list v);

		// append, set
		DKStringU8& Append(const DKStringU8& str);
		DKStringU8& Append(const DKUniChar8* str, size_t len = (size_t)-1);
		DKStringU8& Append(const DKUniCharW* str, size_t len = (size_t)-1);
		DKStringU8& Append(const void* str, size_t bytes, DKStringEncoding e);
		DKStringU8& SetValue(const DKStringU8& str);
		DKStringU8& SetValue(const DKUniChar8* str, size_t len = (size_t)-1);
		DKStringU8& SetValue(const DKUniCharW* str, size_t len = (size_t)-1);
		DKStringU8& SetValue(const void* str, size_t bytes, DKStringEncoding e);

		DKObject<DKData> Encode(DKStringEncoding e) const;

		size_t Length() const;		///< number of characters. (not bytes!)
		size_t Bytes() const;

		int Compare(const DKUniChar8* str) const;
		int Compare(const DKStringU8& str) const;
		int CompareNoCase(const DKUniChar8* str) const;
		int CompareNoCase(const DKStringU8& str) const;

		// assignment operators
		DKStringU8& operator = (DKStringU8&& str);
		DKStringU8& operator = (const DKStringU8& str);
		DKStringU8& operator = (const DKUniCharW* str);
		DKStringU8& operator = (const DKUniChar8* str);
		DKStringU8& operator = (DKUniCharW ch);
		DKStringU8& operator = (DKUniChar8 ch);

		// conversion operators
		operator const DKUniChar8* () const;

		// concatenation operators
		DKStringU8& operator += (const DKStringU8& str);
		DKStringU8& operator += (const DKUniCharW* str);
		DKStringU8& operator += (const DKUniChar8* str);
		DKStringU8& operator += (DKUniCharW ch);
		DKStringU8& operator += (DKUniChar8 ch);
		DKStringU8 operator + (const DKStringU8& str) const;
		DKStringU8 operator + (const DKUniCharW* str) const;
		DKStringU8 operator + (const DKUniChar8* str) const;
		DKStringU8 operator + (DKUniCharW c) const;
		DKStringU8 operator + (DKUniChar8 c) const;

		// comparison operators
		bool operator > (const DKStringU8& str) const			{return Compare(str) > 0;}
		bool operator > (const DKUniChar8* str) const			{return Compare(str) > 0;}
		bool operator < (const DKStringU8& str) const			{return Compare(str) < 0;}
		bool operator < (const DKUniChar8* str) const			{return Compare(str) < 0;}
		bool operator >= (const DKStringU8& str) const			{return Compare(str) >= 0;}
		bool operator >= (const DKUniChar8* str) const			{return Compare(str) >= 0;}
		bool operator <= (const DKStringU8& str) const			{return Compare(str) <= 0;}
		bool operator <= (const DKUniChar8* str) const			{return Compare(str) <= 0;}
		bool operator == (const DKStringU8& str) const			{return Compare(str) == 0;}
		bool operator == (const DKUniChar8* str) const			{return Compare(str) == 0;}
		bool operator != (const DKStringU8& str) const			{return Compare(str) != 0;}
		bool operator != (const DKUniChar8* str) const			{return Compare(str) != 0;}

		// convert numeric values (like atoi, atof)
		int64_t ToInteger() const;
		uint64_t ToUnsignedInteger() const;
		double ToRealNumber() const;

	private:
		CharT* stringData;
	};
}
