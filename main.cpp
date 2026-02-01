#include "Process_PageLookup.h"
#include "FormattedTable.h"
#include "TLB.h"

#include <iostream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: ./tlbview [pid]";
        return 1;
    }

    // print cold TLB
    TLB tlb;
    std::cout << "TLB starts off cold (empty):\n";
    tlb.print();

ask_for_input:
    std::string va;
    std::cout << "\nEnter a virtual address to access for PID=" << argv[1] << "> ";

    if (!(std::cin >> va) || va == "exit" || va == "quit")
    {
        std::cout << std::endl;
        return 0;
    }

    uint64_t virt_addr = std::stoull(va, nullptr, 16);
    uint64_t virt_pg_num = virt_addr >> 12;
    short pg_offset = virt_addr & ((1 << 12) - 1);

    Process_PageLookup lookup(argv[1], virt_addr);
    std::string permissions = lookup.permissions();
    PageTable_Entry pte(lookup.pte());

    if (!pte.present)
    {
        if (pte.swapped)
            std::cerr << "\nVirtual address goes to swapped space! The TLB only caches translations for memory-resident pages.\n";
        else
            std::cerr << "\nVirtual address goes to invalid page table entry!\n";
        goto ask_for_input;
    }

    std::string flags = pte.get_flags();
    uint64_t pgcnt = pte.get_PageCount();

    if (tlb.access(virt_pg_num, pte.pfn, permissions, flags, pgcnt, pte.soft_dirty))
        std::cout << "\nTLB hit for virtual page 0x";
    else
        std::cout << "\nTLB miss for virtual page 0x";
        
    std::cout << std::hex << virt_pg_num << " with offset = 0x" << pg_offset << ":\n";
    tlb.print();
    goto ask_for_input;
}