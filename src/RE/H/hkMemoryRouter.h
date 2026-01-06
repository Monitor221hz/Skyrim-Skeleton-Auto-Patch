#pragma once

class hkMemoryAllocator;
class hkReferencedObject;

#include "RE/H/hkMemoryAllocator.h"

namespace RE
{
	class hkMemoryRouter
	{
	public:
		static hkMemoryRouter& getInstance();
		static hkMemoryRouter* getInstancePtr();

		/// Helper to make an allocation which remembers the size internally.
		static void* easyAlloc(hkMemoryAllocator& b, int32_t nbytes);

		/// Free a block allocated with easyAlloc.
		static void easyFree(hkMemoryAllocator& b, void* p);

		inline hkMemoryAllocator& temp() { return *m_temp; }
		inline hkMemoryAllocator& heap() { return *m_heap; }
		inline hkMemoryAllocator& debug() { return *m_debug; }
		inline hkMemoryAllocator& solver() { return *m_solver; }

		// members
		char               stack[0x50];  // 00
		hkMemoryAllocator* m_temp;       // 50
		hkMemoryAllocator* m_heap;       // 58
		hkMemoryAllocator* m_debug;      // 60
		hkMemoryAllocator* m_solver;     // 68
		void*              m_userData;   // 70

	private:
		static uint32_t s_memoryRouter();
	};
	static_assert(sizeof(hkMemoryRouter) == 0x78);
}

#define HK_HEAP_REDEFINE_NEW()                                                                                                                      \
	void* operator new(size_t a_count)                                                                                                              \
	{                                                                                                                                               \
		hkReferencedObject* ans = static_cast<hkReferencedObject*>(hkMemoryRouter::getInstance().heap().BlockAlloc(static_cast<int32_t>(a_count))); \
		return ans;                                                                                                                                 \
	}                                                                                                                                               \
                                                                                                                                                    \
	void operator delete(void* p, size_t a_count)                                                                                                   \
	{                                                                                                                                               \
		hkReferencedObject* b = static_cast<hkReferencedObject*>(p);                                                                                \
		hkMemoryRouter::getInstance().heap().BlockFree(p, (b->memSizeAndFlags == kMemSize) ? static_cast<int32_t>(a_count) : b->memSizeAndFlags);   \
	}                                                                                                                                               \
                                                                                                                                                    \
	[[nodiscard]] constexpr void* operator new(size_t, void* a_ptr) noexcept { return a_ptr; }                                                      \
	[[nodiscard]] constexpr void* operator new[](size_t, void* a_ptr) noexcept { return a_ptr; }                                                    \
	inline void                   operator delete(void*, void*) {}                                                                                  \
	inline void                   operator delete[](void*, void*) {}

#define HK_HEAP_REDEFINE_NEW_NOVIRTUAL(CLASS_TYPE)                                                  \
	void* operator new(size_t a_count)                                                              \
	{                                                                                               \
		assert(a_count == sizeof(CLASS_TYPE));                                                      \
		void* obj = hkMemoryRouter::getInstance().heap().BlockAlloc(static_cast<int32_t>(a_count)); \
		return obj;                                                                                 \
	}                                                                                               \
	void operator delete(void* p, size_t)                                                           \
	{                                                                                               \
		if (p) {                                                                                    \
			hkMemoryRouter::getInstance().heap().BlockFree(p, sizeof(CLASS_TYPE));                  \
		}                                                                                           \
	}                                                                                               \
	void* operator new(size_t n, void* p)                                                           \
	{                                                                                               \
		assert(n == sizeof(CLASS_TYPE));                                                            \
		return p;                                                                                   \
	}                                                                                               \
	void* operator new[](size_t, void* p) { return p; }                                             \
	void  operator delete(void*, void*) {}                                                          \
	void  operator delete[](void*, void*) {}
