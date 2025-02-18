#ifndef _LIDAR_BASE_FMT_
#define _LIDAR_BASE_FMT_

#include <vlstd.hpp>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <window_io.hpp>
#include <lidar_res.hpp>

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
    virtual bool printSerialLog(int flag)        const = 0; // Todo : 추후 라이더 모듈이 추가되면 완성



}; // namespace vl

} // class BaseLidarController

#endif /*LIDARBASEFMT*/