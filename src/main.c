/*
@author: d4rl
@description: Simple module disassembler to search for User-Mode hooks 

TODO:
    [] Get loaded modules from PEB
    [] "Disassembly" function code to verify JMPs/CALLs/RETs

Know issues:
    Uses CreateToolhelp32Snapshot
*/

// Windows
#include <windows.h>
#include <TlHelp32.h>

// God's C
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, void **argv) {
    MODULEENTRY32 m_entry;
    HANDLE h_snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

    while (Module32Next(h_snap, &m_entry)) {
        if (strstr(m_entry.szModule, "ntdll.dll") != NULL) {
            printf("[+] Ntdll found [Base Address: %p]\n", m_entry.modBaseAddr);

            // Get Export Address Table
            PIMAGE_DOS_HEADER p_dos = (PIMAGE_DOS_HEADER)m_entry.modBaseAddr;
            PIMAGE_NT_HEADERS p_nt = (PIMAGE_NT_HEADERS)((DWORD_PTR)m_entry.modBaseAddr + p_dos->e_lfanew);
            PIMAGE_DATA_DIRECTORY r_eat = &(p_nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]);
            PIMAGE_EXPORT_DIRECTORY p_eat = (PIMAGE_EXPORT_DIRECTORY)((DWORD_PTR)m_entry.modBaseAddr + r_eat->VirtualAddress);

            // Iterate over EAT function names
            DWORD n_names = p_eat->NumberOfNames;
            PWORD p_ordinals = (PWORD)((DWORD_PTR)m_entry.modBaseAddr + p_eat->AddressOfNameOrdinals);
            PDWORD eat = (PDWORD)((DWORD_PTR)m_entry.modBaseAddr + p_eat->AddressOfFunctions);
            PDWORD eat_nameTable = (PDWORD)((DWORD_PTR)m_entry.modBaseAddr + p_eat->AddressOfNames);

            for (int i = 0; i < n_names; i++) {
                const char *name = (char*)((DWORD_PTR)m_entry.modBaseAddr + eat_nameTable[i]);

                // We only care about "kernel" functions
                if (!strncmp(name, "Nt", 2) || !strncmp(name, "Zw", 2)) {
                    printf("\t[~] Checking %s\n", name);

                    // Get function address
                    WORD f_ordinal = p_ordinals[i];
                    PDWORD f_address = (PDWORD)((DWORD_PTR)m_entry.modBaseAddr + eat[f_ordinal]);
                    
                    // Checks if function address is in same range that ntdll
                    if (!memcmp(f_address, m_entry.modBaseAddr, 2))
                        printf("\t\t| Address Hooked: yes\n");
                    else
                        printf("\t\t| Address Hooked: no\n");

                    printf("\t\t| Function Address: %p\n", f_address);

                    // Check 5 bytes
                    if (!memcmp("\xE9", f_address, 2))
                        printf("\t\t| Five Bytes: %#02X [Hooked!]\n\n");
                    else
                        printf("\t\t| Five Bytes: %#02X [Not hooked]\n\n");

                    putchar('\n');
                }                
            }
            break;
        }
    }
}