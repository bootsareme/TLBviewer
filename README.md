# TLB viewer
This quasi-TLB simulates the contents of a potentially real hardware translation lookaside buffer as you access virtual memory in userspace. It uses actual page table entries provided by the Linux kernel, which allows you to view real physical addresses rather than toy examples.

*IMPORTANT NOTE:* You must run this program as **root**. Otherwise, the output will look convincing but meaningless.

## Assumptions
Although the design of TLBs may vary between processors, the general layout remains the same. This TLB notably includes some interesting stats that are not present in actual hardware TLBs, such as the number of times the physical frame was mapped and the software-controlled dirty bit (hardware tracks differently). Other assumptions include:
- TLB is fully associative,
- 64-bit x86 page tables with 48-bit virtual addresses,
- default number of ways is 16 (can be adjusted in `TLB.h`),
- replacement policy is random (works fairly well, see comment in `TLB.cpp`).

Some microarchitectural fields present in the actual TLB are impossible to infer from software and therefore cannot be displayed.

## Example Usage
1. Download the executable in *Releases*.
2. Give it appropriate permissions (`chmod +x tlbview`).
3. Get the PID of process you want to analyze using something like `pgrep process_name`.
4. Pass the PID into the viewer: `sudo ./tlbview PID`. 
5. Enter virtual addresses that you want to access (check `/proc/PID/maps` for valid ranges) and the TLB will populate appropriately.
6. Type "exit" or "quit" to terminate the program.

## Advanced Usage
As `tlbview` is a standard text-based tool that takes input from both `argv` and `stdin`, you can use shell commands to speed up your workflow and deepen your analysis. For example, the following allows you to see the TLB's simulated behavior when **every** mapped page is accessed:

```sh
PID=1234;./pagemap2 $PID | grep -v '=' | awk '{print $1}' | sudo ./tlbview $PID
```

The `pagemap2` executable can be built by cloning https://github.com/dwks/pagemap and `make`ing. You can also run the following to get hit/miss stats:

```sh
PID=1234;./pagemap2 $PID | grep -v '=' | awk '{print $1}' | sudo ./tlbview $PID
 | grep "hit" | wc -l
```
Output can be redirected to a text file for further inspection, packaging, and parsing.
## Sample Output

```
Enter a virtual address to access for PID=295> 0x7d6fb9919437

TLB hit for virtual page 0x7d6fb9919 with offset = 0x437:
    tag (virtual page #) | physical frame # | permissions |                  flags                  | times mapped | dirty?
   ----------------------+------------------+-------------+-----------------------------------------+--------------+--------
        0x7d6fb97d8      |     0x1799ae     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      18      |  true
        0x7d6fb9918      |     0x188c36     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      7       |  true
        0x7d6fb97ff      |     0x1a3c9a     |    r--p     | UPTODATE,DIRTY,LRU,MMAP,ANON,SWAPBACKED |      1       |  true
        0x7d6fb9809      |     0x195f8c     |    rw-p     |    UPTODATE,LRU,MMAP,ANON,SWAPBACKED    |      1       |  true
        0x7d6fb97cf      |     0x1799a5     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      21      |  true
        0x7d6fb9803      |     0x10e9c9     |    rw-p     |    UPTODATE,LRU,MMAP,ANON,SWAPBACKED    |      1       |  true
        0x7d6fb9916      |     0x188c34     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      7       |  true
        0x7d6fb97db      |     0x1799b1     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      18      |  true
        0x7d6fb9800      |     0x187c97     |    r--p     | UPTODATE,DIRTY,LRU,MMAP,ANON,SWAPBACKED |      1       |  true
        0x7d6fb97de      |     0x1796b2     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      18      |  true
 ->     0x7d6fb9919      |     0x188c37     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      7       |  true
        0x7d6fb97ab      |     0x17968e     |    r-xp     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      16      |  true
        0x7d6fb9917      |     0x188c35     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      7       |  true
        0x7d6fb9810      |     0x16995c     |    rw-p     | UPTODATE,DIRTY,LRU,MMAP,ANON,SWAPBACKED |      1       |  true
        0x7d6fb97d4      |     0x1799aa     |    r--p     |   REFERENCED,UPTODATE,LRU,ACTIVE,MMAP   |      18      |  true
        0x7d6fb9811      |     0x195f8a     |    rw-p     |    UPTODATE,LRU,MMAP,ANON,SWAPBACKED    |      1       |  true```
```