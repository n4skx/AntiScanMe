// Windows libraries
#include <windows.h>

// C Libraries
#include <stdio.h>
#include <stdint.h>

#include "../src/antiscan.h"

int main(void) {
    // Hooked modules array
    MODULE h_mods[32];

    // Initialize
    AS_Init();
    printf("[LOG] Module initialized\n");

    // Check debugger
    if (AS_CheckDebugger())
        printf("[LOG] Debugger attached [OK]\n");
    else
        printf("[LOG] Debugger attached [OK]\n");
    
    // Check modules
    BOOL nt_check = AS_CheckNTDLL(h_mods);
    if (nt_check || !nt_check) {
        printf("[LOG] Check modules [OK]\n");
    }

    // Get parrent process
    DWORD p_pid = AS_GetParentProcess();
    printf("[Log] Parent process [OK] (%d).\n", p_pid);

    // VM detection techniques
    BOOL cpuid = AS_CpuidCheck();
    u_int64 sldt = AS_SldtCheck();

    // Cleanup
    AS_Clean();
    return 0;
}