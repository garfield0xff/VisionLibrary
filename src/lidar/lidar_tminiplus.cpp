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
    m_response_type = RESPONSE_CONTINUOUS;
    if(!m_serial->write(start_bit, sizeof(start_bit))) {
        VL_LOG_DEBUG("WRITE", "START BIT FAILED");
    };

    m_isScanning = true;

    if(!createThread()) VL_LOG_DEBUG("THREAD", "CREATE ERROR");
    return true;
}

bool YDLidarController::createThread()
{
    tp = new ThreadPool(1);
    tp->createThread(&YDLidarController::readScanData, this);
    return true;
}

std::vector<PointCloud> YDLidarController::getLatestPointCloud() {
    std::unique_lock<std::mutex> lock(point_cloud_mutex);
    data_ready.wait(lock, [this] { return !point_cloud_queue.empty(); });

    std::vector<PointCloud> latest = std::move(point_cloud_queue.front());
    point_cloud_queue.pop();
    return latest;
}

std::queue<std::vector<PointCloud>> YDLidarController::getPointCloud() {
    std::unique_lock<std::mutex> lock(point_cloud_mutex);
    data_ready.wait(lock, [this] {return !point_cloud_queue.empty();});
    return point_cloud_queue;
}

void YDLidarController::showPointCloud() {
    float camera_fov_deg = 90.0f;
    std::vector<float> verticies;
    std::ofstream pcl_file("pointcloud_output.pcl", std::ofstream::trunc);

    if (!pcl_file.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return;
    }

    while (m_isScanning && !m_wfb->isCloseWindow()) {
        std::vector<PointCloud> latest_cloud = this->getLatestPointCloud();
        verticies.clear();
        
        for(int i = 0; i < latest_cloud.size(); i++) {
            float x = latest_cloud[i].x;
            float y = latest_cloud[i].y;
            float dist = latest_cloud[i].distance;
            verticies.push_back(latest_cloud[i].x);
            verticies.push_back(latest_cloud[i].y);
            verticies.push_back(0.0f);
            pcl_file << x << " " << y << " " << dist << "\n";
        }        

        m_wfb->render2dPoint(verticies, camera_fov_deg);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); 
    }
    pcl_file.close();
    m_wfb->closeWindow();
}

bool YDLidarController::runSerialLogger() {
    // m_scanThread = std::thread([this]() { readSerialLog();});
    return true;
}

bool YDLidarController::updateSerialState(int newFlag) {
    m_response_type = newFlag;
    return true;
}

void YDLidarController::readScanData()
{
    uint8_t buffer[512];
    size_t buffer_offset = 0;

    while (m_isScanning) {
        if (sizeof(buffer) - buffer_offset == 0) {
            buffer_offset = 0;
            continue;
        }

        size_t bytes_read = m_serial->read(buffer + buffer_offset, sizeof(buffer) - buffer_offset);
        if (bytes_read == 0) continue;

        buffer_offset += bytes_read;

        while (buffer_offset >= sizeof(TMiniHeader)) {
            TMiniHeader *hdr = reinterpret_cast<TMiniHeader *>(buffer);

            if (hdr->header != 0x55AA) {
                memmove(buffer, buffer + 1, --buffer_offset);
                continue;
            }

            size_t packet_size = sizeof(TMiniHeader) + static_cast<size_t>(hdr->lsn * 3);
            if (buffer_offset < packet_size) {
                break; 
            }

            float fsa_deg = (static_cast<float>(hdr->fsa >> 1)) / 64.0f;
            float lsa_deg = (static_cast<float>(hdr->lsa >> 1)) / 64.0f;

            std::vector<PointCloud> point_cloud;
            point_cloud.reserve(hdr->lsn);
        
        
            const uint8_t *sampleNode_ptr = buffer + sizeof(TMiniHeader);
            std::vector<PointCloud> point_cloud_per_node;
            point_cloud_per_node.reserve(hdr->lsn);

            for (size_t i = 0; i < hdr->lsn; i++) {
                uint8_t s1 = sampleNode_ptr[0];
                uint8_t s2 = sampleNode_ptr[1];
                uint8_t s3 = sampleNode_ptr[2];

                uint16_t distance = (static_cast<uint16_t>(s3) << 6 | static_cast<uint16_t>(s2) >> 2);

                float angle_deg = (hdr->lsn > 1)
                    ? fsa_deg + (lsa_deg - fsa_deg) * (static_cast<float>(i) / (hdr->lsn - 1))
                    : fsa_deg;

                float angle_rad = angle_deg * static_cast<float>(M_PI) / 180.0f;
                
                PointCloud pc;
                pc.x = (s1 > 50) ? static_cast<float>(distance) * std::cos(angle_rad) : 0;
                pc.y = (s1 > 50) ? static_cast<float>(distance) * std::sin(angle_rad) : 0;
                pc.intensity = s1;
                pc.distance  = static_cast<float>(distance);
                pc.angle_deg = angle_deg;

                point_cloud_per_node.push_back(pc);
            }

            this->updatePointCloud(std::move(point_cloud_per_node));
            

            buffer_offset -= packet_size;
            if (buffer_offset > 0) {
                memmove(buffer, buffer + packet_size, buffer_offset);
            }
        }
    }
}

