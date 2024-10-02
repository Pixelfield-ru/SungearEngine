#pragma once
#include <mutex>
#include "SGCore/Utils/Macroses.h"
#include "SGCore/Memory/Deallocator.h"

/*
* MisterElect (MisterChoose)
* 02.10.2024
*/

namespace SGCore::Threading {
	template<typename ObjT, typename MutexT = std::mutex>
	struct WrappedObject {
		using deallocator = Deallocator<ObjT>::deallocator;

		copy_operator(WrappedObject)
		{
			m_deallocImpl(m_wrappedObject);

			m_wrappedObject = other.m_wrappedObject;
			m_deallocImpl = other.m_deallocImpl;

			return *this;
		}

		copy_constructor(WrappedObject)
			: m_deallocImpl(other.m_deallocImpl), m_wrappedObject(other.m_wrappedObject)
		{}

		move_operator(WrappedObject)
		{
			m_deallocImpl(m_wrappedObject);

			m_wrappedObject = std::move(other.m_wrappedObject);
			m_deallocImpl = other.m_deallocImpl;

			return *this;
		}

		move_constructor(WrappedObject)
			: m_deallocImpl(other.m_deallocImpl), m_wrappedObject(std::move(other.m_wrappedObject))
		{}

		explicit WrappedObject() noexcept
			: m_wrappedObject(std::move(ObjT())), m_deallocImpl(Deallocator<ObjT>::defaultDeallocator)
		{}

		template<typename InitQual>
		explicit WrappedObject(InitQual wrappedObject, deallocator* deallocImpl = Deallocator<InitQual>::defaultDeallocator) noexcept
			: m_wrappedObject(std::forward<InitQual>(wrappedObject)), m_deallocImpl(deallocImpl)
		{}

		void lock() noexcept
		{
			m_mutex.lock();
			m_locked = true;
		}

		void unlock() noexcept
		{
			m_mutex.unlock();
			m_locked = false;
		}

		[[nodiscard]] const bool& isLocked() const noexcept
		{
			return m_locked;
		}

		[[nodiscard]] ObjT& getWrapped() noexcept
		{
			return m_wrappedObject;
		}

		~WrappedObject() noexcept {
			m_deallocImpl(m_wrappedObject);
		}

	private:
		bool m_locked;
		deallocator* m_deallocImpl;

		ObjT m_wrappedObject;
		MutexT m_mutex;
	};
}