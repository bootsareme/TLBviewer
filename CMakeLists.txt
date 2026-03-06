# CMakeList.txt : CMake project for TLBviewer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("TLBviewer")

# Add source to this project's executable.
add_executable (TLBviewer "main.cpp" "FormattedTable.h" "TLB.h" "PageTable_Entry.h" "Process_PageLookup.h" "FormattedTable.cpp" "Process_PageLookup.cpp" "TLB.cpp" "PageTable_Entry.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET TLBviewer PROPERTY CXX_STANDARD 20)
endif()