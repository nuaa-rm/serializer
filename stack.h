//
// Created by bismarck on 22-7-3.
//

#ifndef MACROMAGIC_STACK_H
#define MACROMAGIC_STACK_H

#include "pp.h"
#include <stdlib.h>
#include <string.h>

typedef enum {
    ERROR_E, NORMAL_E
} STACK_STATUS;

#define STACK(CTX) PP_CONCAT(STACK_, CTX)
#define STACK_S(CTX) struct PP_CONCAT(STACK(CTX), _S)
#define STACK_DEF(CTX) typedef struct PP_CONCAT(STACK(CTX), _S) {               \
    CTX* data;                                                                  \
    int max;                                                                    \
    int n;                                                                      \
    void (*deinit)(STACK_S(CTX)* _in);                                          \
    void (*insert)(STACK_S(CTX)* _in, int n, CTX obj);                          \
    STACK_STATUS (*push)(STACK_S(CTX)* _in, CTX obj);                           \
    CTX (*pop)(STACK_S(CTX)* _in, int n);                                       \
    PP_IF(MACROMAGIC_SERIALIZE_H,                                               \
        void (*serialize)(STACK_S(CTX)* _in, uint8_t* buf, int* n);             \
        void (*deserialize)(STACK_S(CTX)* _in, uint8_t* buf, int* n);,)         \
} STACK(CTX);                                                                   \
void PP_CONCAT(STACK_DEINIT_, CTX)(STACK(CTX)* _in) {                           \
    free(_in->data);                                                            \
    _in->data = NULL;                                                           \
}                                                                               \
PP_IF(MACROMAGIC_SERIALIZE_H,                                                   \
void PP_CONCAT(SERIALIZE_, STACK(CTX))(STACK(CTX)* _in, uint8_t* buf, int* n) { \
    SERIALIZE(int)(&(_in->n), buf, n);                                          \
    for(int i = 0; i < _in->n; i++) {                                           \
        SERIALIZE(CTX)(_in->data + i, buf, n);                                  \
    }                                                                           \
}                                                                               \
void PP_CONCAT(DESERIALIZE_, STACK(CTX))(STACK(CTX)* _in, uint8_t* buf, int* n){\
    DESERIALIZE(int)(&(_in->n), buf, n);                                        \
    for(int i = 0; i < _in->n; i++) {                                           \
        DESERIALIZE(CTX)(_in->data + i, buf, n);                                \
    }                                                                           \
},)                                                                             \
void PP_CONCAT(STACK_INSERT_, CTX)(STACK(CTX)* _in, int n, CTX obj) {           \
    int num = _in->n;                                                           \
    if (_in->n == _in->max) {                                                   \
        num -= 1;                                                               \
    } else {                                                                    \
        _in->n += 1;                                                            \
    }                                                                           \
    memmove(_in->data + n + 1, _in->data + n, (num - n) * sizeof(CTX));         \
    _in->data[n] = obj;                                                         \
}                                                                               \
STACK_STATUS PP_CONCAT(STACK_PUSH_, CTX)(STACK(CTX)* _in, CTX obj) {            \
    if (_in->n == _in->max) {                                                   \
        return ERROR_E;                                                         \
    } else {                                                                    \
        PP_CONCAT(STACK_INSERT_, CTX)(_in, _in->n, obj);                        \
        return NORMAL_E;                                                        \
    }                                                                           \
}                                                                               \
CTX PP_CONCAT(STACK_POP_, CTX)(STACK(CTX)* _in, int n) {                        \
    CTX t;                                                                      \
    if (n >= _in->n) {                                                          \
        return t;                                                               \
    }                                                                           \
    if (n == -1) {                                                              \
        n = _in->n - 1;                                                         \
    }                                                                           \
    t = _in->data[n];                                                           \
    _in->n -= 1;                                                                \
    if (_in->n >= n + 1) {                                                      \
        memmove(_in->data + n, _in->data + n + 1, (_in->n - n) * sizeof(CTX));  \
    }                                                                           \
    return t;                                                                   \
}                                                                               \
STACK(CTX) PP_CONCAT(STACK_INIT_, CTX)(int num) {                               \
    STACK(CTX) t;                                                               \
    t.data = (CTX*)malloc(num * sizeof(CTX));                                   \
    t.max = num;                                                                \
    t.n = 0;                                                                    \
    t.deinit = &STACK_DEINIT(CTX);                                              \
    t.insert = &STACK_INSERT(CTX);                                              \
    t.push = &STACK_PUSH(CTX);                                                  \
    t.pop = &STACK_POP(CTX);                                                    \
    PP_IF(MACROMAGIC_SERIALIZE_H,                                               \
        t.serialize = &PP_CONCAT(SERIALIZE_, STACK(CTX));                       \
        t.deserialize = &PP_CONCAT(DESERIALIZE_, STACK(CTX));,)                 \
    return t;                                                                   \
}

#define STACK_INIT(CTX, NUM) PP_CONCAT(STACK_INIT_, CTX)(NUM)
#define STACK_DEINIT(CTX) PP_CONCAT(STACK_DEINIT_, CTX)
#define STACK_INSERT(CTX) PP_CONCAT(STACK_INSERT_, CTX)
#define STACK_PUSH(CTX) PP_CONCAT(STACK_PUSH_, CTX)
#define STACK_POP(CTX) PP_CONCAT(STACK_POP_, CTX)

#endif //MACROMAGIC_STACK_H
