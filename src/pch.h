#pragma once

#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

namespace logs = SKSE::log;
using namespace std::literals;
namespace stl 
{
    template <class T, class U>
    [[nodiscard]] auto adjust_pointer(U *a_ptr, std::ptrdiff_t a_adjust) noexcept
    {
        auto addr = a_ptr ? reinterpret_cast<std::uintptr_t>(a_ptr) + a_adjust : 0;
        if constexpr (std::is_const_v<U> && std::is_volatile_v<U>)
        {
            return reinterpret_cast<std::add_cv_t<T> *>(addr);
        }
        else if constexpr (std::is_const_v<U>)
        {
            return reinterpret_cast<std::add_const_t<T> *>(addr);
        }
        else if constexpr (std::is_volatile_v<U>)
        {
            return reinterpret_cast<std::add_volatile_t<T> *>(addr);
        }
        else
        {
            return reinterpret_cast<T *>(addr);
        }
    }
}