#ifndef _LIDAR_RES_HPP_
#define _LIDAR_RES_HPP_

#include <cstdint>
#include <cstdio>
#include <memory>


namespace vl
{

/**
 * @brief little endian two byte -> 4btyte
 */
static inline uint16_t ToUint16LE(uint8_t low, uint8_t high)
{
    return static_cast<uint16_t>((high << 8) | low);
}


namespace lidar
{

struct PointCloud {
    float x;
    float y;
    float distance;
    uint8_t intensity;
    float angle_deg;
};


#pragma pack(push, 1)
/**
 * @brief A Struct that Decode YDLidarResponse
 */
struct TMiniHeader
{
    uint16_t header;                ///< PH  - 0x55AA Packet Header
    uint8_t  ct;                    ///< CT  - Scanning Frequency & package type
    uint8_t  lsn;                   ///< LSN - Sample Count
    uint16_t fsa;                   ///< FSA - angle in first sample point
    uint16_t lsa;                   ///< LSA - angle in last  sample point
    uint16_t cs;                    ///< CS  - xor check code
};
#pragma pack(pop)


enum class ResponseMode : uint8_t {
    SingleResponse      = 0x0,
    ContinuousResponse  = 0x1,
};

#pragma pack(push, 1)
struct SystemMessageHeader
{
    uint16_t start_sign;     // 0xA55A (리틀 엔디안이면 메모리에 0x5A,0xA5 형태)
    uint32_t length_mode;    // 하위 30비트 = length, 상위 2비트 = mode
    uint8_t  type_code;      // e.g. 0x81 (scan data)
};
#pragma pack(pop)

struct SampleNode
{
    uint8_t intensity;              ///< Si(1)  - xor check code
    uint8_t distanceLow;            ///< Si(2)  - distance low byte
    uint8_t distanceHighAndFlag;    ///< Si(3)  - distance hight byte
};


/**
 * @brief Decode YDLidar Response
 * @note  추후에 업캐스팅 클래스 만들어서 오버라이드 시켜야함, switch문도 수정 필요
 * 
 */
// bool decode(const uint8_t* data, size_t length);


} // namespace lidar
} // namespace vl

#endif // _LIDAR_RES_HPP