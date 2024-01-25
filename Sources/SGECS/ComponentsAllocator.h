//
// Created by stuka on 05.01.2024.
//

#ifndef ECS_COMPONENTSALLOCATOR_H
#define ECS_COMPONENTSALLOCATOR_H

#include <cstddef>
#include <memory_resource>
#include <iostream>
#include <unordered_map>
#include <functional>
#include <tsl/sparse_map.h>
#include <stack>

#include "TypeInfo.h"
#include "ComponentWrapper.h"
#include "SGUtils/TypeInfo.h"

namespace SGECS
{
    struct Pool
    {
        friend class RegistryAllocator;

        friend class Registry;

        struct BidirectionalIterator
        {
            friend struct Pool;

            void* operator*() const noexcept
            {
                return m_objPtr;
            }

            bool operator==(const BidirectionalIterator& other) const noexcept
            {
                return m_objPtr == other.m_objPtr && m_next == other.m_next && m_previous == other.m_previous;
            }

            bool operator!=(const BidirectionalIterator& other) const noexcept
            {
                return !(*this == other);
            }

            BidirectionalIterator& operator++() noexcept
            {
                if(m_next)
                {
                    m_objPtr = m_next->m_objPtr;
                    m_previous = m_next->m_previous;
                    m_next = m_next->m_next;
                }

                return *this;
            }

            BidirectionalIterator& operator--() noexcept
            {
                if(m_previous)
                {
                    m_objPtr = m_previous->m_objPtr;
                    m_next = m_previous->m_next;
                    m_previous = m_previous->m_previous;
                }

                return *this;
            }

        private:
            void* m_objPtr = nullptr;
            BidirectionalIterator* m_next = nullptr;
            BidirectionalIterator* m_previous = nullptr;
        };

        struct ForwardIterator
        {
            friend struct Pool;

            void* operator*() const noexcept
            {
                return m_objPtr;
            }

            bool operator==(const ForwardIterator& other) const noexcept
            {
                return m_objPtr == other.m_objPtr && m_next == other.m_next;
            }

            bool operator!=(const ForwardIterator& other) const noexcept
            {
                return !(*this == other);
            }

            ForwardIterator& operator++() noexcept
            {
                if(m_next)
                {
                    m_objPtr = m_next->m_objPtr;
                    m_next = m_next->m_next;
                }

                return *this;
            }

        private:
            void* m_objPtr = nullptr;
            ForwardIterator* m_next = nullptr;
        };

        Pool() = default;

        Pool(const Pool& other)
        {
            construct(other.m_buffer, other.m_maxPoolSize, other.m_chunkSize, other.m_alignment);
        }

        Pool(void* buffer, const size_t& poolSize, const size_t& chunkSize, const size_t& alignment)
        {
            construct(buffer, poolSize, chunkSize, alignment);
        }

        ~Pool()
        {
            destruct();
        }

        void* allocate() noexcept
        {
            auto* ptr = m_currentFreeIter.m_objPtr;

            if(ptr && ptr > m_farthestElement)
            {
                m_farthestElement = ptr;
            }

            if(m_currentFreeIter.m_next)
            {
                m_currentFreeIter = *m_currentFreeIter.m_next;
            }

            ++m_allocatedChunksCount;

            return ptr;
        }

        void deallocate(void* ptr)
        {
            ForwardIterator foundIter;
            ForwardIterator* curIter = &m_forwardBegin;
            while(curIter)
            {
                if(curIter->m_objPtr == ptr)
                {
                    foundIter = *curIter;
                    break;
                }
                curIter = curIter->m_next;
            }

            if(curIter)
            {
                foundIter.m_next->m_next = m_currentFreeIter.m_next;
                foundIter.m_next->m_objPtr = m_currentFreeIter.m_objPtr;

                m_currentFreeIter = foundIter;

                --m_allocatedChunksCount;
            }
        }

        void erase(ForwardIterator iterator)
        {
            iterator.m_next->m_next = m_currentFreeIter.m_next;
            iterator.m_next->m_objPtr = m_currentFreeIter.m_objPtr;

            m_currentFreeIter = iterator;

            --m_allocatedChunksCount;
        }

        template<typename T>
        T* getObjectByIndex(const size_t& idx)
        {
            const size_t skipBytes = this->m_chunkSize - sizeof(T);

            if(skipBytes > 0)
            {
                return ((T*) m_buffer) + idx + (skipBytes * idx);
            }
            else
            {
                return ((T*) m_buffer) + idx;
            }
        }

        /**
         * @return Begin (has next iterator (next element or end()), has previous iterator (strictly rend()), pointer to object (possibly)) of allocator (NOT LAST ELEMENT).
         * So you need to check if the object that the iterator points to is not nullptr.
         */
        [[nodiscard]] BidirectionalIterator begin() const noexcept
        {
            return m_biDirBegin;
        }

        /**
         * @return End (has only previous iterator (previous element or begin()) of allocator (NOT LAST ELEMENT).
         * So you need to check if the object that the iterator points to is not nullptr.
         */
        [[nodiscard]] BidirectionalIterator end() const noexcept
        {
            return m_biDirEnd;
        }