bool YDLidarController::stopScan()
{
    uint8_t stop_bit[2] = {START_BIT, LIDAR_STOP_SCAN};
    m_serial->write(stop_bit, sizeof(stop_bit));
    m_isScanning = false;

    tp->joinAll();
    
    return true;
}

bool YDLidarController::setWindowFrameBuffer(WindowFrameBuffer* wfb) {
    m_wfb = wfb;
    return true;
}

bool YDLidarController::getScanFreq()
{
    uint8_t get_scan_freq_bit[2] = { START_BIT, LIDAR_GET_FREQ };
    m_serial->write(get_scan_freq_bit, sizeof(get_scan_freq_bit));
    m_isScanning = false;
    return true;
}

bool YDLidarController::getDeviceInfo()
{
    uint8_t get_device_info_header[2] = { START_BIT, LIDAR_GET_HEALTH };
    m_serial->write(get_device_info_header, sizeof(get_device_info_header));
    return true;
}


bool YDLidarController::IncreaseScanFreq0_1Hz() 
{
    uint8_t increase_0_1Hz_header[2] = { START_BIT, LIDAR_FREQ_UP_0_1HZ };
    m_serial->write(increase_0_1Hz_header, sizeof(increase_0_1Hz_header));
    return true;
}

bool YDLidarController::decreaseScanFreq0_1Hz()
{
    uint8_t decrease_0_1Hz_header[2] = { START_BIT, LIDAR_FREQ_DOWN_0_1HZ };
    m_serial->write(decrease_0_1Hz_header, sizeof(decrease_0_1Hz_header));
    return true;
}

bool YDLidarController::IncreaseScanFreq1Hz()
{
    uint8_t increase_1Hz_header[2] = { START_BIT, LIDAR_FREQ_UP_1HZ };
    m_serial->write(increase_1Hz_header, sizeof(increase_1Hz_header));
    return true;
}

bool YDLidarController::decreaseScanFreq1Hz()
{
    uint8_t decrease_1Hz_header[2] = { START_BIT, LIDAR_FREQ_DOWN_1HZ };
    m_serial->write(decrease_1Hz_header, sizeof(decrease_1Hz_header));
    return true;
}

bool YDLidarController::restart()
{
    uint8_t restart_header[2] = { START_BIT, LIDAR_SOFT_RESTART };
    m_serial->write(restart_header, sizeof(restart_header));
    return true;
}


LidarController YDLidarController::newController() const 
{
    return std::make_shared<YDLidarController>();
}
} //namespace lidar
} //namespace vl