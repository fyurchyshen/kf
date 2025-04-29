#pragma once
#include "DoubleLinkedList.h"

namespace kf
{
    class DoubleLinkedListDestroyer
    {
    public:
        template<class TList>
        static void destroy(TList& list)
        {
            auto it = list.iterator();
            while (it.hasNext())
            {
                auto val = it.next();
                it.remove();
                delete val;
            }
        }
    };
}