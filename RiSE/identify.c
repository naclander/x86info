/*
 *  $Id: identify.c,v 1.3 2001/12/10 17:52:15 davej Exp $
 *  This file is part of x86info.
 *  (C) 2001 Dave Jones.
 *
 *  Licensed under the terms of the GNU GPL License version 2.
 *
 *  Rise specific parts.
 */
#include <stdio.h>
#include "../x86info.h"

void Identify_RiSE (unsigned int maxi, unsigned int maxei, struct cpudata *cpu)
{
	unsigned int i;
	unsigned long eax, ebx, ecx, edx;
	cpu->vendor = VENDOR_RISE;

	/* Do standard stuff */
	if (maxi >= 1) {
		cpuid (cpu->number, 1, &eax, &ebx, &ecx, &edx);
		cpu->stepping = eax & 0xf;
		cpu->model = (eax >> 4) & 0xf;
		cpu->family = (eax >> 8) & 0xf;

		switch (cpu->family) {
		case 5:
			switch (cpu->model) {
				case 0:		sprintf (cpu->name, "%s", "iDragon (0.25um)");		break;
				case 2:		sprintf (cpu->name, "%s", "iDragon (0.18um)");		break;
				case 8:		sprintf (cpu->name, "%s", "iDragon II (0.25um)");	break;
				case 9:		sprintf (cpu->name, "%s", "iDragon II (0.18um)");	break;
				default:	sprintf (cpu->name, "%s", "Unknown CPU");			break;
			}
			break;

		default:
			printf ("Unknown CPU");
			break;
		}

	}

	/* Check for presence of extended info */
	if (maxei == 0)
		return;

	if (maxei >= 0x80000002) {
		/* Processor identification string */
		char namestring[49], *cp;
		unsigned int j;
		cp = namestring;
		for (j = 0x80000002; j <= 0x80000004; j++) {
			cpuid (cpu->number, j, &eax, &ebx, &ecx, &edx);

			for (i = 0; i < 4; i++)
				*cp++ = eax >> (8 * i);
			for (i = 0; i < 4; i++)
				*cp++ = ebx >> (8 * i);
			for (i = 0; i < 4; i++)
				*cp++ = ecx >> (8 * i);
			for (i = 0; i < 4; i++)
				*cp++ = edx >> (8 * i);
		}
		*cp++ = '\n';
		sprintf (cpu->name, "%s", namestring);
	}
}


void display_RiSE_info(unsigned int maxi, unsigned int maxei, struct cpudata *cpu)
{
	unsigned int i;
	unsigned long eax, ebx, ecx, edx;

	if (maxi != 0 && show_registers) {
		/* Dump extended info in raw hex */
		for (i = 0x00000000; i <= maxi; i++) {
			cpuid (cpu->number, i, &eax, &ebx, &ecx, &edx);
			printf ("eax in: 0x%x, eax = %08lx ebx = %08lx ecx = %08lx edx = %08lx\n", i, eax, ebx, ecx, edx);
		}
		printf ("\n");
	}

	if (maxei != 0 && show_registers) {
		/* Dump extended info in raw hex */
		for (i = 0x80000000; i <= maxei; i++) {
			cpuid (cpu->number, i, &eax, &ebx, &ecx, &edx);
			printf ("eax in: 0x%x, eax = %08lx ebx = %08lx ecx = %08lx edx = %08lx\n", i, eax, ebx, ecx, edx);
		}
		printf ("\n");
	}

	cpuid (cpu->number, 0x00000001, &eax, &ebx, &ecx, &edx);
	decode_feature_flags (cpu, edx);

	if (maxei >= 0x80000001) {
		cpuid (cpu->number, 0x80000001, &eax, &ebx, &ecx, &edx);
		decode_feature_flags (cpu, edx);
	}
}
