#ifndef CHECKS_H
#define CHECKS_H

// Includes
#include <windows.h>
#include <winternl.h>
#include <TlHelp32.h>

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
BOOL antiscan_initialize(void);
BOOL antiscan_cleanup(void);

BOOL antiscan_hasDebuggerAttached(void);

BOOL antiscan_checkNtdll(void *modules);
DWORD antiscan_getParentProcess(void);
#endif