#ifndef _VLSTD_H_
#define _VLSTD_H_

#include <string.h>
#include <iostream>


namespace vl {

typedef std::string String;

// lidar
typedef unsigned long Bps;

// Descriptor
typedef int FileDescriptor;

inline uint16_t ToUint16LE(uint8_t low, uint8_t high) {
    return static_cast<uint16_t>(low) | (static_cast<uint16_t>(high) << 8);
}

}

#endif /*_VLSTD_H_*/