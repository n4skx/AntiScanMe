#ifndef CHECKS_H
#define CHECKS_H

// Includes
#include <windows.h>
#include <winternl.h>
#include <TlHelp32.h>

// Global variables
TEB* p_teb;
PEB* p_peb;

// Global functions
BOOL initialize(void);
BOOL cleanup(void);

BOOL isDebugged(void);

BOOL checkModules(void);
BOOL checkParentProcess(void);
#endif