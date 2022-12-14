/*
    Redpill-like techniques, credits:
    ~ a0rtega: https://github.com/a0rtega
*/

// Stuctures and more
#include "vm_detect.h"

// Windows imports
#include <windows.h>

// C Imports
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static inline unsigned long long rdtsc_diff() {
	unsigned long long ret, ret2;
	unsigned eax, edx;

	__asm__ volatile ("rdtsc" : "=a" (eax), "=d" (edx));
	ret  = ((unsigned long long)eax) | (((unsigned long long)edx) << 32);
	
    __asm__ volatile ("rdtsc" : "=a" (eax), "=d" (edx));
	ret2  = ((unsigned long long)eax) | (((unsigned long long)edx) << 32);
	
    return ret2 - ret;
}

static inline int cpuid_hv_bit() {
    int ecx;

    __asm__ volatile("cpuid" \
        : "=c"(ecx) \
        : "a"(0x01)
    );

    return (ecx >> 31) & 0x1;
}

static inline void cpuid_hv_vendor_00 (char *vendor){
    int ebx = 0, ecx = 0, edx = 0;

    __asm__ volatile("cpuid" \
        : "=b"(ebx), \
        "=c"(ecx), \
        "=d"(edx) \
        : "a"(0x00)
    );

    sprintf(vendor, "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
    sprintf(vendor+4, "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
    sprintf(vendor+8, "%c%c%c%c", ebx, (ebx >> 8), (ebx >> 16), (ebx >> 24));
    
    vendor[12] = 0x00;
}

BOOL AS_RdtscCheck(void) {
	int i;
	unsigned long long avg = 0;

	for (i = 0; i < 10; i++) {
		avg = avg + rdtsc_diff();
		Sleep(500);
	}
	
    avg = avg / 10;
	return (avg < 750 && avg > 0) ? FALSE : TRUE;
}

u_int64 AS_SldtCheck(void) {
    long long int idtr;

    __asm__ volatile ("sidt %0" : "=m" (idtr));

    return idtr;
}

BOOL AS_StrCheck(void) {
    int tr;

    
    __asm__ volatile (
        "str %0"
        : "=m" (tr)
    );

    if (tr != 0xFF)
        return TRUE;

    return FALSE;
}

BOOL AS_SmswCheck(void) {
    long int status;

    __asm__ volatile (
        "smsw %0"
        : "=m" (status)
    );

    if (status != 0xFF)
        return TRUE;
    
    return FALSE;
}

BOOL AS_CpuidCheck(void) {
    const int count = 6;
    int i;
    char cpu_hv_vendor[13];
    char *strs[6];

    strs[0] = "KVMKVMKVM\0\0\0";    /* KVM */
    strs[1] = "Microsoft Hv";       /* Microsoft Hyper-V or Windows Virtual PC */
    strs[2] = "VMwareVMware";       /* VMware */
    strs[3] = "XenVMMXenVMM";       /* Xen */
    strs[4] = "prl hyperv";         /* Parallels */
    strs[5] = "VBoxVBoxVBox";       /* VirtualBox */
    
    cpuid_hv_vendor_00(cpu_hv_vendor);
    for (i=0; i < count; i++){
        if (!memcmp(cpu_hv_vendor, strs[i], 12)) return TRUE;
    }

    return FALSE;  
}