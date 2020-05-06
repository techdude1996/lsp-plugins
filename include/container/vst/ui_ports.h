/*
 * ui_ports.h
 *
 *  Created on: 16 янв. 2016 г.
 *      Author: sadko
 */

#ifndef CONTAINER_VST_UI_PORTS_H_
#define CONTAINER_VST_UI_PORTS_H_

namespace lsp
{
    class VSTUIPort: public CtlPort
    {
        protected:
            VSTPort                *pPort;

        public:
            explicit VSTUIPort(const port_t *meta, VSTPort *port):
                CtlPort(meta)
            {
                pPort       = port;
            }

            virtual ~VSTUIPort()
            {
                pPort       = NULL;
            }

        public:
            virtual bool sync()         { return false; };

            virtual bool sync_again()   { return false; };

            virtual void resync()       { };
    };

    class VSTUIPortGroup: public VSTUIPort
    {
        private:
            VSTPortGroup           *pPG;

        public:
            explicit VSTUIPortGroup(VSTPortGroup *port) : VSTUIPort(port->metadata(), port)
            {
                pPG                 = port;
            }

            virtual ~VSTUIPortGroup()
            {
            }

        public:
            virtual float get_value()
            {
                return pPort->getValue();
            }

            virtual void set_value(float value)
            {
                pPort->setValue(value);
            }

        public:
            inline size_t rows() const  { return pPG->rows(); }
            inline size_t cols() const  { return pPG->cols(); }
    };

    class VSTUIParameterPort: public VSTUIPort
    {
        protected:
            float           fValue;
            vst_serial_t    nSID;

        public:
            explicit VSTUIParameterPort(const port_t *meta, VSTParameterPort *port):
                VSTUIPort(meta, port)
            {
                fValue      = meta->start;
                nSID        = port->getSID() - 1;
            }

            virtual ~VSTUIParameterPort()
            {
                fValue      = pMetadata->start;
            }

        public:
            virtual float get_value()
            {
                return fValue;
            }

            virtual void set_value(float value)
            {
                fValue = value;
                if (pPort != NULL)
                    pPort->writeValue(value);
            }

            virtual bool sync()
            {
                vst_serial_t sid = static_cast<VSTParameterPort *>(pPort)->getSID();
                if (sid == nSID)
                    return false;

                fValue      = pPort->getValue();
                nSID        = sid;
                return true;
            }

            virtual void resync()
            {
                if (pPort == NULL)
                    return;
                nSID    = static_cast<VSTParameterPort *>(pPort)->getSID() - 1;
            }

            virtual void *get_buffer()
            {
                return pPort->getBuffer();
            }
    };

    class VSTUIMeterPort: public VSTUIPort
    {
        private:
            float   fValue;

        public:
            explicit VSTUIMeterPort(const port_t *meta, VSTPort *port):
                VSTUIPort(meta, port)
            {
                fValue      = meta->start;
            }

            virtual ~VSTUIMeterPort()
            {
                fValue      = pMetadata->start;
            }

        public:
            virtual float get_value()
            {
                return fValue;
            }

            virtual bool sync()
            {
                float value = fValue;
                if (pMetadata->flags & F_PEAK)
                {
                    VSTMeterPort *mp = static_cast<VSTMeterPort *>(pPort);
                    fValue      = mp->syncValue();
                }
                else
                    fValue      = pPort->getValue();
                return value != fValue;
            }

            virtual void resync()
            {
                if (pMetadata != NULL)
                    fValue      = pMetadata->start;
            }
    };

    class VSTUIMeshPort: public VSTUIPort
    {
        private:
            mesh_t      *pMesh;

        public:
            explicit VSTUIMeshPort(const port_t *meta, VSTPort *port):
                VSTUIPort(meta, port)
            {
                pMesh       = vst_create_mesh(meta);
            }

            virtual ~VSTUIMeshPort()
            {
                vst_destroy_mesh(pMesh);
                pMesh = NULL;
            }

        public:
            virtual bool sync()
            {
                mesh_t *mesh = reinterpret_cast<mesh_t *>(pPort->getBuffer());
                if ((mesh == NULL) || (!mesh->containsData()))
                    return false;

                // Copy mesh data
                for (size_t i=0; i < mesh->nBuffers; ++i)
                    dsp::copy_saturated(pMesh->pvData[i], mesh->pvData[i], mesh->nItems);
                pMesh->data(mesh->nBuffers, mesh->nItems);

                // Clean source mesh
                mesh->cleanup();

                return true;
            }

