#include <stdint.h>

namespace response
{

namespace lidar
{

#pragma pack(push, 1)
struct YDLidarScanResponse
{
    uint16_t    pakcet_header;      //  2B PH   -  0x55AA 패킷 헤더
    uint8_t     packet_type;        //  1B CT   -  패킷 타입 및 주파수 정보
    uint8_t     sample_count;       //  1B LSN  -  샘플링 포인트 개수 -> 시작 패킷인경우 LSN = 1 else >1
    uint16_t    start_angle;        //  2B FSA  -  첫번째 포인트 클라우드 시작 각도
    uint16_t    end_angle;          //  2B LSA  -  마지막 포인트 클라우드 종료 각도
    uint16_t    check_code;         //  2B CS   -  XOR 체크 코드
    struct SampleNode               //  3B SI 
    {
        uint8_t     intensity;      //  1B SI(1B)  -   SI(1) 강도
        uint16_t    distance;       //  2B SI(2B)  -   Lshiftbit(Si(3)) + Rshiftbit(Si(2))
    } samples[];
};
#pragma pack(pop)

#pragma pack(push, 1)
struct YDLidarHealthResponse
{
    uint8_t     status_code;        //  1B  - 상태 확인 코드
    uint16_t    error_code;         //  2B  - 오류 확인 코드 
};
#pragma pack(pop)


#pragma pack(push, 1)
struct YDLidarDeviceInfoResponse
{
    uint8_t     model;               // 1B - 모델 코드
    uint8_t     firmware_minor;      // 1B - 펌웨어 버전 (Minor)
    uint8_t     firmware_major;      // 1B - 펌웨어 버전 (Major)
    uint8_t     hardware;            // 1B - 하드웨어 버전
    uint16_t    serial_number[16];    // 16B - 시리얼 넘버  
};
#pragma pack(pop)

#pragma pack(push, 1)
struct YDLidarScanFreqResponse
{
    uint32_t freq_data;             // 4B - 주파수 데이터 (Hz 단위, little-endian )
};
#pragma pack(pop)

}

}