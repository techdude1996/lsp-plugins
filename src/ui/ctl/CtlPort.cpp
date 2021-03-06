/*
 * CtlPort.cpp
 *
 *  Created on: 23 июн. 2017 г.
 *      Author: sadko
 */

#include <ui/ctl/ctl.h>

namespace lsp
{
    namespace ctl
    {
        CtlPort::CtlPort(const port_t *meta)
        {
            pMetadata       = meta;
        }
        
        CtlPort::~CtlPort()
        {
            vListeners.flush();
        }

        void CtlPort::bind(CtlPortListener *listener)
        {
            vListeners.add(listener);
        }

        void CtlPort::unbind(CtlPortListener *listener)
        {
            vListeners.remove(listener, true);
        }

        void CtlPort::unbind_all()
        {
            vListeners.flush();
        }

        void CtlPort::write(const void *buffer, size_t size)
        {
        }

        void *CtlPort::get_buffer()
        {
            return NULL;
        }

        float CtlPort::get_value()
        {
            return NAN;
        }

        float CtlPort::get_default_value()
        {
            return (pMetadata != NULL) ? pMetadata->start : NAN;
        }

        void CtlPort::set_value(float value)
        {
        }

        const char *CtlPort::id() const
        {
            return (pMetadata != NULL) ? pMetadata->id : NULL;
        }

        void CtlPort::notify_all()
        {
            size_t count = vListeners.size();
            for (size_t i=0; i<count; ++i)
                vListeners[i]->notify(this);
        }
    
    } /* namespace tk */
} /* namespace lsp */
