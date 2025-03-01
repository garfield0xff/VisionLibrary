#include <lidar_res.hpp>
#include <logger.hpp>
#include <string.h>
#include <ostream>

namespace vl {
namespace lidar {

void decode(const uint8_t* data, size_t size)
{
    
    if (size < sizeof(YDLidarDecoder)) return;

    const YDLidarDecoder* decoder = reinterpret_cast<const YDLidarDecoder*>(data);

    printf("Packet Type: %u\n", decoder->packet_type);

    switch (decoder->packet_header)
    {
        case 0x55AA: // Scan Response
            if(decoder->sample_count == 0x01)break;
            

            for(int i = 0; i < decoder->sample_count; i++)
            {
                auto& sample = decoder->data.scan.samples[i];
                uint16_t distance = (static_cast<uint16_t>(sample.distance_high) << 6) | 
                        (static_cast<uint16_t>(sample.distance_low) >> 2);

                uint8_t flag = (sample.distance_low & 0xC0) >> 6;
                if (sample.intensity == 0 || sample.intensity == 255) continue;
                if (distance == 0 || distance > 4000) continue;
                if (flag == 2 || flag == 3) continue;
    
                printf("Sample %d - Intensity: %u, Distance: %.4f\n", i, sample.intensity, distance * 0.001);
            }

            // printf("Scan Response - Start Angle: %u, End Angle: %u\n",
            //        decoder->data.scan.start_angle,
            //        decoder->data.scan.end_angle);
            break;

        case 0x02: // Health Response
            printf("Health Response - Status: %u, Error Code: %u\n",
                   decoder->data.health.status_code,
                   decoder->data.health.error_code);
            break;

        case 0x03: // Device Info Response
            printf("Device Info - Model: %u, Firmware: %u.%u\n",
                   decoder->data.device_info.model,
                   decoder->data.device_info.firmware_major,
                   decoder->data.device_info.firmware_minor);
            break;

        case 0x04: // Scan Frequency Response
            printf("Scan Frequency: %u Hz\n", decoder->data.scan_freq.freq_data);
            break;

        default:
            printf("Unknown Response Type\n");
            break;
    }
}

} // namespace vl
} // namespace lidar