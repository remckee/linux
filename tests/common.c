// SPDX-License-Identifier: GPL-2.0-or-later
#include "tests/common.h"
#include <string.h>

#define INIT_MEMBLOCK_REGIONS			128
#define INIT_MEMBLOCK_RESERVED_REGIONS		INIT_MEMBLOCK_REGIONS
#define PREFIXES_LEN_MAX			256
#define DELIM					": "
#define DELIM_LEN				strlen(DELIM)

static struct test_memory memory_block;
static char __maybe_unused prefixes[PREFIXES_LEN_MAX];

void reset_memblock_regions(void)
{
	memset(memblock.memory.regions, 0,
	       memblock.memory.cnt * sizeof(struct memblock_region));
	memblock.memory.cnt	= 1;
	memblock.memory.max	= INIT_MEMBLOCK_REGIONS;
	memblock.memory.total_size = 0;

	memset(memblock.reserved.regions, 0,
	       memblock.reserved.cnt * sizeof(struct memblock_region));
	memblock.reserved.cnt	= 1;
	memblock.reserved.max	= INIT_MEMBLOCK_RESERVED_REGIONS;
	memblock.reserved.total_size = 0;
}

void reset_memblock_attributes(void)
{
	memblock.memory.name	= "memory";
	memblock.reserved.name	= "reserved";
	memblock.bottom_up	= false;
	memblock.current_limit	= MEMBLOCK_ALLOC_ANYWHERE;
}

void setup_memblock(void)
{
	reset_memblock_regions();
	memblock_add((phys_addr_t)memory_block.base, MEM_SIZE);
}

void dummy_physical_memory_init(void)
{
	memory_block.base = malloc(MEM_SIZE);
	assert(memory_block.base);
}

void dummy_physical_memory_cleanup(void)
{
	free(memory_block.base);
}

#ifdef VERBOSE
void test_fail(void)
{
	ksft_test_result_fail(": %sfailed\n", prefixes);
}

void test_pass(void)
{
	ksft_test_result_pass(": %spassed\n", prefixes);
}

void test_print(const char *fmt, ...)
{
	int saved_errno = errno;
	va_list args;

	va_start(args, fmt);
	errno = saved_errno;
	vprintf(fmt, args);
	va_end(args);
}

void prefix_reset(void)
{
	memset(prefixes, 0, PREFIXES_LEN_MAX);
}

void prefix_push(const char *prefix)
{
	int prefix_max = PREFIXES_LEN_MAX - strlen(prefixes) - DELIM_LEN - 1;

	assert(strlen(prefix) <= prefix_max);
	strncat(prefixes, prefix, prefix_max);
	strncat(prefixes, DELIM, PREFIXES_LEN_MAX - strlen(prefixes) - 1);
}

void prefix_pop(void)
{
	char *ptr;

	ptr = strrchr(prefixes, DELIM[0]);
	if (!ptr)
		prefixes[0] = 0;
	else
		*(ptr) = 0;

	ptr = strrchr(prefixes, DELIM[0]);
	if (!ptr)
		prefixes[0] = 0;
	else
		*(ptr + DELIM_LEN) = 0;
}
#endif /* VERBOSE */
