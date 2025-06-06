#pragma once
#include "EResource.h"

namespace kf
{
    //////////////////////////////////////////////////////////////////////////
    // FltResource

    class FltResource : public EResource
    {
    public:
        static FltResource* fromEResource(PERESOURCE resource)
        {
            return reinterpret_cast<FltResource*>(resource);
        }

        FltResource()
        {
        }

        void fltAcquireExclusive()
        {
            ::FltAcquireResourceExclusive(*this);
        }

        void fltAcquireShared()
        {
            ::FltAcquireResourceShared(*this);
        }

        void fltRelease()
        {
            ::FltReleaseResource(*this);
        }

        void lock()
        {
            fltAcquireExclusive();
        }

        void unlock()
        {
            fltRelease();
        }

        void lock_shared()
        {
            fltAcquireShared();
        }

        void unlock_shared()
        {
            fltRelease();
        }

        bool try_lock() = delete;
        bool try_lock_shared() = delete;

    private:
        FltResource(const FltResource&);
        FltResource& operator=(const FltResource&);
    };
}
