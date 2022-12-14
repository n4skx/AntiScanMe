#ifndef VM_DETECT_H
#define VM_DETECT_H

// Windows imports
#include <windows.h>

// C Imports
#include <stdint.h>

u_int64 AS_SldtCheck(void);
BOOL AS_CpuidCheck(void);

#endif