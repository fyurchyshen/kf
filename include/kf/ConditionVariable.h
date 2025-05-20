#pragma once

#include "EResource.h"
#include "Semaphore.h"

namespace kf
{
    //////////////////////////////////////////////////////////////////////////
    // ConditionVariable

    class ConditionVariable
    {
    public:
        ConditionVariable() : m_semaphore(0, MAXLONG)
        {
        }

        ConditionVariable(const ConditionVariable&) = delete;
        ConditionVariable& operator=(const ConditionVariable&) = delete;

        enum class Status
        {
            Timeout = 0,
            Success
        };

        void wait(EResource& external)
        {
            waitFor(external, nullptr);
        }

        template<class Predicate>
        void wait(EResource& external, Predicate predicate)
        {
            waitFor(external, nullptr, predicate);
        }

        Status waitFor(EResource& external, PLARGE_INTEGER timeout)
        {
            Status result = Status::Success;

            m_resource.acquireExclusive();
            ++m_waitersCount;
            m_resource.release();

            external.release();
            auto status = m_semaphore.wait(timeout);
            external.acquireExclusive();

            if (!NT_SUCCESS(status))
            {
                result = Status::Timeout;
            }

            return result;
        }

        template<class Predicate>
        bool waitFor(EResource& external, PLARGE_INTEGER timeout, Predicate predicate)
        {
            while (!predicate())
            {
                if (waitFor(external, timeout) == Status::Timeout)
                {
                    return predicate();
                }
            }

            return true;
        }

        void notifyOne()
        {
            m_resource.acquireExclusive();
            auto shouldRelease = m_waitersCount > 0;
            m_resource.release();

            if (shouldRelease)
            {
                m_semaphore.release();
            }
        }

        void notifyAll()
        {
            m_resource.acquireExclusive();
            auto waitersCount = m_waitersCount;
            m_resource.release();

            if (waitersCount > 0)
            {
                m_semaphore.release(waitersCount);
            }
        }

    private:
        long m_waitersCount = 0;
        EResource m_resource;
        Semaphore m_semaphore;
    };
}
