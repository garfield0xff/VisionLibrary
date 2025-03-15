#ifndef _LIDAR_BASE_FMT_
#define _LIDAR_BASE_FMT_

/**
 * @file    lidar_base.hpp
 * @brief   Defines the BaseLidarController class for controlling various LIDAR devices.
 * @details This file declares the BaseLidarController class and related types/enumerations,
 *          providing basic methods to start, stop, and manage communication with LIDAR sensors.
 */

#include "serial/serial.h"
#include "thread.h"
#include "logger.hpp"
#include "window_fb.hpp"

#include <vlstd.hpp>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <logger.hpp>
#include <memory>   // for std::shared_ptr


/**
 * @namespace vl
 */
namespace vl
{

namespace lidar 
{

struct PointCloud {
    float x;
    float y;
    float distance;
    uint8_t intensity;
    float angle_deg;
};
    

class BaseLidarController;

/**
 * @typedef LidarController
 * @brief   A shared pointer to a BaseLidarController.
 */
typedef std::shared_ptr<BaseLidarController> LidarController;


/**
 * @typedef Header
 * @brief   Represents the byte header used for serial communication with the LIDAR.
 */
typedef std::vector<uint8_t> Header;

/**
 * @enum  LidarDevice
 * @brief Enumeration of supported LIDAR device types.
 * @var   LidarDevice::YDLidar
 *        Represents a YDLidar device.
 */
enum LidarDevice 
{
    YDLidar = 1 << 0,  ///< Bitmask representing the YDLidar device.
};

/**
 * @class BaseLidarController
 * @brief A class that provides an abstract interface for control lidar
 * @details Provides a common interface for setting up a serial port, starting/stopping scans,
 *          and managing threads for reading LIDAR data.
 */
class BaseLidarController
{

public:
    /**
     * @brief  Default constructor for BaseLidarController.
     */
    BaseLidarController() {};

    /**
     * @brief  Destructor for BaseLidarController.
     */
    ~BaseLidarController() {};

    /**
     * @brief show lidar ports and set ports
     */
    bool loadLidar();


    bool isScanning() { return m_isScanning; };

    
    bool setBuadrate(int baudrate);

    /**
     * @brief  Starts scanning (data acquisition) from the LIDAR.
     * @return True if the scan is successfully started, false otherwise.
     */
    virtual bool startScan();

    /**
     * @brief  Stops scanning from the LIDAR.
     * @return True if the scan is successfully stopped, false otherwise.
     */
    virtual bool stopScan();

    /**
     * @brief Prints or logs serial status information.
     * @return True if logging is successful, false otherwise.
     * @note  추후에 Lidar 모듈 추가될때 수정 예정
     */
    virtual void readScanData() = 0; // Todo: Implement in derived LIDAR modules

    virtual bool initialize();

    virtual std::queue<std::vector<PointCloud>> getPointCloud();

    virtual std::vector<PointCloud> getLatestPointCloud();

    virtual void updatePointCloud(std::vector<PointCloud>&& _point_cloud);
        
    /**
     * @brief  Creates a new instance (shared pointer) of the controller.
     * @return A shared pointer to the newly created BaseLidarController.
     */
    virtual LidarController newController() const;

protected:

    LidarDevice m_device;

    serial::Serial *m_serial = nullptr;

    std::condition_variable data_ready;
    std::queue<std::vector<PointCloud>> point_cloud_queue;
    std::mutex point_cloud_mutex;

    /**
     * @brief The serial device port path or identifier.
     */
    String m_port;

    /**
     * @brief The baudrate used for serial communication.
     */
    Bps m_baudrate;

    /**
     * @brief The file descriptor for the serial port.
     */
    FileDescriptor m_fd;

    /**
     * @brief Timeout value in milliseconds for serial operations.
     */
    unsigned int m_timeoutMS;

    /**
     * @brief Indicates whether the controller is connected to a LIDAR device.
     */
    bool m_connected;

    /**
     * @brief run serial loggger
     * @return True if fetch serial Log success 
     */
    virtual bool runSerialLogger() = 0;

    
    virtual bool createThread() = 0;

    /**
     * @brief update header flag
     * 
     * @param newFlag 
     * @return True if update flag success
     */
    virtual bool updateSerialState(int newFlag) = 0;


    /**
     * @brief Flag indicating whether the controller is currently scanning.
     */
    std::atomic<bool> m_isScanning;

    

    /**
     * @brief Mutex for synchronizing access to shared resources.
     */
    std::mutex m_mutex;

    /**
     * @brief type for response that lidar header
     */
    uint8_t m_response_type;


}; // class BaseLidarController
} // namespace lidar
} // namespace vl

#endif /* _LIDAR_BASE_FMT_ */
