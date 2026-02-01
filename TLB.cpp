#include "TLB.h"
#include "FormattedTable.h"

#include <iostream>
#include <sstream>

#define BOOL2STR(x) x ? "true" : "false"

TLB::TLB()
{
	for (size_t i = 0; i < WAYS; i++)
		this->tlb[i] = { "N/A", "N/A", "----", "N/A", "N/A", "N/A" };
}

void TLB::print()
{
	FormattedTable tbl({ "tag (virtual page #)", "physical frame #", "permissions", "flags", "times mapped", "dirty?" });
	for (short i = 0; i < WAYS; i++)
		tbl.addRow(this->tlb[i]);
	std::cout << tbl.format(this->just_accessed);
}

// returns true if hit, false if miss
bool TLB::access(uint64_t tag, uint64_t pfn, std::string permissions, std::string flags, uint64_t times_mapped, bool soft_dirty)
{
	// search TLB for tag
	for (short i = 0; i < WAYS; i++)
	{
		std::stringstream ss;
		ss << std::hex << tag;

		if (this->tlb[i][0] == "0x" + ss.str())
		{
			// hit!
			this->just_accessed = i;
			return true;
		}
	}

	// random replacement policy works surprisingly well:
	//	see https://www.cs.virginia.edu/~cr4bd/3130/S2026/slides/caching.pdf#page=84,
	//	and https://pages.cs.wisc.edu/~shivaram/cs537-sp23-notes/vm-tlbs/cs537-vm-tlbs-notes.pdf#page=18,
	//	and https://www.cs.utexas.edu/~dahlin/Classes/UGOS/lectures/lec16.pdf#page=3

	std::stringstream ss;
	ss << std::hex << "0x" << tag;
	std::string formatted_tag = ss.str();

	ss.str("");
	ss.clear();

	ss << std::hex << "0x" << pfn;
	std::string formatted_pfn = ss.str();

	this->just_accessed = static_cast<short>(rand() % WAYS);
	this->tlb[this->just_accessed] = { formatted_tag, formatted_pfn, permissions, flags, std::to_string(times_mapped), BOOL2STR(soft_dirty)};
	return false;
}