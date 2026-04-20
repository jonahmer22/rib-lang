#include "cortex-vm.h"

int main(void) {
    const char *src =
        "main:\n"
        "    addi a0, zero, msg\n"
        "    addi a13, zero, 5\n"   // SYS_PRINT_STR
        "    syscall\n"
        "    addi a0, zero, 0\n"
        "    addi a13, zero, 0\n"   // SYS_EXIT
        "    syscall\n"
        ".data\n"
        "    msg: \"Hello from embedded Cortex-VM!\\n\"\n";

    return cortexExecSource(src);
}
