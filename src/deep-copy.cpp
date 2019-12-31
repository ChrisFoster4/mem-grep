#include "deep-copy.hpp"

char* DeepCopy(const pid_t& pid, const void* start, const size_t& size) {
  if (start == nullptr){
    std::cerr << "DeepCopy requested to copy from nullptr\n";
    return nullptr;
  }

  char* mem_area = nullptr;
  try {
    mem_area = new char[size];
  } catch (std::bad_alloc& ac) {
    std::cerr << "WARNING: DeepCopy was requested to copy a large memory section of "
		 "size: "
	      << size << " memory could not be allocated to store this section as it is too large\n";
    return mem_area;
  }
  assert(mem_area != nullptr);

  struct iovec local[1];
  struct iovec remote[1];
  local[0].iov_base = mem_area;
  local[0].iov_len = size;
  remote[0].iov_base = const_cast<void*>(start);
  remote[0].iov_len = size;

  // process_vm_readv returns the number of bytes it managed to read.
  // This may be less than the number requested.
  // On error(i.e. invalid start address) the function returns -1 bytes read
  ssize_t nread = 0;
  nread = process_vm_readv(pid, local, 1, remote, 1, 0);
  if (nread != (ssize_t)size) {
    std::cerr << "Failed to copy memory from remote process at line:"
	      << __LINE__ << " in file:" << __FILE__ << "\n";
    std::cerr << "Read:" << nread << " bytes when: " << size
	      << " bytes should have been read from address: " << start << "\n";
    std::cerr << "Note this is NOT due to failing to allocate memory to store "
		 "the result of the copy\n";
    std::cerr << "Error string: " << strerror(errno) << std::endl;
    delete[] mem_area;
    return nullptr;
  }
  return mem_area;
}
