#include <loadsave.hpp>
#include <string>

int main()
{
    std::string portName = "/dev/cu.usbserial-0001";
    if(!vl::lidar::loadLidar(portName, vl::lidar::YDLidar))
    {
        std::cerr << "error" << std::endl;
    };
    return 0;
}