#include "Process_PageLookup.h"

#include <fcntl.h>
#include <unistd.h>

Process_PageLookup::Process_PageLookup(std::string pid, uint64_t va)
{
	this->virtual_addr = va;
	this->pid = pid;
}

const uint64_t Process_PageLookup::pte() const
{
    const std::string path = "/proc/" + this->pid + "/pagemap";
    int fd = open(path.c_str(), O_RDONLY);

    if (fd == -1)
    {
        close(fd);
        return 0; // page present determined by MSB
    }

    const long page_size = sysconf(_SC_PAGESIZE);
    const uint64_t pg_idx = this->virtual_addr / page_size * sizeof(uint64_t);
    uint64_t entry = 0;
    const ssize_t bytes_read = pread(fd, &entry, sizeof(entry), pg_idx);
    close(fd);

    if (bytes_read != sizeof(entry))
        return 0; // page present determined by MSB

    return entry;
}

const std::string Process_PageLookup::permissions() const
{
    const std::string path = "/proc/" + this->pid + "/maps";
    int fd = open(path.c_str(), O_RDONLY);

    if (fd == -1)
    {
        close(fd);
        return "----";
    }

    std::string buf;
    buf.reserve(4096);
    char tmp[1024];
    ssize_t n;

    while ((n = read(fd, tmp, sizeof(tmp))) > 0)
        buf.append(tmp, n);

    close(fd);
    size_t pos = 0;

    while (pos < buf.size())
    {
        size_t end = buf.find('\n', pos);
        if (end == std::string::npos)
            break;

        std::string line = buf.substr(pos, end - pos);
        pos = end + 1;
        const size_t dash = line.find('-');
        const size_t space = line.find(' ', dash + 1);

        if (dash == std::string::npos || space == std::string::npos)
            continue;

        uintptr_t start = std::stoull(line.substr(0, dash), nullptr, 16);
        uintptr_t end_addr = std::stoull(line.substr(dash + 1, space - dash - 1), nullptr, 16);

        if (this->virtual_addr < start || this->virtual_addr >= end_addr)
            continue;

        const size_t perms_start = space + 1;
        const size_t perms_end = line.find(' ', perms_start);

        if (perms_end == std::string::npos)
            continue;

        return line.substr(perms_start, perms_end - perms_start);
    }

    return "----";
}