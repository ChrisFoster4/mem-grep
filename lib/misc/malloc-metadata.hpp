#ifndef MEMGREP_MALLOC_METADATA
#define MEMGREP_MALLOC_METADATA
#include <iostream>
#include "remote-memory.hpp"
#include "utils.hpp"

[[nodiscard]] size_t GetMallocMetadata(const void *heap_address, pid_t pid, size_t max_size,
		bool print_warnings = false, const bool is_local_address = false);
#endif
