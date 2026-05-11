// include pre-installed cortex
#include <cortex-vm.h>

// system includes

// Header includes

int main(){
    // minimal example that just prints 5 and exits with that value
    const char *src=
        "addi a0, zero, 5"
        "addi a13, zero, 1"
        "syscall"
        "addi a13, zero, 0"
        "syscall";

    return cortexExecSource(src);
}