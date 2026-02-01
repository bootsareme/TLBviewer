#pragma once


#include "PageTable_Entry.h"

class Process_PageLookup
{
public:
	explicit Process_PageLookup(std::string pid, uint64_t va);
	const uint64_t pte() const;
	const std::string permissions() const;

private:
	std::string pid;
	uint64_t virtual_addr;
};