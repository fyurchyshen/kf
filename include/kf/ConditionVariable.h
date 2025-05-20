#pragma once

#include "Semaphore.h"

namespace kf
{
    //////////////////////////////////////////////////////////////////////////
    // ConditionVariable

    class ConditionVariable
    {
    public:
        ConditionVariable()
            : m_event(NotificationEvent, false), m_semaphore(0, MAXLONG)
        {
        }

        ConditionVariable(const ConditionVariable&) = delete;
        ConditionVariable& operator=(const ConditionVariable&) = delete;

        // Waits indefinitely by default
        // Returns STATUS_SUCCESS on success, STATUS_TIMEOUT on timeout
        NTSTATUS wait(EResource& external, LONGLONG timeoutMs = -1)
        {
            LARGE_INTEGER timeout{ .QuadPart = timeoutMs * -10'000 };

            m_resource.acquireExclusive();
            InterlockedIncrement(&m_waitersCount);
            m_resource.release();

            external.release();
            auto status = m_semaphore.wait(timeoutMs < 0 ? nullptr : &timeout);

            m_resource.acquireExclusive();
            auto remaining = InterlockedDecrement(&m_waitersCount);
            if (m_broadcast && remaining == 0)
            {
                m_event.set();
            }
            m_resource.release();

            external.acquireExclusive();

            return status;
        }

        void notifyOne()
        {
            m_resource.acquireExclusive();
            auto shouldRelease = m_waitersCount > 0;
            m_resource.release();

            if (shouldRelease)
            {
                m_semaphore.release(1);
            }
        }

        void notifyAll()
        {
            m_resource.acquireExclusive();

            auto waitersCount = m_waitersCount;
            if (waitersCount > 0)
            {
                m_broadcast = true;
                m_event.clear();
                m_resource.release();

                m_semaphore.release(waitersCount);
            }
            else
            {
                m_resource.release();
            }

            if (waitersCount > 0)
            {
                m_event.wait();

                m_resource.acquireExclusive();
                m_broadcast = false;
                m_resource.release();
            }
        }

    private:
        long m_waitersCount = 0;
        bool m_broadcast = false;
        EResource m_resource;
        Event m_event;
        Semaphore m_semaphore;
    };
}
