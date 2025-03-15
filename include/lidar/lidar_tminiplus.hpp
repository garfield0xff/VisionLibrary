#ifndef _LIDAR_TMINIPLUS_FMT_
#define _LIDAR_TMINIPLUS_FMT_

#include "lidar/lidar_base.hpp"

namespace vl {

namespace lidar 
{

#pragma pack(push, 1)
/**
 * @brief A Struct that Decode YDLidarResponse
 */
struct TMiniHeader
{
    uint16_t header;                ///< PH  - 0x55AA Packet Header
    uint8_t  ct;                    ///< CT  - Scanning Frequency & package type
    uint8_t  lsn;                   ///< LSN - Sample Count
    uint16_t fsa;                   ///< FSA - angle in first sample point
    uint16_t lsa;                   ///< LSA - angle in last  sample point
    uint16_t cs;                    ///< CS  - xor check code
};
#pragma pack(pop)


struct SampleNode
{
    uint8_t intensity;              ///< Si(1)  - xor check code
    uint8_t distanceLow;            ///< Si(2)  - distance low byte
    uint8_t distanceHighAndFlag;    ///< Si(3)  - distance hight byte
};


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


enum YDLidarResponseMode
{
    RESPONSE_CONTINUOUS,
    RESPONSE_NO_ANSWER,
    RESPONSE_SINGLE_RESPONSE,
};

/**
 * @class YDLidarController
 * @brief A class that provides to control YDLidar Device
 * 
 */
class YDLidarController : public BaseLidarController 
{

public:

    /**
     * @brief  Default constructor for YDLidarController.
     */
    YDLidarController();

    /**
     * @brief  Destructor for YDLidarController.
     */
    ~YDLidarController();


    bool startScan()                            override;
    bool stopScan()                             override;
    void readScanData()                         override;
    LidarController newController()      const  override;
    std::vector<PointCloud> getLatestPointCloud() override;
    virtual std::queue<std::vector<PointCloud>> getPointCloud() override;


    void showPointCloud(); 
    bool getDeviceInfo();


    bool getScanFreq();
    bool IncreaseScanFreq0_1Hz();
    bool decreaseScanFreq0_1Hz();
    bool IncreaseScanFreq1Hz();
    bool decreaseScanFreq1Hz();
    bool restart();
    bool setWindowFrameBuffer(WindowFrameBuffer* wfb);
    
    
protected:
    bool runSerialLogger()                         override;
    bool createThread()                            override;
    bool updateSerialState(int newFlag)            override;


private:
    vl::ThreadPool *tp;
    WindowFrameBuffer *m_wfb;
    std::mutex m_pointCloudMutex;
    std::vector<PointCloud> m_lastFrame;
    
    
}; // class YDLidarController

} // namespace lidar
} // namespace vl

#endif /*LIDARTMINIPLUSFMT*/