        /**
         * @return Rbegin (has previous iterator (previous element or rend()) and pointer to object (possibly)) of allocator (NOT LAST ELEMENT).
         * So you need to check if the object that the iterator points to is not nullptr.
         */
        [[nodiscard]] BidirectionalIterator rbegin() const noexcept
        {
            return *m_biDirEnd.m_previous;
        }

        /**
         * @return Rend (has only next iterator (strictly begin()) of allocator (NOT LAST ELEMENT).
         * So you need to check if the object that the iterator points to is not nullptr.
         */
        [[nodiscard]] BidirectionalIterator rend() const noexcept
        {
            return m_biDirREnd;
        }

        [[nodiscard]] size_t getMaxSize() const noexcept
        {
            return m_maxPoolSize;
        }

        Pool& operator=(const Pool& other) noexcept
        {
            if(this == std::addressof(other)) return *this;

            destruct();
            construct(other.m_buffer, other.m_maxPoolSize, other.m_chunkSize, other.m_alignment);

            return *this;
        }

        [[nodiscard]] size_t getAllocatedChunksCount() const noexcept
        {
            return m_allocatedChunksCount;
        }

        [[nodiscard]] size_t getFarthestIndex() const noexcept
        {
            if(!m_buffer || !m_farthestElement) return 0;
            return ((char*) m_farthestElement - (char*) m_buffer) / (m_chunkSize) + 1;
        }


    private:
        void destruct() noexcept
        {
            // m_allocatedChunksCount = 0;
            BidirectionalIterator* nextBiDirIter = m_biDirBegin.m_next;
            while(nextBiDirIter)
            {
                BidirectionalIterator* curIter = nextBiDirIter;
                nextBiDirIter = nextBiDirIter->m_next;
                delete curIter;
            }

            ForwardIterator* nextForwardIter = m_forwardBegin.m_next;
            while(nextForwardIter)
            {
                ForwardIterator* curIter = nextForwardIter;
                nextForwardIter = nextForwardIter->m_next;
                delete curIter;
            }
        }

        void construct(void* buffer, const size_t& poolSize, const size_t& chunkSize, const size_t& alignment) noexcept
        {
            m_buffer = buffer;
            m_biDirBegin.m_objPtr = m_buffer;
            m_biDirBegin.m_previous = &m_biDirREnd;
            m_biDirREnd.m_next = &m_biDirBegin;

            m_forwardBegin.m_objPtr = m_biDirBegin.m_objPtr;

            m_currentPoolSize = poolSize;
            m_chunkSize = chunkSize;
            m_maxPoolSize = poolSize;
            m_alignment = alignment;

            void* tmpPtr = m_buffer;

            BidirectionalIterator* curBiDirIter = &m_biDirBegin;
            ForwardIterator* curForwardIter = &m_forwardBegin;
            while(std::align(m_alignment, m_chunkSize, tmpPtr, m_currentPoolSize))
            {
                curBiDirIter->m_objPtr = tmpPtr;
                curBiDirIter->m_next = new BidirectionalIterator;
                curBiDirIter->m_next->m_previous = curBiDirIter;

                curForwardIter->m_objPtr = tmpPtr;
                curForwardIter->m_next = new ForwardIterator;

                curBiDirIter = curBiDirIter->m_next;

                curForwardIter = curForwardIter->m_next;

                tmpPtr = (char*) tmpPtr + m_chunkSize;
                m_currentPoolSize -= m_chunkSize;
            }

            m_currentFreeIter = m_forwardBegin;

            m_biDirEnd = *curBiDirIter;
        }

        void* m_buffer = nullptr;

        // крайний элемент от начала
        void* m_farthestElement = nullptr;

        BidirectionalIterator m_biDirBegin;
        BidirectionalIterator m_biDirEnd;
        BidirectionalIterator m_biDirREnd;
        ForwardIterator m_currentFreeIter;
        ForwardIterator m_forwardBegin;

        size_t m_currentPoolSize = 0;
        size_t m_chunkSize = 0;
        size_t m_maxPoolSize = 0;
        size_t m_alignment = 0;

        size_t m_allocatedChunksCount = 0;
    };

    class RegistryAllocator
    {
    public:
        RegistryAllocator() noexcept = default;
        ~RegistryAllocator()
        {
            delete m_rawBuffer;
        }

        template<typename T, typename... Args>
        ComponentWrapper<T>* allocate(Args&& ... args) noexcept
        {
            const size_t typeID = SGUtils::TypeInfoUtils::getTypeInfo<T>().getID();

            auto foundAllocator = m_pools.find(typeID);
            if(foundAllocator == m_pools.cend())
            {
                if(!declareType<T>())
                {
                    return nullptr;
                }
            }

            void* ptr = m_pools.at(typeID).allocate();

            if(!ptr)
            {
                return nullptr;
            }
            else
            {
                return new(ptr)ComponentWrapper<T>(std::forward<Args>(args)...);
            }
        }

