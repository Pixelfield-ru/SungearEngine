#pragma once

/*
* MisterElect (MisterChoose)
* 02.10.2024
*/

// Utility class for (only for at 02.10.2024) for Threading::WrappedObject
namespace SGCore {
	template<typename T>
	struct Deallocator {
		// nothing to do...
		static inline void defaultDeallocator(T handler) noexcept {}
		static inline void defaultDeallocator(T* handler) noexcept {}

		static inline void heapDeallocator(T* handler) {
			delete handler;
		}

		static inline void heapArrayDeallocator(T* handler) {
			delete[] handler;
		}

		using deallocator = void(T);
	};
}