/* acpi.c - here is the slightly complicated ACPI power off code */
/* Copyright (C) 2016  Chris Smith */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <log.h>
#include <int.h>

#include <utils/utils.h>
#include <kernel/ports.h>

uint32 *SMI_CMD;
uint8 ACPI_ENABLE;
uint8 ACPI_DISABLE;
uint32 *PM1a_CNT;
uint32 *PM1b_CNT;
uint16 SLP_TYPa;
uint16 SLP_TYPb;
uint16 SLP_EN;
uint16 SCI_EN;
uint8 PM1_CNT_LEN;

struct RSDPtr {
	uint8 Signature[8];
	uint8 CheckSum;
	uint8 OemID[6];
	uint8 Revision;
	uint32 *RsdtAddress;
};

struct FACP {
	uint8 Signature[4];
	uint32 Length;
	uint8 unneded1[40 - 8];
	uint32 *DSDT;
	uint8 unneded2[48 - 44];
	uint32 *SMI_CMD;
	uint8 ACPI_ENABLE;
	uint8 ACPI_DISABLE;
	uint8 unneded3[64 - 54];
	uint32 *PM1a_CNT_BLK;
	uint32 *PM1b_CNT_BLK;
	uint8 unneded4[89 - 72];
	uint8 PM1_CNT_LEN;
};

// check if the given address has a valid header
unsigned int *acpiCheckRSDPtr(unsigned int *ptr) {
	char *sig = "RSD PTR ";
	struct RSDPtr *rsdp = (struct RSDPtr *) ptr;
	uint8 *bptr;
	uint8 check = 0;
	unsigned int i;

	if (memcmp(sig, rsdp, 8) == 0) {
		// check checksum rsdpd
		bptr = (uint8 *) ptr;
		for (i = 0; i < sizeof(struct RSDPtr); i++) {
			check += *bptr;
			bptr++;
		}

		// found valid rsdpd
		if (check == 0) {
			/*
			 if (desc->Revision == 0)
			 log("acpi 1");
			 else
			 log("acpi 2");
			 */
			return (unsigned int *) rsdp->RsdtAddress;
		}
	}

	return NULL;
}

// finds the acpi header and returns the address of the rsdt
unsigned int *acpiGetRSDPtr(void) {
	unsigned int *addr;
	unsigned int *rsdp;

	// search below the 1mb mark for RSDP signature
	for (addr = (unsigned int *) 0x000E0000; (uint64) addr < 0x00100000;
			addr += 0x10 / sizeof(addr)) {
		rsdp = acpiCheckRSDPtr(addr);
		if (rsdp != NULL)
			return rsdp;
	}

	// at address 0x40:0x0E is the RM segment of the ebda
	int ebda = *((short *) 0x40E);   // get pointer
	ebda = ebda * 0x10 & 0x000FFFFF;   // transform segment into linear address

	// search Extended BIOS Data Area for the Root System Description Pointer signature
	for (addr = (unsigned int *) ebda; (uint64) addr < ebda + 1024;
			addr += 0x10 / sizeof(addr)) {
		rsdp = acpiCheckRSDPtr(addr);
		if (rsdp != NULL)
			return rsdp;
	}

	return NULL;
}

// checks for a given header and validates checksum
int acpiCheckHeader(unsigned int *ptr, char *sig) {
	if (memcmp(ptr, sig, 4) == 0) {
		char *checkPtr = (char *) ptr;
		int len = *(ptr + 1);
		char check = 0;
		while (0 < len--) {
			check += *checkPtr;
			checkPtr++;
		}
		if (check == 0)
			return 0;
	}
	return -1;
}

int acpiEnable(void) {
	// check if acpi is enabled
	if ((inw((uint64) PM1a_CNT) & SCI_EN) == 0) {
		// check if acpi can be enabled
		if (SMI_CMD != 0 && ACPI_ENABLE != 0) {
			outb((uint64) SMI_CMD, ACPI_ENABLE); // send acpi enable command
			// give 3 seconds time to enable acpi
			int i;
			for (i = 0; i < 300; i++) {
				if ((inw((uint64) PM1a_CNT) & SCI_EN) == 1)
					break;
				io_wait();
			}
			if (PM1b_CNT != 0)
				for (; i < 300; i++) {
					if ((inw((uint64) PM1b_CNT) & SCI_EN) == 1)
						break;
					io_wait();
				}
			if (i < 300) {
				log("enabled acpi.\n");
				return 0;
			} else {
				log("couldn't enable acpi.\n");
				return -1;
			}
		} else {
			log("no known way to enable acpi.\n");
			return -1;
		}
	} else {
		//log("acpi was already enabled.\n");
		return 0;
	}
}

