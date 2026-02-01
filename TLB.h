#pragma once

#include <vector>
#include <string>
#include <cstdint>

#define WAYS 16 // TLB is usually fully associative with exactly 1 set and N ways

class TLB
{
public:
	explicit TLB(); // TLB starts cold (empty)
	void print(); // prints TLB contents in a formatted, text-based table
	bool access(uint64_t tag, uint64_t pfn, std::string permissions, std::string flags, uint64_t times_mapped, bool soft_dirty);

private:
	using Line = std::vector<std::string>; // TLB line is the meaningful contents (PFN) + metadata
	Line tlb[WAYS]; // internal array that stores the contents of the TLB
	short just_accessed; // internal marker for position that was just accessed
};