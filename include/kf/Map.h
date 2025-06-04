#pragma once

#include "stl/cassert"
#include "PreAllocator.h"
#include <map>
#include <optional>

#if _KERNEL_MODE && _ITERATOR_DEBUG_LEVEL > 0
#error "_ITERATOR_DEBUG_LEVEL must not be greater than 0"
#endif

namespace kf
{
    //////////////////////////////////////////////////////////////////////////
    // map

    template<class Key, class T, POOL_TYPE PoolType, class Allocator = PreAllocator<std::pair<const Key, T>>>
    class map
    {
    public:
        using value_type = Allocator::value_type;
        using size_type  = Allocator::size_type;
        using map_type   = std::map<Key, T, std::less<Key>, Allocator>;
        using iterator               = map_type::iterator;
        using const_iterator         = map_type::const_iterator;
        using reverse_iterator       = map_type::reverse_iterator;
        using const_reverse_iterator = map_type::const_reverse_iterator;
        using node_type              = map_type::node_type;

        using node = std::_Tree_node<value_type, void*>;

        //
        // Member functions
        //
        map() noexcept = default;

        bool initialize()
        {
            PreAllocator<node> alloc;
            if (!alloc.initialize<PoolType>(1))
            {
                return false;
            }

            m_map.emplace(alloc);

            return true;
        }

        map(const map&) = delete;
        map& operator=(const map&) = delete;

        map(map&& other) noexcept = default;
        map& operator=(map&& other) noexcept = default;

        Allocator get_allocator() const noexcept
        {
            assert(m_map);
            return m_map->get_allocator();
        }

        //
        // Element access
        //

        std::optional<std::reference_wrapper<T>> at(const Key& key) noexcept
        {
            assert(m_map);
            auto it = m_map->find(key);
            if (it == m_map->end())
            {
                return std::nullopt;
            }

            return std::ref(it->second);
        }

        std::optional<std::reference_wrapper<const T>> at(const Key& key) const noexcept
        {
            assert(m_map);
            auto it = m_map->find(key);
            if (it == m_map->end())
            {
                return std::nullopt;
            }

            return std::cref(it->second);
        }

        T& operator[](const Key& key)
        {
            assert(m_map);
            return (*m_map)[key];
        }

        T& operator[](Key&& key)
        {
            assert(m_map);
            return (*m_map)[key];
        }

        //
        // Iterators
        //

        iterator begin() noexcept
        {
            assert(m_map);
            return m_map->begin();
        }

        const_iterator begin() const noexcept
        {
            assert(m_map);
            return m_map->begin();
        }

        const_iterator cbegin() const noexcept
        {
            assert(m_map);
            return m_map->cbegin();
        }

        iterator end() noexcept
        {
            assert(m_map);
            return m_map->end();
        }

        const_iterator end() const noexcept
        {
            assert(m_map);
            return m_map->end();
        }

        const_iterator cend() const noexcept
        {
            assert(m_map);
            return m_map->cend();
        }

        reverse_iterator rbegin() noexcept
        {
            assert(m_map);
            return m_map->rbegin();
        }

        const_reverse_iterator rbegin() const noexcept
        {
            assert(m_map);
            return m_map->rbegin();
        }

        const_reverse_iterator crbegin() const noexcept
        {
            assert(m_map);
            return m_map->crbegin();
        }

        reverse_iterator rend() noexcept
        {
            assert(m_map);
            return m_map->rend();
        }

        const_reverse_iterator rend() const noexcept
        {
            assert(m_map);
            return m_map->rend();
        }

        const_reverse_iterator crend() const noexcept
        {
            assert(m_map);
            return m_map->crend();
        }

        //
        // Capacity
        //

        bool empty() const noexcept
        {
            assert(m_map);
            return m_map->empty();
        }

        size_type size() const noexcept
        {
            assert(m_map);
            return m_map->size();
        }

        size_type max_size() const noexcept
        {
            assert(m_map);
            return m_map->max_size();
        }

        //
        // Modifiers
        //

        void clear() noexcept
        {
            assert(m_map);
            m_map->clear();
        }

        std::optional<std::pair<iterator, bool>> insert(const value_type& value) noexcept
        {
            assert(m_map);
            auto ptr = m_map->get_allocator().initialize<PoolType>(1);
            if (!ptr)
            {
                return std::nullopt;
            }

            auto result = m_map->insert(value);
            if (!result.second) // deallocate if the insertion did not happen
            {
                m_map->get_allocator().deallocate(ptr, 1);
            }

            return result;
        }

