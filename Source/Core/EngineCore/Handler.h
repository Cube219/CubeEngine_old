#pragma once

#include "EngineCoreHeader.h"

#include "Assertion.h"

namespace cube
{
	template <typename T>
	class Handler
	{
	public:
		Handler() :
			mTable(nullptr),
			mTableIndex(Uint64InvalidValue),
			mID(Uint64InvalidValue)
		{}
			
		Handler(HandlerTable* table, Uint64 tableIndex, Uint64 id):
			mTable(table),
			mTableIndex(tableIndex),
			mID(id)
		{}

		template <typename T2>
		Handler(const Handler<T2>& other) :
			mTable(other.mTable),
			mTableIndex(other.mTableIndex),
			mID(other.mID)
		{
		}
		template <typename T2>
		Handler& operator=(const Handler<T2>& other)
		{
			mTable = other.mTable;
			mTableIndex = other.mTableIndex;
			mID = other.mID;

			return *this;
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

		bool operator==(const Handler& rhs)
		{
			return (mTable == rhs.mTable) && (mTableIndex == rhs.mTableIndex) && (mID == rhs.mID);
		}

		bool IsDestroyed() const
		{
			return GetData() == nullptr;
		}

	protected:
		friend T;
		template <typename T2>
		friend class Handler;
		friend class HandlerTable;

		T* GetData() const
		{
			CHECK(mTable != nullptr, "Cannot access the data from null handler.");

			T* data = mTable->GetData<T>(mTableIndex);

			// Check stale object
			if(data->GetID() != mID)
				return nullptr;

			return data;
		}

		HandlerTable* mTable;
		Uint64 mTableIndex;
		Uint64 mID;
	};

	class Handlable
	{
	public:
		Handlable() : 
			mID(Uint64InvalidValue)
		{}
		virtual ~Handlable() {}

		Handler<Handlable> GetHandler() const
		{
			ASSERTION_FAILED("You must overload GetHandler function in a child class.");
		}

		Uint64 GetID() const { return mID; }
		void SetID(Uint64 id) { mID = id; }

	protected:
		friend class HandlerTable;

		template <typename T>
		void SetHandler(Handler<T>& handler)
		{
			mMyHandler = handler;
		}

		Uint64 mID;
		Handler<Handlable> mMyHandler;
	};

	class HandlerTable
	{
	public:
		HandlerTable(Uint64 initialSize) : 
			mLastAllocatedIndex(initialSize - 1)
		{
			mTable.resize(initialSize);
		}
		~HandlerTable() {}

		HandlerTable(const HandlerTable& other) = delete;
		HandlerTable& operator=(const HandlerTable& other) = delete;

		template <typename T>
		Handler<T> CreateNewHandler(UPtr<T>&& data)
		{
			Uint64 index = FindFreeSlot();
			mTable[index] = std::move(data);
			mLastAllocatedIndex = index;

			auto h = Handler<T>(this, index, mTable[index]->GetID());
			mTable[index]->SetHandler(h);

			return h;
		}

		template <typename T>
		UPtr<T> ReleaseHandler(Handler<T>& handler)
		{
			UPtr<T> d(DCast(T*)(mTable[handler.mTableIndex].release()));
			return d;
		}

		void ReleaseAll()
		{
			mTable.clear();
		}

		template <typename T>
		T* GetData(Uint64 index) const { return DCast(T*)(&*mTable[index]); }

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
