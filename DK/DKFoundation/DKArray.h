//
//  File: DKArray.h
//  Author: Hongtae Kim (tiff2766@gmail.com)
//
//  Copyright (c) 2004-2017 Hongtae Kim. All rights reserved.
//

#pragma once
#include <initializer_list>
#include "../DKInclude.h"
#include "DKTypeTraits.h"
#include "DKDummyLock.h"
#include "DKCriticalSection.h"
#include "DKMemory.h"
#include "DKFunction.h"
#include "DKStaticArray.h"

namespace DKFoundation
{
	/// sort function for ascending order
	template <typename T>
	bool DKArraySortAscending(const T& lhs, const T& rhs) // ascending, smaller first
	{
		return lhs < rhs;
	}
	/// sort function for descending order
	template <typename T>
	bool DKArraySortDescending(const T& lhs, const T& rhs) // descending, bigger first
	{
		return lhs > rhs;
	}
	/// DKArray element comparision function
	template <typename T>
	bool DKArrayCompareEqual(const T& lhs, const T& rhs) // equal (comparision)
	{
		return lhs == rhs;
	}

	/**
	 @brief basic array class.
	 If you put a lock into template parameter LOCK, It provides thread-safe
	 insertion, deletion.
	 @note
	  following types are locking class:
	  DKSpinLock, DKLock, DKMutex, DKCondition, DKSharedLock \n
	  You can implement your own locking class, see DKDummyLock.
	
	 You can also lock array object from outside to modify element directly.
	
	 @code
		{
			typename MyArrayType::CriticalSection section(array.lock);	// lock with critical-section
			array[x] = .... // do something with array[]
		}	// auto-unlock by critical-section end
	 @endcode
	
	 @note
	  When two objects has same VALUE type and different LOCK type,
	  ONLY Add(), Insert() can be used.
	
	 @note
	  If you have to obtain element's pointer or reference, beware of thread-safety.
	  CopyValue() function is always thread-safe. 
	  (assume that template parameter LOCK is not DKDummyLock)
	 */
	template <typename VALUE, typename LOCK = DKDummyLock, typename ALLOC = DKMemoryDefaultAllocator>
	class DKArray
	{
		enum {InitialSize = 4,};
	public:
		typedef LOCK					Lock;
		typedef DKCriticalSection<Lock>	CriticalSection;
		typedef size_t					Index;
		typedef DKTypeTraits<VALUE>		ValueTraits;
		typedef ALLOC					Allocator;

		constexpr static size_t NodeSize()	{ return sizeof(VALUE); }

		enum : Index { IndexNotFound = ~Index(0) };

		/// lock is public. (object can be locked from outside, to use modify element directly.)
		/// in this case, You can use VALUE* casting-operator and CountNoLock() only.
		Lock	lock;

		typedef DKArrayRBIterator<DKArray, VALUE&>				RBIterator;					///< implemented for range-based loop
		typedef DKArrayRBIterator<const DKArray, const VALUE&>	ConstRBIterator;			///< implemented for range-based loop
		RBIterator begin()				{return RBIterator(*this, 0);}					///< implemented for range-based loop
		ConstRBIterator begin() const	{return ConstRBIterator(*this, 0);}				///< implemented for range-based loop
		RBIterator end()				{return RBIterator(*this, this->Count());}		///< implemented for range-based loop
		ConstRBIterator end() const		{return ConstRBIterator(*this, this->Count());}	///< implemented for range-based loop