        template<typename T>
        bool deallocate(T* obj) noexcept
        {
            size_t typeID = SGUtils::TypeInfoUtils::getTypeInfo<T>().getID();

            if(m_pools.find(typeID) != m_pools.end())
            {
                auto& pool = m_pools.at(typeID);

                auto curIter = pool.m_forwardBegin;
                for(size_t i = 0; i < pool.getFarthestIndex(); ++i)
                {
                    auto* wrapper = (ComponentWrapper<T>*) *curIter;
                    if(&(wrapper->m_component) == obj)
                    {
                        wrapper->~ComponentWrapper();
                        wrapper->m_entityID = -1;
                        pool.erase(curIter);
                        return true;
                    }
                    ++curIter;
                }
            }

            return false;
        }

        template<typename T>
        Pool* getPoolByType() noexcept
        {
            const size_t typeID = SGUtils::TypeInfoUtils::getTypeInfo<T>().getID();

            if(m_pools.find(typeID) != m_pools.end())
            {
                return &m_pools.at(typeID);
            }
            else return nullptr;
        }

        Pool* getPoolByTypeID(const size_t& typeID) noexcept
        {
            if(m_pools.find(typeID) != m_pools.end())
            {
                return &m_pools.at(typeID);
            }
            else return nullptr;
        }

        /*template<typename T>
        std::set<Pool*> getAllPoolsByType() noexcept
        {
            const size_t componentsWrapperTypeID = TypeInfoUtils::getTypeInfo<ComponentWrapper<T>>().getID();
            const size_t rawTypeId = TypeInfoUtils::getTypeInfo<T>().getID();

            std::set<Pool*> pools;

            if(m_pools.find(typeID) != m_pools.end())
            {
                return &m_pools[typeID];
            }

            else return nullptr;
        }*/

        template<typename T>
        bool declareType() noexcept
        {
            return declareType<T>(m_allocatorsStandardSize);
        }

        template<typename T>
        bool declareType(const size_t& allocatorSize) noexcept
        {
            size_t typeID = SGUtils::TypeInfoUtils::getTypeInfo<T>().getID();

            // auto foundAllocator = std::find(m_pools.begin(), m_pools.end(), );
            // if using tsl::sparse_map::find, then WTF???? its creating instance of POOL?????
            auto foundAllocator = m_pools.find(typeID);
            if(foundAllocator == m_pools.end())
            {
                if(m_bufferEnd - m_lastBlock >= allocatorSize)
                {
                    m_pools[typeID] = Pool(m_lastBlock, allocatorSize,
                                           sizeof(ComponentWrapper<T>), alignof(ComponentWrapper<T>));

                    // std::cout << "block: " << m_lastBlock << std::endl;

                    m_lastBlock += allocatorSize;

                    return true;
                }
                else
                {
                    // sorry, not enough space. cannot allocate
                    if(m_freeBlocks.empty())
                    {
                        return false;
                    }
                    else
                    {
                        PoolAllocatorDescription* description = nullptr;
                        auto curIter = m_freeBlocks.begin();
                        while(curIter != m_freeBlocks.end())
                        {
                            if(curIter->m_bufferSize >= allocatorSize)
                            {
                                description = &*curIter;
                                m_freeBlocks.erase(curIter);
                                break;
                            }
                            ++curIter;
                        }

                        // sorry, not enough space. cannot allocate
                        if(!description) return false;

                        m_pools[typeID] = Pool(description->m_bufferPtr, description->m_bufferSize,
                                               sizeof(ComponentWrapper<T>), alignof(ComponentWrapper<T>));

                        return true;
                    }
                }
            }

            return true;
        }

        template<typename T>
        void removeType()
        {
            const size_t typeID = SGUtils::TypeInfoUtils::getTypeInfo<T>().getID();

            auto foundIter = m_pools.find(typeID);
            if(foundIter != m_pools.cend())
            {
                m_freeBlocks.emplace_back(foundIter->second.getMaxSize(), (char*) foundIter->second.m_buffer);

                m_pools.erase(foundIter);
            }
        }

    private:
        struct PoolAllocatorDescription
        {
            size_t m_bufferSize = 0;
            char* m_bufferPtr = nullptr;
        };

        // 2 megabytes
        static inline constexpr const size_t s_bufStartSize = 1048576 * 16.0;

        size_t m_bufSize = s_bufStartSize;
        size_t m_allocatorsStandardCount = 16;
        // 131 072
        size_t m_allocatorsStandardSize = m_bufSize / m_allocatorsStandardCount;

        std::byte* m_rawBuffer = (std::byte*) malloc(s_bufStartSize);
        // std::byte m_rawBuffer[s_bufStartSize] { };
        std::byte* m_bufferEnd = m_rawBuffer + m_bufSize;
        std::vector<PoolAllocatorDescription> m_freeBlocks;
        std::byte* m_lastBlock = m_rawBuffer;

        std::unordered_map<size_t, Pool> m_pools;
        // tsl::sparse_map<size_t, Pool> m_pools;
    };
}

#endif //ECS_COMPONENTSALLOCATOR_H
