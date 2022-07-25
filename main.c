#include "serialize.h"
#include "stack.h"
#include <stdio.h>

STACK_DEF(uint8_t)
typedef struct {
    int a;
    float b;
    char* c;
} test;
SERIALIZE_DEF(test, (int, a), (float, b));
STACK_DEF(test)

int main() {
    STACK(test) tt = STACK_INIT(test, 10);
    test t1 = {2, 3.5f};
    test t2 = {4, 4.11f};
    tt.push(&tt, t1);
    tt.push(&tt, t2);

    STACK(uint8_t) buf = STACK_INIT(uint8_t, 100);
    tt.serialize(&tt, buf.data, &(buf.n));
    tt.deinit(&tt);

    for (int i = 0; i < buf.n; i++) {
        printf("%.2x ", buf.data[i]);
    }

    int t = 0;
    STACK(test) ttr;
    DESERIALIZE(STACK(test))(&ttr, buf.data, &t);
    ttr.deinit(&ttr);
}
