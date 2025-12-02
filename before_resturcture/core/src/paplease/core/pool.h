#pragma once
#include "paplease/common/common.h"
#include "paplease/types.h"

#include <type_traits>

namespace paplease {
    namespace core {

        struct nobound_t
        {
            struct _Tag {};
            constexpr explicit nobound_t(_Tag) {}
        };
        inline constexpr nobound_t nobound{ nobound_t::_Tag{} };

        template<typename TObject>
        class PoolBase
        {
        public:
            constexpr virtual ~PoolBase() = default;
            constexpr virtual void IncRefCount(TObject* ptr) = 0;
            constexpr virtual void DecRefCount(TObject* ptr) = 0;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
            constexpr virtual u32 GenerationOf(TObject* ptr) = 0;
#endif
        };

        template<class TObject>
        class PoolBound
        {
        public:
            constexpr PoolBound() noexcept : m_object(nullptr), m_pool(nullptr)
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                , m_generation(0)
#endif
            {}
            constexpr PoolBound(nobound_t) noexcept : m_object(nullptr), m_pool(nullptr)
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                , m_generation(0)
#endif
            {}

            constexpr PoolBound(const PoolBound& other) noexcept
                : m_object(other.m_object), m_pool(other.m_pool)
            {
                if (m_object && m_pool)
                {
                    m_pool->IncRefCount(m_object);
                }
            }
            constexpr PoolBound& operator=(const PoolBound& other) noexcept
            {
                if (this != &other)
                {
                    Free();
                    m_object = other.m_object;
                    m_pool = other.m_pool;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                    m_generation = other.m_generation;
#endif
                    if (m_pool && m_object)
                    {
                        m_pool->IncRefCount(m_object);
                    }
                }
                return *this;
            }

            constexpr PoolBound(PoolBound&& other) noexcept
                : m_object(other.m_object), m_pool(other.m_pool)
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                , m_generation(other.m_generation)
#endif
            {
                other.m_object = nullptr;
                other.m_pool = nullptr;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                other.m_generation = 0;
#endif
            }

            constexpr PoolBound& operator=(PoolBound&& other) noexcept
            {
                if (this != &other)
                {
                    Free();
                    m_object = other.m_object;
                    m_pool = other.m_pool;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                    m_generation = other.m_generation;
#endif
                    other.m_object = nullptr;
                    other.m_pool = nullptr;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                    other.m_generation = 0;
#endif
                }
                return *this;
            }

            constexpr PoolBound& operator=(nobound_t) noexcept
            {
                Free();
                m_object = nullptr;
                m_pool = nullptr;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                m_generation = 0;
#endif
                return *this;
            }

            constexpr ~PoolBound()
            {
                Free();
            }

            constexpr TObject& Value() noexcept
            {
                assert(IsValid() && "Attempted to access value of null or invalid PoolBound");
                return *m_object;
            }

            constexpr const TObject& Value() const noexcept
            {
                assert(IsValid() && "Attempted to access value of null or invalid PoolBound");
                return *m_object;
            }

            constexpr bool operator==(const PoolBound<TObject>& right) const noexcept
            {
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                return m_object == right.m_object && m_generation == right.m_generation;
#else
                return m_object == right.m_object;
#endif
            }

            constexpr bool IsValid() const noexcept
            {
                return m_object && m_pool
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                    && m_pool->GenerationOf(m_object) == m_generation
#endif
                    ;
            }

            constexpr TObject* operator->() noexcept { return m_object; }
            constexpr const TObject* operator->() const noexcept { return m_object; }
            constexpr TObject& operator*() noexcept { return *m_object; }
            constexpr const TObject& operator*() const noexcept { return *m_object; }

        private:
            template<class T, size_t N>
            friend class Pool;

#if EXPERIMENTAL_USE_POOL_GENERATIONS
            constexpr PoolBound(TObject* object, PoolBase<TObject>* pool, u32 generation)
                : m_object(object), m_pool(pool), m_generation(generation)
            {
                if (m_object && m_pool)
                {
                    m_pool->IncRefCount(m_object);
                }
            }
#else
            constexpr PoolBound(TObject* object, PoolBase<TObject>* pool)
                : m_object(object), m_pool(pool)
            {
                if (m_object && m_pool)
                {
                    m_pool->IncRefCount(m_object);
                }
            }
#endif

