#include "lidar/lidar_base.hpp"


namespace vl 
{
namespace lidar
{

bool BaseLidarController::loadLidar()
{

    std::vector<serial::PortInfo> lst = serial::list_ports();
    int select_id;

    int id = 0;

    if(lst.size() < 1)  {
        VL_LOG_DEBUG("ERROR", "DEVICE PORT DOES NOT FOUND");
        return false;
    }
    for(std::vector<serial::PortInfo>::iterator it = lst.begin(); it != lst.end(); it++) {
        printf("[%d] %s  %s \n", id, (*it).device_id.c_str(), (*it).friendly_name.c_str());   
        id++;
    }

    printf("Please select lidar port (number) : \n");

    cin >> select_id;

    if(select_id > lst.size() || select_id < 0) {
        return false;
    }

    m_port = lst[select_id].port;
    return true;
}

bool BaseLidarController::setBuadrate(int baudrate)
{
    m_baudrate = baudrate;
    return true;
}



bool BaseLidarController::startScan()
{
    
    uint8_t start_bit[2] = { 0xA5, 0x65 };
    m_serial->write(start_bit, sizeof(start_bit));
    return true;
}

bool BaseLidarController::stopScan()
{
    // sampling code
    uint8_t stop_bit[2] = { 0xA5, 0x65 };
    m_serial->write(stop_bit, sizeof(stop_bit));
    return true;
}

std::vector<PointCloud> BaseLidarController::getLatestPointCloud() {
    std::unique_lock<std::mutex> lock(point_cloud_mutex);
    data_ready.wait(lock, [this] { return !point_cloud_queue.empty(); });

    std::vector<PointCloud> latest = std::move(point_cloud_queue.front());
    point_cloud_queue.pop();
    return latest;
}

std::queue<std::vector<PointCloud>> BaseLidarController::getPointCloud() {
    std::unique_lock<std::mutex> lock(point_cloud_mutex);
    data_ready.wait(lock, [this] {return !point_cloud_queue.empty();});
    return point_cloud_queue;
}

void BaseLidarController::updatePointCloud(std::vector<PointCloud>&& _point_cloud) {
    if (_point_cloud.empty()) return;
    {
        std::lock_guard<std::mutex> lock(point_cloud_mutex);
        point_cloud_queue.push(std::move(_point_cloud));
    }

    data_ready.notify_one();
}


bool BaseLidarController::initialize()
{
    m_serial = new serial::Serial(m_port, m_baudrate);
    if(!m_serial->open()) {
        return false;
    };
    if(!m_serial->setBaudrate()) {
        return false;
    };
    return true;
}

LidarController BaseLidarController::newController() const
{
    return LidarController();
}

} // namespace lidar
} // namespace vl