        template<class P>
        std::optional<std::pair<iterator, bool>> insert(P&& value) noexcept
        {
            assert(m_map);
            auto ptr = m_map->get_allocator().initialize<PoolType>(1);
            if (!ptr)
            {
                return std::nullopt;
            }

            auto result = m_map->insert(std::move(value));
            if (!result.second) // deallocate if the insertion did not happen
            {
                m_map->get_allocator().deallocate(ptr, 1);
            }

            return result;
        }

        std::optional<std::pair<iterator, bool>> insert(value_type&& value) noexcept
        {
            assert(m_map);
            auto ptr = m_map->get_allocator().initialize<PoolType>(1);
            if (!ptr)
            {
                return std::nullopt;
            }

            auto result = m_map->insert(std::move(value));
            if (!result.second) // deallocate if the insertion did not happen
            {
                m_map->get_allocator().deallocate(ptr, 1);
            }

            return result;
        }

        template<class... Args>
        std::optional<std::pair<iterator, bool>> emplace(Args&&... args)
        {
            assert(m_map);
            auto ptr = m_map->get_allocator().initialize<PoolType>(1);
            if (!ptr)
            {
                return std::nullopt;
            }

            auto result = m_map->emplace(std::forward<Args>(args)...);
            if (!result.second) // deallocate if the insertion did not happen
            {
                m_map->get_allocator().deallocate(ptr, 1);
            }

            return result;
        }

        template<class... Args>
        std::optional<std::pair<iterator, bool>> try_emplace(const Key& k, Args&&... args)
        {
            assert(m_map);
            auto ptr = m_map->get_allocator().initialize<PoolType>(1);
            if (!ptr)
            {
                return std::nullopt;
            }

            auto result = m_map->try_emplace(k, std::forward<Args>(args)...);
            if (!result.second) // deallocate if the insertion did not happen
            {
                m_map->get_allocator().deallocate(ptr, 1);
            }

            return result;
        }

        template<class... Args>
        std::optional<std::pair<iterator, bool>> try_emplace(Key&& k, Args&&... args)
        {
            assert(m_map);
            auto ptr = m_map->get_allocator().initialize<PoolType>(1);
            if (!ptr)
            {
                return std::nullopt;
            }

            auto result = m_map->try_emplace(std::move(k), std::forward<Args>(args)...);
            if (!result.second) // deallocate if the insertion did not happen
            {
                m_map->get_allocator().deallocate(ptr, 1);
            }

            return result;
        }

        iterator erase(iterator pos)
        {
            assert(m_map);
            return m_map->erase(pos);
        }

        iterator erase(const_iterator pos)
        {
            assert(m_map);
            return m_map->erase(pos);
        }

        iterator erase(iterator first, iterator last)
        {
            assert(m_map);
            return m_map->erase(first, last);
        }

        iterator erase(const_iterator first, const_iterator last)
        {
            assert(m_map);
            return m_map->erase(first, last);
        }

        size_type erase(const Key& key)
        {
            assert(m_map);
            return m_map->erase(key);
        }

        void swap(map& other) noexcept
        {
            assert(m_map);
            m_map->swap(*other.m_map);
        }

        node_type extract(const_iterator pos)
        {
            assert(m_map);
            return m_map->extract(pos);
        }

        node_type extract(const Key& k)
        {
            assert(m_map);
            return m_map->extract(k);
        }

        //
        // Lookup
        //

        iterator find(const Key& key)
        {
            assert(m_map);
            return m_map->find(key);
        }

        const_iterator find(const Key& key) const
        {
            assert(m_map);
            return m_map->find(key);
        }

        bool contains(const Key& key) const
        {
            assert(m_map);
            return m_map->contains(key);
        }

        iterator lower_bound(const Key& key)
        {
            assert(m_map);
            return m_map->lower_bound(key);
        }

        const_iterator lower_bound(const Key& key) const
        {
            assert(m_map);
            return m_map->lower_bound(key);
        }

        iterator upper_bound(const Key& key)
        {
            assert(m_map);
            return m_map->upper_bound(key);
        }

        const_iterator upper_bound(const Key& key) const
        {
            assert(m_map);
            return m_map->upper_bound(key);
        }

    private:
        std::optional<map_type> m_map;
    };
}
