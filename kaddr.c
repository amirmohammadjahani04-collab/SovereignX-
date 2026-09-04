#include "kaddr.h"
#include "limine.h"

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request kaddr_request = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
    .revision = 0
};

uint64_t virt_to_phys(uint64_t virt)
{
    uint64_t vbase = kaddr_request.response->virtual_base;
    uint64_t pbase = kaddr_request.response->physical_base;
    return virt - vbase + pbase;
}
