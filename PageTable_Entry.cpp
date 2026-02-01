#include "PageTable_Entry.h"

#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <sstream>

static const char* flag_names[64] = {
	"LOCKED",
	"ERROR",
	"REFERENCED",
	"UPTODATE",
	"DIRTY",
	"LRU",
	"ACTIVE",
	"SLAB",
	"WRITEBACK",
	"RECLAIM",
	"BUDDY",
	"MMAP",
	"ANON",
	"SWAPCACHE",
	"SWAPBACKED",
	"COMPOUND_HEAD",
	"COMPOUND_TAIL",
	"HUGE",
	"UNEVICTABLE",
	"HWPOISON",
	"NOPAGE",
	"KSM",
	"THP",
	"BALLOON",
	"ZERO_PAGE",
	"IDLE",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "",
	"", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""
};

PageTable_Entry::PageTable_Entry(uint64_t pte)
{
	this->present = (pte >> 63) & 1;
	this->swapped = (pte >> 62) & 1;
	this->soft_dirty = (pte >> 55) & 1;

	if (this->present) // bit 63 indicates if page mapping is present
		this->pfn = pte & ((1ULL << 55) - 1);
}

std::string PageTable_Entry::get_flags()
{
	uint64_t flags;
	int fd = open("/proc/kpageflags", O_RDONLY);

	if (fd == -1 || !this->pfn)
	{
		close(fd);
		return "N/A"; // cannot open file
	}

	off_t offset = static_cast<off_t>(this->pfn) * sizeof(uint64_t);
	pread(fd, &flags, sizeof(uint64_t), offset);
	close(fd);
	std::vector<std::string> set_flags;

	for (int i = 0; i < 64; ++i) 
		if (flags & (1ULL << i))
			if (flag_names[i][0] != '\0')  // only include known flags
				set_flags.push_back(flag_names[i]);

	// Join the set flags into a single string
	std::ostringstream oss;
	for (size_t i = 0; i < set_flags.size(); ++i) {
		if (i > 0) oss << ",";
		oss << set_flags[i];
	}
	return oss.str();
}

uint64_t PageTable_Entry::get_PageCount()
{
	uint64_t count;
	int fd = open("/proc/kpagecount", O_RDONLY);

	if (fd == -1 || !this->pfn)
	{
		close(fd);
		return fd; // cannot open file
	}

	off_t offset = static_cast<off_t>(this->pfn) * sizeof(uint64_t);
	pread(fd, &count, sizeof(uint64_t), offset);
	close(fd);
	return count;
}