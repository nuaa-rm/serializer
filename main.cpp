#include "serialize.h"
#include <vector>

typedef struct {
    int a;
    float b;
    char c;
} test;

SERIALIZE_DEF(test, a, b)

int main() {
    std::vector<test> tt;
    test t1 = {2, 3.5f};
    test t2 = {4, 4.1f};
    tt.push_back(t1);
    tt.push_back(t2);

    std::string buf;
    SERIALIZE(tt, buf);

    for (const auto& it: buf) {
        printf("%.2x ", (uint8_t)it);
    }

    std::vector<test> ttr;
    DESERIALIZE(ttr, buf);
}
