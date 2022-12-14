#ifndef CHECKS_H
#define CHECKS_H

// Imports
#include <windows.h>
#include <winternl.h>

extern BOOL as_initialized;
extern TEB* p_teb;
extern PEB* p_peb;

// Structures
typedef struct {
    char* name;
    PDWORD address;
} MODULE_FUNCTION;

typedef struct {
    LPVOID i_code;              // DLL image code
    wchar_t* f_name;            // Full name, e.g: C:\Windows\System32\ntdll.dll
} MODULE; 

// Global functions

BOOL AS_CheckDebugger(void);

BOOL AS_CheckNTDLL(void *modules);
DWORD AS_GetParentProcess(void);
#endif