            virtual void *get_buffer()
            {
                return pMesh;
            }
    };

    class VSTUIFrameBufferPort: public VSTUIPort
    {
        private:
            frame_buffer_t      sFB;

        public:
            explicit VSTUIFrameBufferPort(const port_t *meta, VSTPort *port):
                VSTUIPort(meta, port)
            {
                sFB.init(pMetadata->start, pMetadata->step);
            }

            virtual ~VSTUIFrameBufferPort()
            {
                sFB.destroy();
            }

        public:
            virtual bool sync()
            {
                // Check if there is data for viewing
                frame_buffer_t *fb = pPort->getBuffer<frame_buffer_t>();
                return (fb != NULL) ? sFB.sync(fb) : false;
            }

            virtual void *get_buffer()
            {
                return &sFB;
            }
    };

    class VSTUIPathPort: public VSTUIPort
    {
        private:
            vst_path_t     *pPath;

        public:
            explicit VSTUIPathPort(const port_t *meta, VSTPort *port): VSTUIPort(meta, port)
            {
                path_t *path    = reinterpret_cast<path_t *>(pPort->getBuffer());
                if (path != NULL)
                    pPath               = static_cast<vst_path_t *>(path);
                else
                    pPath               = NULL;
            }

            virtual ~VSTUIPathPort()
            {
                pPath       = NULL;
            }

        public:
            virtual bool sync()
            {
                return pPath->ui_sync();
            }

            virtual void *get_buffer()
            {
                return (pPath != NULL) ? pPath->sUiPath : NULL;
            }

            virtual void write(const void *buffer, size_t size)
            {
                write(buffer, size, 0);
            }

            virtual void write(const void *buffer, size_t size, size_t flags)
            {
                if (pPath != NULL)
                    pPath->submit(reinterpret_cast<const char *>(buffer), size, true, flags);
            }
    };

    class VSTUIOscPortIn: public VSTUIPort
    {
        private:
            osc::packet_t   sPacket;
            size_t          nCapacity;
            bool            bSyncAgain;

        public:
            explicit VSTUIOscPortIn(const port_t *meta, VSTPort *port): VSTUIPort(meta, port)
            {
                bSyncAgain      = false;
                nCapacity       = 0x100;
                sPacket.data    = reinterpret_cast<uint8_t *>(::malloc(nCapacity));
                sPacket.size    = 0;
            }

            virtual ~VSTUIOscPortIn()
            {
                if (sPacket.data != NULL)
                {
                    ::free(sPacket.data);
                    sPacket.data    = NULL;
                }
            }

        public:
            virtual bool sync()
            {
                // Check if there is data for viewing
                bSyncAgain          = false;
                osc_buffer_t *fb    = pPort->getBuffer<osc_buffer_t>();

                while (true)
                {
                    // Try to fetch record from buffer
                    status_t res = fb->fetch(&sPacket, nCapacity);

                    switch (res)
                    {
                        case STATUS_OK:
                        {
                            bSyncAgain    = true;
                            lsp_trace("Received OSC message of %d bytes", int(sPacket.size));
                            osc::dump_packet(&sPacket);
                            return true;
                        }

                        case STATUS_NO_DATA:
                            return false;

                        case STATUS_OVERFLOW:
                        {
                            // Reallocate memory
                            uint8_t *newptr    = reinterpret_cast<uint8_t *>(::realloc(sPacket.data, nCapacity << 1));
                            if (newptr == NULL)
                                fb->skip();
                            else
                                sPacket.data    = newptr;
                            break;
                        }

                        default:
                            return false;
                    }
                }
            }

            virtual bool sync_again() { return bSyncAgain; }

            virtual void *get_buffer()
            {
                return &sPacket;
            }
    };

    class VSTUIOscPortOut: public VSTUIPort
    {
        public:
            VSTUIOscPortOut(const port_t *meta, VSTPort *port): VSTUIPort(meta, port)
            {
            }

            virtual ~VSTUIOscPortOut()
            {
            }

        public:
            virtual void *get_buffer() { return NULL; }

            virtual void write(const void *buffer, size_t size)
            {
                osc_buffer_t *fb = pPort->getBuffer<osc_buffer_t>();
                if (fb != NULL)
                    fb->submit(buffer, size);
            }
    };
}

#endif /* CONTAINER_VST_UI_PORTS_H_ */
