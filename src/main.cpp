#include <loadsave.hpp>
#include <string>

int main()
{
    std::string portName = "/dev/cu.usbserial-0001";
    if(!vl::loadLidar(portName, vl::YDLidar))
    {
        std::cerr << "error" << std::endl;
    };
    return 0;
}