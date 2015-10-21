#include "StringBuf.h"

#include <string.h>

using namespace eleveldb;

/**.......................................................................                                                                  
 * Bare constructor just calls initialize()
 */
StringBuf::StringBuf()
{
    initialize();
}

/**.......................................................................                                                                  
 * Constructor initializes size to minBufSize_, and sets internal buf                                                                       
 * pointer to point to the stack buffer                                                                                                     
 */
StringBuf::StringBuf(size_t size)
{
    initialize();
    resize(size);
}

/**.......................................................................                                                                  
 * Initializes size to minBufSize_, and sets internal buf pointer to                                                                        
 * point to the stack buffer                                                                                                                
 */
void StringBuf::initialize()
{
    bufSize_  = STRING_BUF_MIN_SIZE;
    dataSize_ = 0;
    heapBuf_  = 0;
    bufPtr_   = fixedBuf_;
}

/**.......................................................................
 * Copy constructors and operator=()s copy the data, not the members
 */
StringBuf::StringBuf(const StringBuf& buf)
{
    initialize();
    operator=(buf);
}

StringBuf::StringBuf(StringBuf& buf)
{
    initialize();
    operator=(buf);
}

void StringBuf::operator=(const StringBuf& buf)
{
    operator=((StringBuf&) buf);
}

void StringBuf::operator=(StringBuf& buf)
{
    copy(buf.bufPtr_, buf.dataSize_);
}

void StringBuf::copy(char* buf, size_t size)
{
    resize(size);
    memcpy(bufPtr_, buf, size);
    dataSize_ = size;
}

/**.......................................................................                                                                  
 * Destructor frees any allocated memory, and resets to init state                                                                          
 */
StringBuf::~StringBuf()
{
    if(heapBuf_) {
        free(heapBuf_);
        initialize();
    }
}
size_t StringBuf::bufSize() const
{
    return bufSize_;
}

size_t StringBuf::dataSize() const
{
    return dataSize_;
}

char* StringBuf::getBuf() const
{
    return bufPtr_;
}

/**.......................................................................
 * If requested size is larger than our stack buffer, allocate memory
 * on the heap, and set internal bufPtr_ to point to it.  Assumes
 * until told otherwise (ie, by copy()) that dataSize_ == bufSize_
 */
void StringBuf::resize(size_t size)
{
    if(size > STRING_BUF_MIN_SIZE) {
        heapBuf_    = (char*)realloc(heapBuf_, size);
        bufPtr_     = heapBuf_;
        bufSize_    = size;
    }

    dataSize_ = size;
}