            constexpr void Free()
            {
                if (m_object && m_pool)
                {
                    m_pool->DecRefCount(m_object);
                    // m_pool->Free(m_object);
                    m_object = nullptr;
                    m_pool = nullptr;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                    m_generation = 0;
#endif
                }
            }

            TObject* m_object;
            PoolBase<TObject>* m_pool;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
            u32 m_generation;
#endif
        };

        template<class TObject, size_t PoolSize>
        class Pool : public PoolBase<TObject>
        {
        public:
            constexpr Pool() = default;

            template<class...Args>
            constexpr PoolBound<TObject> Allocate(Args&& ...args)
            {
                for (size_t i = 0; i < PoolSize; i++)
                {
                    if (!m_inUse[i])
                    {
                        std::construct_at(&m_objects[i], std::forward<Args>(args)...);
                        m_inUse[i] = true;
                        m_refCount[i] = 0;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                        m_generations[i] = m_currentGeneration;
                        return PoolBound<TObject>{ &m_objects[i], this, m_generations[i] };
#else
                        return PoolBound<TObject>{ &m_objects[i], this };
#endif
                    }
                }
                return nobound;
            }

            constexpr PoolBound<TObject> Register(TObject&& object)
            {
                for (size_t i = 0; i < PoolSize; i++)
                {
                    if (!m_inUse[i])
                    {
                        m_objects[i] = std::move(object);
                        m_inUse[i] = true;
                        m_refCount[i] = 0;
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                        m_generations[i] = m_currentGeneration;
                        return PoolBound<TObject>{ &m_objects[i], this, m_generations[i] };
#else
                        return PoolBound<TObject>{ &m_objects[i], this };
#endif
                    }
                }
                return nobound;
            }

            constexpr void IncRefCount(TObject* ptr) override
            {
                size_t index = static_cast<size_t>(ptr - m_objects);
                if (index < PoolSize && m_inUse[index])
                {
                    ++m_refCount[index];
                }
            }

            constexpr void DecRefCount(TObject* ptr) override
            {
                size_t index = static_cast<size_t>(ptr - m_objects);
                if (index < PoolSize && m_inUse[index] && m_refCount[index] > 0)
                {
                    if (--m_refCount[index] == 0)
                    {
                        Free(&m_objects[index]);
                    }
                }
            }

            constexpr void Clear()
            {
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                ++m_currentGeneration;
#endif
                for (size_t i = 0; i < PoolSize; i++)
                {
                    if (m_inUse[i])
                    {
                        std::destroy_at(&m_objects[i]);
                        m_inUse[i] = false;
                    }
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                    m_generations[i] = m_currentGeneration;
#endif
                }
            }

#if EXPERIMENTAL_USE_POOL_GENERATIONS
            constexpr u32 GenerationOf(TObject* ptr) override
            {
                size_t index = static_cast<size_t>(ptr - m_objects);
                return index < PoolSize ? m_generations[index] : ~0u;
            }
#endif
        private:
            constexpr void Free(TObject* ptr)
            {
                size_t index = static_cast<size_t>(ptr - m_objects);
                if (index < PoolSize && m_inUse[index])
                {
                    std::destroy_at(ptr);
#if EXPERIMENTAL_USE_POOL_GENERATIONS
                    ++m_generations[index];
#endif
                    m_inUse[index] = false;
                }
            }

        private:
            TObject m_objects[PoolSize]{};
            bool m_inUse[PoolSize]{};
            u32 m_refCount[PoolSize]{};
#if EXPERIMENTAL_USE_POOL_GENERATIONS
            u32 m_generations[PoolSize]{};
            u32 m_currentGeneration = 1;
#endif
        };

    }  // namespace core
}  // namespace paplease