		DKArray()
			: data(NULL), count(0), capacity(0)
		{
		}
		DKArray(const VALUE* v, size_t c)
			: data(NULL), count(0), capacity(0)
		{
			ReserveNL(c);
			Add(v, c);
		}
		DKArray(const VALUE& v, size_t c)
			: data(NULL), count(0), capacity(0)
		{
			ReserveNL(c);
			Add(v, c);
		}
		DKArray(DKArray&& v)
			: data(NULL), count(0), capacity(0)
		{
			data = v.data;
			count = v.count;
			capacity = v.capacity;
			v.data = NULL;
			v.count = 0;
			v.capacity = 0;
		}
		DKArray(const DKArray& v)
			: data(NULL), count(0), capacity(0)
		{
			CriticalSection guard(v.lock);
			ReserveNL(v.count);
			Add((const VALUE*)v, v.count);
		}
		template <typename ...Args>
		DKArray(const DKArray<VALUE, Args...>& v)
			: data(NULL), count(0), capacity(0)
		{
			typename DKArray<VALUE, Args...>::CriticalSection guard(v.lock);
			ReserveNL(v.count);
			Add((const VALUE*)v, v.count);
		}
		DKArray(std::initializer_list<VALUE> il)
			: data(NULL), count(0), capacity(0)
		{
			if (il.size() > 0)
			{
				ReserveNL(il.size());
				for (const VALUE& v : il)
				{
					new(std::addressof(data[count])) VALUE(v);
					count++;
				}
			}
		}
		~DKArray()
		{
			Clear();

			if (data)
				Allocator::Free(data);
		}
		bool IsEmpty() const
		{
			CriticalSection guard(lock);
			return count == 0;
		}
		/// append other array's elements to tail.
		template <typename ...Args>
		Index Add(const DKArray<VALUE, Args...>& value)
		{
			typename DKArray<VALUE, Args...>::CriticalSection guard(value.lock);
			return Add((const VALUE*)value, value.count);
		}
		/// append one item to tail.
		Index Add(const VALUE& value)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(1);
			new(std::addressof(data[count])) VALUE(value);
			return count++;
		}
		/// move one item into array's tail
		Index Add(VALUE&& value)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(1);
			new(std::addressof(data[count])) VALUE(static_cast<VALUE&&>(value));
			return count++;
		}
		/// append 's' length of value to tail.
		Index Add(const VALUE* value, size_t s)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			for (Index i = 0; i < s; i++)
				new(std::addressof(data[count+i])) VALUE(value[i]);
			count += s;
			return count - s;
		}
		/// append value to tail 's' times. (value x s)
		Index Add(const VALUE& value, size_t s)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			for (Index i = 0; i < s; i++)
				new(std::addressof(data[count+i])) VALUE(value);
			count += s;
			return count - s;
		}
		/// append initializer-list items to tail.
		Index Add(std::initializer_list<VALUE> il)
		{
			size_t s = il.size();
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			for (const VALUE& v : il)
			{
				new(std::addressof(data[count])) VALUE(v);
				count++;
			}
			return count - s;
		}
		/// insert array's elements into position 'pos'.
		template <typename ...Args>
		Index Insert(const DKArray<VALUE, Args...>& value, Index pos)
		{
			typename DKArray<VALUE, Args...>::CriticalSection guard(value.lock);
			return Insert((const VALUE*)value, value.count, pos);
		}
		/// insert one value into position 'pos'.
		Index Insert(const VALUE& value, Index pos)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(1);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove(std::addressof(data[pos+1]), std::addressof(data[pos]), sizeof(VALUE) * (count - pos));
			new(std::addressof(data[pos])) VALUE(value);
			count++;
			return pos;
		}
		/// move one value into position 'pos'.
		Index Insert(VALUE&& value, Index pos)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(1);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove(std::addressof(data[pos+1]), std::addressof(data[pos]), sizeof(VALUE) * (count - pos));
			new(std::addressof(data[pos])) VALUE(static_cast<VALUE&&>(value));
			count++;
			return pos;
		}
		/// insert 's' length of value into position 'pos'.
		Index Insert(const VALUE* value, size_t s, Index pos)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove(std::addressof(data[pos+s]), std::addressof(data[pos]), sizeof(VALUE) * (count - pos));
			for (Index i = 0; i < s; i++)
				new(std::addressof(data[pos+i])) VALUE(value[i]);
			count += s;
			return pos;
		}
		/// insert value 's' times into position 'pos'.
		Index Insert(const VALUE& value, size_t s, Index pos)
		{
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove(std::addressof(data[pos+s]), std::addressof(data[pos]), sizeof(VALUE) * (count - pos));
			for (Index i = 0; i < s; i++)
				new(std::addressof(data[pos+i])) VALUE(value);
			count += s;
			return pos;
		}
		/// insert initializer-list into position 'pos'.
		Index Insert(std::initializer_list<VALUE> il, Index pos)
		{
			size_t s = il.size();
			CriticalSection guard(lock);
			ReserveItemCapsNL(s);
			if (pos > count)
				pos = count;
			if (pos < count)
				memmove(std::addressof(data[pos+s]), std::addressof(data[pos]), sizeof(VALUE) * (count - pos));
			for (const VALUE& v : il)
			{
				new(std::addressof(data[pos])) VALUE(v);
				pos++;
			}
			count += s;
			return pos - s;
		}
		/// remove one element at pos.
		size_t Remove(Index pos)
		{
			CriticalSection guard(lock);
			if (pos < count)
			{
				data[pos].~VALUE();
				if (count - pos > 1)
					memmove(std::addressof(data[pos]), std::addressof(data[pos+1]), sizeof(VALUE) * (count-pos-1));
				count--;
			}
			return count;
		}
		/// remove 'c' items at pos. (c = count)
		size_t Remove(Index pos, size_t c)
		{
			CriticalSection guard(lock);
			if (pos < count)
			{
				Index i = 0;
				for (; i < count - pos && i < c; i++)
					data[pos+i].~VALUE();
				if (i > 0)
					memmove(std::addressof(data[pos]), std::addressof(data[pos+i]), sizeof(VALUE) * (count-pos-i));
				count -= i;
			}
			return count;
		}
		void Clear()
		{
			CriticalSection guard(lock);
			for (Index i = 0; i < count; i++)
				data[i].~VALUE();

			count = 0;
		}
		size_t Count() const
		{
			CriticalSection guard(lock);
			return count;
		}
		size_t Capacity() const
		{
			CriticalSection guard(lock);
			return capacity;
		}
		void ShrinkToFit()
		{
			CriticalSection guard(lock);
			if (count < capacity)
			{
				DKASSERT_DEBUG(data);
				if (count > 0)
				{
					VALUE* tmp = (VALUE*)Allocator::Realloc(data, sizeof(VALUE) * count);
					DKASSERT_DESC_DEBUG(tmp, "Out of memory!");
					if (tmp)
					{
						data = tmp;
						capacity = count;
					}
				}
				else
				{
					Allocator::Free(data);
					data = NULL;
					capacity = 0;
				}
			}
		}
		void Resize(size_t s)
		{
			CriticalSection guard(lock);
			if (count > s)			// shrink
			{
				for (Index i = s; i < count; i++)
					data[i].~VALUE();
			}
			else if (count < s)		// extend
			{
				ReserveNL(s);
				for (Index i = count; i < s; i++)
					new(std::addressof(data[i])) VALUE();
			}
			count = s;
		}
		void Resize(size_t s, const VALUE& val)
		{
			CriticalSection guard(lock);
			if (count > s)			// shrink
			{
				for (Index i = s; i < count; i++)
					data[i].~VALUE();
			}
			else if (count < s)		// extend
			{
				ReserveNL(s);
				for (Index i = count; i < s; i++)
					new(std::addressof(data[i])) VALUE(val);
			}
			count = s;
		}
		void Reserve(size_t c)
		{
			CriticalSection guard(lock);
			ReserveNL(c);
		}
		bool CopyValue(VALUE& value, Index index) const
		{
			CriticalSection guard(lock);
			if (count > index)
			{
				value = data[index];
				return true;
			}
			return false;
		}
		VALUE& Value(Index index)
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(index >= 0);
			DKASSERT_DEBUG(count > index);
			return data[index];
		}
		const VALUE& Value(Index index) const
		{
			CriticalSection guard(lock);
			DKASSERT_DEBUG(index >= 0);
			DKASSERT_DEBUG(count > index);
			return data[index];
		}
		/// To use items directly (You may need lock array.)
		operator VALUE* ()
		{
			if (count > 0)
				return data;
			return NULL;
		}
		operator const VALUE* () const
		{
			if (count > 0)
				return data;
			return NULL;
		}
		DKArray& operator = (DKArray&& other)
		{
			if (this != &other)
			{
				CriticalSection guard(lock);
				for (Index i = 0; i < count; i++)
					data[i].~VALUE();
				if (data)
					Allocator::Free(data);

				data = other.data;
				count = other.count;
				capacity = other.capacity;
				other.data = NULL;
				other.count = 0;
				other.capacity = 0;
			}
			return *this;
		}
		DKArray& operator = (const DKArray& value)
		{
			if (this != &value)
			{
				CriticalSection guard1(value.lock);
				CriticalSection guard2(lock);
				for (Index i = 0; i < count; i++)
					data[i].~VALUE();

				count = 0;
				ReserveNL(value.count);
				for (Index i = 0; i < value.count; i++)
					new(std::addressof(data[i])) VALUE(value.data[i]);
				count = value.count;
			}
			return *this;
		}
		DKArray& operator = (std::initializer_list<VALUE> il)
		{
			CriticalSection guard(lock);
			for (Index i = 0; i < count; i++)
				data[i].~VALUE();

			count = 0;
			ReserveNL(il.size());
			for (const VALUE& v : il)
			{
				new(std::addressof(data[count])) VALUE(v);
				count++;
			}
			return *this;
		}
		DKArray operator + (const VALUE& v) const
		{
			DKArray	ret(*this);
			ret.Add(v);
			return ret;
		}
		DKArray operator + (const DKArray& value) const
		{
			DKArray ret(*this);
			ret.Add(value);
			return ret;
		}
		DKArray operator + (std::initializer_list<VALUE> il) const
		{
			DKArray ret(*this);
			ret.Add(il);
			return ret;
		}
		DKArray& operator += (const VALUE& v) const
		{
			Add(v);
			return *this;
		}
		DKArray& operator += (const DKArray& value)
		{
			Add(value);
			return *this;
		}
		DKArray& operator += (std::initializer_list<VALUE> il)
		{
			Add(il);
			return *this;
		}
		void LeftRotate(size_t n)
		{
			CriticalSection guard(lock);
			if (count > 1)
			{
				n = n % count;
				if (n > 0)
				{
					DKStaticArray<VALUE>(data, count).LeftRotate(n);					
				}
			}
		}
		void RightRotate(size_t n)
		{
			CriticalSection guard(lock);
			if (count > 1)
			{
				n = n % count;
				if (n > 0)
				{
					DKStaticArray<VALUE>(data, count).RightRotate(n);
				}
			}
		}
		template <typename T, typename Comparator>
		Index LowerBound(T&& value, Comparator&& cmp) const
		{
			CriticalSection guard(lock);
			return DKStaticArray<VALUE>(data, count).LowerBound(std::forward<T>(value), std::forward<Comparator>(cmp));
		}
		template <typename T, typename Comparator>
		Index UpperBound(T&& value, Comparator&& cmp) const
		{
			CriticalSection guard(lock);
			return DKStaticArray<VALUE>(data, count).UpperBound(std::forward<T>(value), std::forward<Comparator>(cmp));
		}
		bool Swap(Index v1, Index v2)
		{
			CriticalSection guard(lock);
			if (v1 != v2 && v1 < count && v2 < count)
			{
				DKStaticArray<VALUE>(data, count).Swap(v1, v2);
				return true;
			}
			return false;
		}
		void Sort(const DKFunctionSignature<bool (const VALUE&, const VALUE&)>* cmp)
		{
			Sort(0, count, cmp);
		}
		void Sort(Index start, size_t count, const DKFunctionSignature<bool (const VALUE&, const VALUE&)>* cmp)
		{
			CriticalSection guard(lock);
			if (count > 1 && (start + count) <= this->count)
			{
				DKStaticArray<VALUE>(std::addressof(data[start]), count).Sort(cmp);
			}			
		}
		template <typename CompareFunc> void Sort(CompareFunc cmp)
		{
			Sort<CompareFunc>(0, count, cmp);
		}
		template <typename CompareFunc> void Sort(Index start, size_t count, CompareFunc cmp)
		{
			CriticalSection guard(lock);
			if (count > 1 && (start + count) <= this->count)
			{
				DKStaticArray<VALUE>(std::addressof(data[start]), count).template Sort<CompareFunc>(cmp);
			}
		}
		/// EnumerateForward / EnumerateBackward: enumerate all items.
		/// You cannot insert, remove items while enumerating. (container is read-only)
		/// enumerator can be lambda or any function type that can receive arguments (VALUE&) or (VALUE&, bool*)
		/// (VALUE&, bool*) type can cancel iteration by set boolean value to true.
		template <typename T> void EnumerateForward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			EnumerateForward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		template <typename T> void EnumerateBackward(T&& enumerator)
		{
			using Func = typename DKFunctionType<T>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<VALUE&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<VALUE&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (VALUE&) or (VALUE&,bool*)");

			EnumerateBackward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		/// lambda enumerator (const VALUE&) or (const VALUE&, bool*) function type.
		template <typename T> void EnumerateForward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");
			
			EnumerateForward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
		template <typename T> void EnumerateBackward(T&& enumerator) const
		{
			using Func = typename DKFunctionType<T>::Signature;
			enum {ValidatePType1 = Func::template CanInvokeWithParameterTypes<const VALUE&>()};
			enum {ValidatePType2 = Func::template CanInvokeWithParameterTypes<const VALUE&, bool*>()};
			static_assert(ValidatePType1 || ValidatePType2, "enumerator's parameter is not compatible with (const VALUE&) or (const VALUE&,bool*)");
			
			EnumerateBackward(std::forward<T>(enumerator), typename Func::ParameterNumber());
		}
	private:
		// lambda enumerator (VALUE&)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>)
		{
			CriticalSection guard(lock);
			for (Index i = 0; i < count; ++i)
				enumerator(data[i]);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>)
		{
			CriticalSection guard(lock);
			for (Index i = 1; i <= count; ++i)
				enumerator(data[count - i]);
		}
		// lambda enumerator (const VALUE&)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			for (Index i = 0; i < count; ++i)
				enumerator(data[i]);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<1>) const
		{
			CriticalSection guard(lock);
			for (Index i = 1; i <= count; ++i)
				enumerator(data[count - i]);
		}
		// lambda enumerator (VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<2>)
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (Index i = 0; i < count && !stop; ++i)
				enumerator(data[i], &stop);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<2>)
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (Index i = 1; i <= count && !stop; ++i)
				enumerator(data[count - i], &stop);
		}
		// lambda enumerator (const VALUE&, bool*)
		template <typename T> void EnumerateForward(T&& enumerator, DKNumber<2>) const
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (Index i = 0; i < count && !stop; ++i)
				enumerator(data[i], &stop);
		}
		template <typename T> void EnumerateBackward(T&& enumerator, DKNumber<2>) const
		{
			CriticalSection guard(lock);
			bool stop = false;
			for (Index i = 1; i <= count && !stop; ++i)
				enumerator(data[count - i], &stop);
		}
		void ReserveNL(size_t c)
		{
			if (c <= capacity)
				return;

			VALUE* old = data;
			if (data)
				data = (VALUE*)Allocator::Realloc(data, sizeof(VALUE) * c);
			else
				data = (VALUE*)Allocator::Alloc(sizeof(VALUE) * c);

			DKASSERT_DESC_DEBUG(data, "Out of memory!");

			if (data)
				capacity = c;
			else	// out of memory!
				data = old;
		}
		void ReserveItemCapsNL(size_t c)
		{
			if (c > 0)
			{
				size_t minimum = c > InitialSize ? c : InitialSize;

				if (capacity < c + count || count == capacity)
				{
					ReserveNL(count + ((count/2) > minimum ? (count/2):minimum));
				}
			}
		}
		VALUE*	data;
		size_t	count;
		size_t	capacity;
	};
}
