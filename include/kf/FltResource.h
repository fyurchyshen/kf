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

    private:
        FltResource(const FltResource&);
        FltResource& operator=(const FltResource&);
    };
}
