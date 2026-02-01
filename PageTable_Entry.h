#pragma once

#include <cstdint>
#include <string>

/* info below from https://www.kernel.org/doc/Documentation/vm/pagemap.txt */
struct PageTable_Entry
{
	explicit PageTable_Entry(uint64_t pte);
	uint64_t get_PageCount();
	std::string get_flags();

	uint64_t pfn = 0; // Bits 0 - 54  page frame number(PFN) if present	
	bool swapped = false; // Bit  62    page swapped
	bool present = false; // Bit  63    page present
	bool soft_dirty = false; // Bit  55    pte is soft-dirty
};