#include <lidar_tminiplus.hpp>
#include <vector>

namespace vl {
namespace lidar{

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
    updateSerialState(RESPONSE_CONTINUOUS);
    runSerialLogger();
    
    return true;
}

bool YDLidarController::runSerialLogger() {
    m_scanThread = std::thread([this]() { readSerialLog();});
    return true;
}

bool YDLidarController::updateSerialState(int newFlag) {
    m_response_type = newFlag;
    return true;
}

bool YDLidarController::readSerialLog() 
{
    uint8_t buffer[2048];

    int total_sample_count = 0;  

    if(m_response_type == RESPONSE_CONTINUOUS)
    {
        while(m_isScanning.load())
        {
            int n = read(m_fd, buffer, sizeof(buffer));

            // TMiniHeader Byte
            if (n < 10) continue;

            auto* hdr = reinterpret_cast<const TMiniHeader*>(buffer);
            
            uint16_t checkSum = 0;
            uint16_t ct_lsn = ToUint16LE(hdr->ct, hdr->lsn);

            checkSum ^= hdr->header;
            checkSum ^= ct_lsn;
            checkSum ^= hdr->fsa;
            checkSum ^= hdr->lsa;
            

            if(hdr->header == 0x55AA) {
                int scan_packet_size = sizeof(TMiniHeader) + hdr -> lsn * 3;
                if(n < scan_packet_size) {
                    int remain = scan_packet_size - n;

                    int more = ::read(m_fd, buffer + n, remain);
                    if(more > 0) {
                        n += more;
                    }
                }

                if(n == scan_packet_size) {
                    const uint8_t* sampleNode_ptr = buffer + 10;

                    for(size_t i = 0; i < hdr->lsn; i++) {
                        uint8_t s1 = sampleNode_ptr[0];
                        uint8_t s2 = sampleNode_ptr[1];
                        uint8_t s3 = sampleNode_ptr[2];
                        
                        checkSum ^= ToUint16LE(s1, 0x00);
                        checkSum ^= ToUint16LE(s2, s3);

                        uint16_t distance = (static_cast<uint16_t>(s3) << 6) | 
                        (static_cast<uint16_t>(s2) >> 2);

                        sampleNode_ptr += 3;
                        std::cout << "distance : " <<  distance << std::endl;
                    }
                }
            } 
            // tcflush(m_fd, TCIOFLUSH);
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


} //namespace lidar
} //namespace vl