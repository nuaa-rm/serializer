#include "serialize.h"
#include "stack.h"
#include <stdio.h>

//PP_FOR_EACH(DO_EACH, (int, a), (test, t), (float ,t))
STACK_DEF(uint8_t)
typedef struct {
    int a;
    float b;
} test;
SERIALIZE_DEF(test, (int, a), (float, b));
STACK_DEF(test)

int main() {
    STACK(test) tt = STACK_INIT(test, 10);
    STACK(uint8_t) buf = STACK_INIT(uint8_t, 100);
    test t1 = {2, 3.5f};
    test t2 = {4, 4.11f};
    tt.push(&tt, t1);
    tt.push(&tt, t2);
    printf("%f\n", tt.pop(&tt, -1).b);
    tt.serialize(&tt, buf.data, &(buf.n));
    SERIALIZE(STACK(test))(&tt, buf.data, &(buf.n));
    tt.deinit(&tt);
    for (int i = 0; i < buf.n; i++) {
        printf("%.2x ", buf.data[i]);
    }
}
