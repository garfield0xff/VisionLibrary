#include <lidar_tminiplus.hpp>

#include <vector>


namespace vl {


YDLidarController::YDLidarController() 
{
    m_buadrate = B230400;
    std::cout << "YDLidarCOntroller called" << std::endl;
}

YDLidarController::~YDLidarController()
{
    std::cout << "YDLidarController deleted" << std::endl;
}

bool YDLidarController::setPort(String port)
{
    m_port = port; 
    m_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC ); 
    if(m_fd < 0) std::cerr << "[Error] failed to open" << std::endl;
    return true;
}


// bool isValidChecksum(uint8_t* buffer, int len) {
//     uint16_t calculated_checksum = 0;
//     for (int i = 0; i < len - 2; i++) {
//         calculated_checksum ^= buffer[i];  // XOR checksum 계산
//     }
//     uint16_t received_checksum = (buffer[len - 2] << 8) | buffer[len - 1];
//     return calculated_checksum == received_checksum;
// }


bool YDLidarController::startScan()
{
    if (m_isScanning.load())
    {
        std::cout << "[INFO] LIDAR iS ALREADY SCANNING" << std::endl;
        return false;
    }

    window_io::SerialWindowBackend sw;
    sw.initTermios(m_fd);

    Header start_header = { START_BIT, LIDAR_START_SCAN };
    sendSerialHeader(start_header);
    
    m_isScanning.store(true);
    int flag = RESPONSE_CONTINUOUS;
    m_scanThread = std::thread([this, flag](){ this->printSerialLog(flag);});

    return true;
}

bool YDLidarController::printSerialLog(int flag) const
{
    typedef response::lidar::YDLidarScanResponse ScanRes;
    uint8_t buffer[2048];

    int total_sample_count = 0;  

    if(flag == RESPONSE_CONTINUOUS)
    {
        while(m_isScanning.load())
        {
            int n = read(m_fd, buffer, sizeof(buffer));
            if (n < 10) continue;

            // 🛑 패킷 헤더 검증
            if (buffer[0] != 0xAA || buffer[1] != 0x55) {
                std::cerr << "[ERROR] Packet Header Mismatch! Skipping packet..." << std::endl;
                continue;
            } 

            ScanRes* res = reinterpret_cast<ScanRes*>(buffer);

            if(res->sample_count != 0x01) {
                total_sample_count += res->sample_count;
                // std::cout << "Current Packet Sample Count: " << int(res->sample_count) << std::endl;
                // std::cout << "Total Accumulated Samples: " << total_sample_count << std::endl;

                for (int i = 0; i < res->sample_count; i++) {
                    ScanRes::SampleNode sample = res->samples[i];
                    if(sample.intensity > 0x00)
                    {
                        uint16_t distance =  static_cast<uint16_t>(( static_cast<uint16_t>(sample.distance_high) << 6 ) + (static_cast<uint16_t>(sample.distance_low) >> 2));
                        uint8_t flag = ( sample.distance_low & 0xC0 ) >> 6;
                        if(distance > 5000) std::cout << "overflow!!!!!!" << std::endl;
                        
                        std::cout << "Sample " << i + 1 << ": ";
                        std::cout << "Flag " << int(flag) << " ";
                        std::cout << "Distance: " << std::dec <<  distance * 0.001 << " m, ";
                        std::cout << "Intensity: " << (int)sample.intensity << " " << std::endl;
                        std::cout << "----------------------------\n" << std::endl;
                        
                    }
                }
            }

            // 🛑 시리얼 버퍼 주기적으로 비우기
            tcflush(m_fd, TCIOFLUSH);
        }
    }

    // std::cout << "[INFO] Final Total Sample Count: " << total_sample_count << std::endl;
    return true;
}



bool YDLidarController::stopScan()
{
    if(!m_isScanning.load())
    {
        std::cout << "[INFO] LIDAR is not currently scanning " << std::endl;
        return false;
    }

    Header stop_header = { START_BIT, LIDAR_STOP_SCAN };
    sendSerialHeader(stop_header);

    m_isScanning.store(false);

    if(m_scanThread.joinable()) m_scanThread.join();
    return true;
}

bool YDLidarController::getScanFreq()
{
    Header get_scan_freq_header = { START_BIT, LIDAR_GET_FREQ };
    sendSerialHeader(get_scan_freq_header);
    return true;
}

bool YDLidarController::getDeviceInfo()
{
    Header get_device_info_header = { START_BIT, LIDAR_GET_HEALTH };
    sendSerialHeader(get_device_info_header);
    return true;
}


bool YDLidarController::IncreaseScanFreq0_1Hz() 
{
    Header increase_0_1Hz_header = { START_BIT, LIDAR_FREQ_UP_0_1HZ };
    sendSerialHeader(increase_0_1Hz_header);
    return true;
}

bool YDLidarController::decreaseScanFreq0_1Hz()
{
    Header decrease_0_1Hz_header = { START_BIT, LIDAR_FREQ_DOWN_0_1HZ };
    sendSerialHeader(decrease_0_1Hz_header);
    return true;
}

bool YDLidarController::IncreaseScanFreq1Hz()
{
    Header increase_1Hz_header = { START_BIT, LIDAR_FREQ_UP_1HZ };
    sendSerialHeader(increase_1Hz_header);
    return true;
}

bool YDLidarController::decreaseScanFreq1Hz()
{
    Header decrease_1Hz_header = { START_BIT, LIDAR_FREQ_DOWN_1HZ };
    sendSerialHeader(decrease_1Hz_header);
    return true;
}

bool YDLidarController::restart()
{
    Header restart_header = { START_BIT, LIDAR_SOFT_RESTART };
    sendSerialHeader(restart_header);
    return true;
}


LidarController YDLidarController::newController() const 
{
    return std::make_shared<YDLidarController>();
}

bool YDLidarController::sendSerialHeader(Header header) const
{
    ssize_t writen = write(m_fd, header.data(), header.size());
    if(writen < 0) return false;
    return true;
}


}