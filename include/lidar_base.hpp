#ifndef _LIDAR_BASE_FMT_

#include <vlstd.hpp>


namespace vl
{

class BaseLidarController;
typedef std::shared_ptr<BaseLidarController> LidarController;

enum LidarDevice 
{
    YDLidar = 1 << 0,
};

class BaseLidarController
{

public:
    BaseLidarController();

    virtual ~BaseLidarController() {};

    virtual bool setPort( String port );
    virtual bool startScan();
    virtual bool stopScan();
    virtual LidarController newController() const;

protected:
    String m_port;
    Bps m_buadrate;
    unsigned int m_timeoutMS;
    bool m_connected;

}; // namespace vl

} // class BaseLidarController

#endif /*LIDARBASEFMT*/