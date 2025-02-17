#include <loadsave.hpp>

#include <fcntl.h>



namespace vl {

static bool
loadLidar(const String port, int flags) 
{
    LidarController controller;

    int fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_SYNC );
    if (fd < 0) 
    {
        std::cerr << "[Error] Failed to open" << std::endl;
        return -1;
    }  
    
    if(flags & YDLidar) 
    {
        YDLidarController yd;
        controller = yd.newController();   
        std::cout << "[INFO] YDLIDAR SELECTED" << std::endl;

        controller->setPort(port);
        controller->startScan();
    }
}

}