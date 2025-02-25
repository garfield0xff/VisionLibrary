#include <loadsave.hpp>

#include <fcntl.h>
#include <unistd.h>



namespace vl {

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
        sleep(30);
        controller->stopScan();
    }

    return true;
}

}