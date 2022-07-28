//
// Created by bismarck on 22-7-28.
//
#include "ringBuffer.h"
#include <stdio.h>

int main() {
    ringBuffer_t buf = createRingBuffer(10);
    char output[100] = {0};

    buf->put(buf, "0123456789", 10);
    buf->get(buf, output, 10);
    printf("%s\n", output);

    buf->pop(buf, 4);
    buf->put(buf, "1\0003", 4);
    buf->get(buf, output, 10);
    printf("%s\n", output);

    buf->deinit(buf);
}
