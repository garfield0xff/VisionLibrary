#include <lidar_res.hpp>
#include <logger.hpp>
#include <string.h>
#include <ostream>
#include <iostream>
#include <format>


namespace vl {
namespace lidar {

bool YDLidarNormalization_Is_Valid(uint8_t intensity ,uint16_t distance, uint8_t flag) {
    uint8_t mask = 0;
    
    mask |= ((intensity & 0xFF) * (intensity ^ 255) == 0) << 0;
    mask |= ((distance == 0) | (distance > 4000)) << 1;
    mask |= ((flag & ~1) == 2) << 2;

    return mask == 0;
}

uint16_t computeTminiCheckSum(const uint8_t* data, size_t length)
{
    if (length < sizeof(TMiniHeader)) {
        throw std::runtime_error("T-mini packet too short");
    }
    const auto* hdr = reinterpret_cast<const TMiniHeader*>(data);

    uint16_t xor_val = 0;
    uint16_t ct_lsn  = ToUint16LE(hdr->ct, hdr->lsn);

    xor_val ^= hdr->header;
    xor_val ^= ct_lsn;
    xor_val ^= hdr->fsa;
    xor_val ^= hdr->lsa;
    // hdr->cs는 계산에 포함 안 함

    size_t sample_count = hdr->lsn;
    const size_t needed = sample_count * 3;
    if (sizeof(TMiniHeader) + needed > length) {
        throw std::runtime_error("Not enough bytes for sample nodes");
    }

    const uint8_t* sample_ptr = data + sizeof(TMiniHeader);
    for (size_t i = 0; i < sample_count; i++)
    {
        uint8_t s1 = sample_ptr[0];
        uint8_t s2 = sample_ptr[1];
        uint8_t s3 = sample_ptr[2];

        // 첫 바이트 -> (0x00, s1) 16비트 XOR
        xor_val ^= ToUint16LE(s1, 0x00);
        // 뒤 2바이트 -> (s2, s3) 16비트 XOR
        xor_val ^= ToUint16LE(s2, s3);

        sample_ptr += 3;
    }

    return xor_val;
}

bool verifyTminiPacket(const uint8_t* data, size_t length)
{
    if (length < sizeof(TMiniHeader))
        return false;
    const auto* hdr = reinterpret_cast<const TMiniHeader*>(data);

    uint16_t cs_in_packet = hdr->cs;

    std::cout << std::format("header 0x{:X}", hdr->header) << std::endl;
    // 0x0

    if(hdr->header == 0x55AA) {
        // 64
        std::cout << "length is : " << length << std::endl;
        std::cout << std::format("header 0x{:X}", hdr->ct) << std::endl;
        // 0x28
        std::cout << std::format("header 0x{:X}", hdr->lsn) << std::endl;
        // 0x6143
        std::cout << std::format("header 0x{:X}", hdr->fsa) << std::endl;
        // 0x6E97
        std::cout << std::format("header 0x{:X}", hdr->lsa) << std::endl;
        // 0x4E88
        std::cout << std::format("header 0x{:X}", hdr->cs) << std::endl;

        size_t sample_count = hdr->lsn;
        

    }
    

    uint16_t calc = 0;
    try {
        
    }
    catch(...) {
        return false;
    }
    return (calc == cs_in_packet);
}

// bool decode(const uint8_t* data, size_t length)
// {

// }


} // namespace vl
} // namespace lidar