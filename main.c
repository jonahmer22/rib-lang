// include pre-installed cortex
#include <cortex-vm.h>

// system includes

// Header includes

int main(){
    const char *src=
        "addi a0, zero, 5"
        "addi a13, zero, 1"
        "syscall";

    return cortexExecSource(src);
}