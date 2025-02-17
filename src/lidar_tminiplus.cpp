#include <lidar_tminiplus.hpp>
#include <vector>


namespace vl {


YDLidarController::YDLidarController() 
{
    m_buadrate = B230400;
}

bool YDLidarController::setPort(String port)
{
    m_port = port;
    return true;
}

bool YDLidarController::startScan()
{
    std::vector<uint8_t> cmd = { START_BIT, LIDAR_START_SCAN };
    return true;
}


LidarController YDLidarController::newController() const 
{
    return std::make_shared<YDLidarController>();
}


}