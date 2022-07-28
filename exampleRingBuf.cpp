//
// Created by bismarck on 22-7-28.
//
#include "ringBuffer.h"
#include <iostream>

int main() {
    ringBuffer buf(10);
    char output[100];

    buf.put("123456789", 9);
    buf.get(output, 9);
    std::cout << output << std::endl;

    buf.pop(4);

    buf.put("123", 4);
    buf.get(output, 9);
    std::cout << output << std::endl;
}
