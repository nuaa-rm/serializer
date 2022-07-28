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

#include <string.h>
#include <stdlib.h>

struct ringBuffer {
    uint8_t* buffer;
    uint8_t* head;
    uint8_t* tail;
    int length;
    int (*size)(struct ringBuffer* this);
    int (*get)(struct ringBuffer* this, void* dst, const int size);
    int (*pop)(struct ringBuffer* this, const int size);
    int (*put)(struct ringBuffer* this, const void* src, const int size);
    void (*deinit)(struct ringBuffer* this);
};
typedef struct ringBuffer* ringBuffer_t;

int rb_outOfSize(ringBuffer_t this) {
    return this->tail < this->head;
}

int rb_remainingSpace(ringBuffer_t this) {
    return this->length - this->size(this);
}

int rb_get(ringBuffer_t this, void* dst, const int size) {
    if (size > this->size(this)) {
        return 0;
    }
    if (rb_outOfSize(this)) {
        int head2size = this->length - (int)(this->head - this->buffer) + 1;
        memcpy(dst, this->head, head2size);
        memcpy((uint8_t*)dst + head2size, this->buffer, (int)(this->tail - this->buffer));
    } else {
        memcpy(dst, this->head, (int)(this->tail - this->head));
    }
    return 1;
}

int rb_pop(ringBuffer_t this, const int size) {
    if (size > this->size(this)) {
        return 0;
    }
    int head2size = this->length - (int)(this->head - this->buffer) + 1;
    if (size >= head2size) {
        this->head = this->buffer + size - head2size;
    } else {
        this->head += size;
    }
    return 1;
}

int rb_put(ringBuffer_t this, const void* src, const int size) {
    if (size > rb_remainingSpace(this)) {
        return 0;
    }
    int tail2size = this->length - (int)(this->tail - this->buffer) + 1;
    if (size > tail2size) {
        memcpy(this->tail, src, tail2size);
        memcpy(this->buffer, (uint8_t*)src + tail2size, size - tail2size);
        this->tail = this->buffer + size - tail2size;
    } else if (size == tail2size) {
        memcpy(this->tail, src, tail2size);
        this->tail = this->buffer;
    } else {
        memcpy(this->tail, src, size);
        this->tail += size;
    }
    return 1;
}

int rb_size(ringBuffer_t this) {
    if (rb_outOfSize(this)) {
        return this->length - (int)(this->head - this->tail) + 1;
    } else {
        return (int)(this->tail - this->head);
    }
}

void rb_deinit(ringBuffer_t this) {
    free(this->buffer);
    free(this);
}

ringBuffer_t createRingBuffer(const int size) {
    ringBuffer_t this = malloc(sizeof(struct ringBuffer));
    this->length = size;
    this->buffer = malloc(sizeof(uint8_t) * (size + 1));
    this->head = this->buffer;
    this->tail = this->buffer;
    this->size = &rb_size;
    this->get = &rb_get;
    this->pop = &rb_pop;
    this->put = &rb_put;
    this->deinit = &rb_deinit;
    return this;
}

#endif


#endif //SERIALIZER_RINGBUFFER_H
