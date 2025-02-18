#ifndef _LIDAR_BASE_FMT_
#define _LIDAR_BASE_FMT_

#include <vlstd.hpp>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <window_io.hpp>

namespace vl
{

class BaseLidarController;
typedef std::shared_ptr<BaseLidarController> LidarController;
typedef std::vector<uint8_t> Header;

enum LidarDevice 
{
    YDLidar = 1 << 0,
};

class BaseLidarController
{

public:
    BaseLidarController() {};
    ~BaseLidarController() {};

    virtual bool setPort( String port );
    virtual bool startScan();
    virtual bool stopScan();
    virtual LidarController newController() const;

protected:
    String m_port;
    Bps m_buadrate;
    FileDescriptor m_fd;
    unsigned int m_timeoutMS;
    bool m_connected;

    virtual bool sendSerialHeader(Header header) const;



}; // namespace vl

} // class BaseLidarController

#endif /*LIDARBASEFMT*/