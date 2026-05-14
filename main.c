// include pre-installed cortex
#include <cortex-vm.h>

// system includes

// Header includes

int main(){
    CortexVM *vm = cortexVMCreate();

    // minimal example that just prints 5 and exits with that value
    const char *src=
        "addi a0, a0, 5"
        "addi a13, zero, 1"
        "syscall"
        "addi a13, zero, 0"
        "syscall";

    cortexVMExecSource(vm, src);

    int exit_c = cortexVMExecSource(vm, src);

    cortexVMDestroy(vm);

    return exit_c;
}