#pragma once

#include "stl/new"

namespace kf
{
    //////////////////////////////////////////////////////////////////////////
    // Allocator

    template <class T>
    class PreAllocator
    {
    public:
        static_assert(!std::is_const_v<T>, "The C++ Standard forbids containers of const elements because allocator<const T> is ill-formed.");

        using value_type = T;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        constexpr PreAllocator() noexcept = default;

        PreAllocator(const PreAllocator&) noexcept = default;

        template<class Other>
        friend class PreAllocator;

        template <typename Other>
        constexpr PreAllocator(const PreAllocator<Other>& other) noexcept : m_ptr(reinterpret_cast<T*>(other.m_ptr)), m_size(other.m_size) {
            DbgPrint("function: %s, m_ptr: %p, m_size: %llu\n", __FUNCTION__, m_ptr, m_size);
        }


        _CONSTEXPR20_DYNALLOC ~PreAllocator() = default;
        _CONSTEXPR20_DYNALLOC PreAllocator& operator=(const PreAllocator&) = default;

        template <POOL_TYPE PoolType>
        T* initialize(const size_t count)
        {
            m_size = count * sizeof(T);
            m_ptr = static_cast<T*>(operator new(m_size, PoolType));

            DbgPrint("function: %s, m_ptr: %p, m_size: %llu, count: %lu\n", __FUNCTION__, m_ptr, m_size, count);

            return m_ptr;
        }

        _CONSTEXPR20_DYNALLOC void deallocate(T* const p, size_t) noexcept
        {
            if (!p)
            {
                std::_Xinvalid_argument("!p");
            }

            DbgPrint("function: %s, ptr: %p\n", __FUNCTION__, p);
            operator delete(p);
        }

        _NODISCARD _CONSTEXPR20_DYNALLOC T* allocate(const size_t count) noexcept
        {
            DbgPrint("function: %s, m_ptr: %p, m_size: %llu, count: %lu\n", __FUNCTION__, m_ptr, m_size, count);

            if (!m_ptr || count * sizeof(T) > m_size)
            {
                std::_Xinvalid_argument("!m_ptr || count * sizeof(T) > m_size");
            }

            return m_ptr;
        }

        //template <typename Other>
        //struct rebind
        //{
        //    using other = PreAllocator<Other>;
        //};

    private:
        T* m_ptr = nullptr;
        size_t m_size = 0;
    };
}