//
// uint8code of the \_S5 object
// -----------------------------------------
//        | (optional) |    |    |    |
// NameOP | \          | _  | S  | 5  | _
// 08     | 5A         | 5F | 53 | 35 | 5F
//
// -----------------------------------------------------------------------------------------------------------
//           |           |              | ( SLP_TYPa   ) | ( SLP_TYPb   ) | ( Reserved   ) | (Reserved    )
// PackageOP | PkgLength | NumElements  | uint8prefix Num | uint8prefix Num | uint8prefix Num | uint8prefix Num
// 12        | 0A        | 04           | 0A         05  | 0A          05 | 0A         05  | 0A         05
//
//----this-structure-was-also-seen----------------------
// PackageOP | PkgLength | NumElements |
// 12        | 06        | 04          | 00 00 00 00
//
// (Pkglength bit 6-7 encode additional PkgLength uint8s [shouldn't be the case here])
//
int initAcpi(void) {
	unsigned int *ptr = acpiGetRSDPtr();

	// check if address is correct  ( if acpi is available on this pc )
	if (ptr != NULL && acpiCheckHeader(ptr, "RSDT") == 0) {
		// the RSDT contains an unknown number of pointers to acpi tables
		int entrys = *(ptr + 1);
		entrys = (entrys - 36) / 4;
		ptr += 36 / 4;   // skip header information

		while (0 < entrys--) {
			// check if the desired table is reached
			if (acpiCheckHeader((unsigned int *) *ptr, "FACP") == 0) {
				entrys = -2;
				struct FACP *facp = (struct FACP *) *ptr;
				if (acpiCheckHeader((unsigned int *) facp->DSDT, "DSDT") == 0) {
					// search the \_S5 package in the DSDT
					char *S5Addr = (char *) facp->DSDT + 36; // skip header
					int dsdtLength = *(facp->DSDT + 1) - 36;
					while (0 < dsdtLength--) {
						if (memcmp(S5Addr, "_S5_", 4) == 0)
							break;
						S5Addr++;
					}
					// check if \_S5 was found
					if (dsdtLength > 0) {
						// check for valid AML structure
						if ((*(S5Addr - 1) == 0x08
								|| (*(S5Addr - 2) == 0x08
										&& *(S5Addr - 1) == '\\'))
								&& *(S5Addr + 4) == 0x12) {
							S5Addr += 5;
							S5Addr += ((*S5Addr & 0xC0) >> 6) + 2; // calculate PkgLength size

							if (*S5Addr == 0x0A)
								S5Addr++;   // skip uint8prefix
							SLP_TYPa = *(S5Addr) << 10;
							S5Addr++;

							if (*S5Addr == 0x0A)
								S5Addr++;   // skip 6prefix
							SLP_TYPb = *(S5Addr) << 10;

							SMI_CMD = facp->SMI_CMD;

							ACPI_ENABLE = facp->ACPI_ENABLE;
							ACPI_DISABLE = facp->ACPI_DISABLE;

							PM1a_CNT = facp->PM1a_CNT_BLK;
							PM1b_CNT = facp->PM1b_CNT_BLK;

							PM1_CNT_LEN = facp->PM1_CNT_LEN;

							SLP_EN = 1 << 13;
							SCI_EN = 1;

							return 0;
						} else {
							log("\\_S5 parse error.\n");
						}
					} else {
						log("\\_S5 not present.\n");
					}
				} else {
					log("DSDT invalid.\n");
				}
			}
			ptr++;
		}
		log("no valid FACP present.\n");
	} else {
		log("no acpi.\n");
	}

	return -1;
}

void acpiPowerOff(void) {
	// SCI_EN is set to 1 if acpi shutdown is possible
	if (SCI_EN == 0)
		return;

	acpiEnable();

	// send the shutdown command
	outw((uint64) PM1a_CNT, SLP_TYPa | SLP_EN);
	if (PM1b_CNT != 0)
		outw((uint64) PM1b_CNT, SLP_TYPb | SLP_EN);

	log("acpi poweroff failed.\n");
}
