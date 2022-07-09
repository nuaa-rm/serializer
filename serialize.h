//
// Created by bismarck on 22-7-3.
//

#ifndef MACROMAGIC_SERIALIZE_H
#define MACROMAGIC_SERIALIZE_H 1

#include "pp.h"

#ifndef _SYS_TYPES_H
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long int uint64_t;
#endif


#define PP_REMOVE_PARENS(T) PP_REMOVE_PARENS_IMPL T


#ifndef __cplusplus

#include <string.h>
#define PP_REMOVE_PARENS_IMPL(...) PP_CONCAT(SERIALIZE_, PP_GET_N(0, __VA_ARGS__))(_in.PP_GET_N(1, __VA_ARGS__)
#define DO_EACH(VAR, IDX) PP_REMOVE_PARENS(VAR), buf, n);
#define SERIALIZE_DEF(CTX, ...) void PP_CONCAT(SERIALIZE_, CTX)(CTX _in, uint8_t* buf, int* n) { \
    PP_FOR_EACH(DO_EACH, __VA_ARGS__)                                                            \
}

#define SERIALIZE_OBJ_DEF(CTX) void PP_CONCAT(SERIALIZE_, CTX)(CTX _in, uint8_t* buf, int* n) { \
    memcpy(buf + *n, &_in, sizeof(CTX));                                                        \
    *n += sizeof(CTX);                                                                          \
}
#define SERIALIZE(CTX) PP_CONCAT(SERIALIZE_, CTX)

#else

#include <string>
#define PP_IDENTITY(VAR) SERIALIZE(_in.VAR
#define PP_REMOVE_PARENS_IMPL(...) SERIALIZE(_in.PP_GET_N(1, __VA_ARGS__)
#define DO_EACH(VAR, IDX) TRY_REMOVE_PARENS(VAR), buf);
#define SERIALIZE_DEF(CTX, ...) void SERIALIZE(const CTX& _in, std::string& buf) { \
    PP_FOR_EACH(DO_EACH, __VA_ARGS__)                                              \
}

#define SERIALIZE_OBJ_DEF(CTX) void SERIALIZE(const CTX& _in, std::string& buf) { \
    buf.append((char*)&_in, sizeof(CTX));                                         \
}

#include <vector>
#include <list>
#include <map>

SERIALIZE_OBJ_DEF(uint64_t);

template<typename T>
void SERIALIZE_ARRAY(const T& _in, std::string& buf) {
    SERIALIZE((uint64_t)_in.size(), buf);
    for(const auto& it: _in) {
        SERIALIZE(it, buf);
    }
}

template<typename T1, typename T2>
void SERIALIZE_ARRAY(const std::pair<T1, T2>& _in, std::string& buf) {
    SERIALIZE(_in.first(), buf);
    SERIALIZE(_in.second(), buf);
}

template<typename T>
void SERIALIZE(const std::vector<T>& _in, std::string& buf) {
    SERIALIZE_ARRAY(_in, buf);
}

template<typename T>
void SERIALIZE(const std::list<T>& _in, std::string& buf) {
    SERIALIZE_ARRAY(_in, buf);
}

template<typename T1, typename T2>
void SERIALIZE(const std::map<T1, T2>& _in, std::string& buf) {
    SERIALIZE_ARRAY(_in, buf);
}

#endif

SERIALIZE_OBJ_DEF(char);
SERIALIZE_OBJ_DEF(uint8_t);
SERIALIZE_OBJ_DEF(short);
SERIALIZE_OBJ_DEF(uint16_t);
SERIALIZE_OBJ_DEF(int);
SERIALIZE_OBJ_DEF(float);
SERIALIZE_OBJ_DEF(double);

#endif //MACROMAGIC_SERIALIZE_H
