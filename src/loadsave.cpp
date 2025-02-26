#include <loadsave.hpp>

#include <fcntl.h>
#include <unistd.h>



namespace vl {
namespace lidar {

bool
loadLidar(const String port, int flags) 
{
    LidarController controller;
    
    if(flags & YDLidar) 
    {
        YDLidarController yd;
        controller = yd.newController();   
        std::cout << "[INFO] YDLIDAR SELECTED" << std::endl;
        controller->setPort(port);
        controller->startScan();
        sleep(20);
        controller->stopScan();
    }

    return true;
}
} // namespace lidar
} // namespace vl