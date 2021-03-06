//
//  File: DKThread.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2016 Hongtae Kim. All rights reserved.
//

#pragma once
#include "../DKInclude.h"
#include "DKObject.h"
#include "DKOperation.h"

#ifdef _WIN32
#undef Yield	// see WinBase.h
#endif

namespace DKFoundation
{
	/// Creating new thread with DKOperation. Because of threads run in parallel,
	/// each thread ownership will be shared with caller and callee.
	/// If you abandon ownership of thread, thread will be detached automatically.
	class DKGL_API DKThread
	{
	public:
		typedef uintptr_t ThreadId;

		/// waiting for join.
		void WaitTerminate() const;
		/// get thread-id (system thread-id)
		/// GetCurrentThreadId() for Win32, pthread_self() for pthread.
		ThreadId Id() const;
		/// determines thread is running(or sleeping).
		bool IsAlive() const;

		/// Set thread priority (from 0.0 to 1.0, 1.0 is highest priority)
		bool SetPriority(double p);
		/// Get thread priority
		double Priority() const;

		/// find thread specified by id.
		static DKObject<DKThread> FindThread(ThreadId id);
		/// get current thread as DKThread object.
		static DKObject<DKThread> CurrentThread();
		/// get current thread-id
		static ThreadId CurrentThreadId();
		/// yeild CPU
		static void Yield();
		/// sleep current thread.
		static void Sleep(double d);

		/// create new thread with DKOperation and run.
		static DKObject<DKThread> Create(const DKOperation* op, size_t stackSize = 0);

		/// a constant of invalid-thread id.
		static const ThreadId invalidId;

	private:
		DKThread();
		~DKThread();

		DKThread(const DKThread&) = delete;
		DKThread& operator = (const DKThread&) = delete;

		mutable ThreadId threadId;
		friend class DKObject<DKThread>;
	};
}
