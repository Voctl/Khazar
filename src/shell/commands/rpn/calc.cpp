/*Its the simple cpp project
 *for RPN calculator
 *of Khazar hobby OS project */

extern "C" {
#include "types.h"
#include "vga.h"
#include "string.h"
#include "rpn/calc.h"
};

#define STACK_MAX 32

class Stack{
public:
    Stack() : top(0) {}
    void push(I64 value);
    I64 pop();
    bool empty() const;
private:
    I64 data[STACK_MAX];
    int top;
};
