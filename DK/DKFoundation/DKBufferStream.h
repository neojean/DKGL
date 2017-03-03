//
//  File: DKBufferStream.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKStream.h"
#include "DKBuffer.h"
#include "DKObject.h"
#include "DKDataStream.h"

namespace DKFoundation
{
	/// @brief Memory stream object using DKBuffer internally.
	/// using memory like stream object.
	class DKGL_API DKBufferStream : public DKDataStream
	{
	public:
		DKBufferStream(void);
		DKBufferStream(DKBuffer&);
		DKBufferStream(DKBuffer*);
		~DKBufferStream(void);

		Position SetPos(Position p);
		Position GetPos(void) const;
		Position RemainLength(void) const;
		Position TotalLength(void) const;

		size_t Read(void* p, size_t s);
		size_t Write(const void* p, size_t s);

		bool IsReadable(void) const {return true;}
		bool IsSeekable(void) const {return true;}
		bool IsWritable(void) const {return true;}

		void ResetStream(const void* p, size_t s);
		void ResetStream(DKBuffer*);

		/// internal storage object. (DKData type)
		DKData* Data(void);
		const DKData* Data(void) const;
		/// internal storage object. (DKBuffer type)
		DKBuffer* Buffer(void);
		const DKBuffer* Buffer(void) const;

	private:
		DKObject<DKBuffer> data;
		size_t offset;

		DKBufferStream(const DKBufferStream&);
		DKBufferStream& operator = (const DKBufferStream&);
	};
}
