#ifndef _LIDAR_TMINIPLUS_FMT_
#define _LIDAR_TMINIPLUS_FMT_

#include <lidar_base.hpp>
#include <sys/termios.h>

namespace vl {

#define START_BIT 0xA5



enum YDLidarScanModes
{
    LIDAR_START_SCAN        = 0x60,
    LIDAR_STOP_SCAN         = 0x65,
    LIDAR_GET_INFO          = 0x90,
    LIDAR_GET_HEALTH        = 0x92,
    LIDAR_FREQ_UP_0_1HZ     = 0x09,
    LIDAR_FREQ_DOWN_0_1HZ   = 0x0A,
    LIDAR_FREQ_UP_1HZ       = 0x0B,
    LIDAR_FREQ_DOWN_1HZ     = 0x0C,
    LIDAR_GET_FREQ          = 0x0D,
    LIDAR_SOFT_RESTART      = 0x40,
};

class YDLidarController : public BaseLidarController 
{

public:
    YDLidarController();
    ~YDLidarController();


    bool setPort( String port)                  override;
    bool startScan()                            override;
    bool stopScan()                             override;
    LidarController newController()      const  override;

    bool getDeviceInfo();
    bool getScanFreq();
    bool IncreaseScanFreq0_1Hz();
    bool decreaseScanFreq0_1Hz();
    bool IncreaseScanFreq1Hz();
    bool decreaseScanFreq1Hz();
    bool restart();


protected:
    bool sendSerialHeader(Header header) const  override;
    bool printSerialLog(int flag)       const  override;

}; // class YDLidarController


} // namespace vl

#endif /*LIDARTMINIPLUSFMT*/