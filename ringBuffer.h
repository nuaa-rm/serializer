//
// Created by bismarck on 22-7-28.
//

#ifndef SERIALIZER_RINGBUFFER_H
#define SERIALIZER_RINGBUFFER_H

#ifndef _SYS_TYPES_H
typedef unsigned char uint8_t;
#endif

#define C_API_OOP_LIKE

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

    uint8_t operator[](const int index) {
        int head2size = length - int(head - buffer);
        if (index > head2size) {
            return buffer[index - head2size];
        } else {
            return buffer[index];
        }
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

struct ringBufferData {
    uint8_t* buffer;
    uint8_t* head;
    uint8_t* tail;
    int length;
};

struct ringBuffer {
    struct ringBufferData* data;
#ifdef C_API_OOP_LIKE
    int (*size)(struct ringBuffer this);
    int (*get)(struct ringBuffer this, void* dst, const int size);
    uint8_t (*get_char)(struct ringBuffer this, const int index);
    int (*pop)(struct ringBuffer this, const int size);
    int (*put)(struct ringBuffer this, const void* src, const int size);
    void (*deinit)(struct ringBuffer this);
#endif
};
typedef struct ringBuffer ringBuffer_t;

int rb_outOfSize(ringBuffer_t this) {
    return this.data->tail < this.data->head;
}

int rb_size(ringBuffer_t this) {
    if (rb_outOfSize(this)) {
        return this.data->length - (int)(this.data->head - this.data->tail) + 1;
    } else {
        return (int)(this.data->tail - this.data->head);
    }
}

int rb_remainingSpace(ringBuffer_t this) {
    return this.data->length - rb_size(this);
}

int rb_get(ringBuffer_t this, void* dst, const int size) {
    if (size > rb_size(this)) {
        return 0;
    }
    if (rb_outOfSize(this)) {
        int head2size = this.data->length - (int)(this.data->head - this.data->buffer) + 1;
        memcpy(dst, this.data->head, head2size);
        memcpy((uint8_t*)dst + head2size, this.data->buffer, (int)(this.data->tail - this.data->buffer));
    } else {
        memcpy(dst, this.data->head, (int)(this.data->tail - this.data->head));
    }
    return 1;
}

uint8_t rb_get_char(ringBuffer_t this, const int index) {
    int head2size = this.data->length - (int)(this.data->head - this.data->buffer);
    if (index > head2size) {
        return this.data->buffer[index - head2size];
    } else {
        return this.data->buffer[index];
    }
}

int rb_pop(ringBuffer_t this, const int size) {
    if (size > rb_size(this)) {
        return 0;
    }
    int head2size = this.data->length - (int)(this.data->head - this.data->buffer) + 1;
    if (size >= head2size) {
        this.data->head = this.data->buffer + size - head2size;
    } else {
        this.data->head += size;
    }
    return 1;
}

int rb_put(ringBuffer_t this, const void* src, const int size) {
    if (size > rb_remainingSpace(this)) {
        return 0;
    }
    int tail2size = this.data->length - (int)(this.data->tail - this.data->buffer) + 1;
    if (size > tail2size) {
        memcpy(this.data->tail, src, tail2size);
        memcpy(this.data->buffer, (uint8_t*)src + tail2size, size - tail2size);
        this.data->tail = this.data->buffer + size - tail2size;
    } else if (size == tail2size) {
        memcpy(this.data->tail, src, tail2size);
        this.data->tail = this.data->buffer;
    } else {
        memcpy(this.data->tail, src, size);
        this.data->tail += size;
    }
    return 1;
}

void rb_deinit(ringBuffer_t this) {
    free(this.data->buffer);
    free(this.data);
    this.data = NULL;
}

ringBuffer_t createRingBuffer(const int size) {
    ringBuffer_t this;
    this.data = malloc(sizeof(struct ringBufferData));
    this.data->length = size;
    this.data->buffer = malloc(sizeof(uint8_t) * (size + 1));
    this.data->head = this.data->buffer;
    this.data->tail = this.data->buffer;
#ifdef C_API_OOP_LIKE
    this.size = &rb_size;
    this.get = &rb_get;
    this.get_char = &rb_get_char;
    this.pop = &rb_pop;
    this.put = &rb_put;
    this.deinit = &rb_deinit;
#endif
    return this;
}

#endif


#endif //SERIALIZER_RINGBUFFER_H
