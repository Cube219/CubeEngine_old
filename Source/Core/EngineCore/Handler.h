#pragma once

#include "EngineCoreHeader.h"

#include "Assertion.h"

namespace cube
{
	template <typename T>
	class Handler
	{
	public:
		// Handler() : mData(nullptr) {}
			
		Handler(HandlerTable& table, Uint64 tableIndex):
			mTable(table),
			mTableIndex(tableIndex)
		{}

		template <typename T2>
		Handler(const Handler<T2>& other) :
			mTable(other.mTable),
			mTableIndex(other.mTableIndex)
		{
		}
		template <typename T2>
		Handler& operator=(const Handler<T2>& other)
		{
			mTable = other.mTable;
			mTableIndex = other.mTableIndex;
		}

		T* operator->() const
		{
			T* data = GetData();

			CHECK(data != nullptr, "Handler<{0}> does not have the data.", typeid(T).name());
			return &*(data);
		}
		T& operator*() const
		{
			T* data = GetData();

			CHECK(data != nullptr, "Handler<{0}> does not have the data.", typeid(T).name());
			return *(data);
		}

		bool IsDestroyed() const
		{
			return GetData() == nullptr;
		}

	protected:
		friend T;

		T* GetData() const
		{
			return mTable.GetData(mTableIndex);
		}

		HandlerTable& mTable;
		Uint64 mTableIndex;
	};

	class Handlable
	{
	public:
		Handlable() {}
		virtual ~Handlable() {}
	};

	class HandlerTable
	{
	public:
		HandlerTable(Uint64 initialSize) : 
			mLastAllocatedIndex(initialSize - 1)
		{
			mTable.resize(initialSize, nullptr);
		}
		~HandlerTable() {}

		template <typename T>
		Handler<T> CreateNewHandler(UPtr<T>&& data)
		{
			Uint64 index = FindFreeSlot();
			mTable[index] = std::move(data);
			mLastAllocatedIndex = index;

			return Handler<T>(*this, index);
		}

		template <typename T>
		void ReleaseHandler(Handler<T>& handler)
		{
			mTable[handler.mTable] = nullptr;
		}

		template <typename T>
		T* GetData(Uint64 index) const { return mTable[index]; }


	private:
		Uint64 FindFreeSlot()
		{
			for(Uint64 i = mLastAllocatedIndex + 1; i < mTable.size(); i++) {
				if(mTable[i] == nullptr) {
					return i;
				}
			}

			for(Uint64 i = 0; i < mLastAllocatedIndex; i++) {
				if(mTable[i] == nullptr) {
					return i;
				}
			}

			Uint64 oldSize = mTable.size();
			mTable.resize(oldSize * 2);

			return oldSize;
		}

		Vector<UPtr<Handlable>> mTable;
		Uint64 mLastAllocatedIndex;
	};
} // namespace cube
