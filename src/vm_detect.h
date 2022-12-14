#ifndef VM_DETECT_H
#define VM_DETECT_H

// Windows imports
#include <windows.h>

// C Imports
#include <stdint.h>

u_int64 AS_SldtCheck(void);
BOOL AS_CpuidCheck(void);
BOOL AS_RdtscCheck(void);
BOOL AS_SmswCheck(void);
BOOL AS_StrCheck(void);

#endif