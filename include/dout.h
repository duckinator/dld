#ifndef DOUT_H
#define DOUT_H

#include <stdint.h>

#define DOUT_MAGIC 0x8badf00d

struct dout_hdr {
	uint32_t magic; /* Always 0x8badf00d */
	uint32_t loc; /* Location to load binary image */
	uint32_t start; /* First code to execute */
	uint32_t len; /* Length of binary image */

	uint32_t text; /* Start of .text */
	uint32_t textloc; /* Location of .text */
	uint32_t textlen; /* Length of .text */
	uint32_t data; /* Start of .data */
	uint32_t dataloc; /* Location of .data */
	uint32_t datalen; /* Length of .data */
	uint32_t bss; /* Start of .bss */
	uint32_t bssloc; /* Location of .bss */
	uint32_t bsslen; /* Length of .bss */

	uint32_t symbols; /* Start of symbol table */
};

#endif
