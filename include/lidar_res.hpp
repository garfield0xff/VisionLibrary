#ifndef _LIDAR_RES_HPP_
#define _LIDAR_RES_HPP_

#include <cstdint>
#include <cstdio>

namespace vl
{
namespace lidar
{

#pragma pack(push, 1)
/**
 * @brief A Class that Decode YDLidarResponse
 */
struct YDLidarDecoder
{
    
    uint16_t    packet_header;          ///< PH  - 0x55AA Packet Header
    uint8_t     packet_type;            ///< CT  - Scanning Frequency & package type
    uint8_t     sample_count;           ///< LSN - Sample Count 

    /**
     * @brief A data structure to store lidar response
     * 
     */
    union Response
    {
        struct ScanResponse
        {
            uint16_t start_angle;       ///< FSA - angle in first sample point
            uint16_t end_angle;         ///< LSA - angle in last  sample point
            uint16_t check_code;        ///< CS  - xor check code 
            struct SampleNode           
            {
                uint8_t intensity;      ///< S(1) - signal intensity
                uint8_t distance_low;   ///< S(2) - distance low byte
                uint8_t distance_high;  ///< S(3) - distance hight byte
            } samples[];
        } scan;

        struct HealthResponse
        {
            uint8_t  status_code;
            uint16_t error_code;
        } health;

        struct DeviceInfoResponse
        {
            uint8_t  model;
            uint8_t  firmware_minor;
            uint8_t  firmware_major;
            uint8_t  hardware;
            uint16_t serial_number[16];
        } device_info;

        struct ScanFreqResponse
        {
            uint32_t freq_data;
        } scan_freq;

    } data;
};
#pragma pack(pop)

/**
 * @brief Decode YDLidar Response
 * @note  추후에 업캐스팅 클래스 만들어서 오버라이드 시켜야함, switch문도 수정 필요
 * 
 */
void decode(const uint8_t* raw_data, size_t size);


} // namespace lidar
} // namespace vl

#endif // _LIDAR_RES_HPP