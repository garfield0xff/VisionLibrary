#include "lidar/lidar_tminiplus.hpp"

#include <vector>

namespace vl {
namespace lidar{

YDLidarController::YDLidarController() 
{
    std::cout << "YDLidarCOntroller called" << std::endl;
}

YDLidarController::~YDLidarController()
{
    std::cout << "YDLidarController deleted" << std::endl;
}



bool YDLidarController::startScan()
{
    uint8_t start_bit[2] = {START_BIT, LIDAR_START_SCAN};
    if(!m_serial->write(start_bit, sizeof(start_bit))) {
        VL_LOG_DEBUG("WRITE", "START BIT FAILED");
    };
    
    return true;
}

std::vector<PointCloud> YDLidarController::getPointCloud() {
    std::lock_guard<std::mutex> lock(m_pointCloudMutex);
    return m_lastFrame;  
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
    
    std::vector<uint8_t> buffer;
    buffer.reserve(1024);

    uint8_t tmpBuffer[512];
    int total_sample_count = 0;

    if (m_response_type == RESPONSE_CONTINUOUS)
    {
        while (m_isScanning.load())
        {
            
            int n = read(m_fd, tmpBuffer, sizeof(tmpBuffer));
            if (n > 0)
            {
                buffer.insert(buffer.end(), tmpBuffer, tmpBuffer + n);
            }

            while (buffer.size() >= sizeof(TMiniHeader))
            {
                TMiniHeader* hdr = reinterpret_cast<TMiniHeader*>(buffer.data());
                if (hdr->header != 0x55AA)
                {
                    buffer.erase(buffer.begin());
                    continue;
                }

                int scan_packet_size = sizeof(TMiniHeader) + hdr->lsn * 3;
                if (buffer.size() < static_cast<size_t>(scan_packet_size))
                {
                    break;
                }

                float fsa_deg = (static_cast<float>(hdr->fsa >> 1)) / 64.0f;
                float lsa_deg = (static_cast<float>(hdr->lsa >> 1)) / 64.0f;

                std::vector<PointCloud> frame_per_pointCloud;
                frame_per_pointCloud.reserve(hdr->lsn);

                const uint8_t* sampleNode_ptr = buffer.data() + sizeof(TMiniHeader);
                uint16_t checkSum = 0;
                uint16_t ct_lsn = ToUint16LE(hdr->ct, hdr->lsn);
                checkSum ^= hdr->header;
                checkSum ^= ct_lsn;
                checkSum ^= hdr->fsa;
                checkSum ^= hdr->lsa;

                for (size_t i = 0; i < hdr->lsn; i++)
                {
                    uint8_t s1 = sampleNode_ptr[0]; // intensity
                    uint8_t s2 = sampleNode_ptr[1];
                    uint8_t s3 = sampleNode_ptr[2];

                    checkSum ^= ToUint16LE(s1, 0x00);
                    checkSum ^= ToUint16LE(s2, s3);

                    uint16_t distance = (static_cast<uint16_t>(s3) << 6) |
                                        (static_cast<uint16_t>(s2) >> 2);

                    float angle_deg;
                    if (hdr->lsn > 1)
                    {
                        angle_deg = fsa_deg + (lsa_deg - fsa_deg) * (static_cast<float>(i) / (hdr->lsn - 1));
                    }
                    else
                    {
                        angle_deg = fsa_deg;
                    }

                    float angle_rad = angle_deg * static_cast<float>(M_PI) / 180.0f;
                    float dist = static_cast<float>(distance);
                    float x = dist * std::cos(angle_rad);
                    float y = dist * std::sin(angle_rad);

                    PointCloud pc;
                    pc.x = x;
                    pc.y = y;
                    pc.distance = dist;
                    pc.intensity = s1;
                    pc.angle_deg = angle_deg;
                    frame_per_pointCloud.push_back(pc);

                    sampleNode_ptr += 3;
                } 

                {
                    std::lock_guard<std::mutex> lock(m_pointCloudMutex);
                    m_lastFrame = frame_per_pointCloud;
                }

                
                if (!frame_per_pointCloud.empty())
                {
                    for(int i = 0; i < frame_per_pointCloud.size(); i++) {
                    std::cout << "sample " << i <<  " : x = " 
                              << frame_per_pointCloud[i].x << ", y = "
                              << frame_per_pointCloud[i].y << std::endl;
                    }
                }
                total_sample_count += hdr->lsn;
 
                buffer.erase(buffer.begin(), buffer.begin() + scan_packet_size);
            }   
        } 
    }

    std::cout << "[INFO] Final Total Sample Count: " << total_sample_count << std::endl;
    return true;
}

bool YDLidarController::stopScan()
{
    uint8_t stop_bit[2] = {START_BIT, LIDAR_STOP_SCAN};
    m_serial->write(stop_bit, sizeof(stop_bit));
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