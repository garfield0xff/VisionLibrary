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

    Header start_header = { START_BIT, LIDAR_START_SCAN };
    sendSerialHeader(start_header);
    return true;
}

bool YDLidarController::stopScan()
{
    Header stop_header = { START_BIT, LIDAR_STOP_SCAN };
    sendSerialHeader(stop_header);
    return true;
}

bool YDLidarController::sendSerialHeader(Header header) const
{
    ssize_t writen = write(m_fd, header.data(), header.size());
    if(writen < 0) return false;
    return true;
}

LidarController YDLidarController::newController() const 
{
    return std::make_shared<YDLidarController>();
}


}