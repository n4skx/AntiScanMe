#include "checks.h"
#include <stdio.h>
#include <wchar.h>

// Global variables
TEB* p_teb;
PEB* p_peb;

BOOL antiscan_initialize(void) {
    p_teb = NtCurrentTeb();
    p_peb = p_teb->ProcessEnvironmentBlock;
}

BOOL antiscan_cleanup(void) {
    // TODO: Cleanup
}

BOOL antiscan_hasDebuggerAttached(void) {
    if (p_peb->BeingDebugged)
        return TRUE;

    return FALSE;
}

DWORD antiscan_getParentProcess(void) {
    // Creates a snapshot of all processes running
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    // Process entry
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);

    // Iterate over all processes from snapshot
    while (Process32Next(h_snap, &entry)) {
        if (entry.th32ProcessID == GetCurrentProcessId()) {
            return entry.th32ParentProcessID;
        }
    }

    return 0;
}


BOOL antiscan_checkNtdll(void *modules) {
    // Return value
    BOOL foundHook = FALSE;

    // Iterate over loaded modules using PEB
    LIST_ENTRY* head = &p_peb->Ldr->InMemoryOrderModuleList;
    LIST_ENTRY* next = head->Flink;

    for (; head != next; next = next->Flink) {
        // Get module entries
        LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(next, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

        if (wcsstr(entry->FullDllName.Buffer, L"ntdll.dll") != NULL) {
            // Clean user provided module array
            int hooks = 0;
            ZeroMemory(modules, sizeof(modules));

            // Get Export Address Table
            PIMAGE_DOS_HEADER p_dos = (PIMAGE_DOS_HEADER)entry->DllBase;
            PIMAGE_NT_HEADERS p_nt = (PIMAGE_NT_HEADERS)((DWORD_PTR)entry->DllBase + p_dos->e_lfanew);
            PIMAGE_DATA_DIRECTORY r_eat = &(p_nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
            PIMAGE_EXPORT_DIRECTORY p_eat = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)entry->DllBase + r_eat->VirtualAddress);

            // Iterate over EAT function names
            DWORD n_names = p_eat->NumberOfNames;
            PWORD p_ordinals = (PWORD)((DWORD_PTR)entry->DllBase + p_eat->AddressOfNameOrdinals);
            PDWORD eat = (PDWORD)((DWORD_PTR)entry->DllBase + p_eat->AddressOfFunctions);
            PDWORD eat_nameTable = (PDWORD)((DWORD_PTR)entry->DllBase + p_eat->AddressOfNames);

            for (int i = 0; i < n_names; i++) {
                const char *name = (char*)((DWORD_PTR)entry->DllBase + eat_nameTable[i]);

                // We only care about "kernel" functions
                if (!strncmp(name, "Nt", 2) || !strncmp(name, "Zw", 2)) {
                    // Get function address
                    WORD f_ordinal = p_ordinals[i];
                    PDWORD f_address = (PDWORD)((DWORD_PTR)entry->DllBase + eat[f_ordinal]);
                    
                    // Checks if function address is in same range that ntdll
                    if (!memcmp(f_address, entry->DllBase, 2)) {
                        MODULE mod = {
                            entry->DllBase,
                            entry->FullDllName.Buffer
                        };
                        hooks++;
                        foundHook = TRUE;
                    }

                    // TODO: Check 5 bytes
                    if (!memcmp("\xE9", f_address, 2)) {
                        MODULE mod = {
                            entry->DllBase,
                            entry->FullDllName.Buffer
                        };
                        hooks++;
                        foundHook = TRUE;
                    }
                }                
            }

            return FALSE;
            break;
        }
    }

    return foundHook;
}
