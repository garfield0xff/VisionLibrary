#include <lidar_base.hpp>

namespace vl 
{
namespace lidar
{

bool BaseLidarController::setPort( String port ) 
{
    m_port = port;
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