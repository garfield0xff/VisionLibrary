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
    // sampling code, 
    Header start_header = { 0xA5, 0x65 };
    
    sendSerialHeader(start_header);
    return true;
}

bool BaseLidarController::stopScan()
{
    // sampling code
    Header stop_header = { 0xA5, 0x60 };
    sendSerialHeader(stop_header);
    return true;
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

bool BaseLidarController::sendSerialHeader(Header header) const
{
    ssize_t writen = write(m_fd, header.data(), header.size());
    if(writen < 0) return false;
    return true;
}

LidarController BaseLidarController::newController() const
{
    return LidarController();
}

} // namespace lidar
} // namespace vl