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



bool YDLidarController::startScan()
{
    window_io::SerialWindowBackend sw;
    sw.initTermios(m_fd);

    Header start_header = { START_BIT, LIDAR_STOP_SCAN };
    sendSerialHeader(start_header);
    // printSerialLog(LIDAR_START_SCAN);
    return true;
}

bool YDLidarController::printSerialLog(int flag) const
{
    typedef response::lidar::YDLidarScanResponse ScanRes;
    uint8_t buffer[2048];

    if(flag == 0x60)
    {
        std::cout << "print log" << std::endl;
        while(true)
        {
            int n = read(m_fd, buffer, sizeof(buffer));
            if (n < 0) {
                perror("Error reading from serial port");
                break;
            }

            if (n < 10) continue;

            if (buffer[0] == 0xAA && buffer[1] == 0x55) 
            {
                ScanRes* res = reinterpret_cast<ScanRes*>(buffer);
                std::cout << "Packet Type: 0x" << std::hex << (int)res->packet_type << std::endl;
                std::cout << "Sample Count: " << std::dec << (int)res->sample_count << std::endl;
                std::cout << "Start Angle: " << (res->start_angle >> 1) / 64.0 << "°" << std::endl;
                std::cout << "End Angle: " << (res->end_angle >> 1) / 64.0 << "°" << std::endl;
                // 샘플 데이터 출력
                for (int i = 0; i < res->sample_count; i++) {
                    ScanRes::SampleNode sample = res->samples[i];
                    float distance = sample.distance * 0.001; // mm -> m
                    std::cout << "Sample " << i + 1 << ": "
                        << "Distance: " << distance << " m, "
                << "Intensity: " << (int)sample.intensity << std::endl;
                }
                std::cout << "----------------------------\n" << std::endl;

            }
        }
    }
    return true;
}

bool YDLidarController::stopScan()
{
    Header stop_header = { START_BIT, LIDAR_STOP_SCAN };
    sendSerialHeader(stop_header);
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