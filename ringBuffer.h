//
// Created by bismarck on 22-7-28.
//

#ifndef SERIALIZER_RINGBUFFER_H
#define SERIALIZER_RINGBUFFER_H

#ifndef _SYS_TYPES_H
typedef unsigned char uint8_t;
#endif

#ifdef __cplusplus
#include <cstring>

class ringBuffer {
private:
    uint8_t* buffer;
    uint8_t* head;
    uint8_t* tail;
    int length;

    bool outOfSize() const {
        return tail < head;
    }

    int remainingSpace() const {
        return length - size();
    }

public:
    explicit ringBuffer(int size) {
        length = size;
        buffer = new uint8_t[size + 1];
        head = buffer;
        tail = buffer;
    }

    ~ringBuffer() {
        delete[] buffer;
    }

    int size() const {
        if (outOfSize()) {
            return length - int(head - tail) + 1;
        } else {
            return int(tail - head);
        }
    }

    bool empty() const {
        return head == tail;
    }

    int capacity() const {
        return length;
    }

    bool get(void* dst, const int size) const {
        if (size > this->size()) {
            return false;
        }
        if (outOfSize()) {
            int head2size = length - int(head - buffer) + 1;
            memcpy(dst, head, head2size);
            memcpy((uint8_t*)dst + head2size, buffer, int(tail - buffer));
        } else {
            memcpy(dst, head, int(tail - head));
        }
        return true;
    }

    bool pop(const int size) {
        if (size > this->size()) {
            return false;
        }
        int head2size = length - int(head - buffer) + 1;
        if (size >= head2size) {
            head = buffer + size - head2size;
        } else {
            head += size;
        }
        return true;
    }

    bool put(const void* src, const int size) {
        if (size > remainingSpace()) {
            return false;
        }
        int tail2size = length - int(tail - buffer) + 1;
        if (size > tail2size) {
            memcpy(tail, src, tail2size);
            memcpy(buffer, (uint8_t*)src + tail2size, size - tail2size);
            tail = buffer + size - tail2size;
        } else if (size == tail2size) {
            memcpy(tail, src, tail2size);
            tail = buffer;
        } else {
            memcpy(tail, src, size);
            tail += size;
        }
        return true;
    }

    bool resize(int size) {
        int _size = this->size();
        if (size < _size) {
            return false;
        }
        auto* _buffer = new uint8_t[size];
        get(_buffer, _size);
        delete[] buffer;
        buffer = _buffer;
        head = buffer;
        tail = buffer + _size;
        return true;
    }

    ringBuffer(const ringBuffer& src) {
        length = src.capacity();
        buffer = new uint8_t[length + 1];
        head = buffer;
        tail = buffer;
        src.get(buffer, src.size());
    }

    ringBuffer& operator=(const ringBuffer& src) {
        if (this != &src) {
            delete[] buffer;
            length = src.capacity();
            buffer = new uint8_t[length + 1];
            head = buffer;
            tail = buffer;
            src.get(buffer, src.size());
        }
        return *this;
    }
};
#else

#endif


#endif //SERIALIZER_RINGBUFFER_H
