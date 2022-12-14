#include "antiscan.h"

// Windows imports
#include <winternl.h>

// Global variables
BOOL as_initialized = FALSE;
TEB* p_teb;
PEB* p_peb;

BOOL AS_Init(void) {
    p_teb = NtCurrentTeb();
    p_peb = p_teb->ProcessEnvironmentBlock;

    as_initialized = TRUE;
}

BOOL AS_Clean(void) {
    // TODO: Cleanup
    as_initialized = FALSE;
}
