#pragma once

#include <atomic>
#include "SGCore/Utils/Macroses.h"
#include "SGCore/Memory/Deallocator.h"

namespace SGCore::Threading {
	template<typename ObjT>
	struct SpinnedObject {
		using deallocator = Deallocator<ObjT>::deallocator;

		template<typename InitQual>
		explicit SpinnedObject(InitQual obj, deallocator* deallocator = Deallocator<ObjT>::defaultDeallocator)
			: m_deallocImpl(deallocator), m_obj(std::forward<InitQual>(obj))
		{}

		const bool lock() noexcept 
		{
			if (!m_spin.exchange(true, std::memory_order_acquire)) return false;

			while (m_spin.load(std::memory_order_relaxed));
		}


	private:
		deallocator* m_deallocImpl;

		ObjT m_obj;
		std::atomic_bool m_spin;
	};
}