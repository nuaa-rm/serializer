#include "serialize.h"
#include <vector>

typedef struct {
    int a;
    char c;
    int b;
} test;

SERIALIZE_DEF(test, a, b)

int main() {
    std::vector<test> tt;
    std::string buf;
    test t1 = {2, 3.5f};
    test t2 = {4, 4.1f};
    tt.push_back(t1);
    tt.push_back(t2);
    SERIALIZE(tt, buf);
    for (const auto& it: buf) {
        printf("%.2x ", (uint8_t)it);
    }
}
