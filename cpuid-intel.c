/* x86info by Dave Jones
 * Based on 'cpuid' by Phil Karn, KA9Q
 * May be used under the terms of the GNU Public License (GPL)
 */

#include <stdio.h>
#include "x86info.h"

/* Decode Intel TLB and cache info descriptors */
void decode_intel_tlb (int x)
{
	switch (x & 0xff) {
	case 0:
		break;
	case 0x1:
		printf ("Instruction TLB: 4KB pages, 4-way set assoc, 32 entries\n");
		break;
	case 0x2:
		printf ("Instruction TLB: 4MB pages, fully assoc, 2 entries\n");
		break;
	case 0x3:
		printf ("Data TLB: 4KB pages, 4-way set assoc, 64 entries\n");
		break;
	case 0x4:
		printf ("Data TLB: 4MB pages, 4-way set assoc, 8 entries\n");
		break;
	case 0x6:
		printf ("Instruction cache: 8KB, 4-way set assoc, 32 byte line size\n");
		break;
	case 0x8:
		printf ("Instruction cache: 16KB, 4-way set assoc, 32 byte line size\n");
		break;
	case 0xa:
		printf ("Data cache: 8KB, 2-way set assoc, 32 byte line size\n");
		break;
	case 0xc:
		printf ("Data cache: 16KB, 2-way or 4-way set assoc, 32 byte line size\n");
		break;
	case 0x40:
		printf ("No L2 cache\n");
		break;
	case 0x41:
		printf ("L2 unified cache: 128KB, 4-way set assoc, 32 byte line size\n");
		break;
	case 0x42:
		printf ("L2 unified cache: 256KB, 4-way set assoc, 32 byte line size\n");
		break;
	case 0x43:
		printf ("L2 unified cache: 512KB, 4-way set assoc, 32 byte line size\n");
		break;
	case 0x44:
		printf ("L2 unified cache: 1MB, 4-way set assoc, 32 byte line size\n");
		break;
	case 0x45:
		printf ("L2 unified cache: 2MB, 4-way set assoc, 32 byte line size\n");
		break;
	default:
		printf ("unknown TLB/cache descriptor: 0x%x\n", x);
		break;
	}
}


