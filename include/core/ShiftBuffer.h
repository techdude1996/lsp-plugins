/*
 * ShiftBuffer.h
 *
 *  Created on: 29 янв. 2016 г.
 *      Author: sadko
 */

#ifndef CORE_SHIFTBUFFER_H_
#define CORE_SHIFTBUFFER_H_

namespace lsp
{
    class ShiftBuffer
    {
        private:
            float      *pData;
            size_t      nCapacity;
            size_t      nHead;
            size_t      nTail;

        public:
            ShiftBuffer();
            ~ShiftBuffer();

        public:
            /** Init buffer, all previously stored data will be lost
             *
             * @param size the requested size of buffer, in terms of optimization may be allocated a bit more data
             * @param gap number of zero samples initially stored in buffer, can not be greater than size
             * @return status of operation
             */
            bool init(size_t size, size_t gap = 0);

            /** Resize buffer, if not initialized is similar to init()
             *
             * @param size the requested size of buffer, in terms of optimization may be allocated a bit more data
             * @param gap number of samples initially stored in buffer, if gap > previous size() then additional samples will be zeroed
             * @return status of operation
             */
            bool resize(size_t size, size_t gap = 0);

            /** Destroy buffer
             *
             */
            void destroy();

            /** Add data to the end of buffer
             *
             * @param data amount of data to push, if NULL then buffer is filled with zeros
             * @param count number of samples
             * @return
             */
            size_t append(const float *data, size_t count);

            /** Remove data from the beginning of the buffer
             *
             * @param data pointer to store the samples removed from buffer
             * @param count number of samples to remove
             * @return number of samples removed
             */
            size_t shift(float *data, size_t count);

            /** Return the number of items in the buffer
             *
             * @return number of items in the buffer
             */
            inline size_t size() const { return nTail - nHead;  };

            /** Get maximum size of the buffer
             *
             * @return maximum size of the buffer
             */
            inline size_t capacity() const { return nCapacity; };

            /** Clear buffer
             *
             */
            inline void clear() { nHead = nTail = 0; }

            /** Get the data pointer at the head of buffer
             *
             * @return data pointer at the head of buffer
             */
            inline const float *head() const
            {
                return (pData != NULL) ? &pData[nHead] : NULL;
            }

            /** Get the data pointer at the tail of buffer
             *
             * @return data pointer at the tail of buffer
             */
            inline const float *tail() const
            {
                return (pData != NULL) ? &pData[nTail] : NULL;
            }

            /** Copy data from the specified ShiftBuffer
             *
             * @param src buffer to copy data from
             */
            void copy(const ShiftBuffer *src);
    };

} /* namespace lsp */

#endif /* CORE_SHIFTBUFFER_H_ */