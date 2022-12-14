#include "checks.h"
#include <stdio.h>

BOOL initialize(void) {
    p_teb = NtCurrentTeb();
    p_peb = p_teb->ProcessEnvironmentBlock;
}

BOOL cleanup(void) {
    // TODO: Cleanup

}

BOOL isDebugged(void) {
    if (p_peb->BeingDebugged)
        return TRUE;

    return FALSE;
}

BOOL checkParentProcess(void) {
    return FALSE;
}

BOOL checkModules(void) {
    // Return value
    BOOL foundHook = FALSE;

    // Iterate over loaded modules using PEB
    LIST_ENTRY* head = &p_peb->Ldr->InMemoryOrderModuleList;
    LIST_ENTRY* next = head->Flink;

    for (; head != next; next = next->Flink) {

        // 
        LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD(next, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
        UNICODE_STRING* mod_name = (UNICODE_STRING*)((BYTE*)&entry->FullDllName + sizeof(UNICODE_STRING));
        
        printf("Test: %s\n", (char*)mod_name);
        return;
    }

    //     if (RtlEqualUnicodeString(mod_name, &dllName, TRUE)) {
    //         printf("[+] Ntdll found [Base Address: %p]\n", m_entry.modBaseAddr);

    //         // Get Export Address Table
    //         PIMAGE_DOS_HEADER p_dos = (PIMAGE_DOS_HEADER)m_entry.modBaseAddr;
    //         PIMAGE_NT_HEADERS p_nt = (PIMAGE_NT_HEADERS)((DWORD_PTR)m_entry.modBaseAddr + p_dos->e_lfanew);
    //         PIMAGE_DATA_DIRECTORY r_eat = &(p_nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
    //         PIMAGE_EXPORT_DIRECTORY p_eat = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)m_entry.modBaseAddr + r_eat->VirtualAddress);

    //         // Iterate over EAT function names
    //         DWORD n_names = p_eat->NumberOfNames;
    //         PWORD p_ordinals = (PWORD)((DWORD_PTR)m_entry.modBaseAddr + p_eat->AddressOfNameOrdinals);
    //         PDWORD eat = (PDWORD)((DWORD_PTR)m_entry.modBaseAddr + p_eat->AddressOfFunctions);
    //         PDWORD eat_nameTable = (PDWORD)((DWORD_PTR)m_entry.modBaseAddr + p_eat->AddressOfNames);

    //         for (int i = 0; i < n_names; i++) {
    //             const char *name = (char*)((DWORD_PTR)m_entry.modBaseAddr + eat_nameTable[i]);

    //             // We only care about "kernel" functions
    //             if (!strncmp(name, "Nt", 2) || !strncmp(name, "Zw", 2)) {
    //                 printf("\t[~] Checking %s\n", name);

    //                 // Get function address
    //                 WORD f_ordinal = p_ordinals[i];
    //                 PDWORD f_address = (PDWORD)((DWORD_PTR)m_entry.modBaseAddr + eat[f_ordinal]);
                    
    //                 // Checks if function address is in same range that ntdll
    //                 if (!memcmp(f_address, m_entry.modBaseAddr, 2)) {
    //                     printf("\t\t| Address Hooked: yes\n");
    //                     foundHook = TRUE;
    //                 }
    //                 else
    //                     printf("\t\t| Address Hooked: no\n");

    //                 printf("\t\t| Function Address: %p\n", f_address);

    //                 // TODO: Check 5 bytes
    //                 if (!memcmp("\xE9", f_address, 2)) {
    //                     printf("\t\t| Five Bytes: %#02X [Hooked!]\n\n");
    //                     foundHook = TRUE;
    //                 }
    //                 else
    //                     printf("\t\t| Five Bytes: %#02X [Not hooked]\n\n");

    //             }                
    //         }
    //         break;
    //     }
    // }

    return foundHook;
}