/* Intel-specific information */
void dointel (int maxi, struct cpudata *cpu)
{
	int i;
	unsigned long eax, ebx, ecx, edx;
	int stepping, model, family, type, brand, reserved, ntlb;

	static char *x86_cap_flags[] = {
		"FPU    Floating Point Unit",
		"VME    Virtual 8086 Mode Enhancements",
		"DE     Debugging Extensions",
		"PSE    Page Size Extensions",
		"TSC    Time Stamp Counter",
		"MSR    Model Specific Registers",
		"PAE    Physical Address Extension",
		"MCE    Machine Check Exception",
		"CX8    COMPXCHG8B Instruction",
		"APIC   On-chip Advanced Programmable Interrupt Controller present and enabled",
		"10     Reserved",
		"SEP    Fast System Call",
		"MTRR   Memory Type Range Registers",
		"PGE    PTE Global Flag",
		"MCA    Machine Check Architecture",
		"CMOV   Conditional Move and Compare Instructions",
		"FGPAT  Page Attribute Table",
		"PSE-36 36-bit Page Size Extension",
		"PN     Processor Serial Number present and enabled",
		"19     reserved",
		"20     reserved",
		"21     reserved",
		"22     reserved",
		"MMX    MMX instruction set",
		"FXSR   Fast FP/MMX Streaming SIMD Extensions save/restore",
		"XMM    Streaming SIMD Extensions instruction set",
		"26     reserved",
		"27     reserved",
		"28     reserved",
		"29     reserved",
		"30     reserved",
		"31     reserved"
	};

	printf ("Intel-specific functions\n");

	if (maxi >= 1) {
		/* Family/model/type etc */
		cpuid (1, &eax, &ebx, &ecx, &edx);
		stepping = eax & 0xf;
		model = (eax >> 4) & 0xf;
		family = (eax >> 8) & 0xf;
		type = (eax >> 12) & 0x3;
		brand = (ebx & 0xf);
		reserved = eax >> 14;

		printf ("Family: %d Model: %d Type %d [", family, model, type);
		switch (family) {
		case 4:
			/* Family 4 */
			printf ("i486 ");
			switch (model) {
			case 0:
				printf ("DX-25/33");
				break;
			case 1:
				printf ("DX-50");
				break;
			case 2:
				printf ("SX");
				break;
			case 3:
				printf ("487/DX2");
				break;
			case 4:
				printf ("SL");
				break;
			case 5:
				printf ("SX2");
				break;
			case 7:
				printf ("write-back enhanced DX2");
				break;
			case 8:
				printf ("DX4");
				break;
			case 9:
				printf ("write-back enhanced DX4");
				break;
			default:
				printf ("Unknown CPU");
				break;
			}
			break;

		case 5:
			/* Family 5 */
			printf ("Pentium ");
			switch (model) {
			case 0:
				printf ("A-step");
				break;
			case 1:
				printf ("60/66");
				break;
			case 2:
				printf ("75-200");
				break;
			case 3:
				printf ("Overdrive");
				break;
			case 4:
				printf ("MMX");
				break;
			case 7:
				printf ("Mobile");
				break;
			case 8:
				printf ("MMX Mobile");
				break;
			}
			break;

		case 6:
			/* Family 6 */
			switch (model) {
			case 0:
				printf ("A-Step");
				break;
			case 1:
				printf ("Pentium Pro;");
				break;
			case 3:
				printf ("Pentium II");
				if (cpu->stepping == 2)
					printf (" (Overdrive)");
				break;
			case 4:
				printf ("Pentium II");
				break;
			case 5:
				/*FIXME: Cache size determine needed here */
				printf ("Pentium II/Xeon/Celeron");
				break;
			case 6:
				printf ("Celeron / Mobile Pentium II");
				break;
			case 7:
				printf ("Pentium III/Pentium III Xeon");
				break;
			case 8:
				printf ("Celeron / Pentium III (Coppermine)");
				break;
			case 0xA:
				switch (brand) {
				case 0:
					printf ("Pentium II Deschutes");
					break;
				case 1:
					printf ("Celeron");
					break;
				case 2:
					printf ("Pentium III");
					break;
				case 3:
					printf ("Pentium III Xeon");
					break;
				}
			default:
				printf ("Unknown CPU");
				break;
			}
			break;

		case 7:
			/* Family 7 */
			printf ("Itanium");
			break;

		case 0xF:
			/* Family 15 */
			if (cpu->model == 0) {
				printf ("Pentium IV");
				if (cpu->stepping == 7)
					printf (" (stepping B-2)");
				if (cpu->stepping == 0xA)
					printf (" (stepping C-1)");
				break;
			} else {
				printf ("Unknown CPU");
			}
		}

		switch (type) {
		case 0:
			printf (" Original OEM");
			break;
		case 1:
			printf (" Overdrive");
			break;
		case 2:
			printf (" Dual-capable");
			break;
		case 3:
			printf (" Reserved");
			break;
		}
		printf ("]\n");
		printf ("Stepping: %d\n", stepping);

		printf ("Reserved: %d\n\n", reserved);

		printf ("Feature flags %08lx:\n", edx);
		for (i = 0; i < 32; i++) {
			if (edx & (1 << i)) {
				printf ("%s\n", x86_cap_flags[i]);
			}
		}
	}

	if (maxi >= 2) {
		/* Decode TLB and cache info */
		ntlb = 255;
		for (i = 0; i < ntlb; i++) {
			cpuid (2, &eax, &ebx, &ecx, &edx);
			ntlb = eax & 0xff;
			decode_intel_tlb (eax >> 8);
			decode_intel_tlb (eax >> 16);
			decode_intel_tlb (eax >> 24);

			if ((ebx & 0x80000000) == 0) {
				decode_intel_tlb (ebx);
				decode_intel_tlb (ebx >> 8);
				decode_intel_tlb (ebx >> 16);
				decode_intel_tlb (ebx >> 24);
			}
			if ((ecx & 0x80000000) == 0) {
				decode_intel_tlb (ecx);
				decode_intel_tlb (ecx >> 8);
				decode_intel_tlb (ecx >> 16);
				decode_intel_tlb (ecx >> 24);
			}
			if ((edx & 0x80000000) == 0) {
				decode_intel_tlb (edx);
				decode_intel_tlb (edx >> 8);
				decode_intel_tlb (edx >> 16);
				decode_intel_tlb (edx >> 24);
			}
		}
	}
	if (maxi >= 3) {
		/* Pentium III CPU serial number */
		unsigned long signature;
		cpuid (1, &eax, NULL, NULL, NULL);
		signature = eax;

		cpuid (3, &eax, &ebx, &ecx, &edx);
		printf ("Processor serial: ");
		printf ("%04lX", signature >> 16);
		printf ("-%04lX", signature & 0xffff);
		printf ("-%04lX", edx >> 16);
		printf ("-%04lX", edx & 0xffff);
		printf ("-%04lX", ecx >> 16);
		printf ("-%04lX\n", ecx & 0xffff);
	}
}
