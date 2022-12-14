// Windows libraries
#include <windows.h>

// C Libraries
#include <stdio.h>
#include "../src/antiscan.h"

int main(void) {
    // Hooked modules array
    MODULE h_mods[32];

    // Initialize
    antiscan_initialize();
    printf("[LOG] Module initialized\n");

    // Check debugger
    if (antiscan_hasDebuggerAttached())
        printf("[LOG] Debugger attached [OK]\n");
    else
        printf("[LOG] Debugger attached [OK]\n");
    
    // Check modules
    if (antiscan_checkNtdll(&h_mods)) {
        printf("[LOG] Check modules [OK]\n");
    }

    // Get parrent process
    DWORD p_pid = antiscan_getParentProcess();
    printf("[Log] Parent process [OK] (%d).\n", p_pid);

    // Cleanup
    antiscan_cleanup();
    return 0;
}