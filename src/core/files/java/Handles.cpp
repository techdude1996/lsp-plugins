/*
 * Handles.cpp
 *
 *  Created on: 29 авг. 2019 г.
 *      Author: sadko
 */

#include <core/files/java/Handles.h>
#include <stdlib.h>

#define CAP_GRANULARITY         0x400

namespace lsp
{
    namespace java
    {
        
        Handles::Handles()
        {
            nCapacity       = 0;
            vItems          = NULL;
            nHandle         = 0;
        }
        
        Handles::~Handles()
        {
            flush();
        }

        Object *Handles::get(size_t handle)
        {
            Object *item = (handle < nHandle) ? vItems[handle] : NULL;
            if (item != NULL)
                item->acquire();
            return item;
        }

        status_t Handles::put(Object *obj)
        {
            if (obj == NULL)
                return STATUS_BAD_ARGUMENTS;

            handle_t handle = obj->handle();
            size_t cap = ((handle + CAP_GRANULARITY) / CAP_GRANULARITY) * CAP_GRANULARITY;
            if (nCapacity < cap)
            {
                Object **x = reinterpret_cast<Object **>(::realloc(vItems, sizeof(Object *) * cap));
                if (x == NULL)
                    return STATUS_NO_MEM;
                for (size_t i=nCapacity; i<cap; ++i)
                    x[i]        = NULL;
                vItems      = x;
                nCapacity   = cap;
            }

            if (vItems[handle] != NULL)
                return STATUS_ALREADY_EXISTS;
            obj->acquire();
            vItems[handle]      = obj;
            return STATUS_OK;
        }

        void Handles::clear()
        {
            if (vItems == NULL)
                return;
            for (size_t i=0; i<nCapacity; ++i)
            {
                if (vItems[i] == NULL)
                    continue;
                if (vItems[i]->release() > 0)
                    delete vItems[i];
                vItems[i] = NULL;
            }

            nHandle     = 0;
        }

        void Handles::flush()
        {
            if (vItems == NULL)
                return;
            for (size_t i=0; i<nCapacity; ++i)
            {
                if (vItems[i] == NULL)
                    continue;
                if (vItems[i]->release() > 0)
                    delete vItems[i];
            }
            ::free(vItems);
            vItems      = NULL;
            nCapacity   = 0;
            nHandle     = 0;
        }
    
    } /* namespace java */
} /* namespace lsp */
