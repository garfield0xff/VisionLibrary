#include <loadsave.hpp>
#include "lidar/lidar_tminiplus.hpp"
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
    vl::lidar::YDLidarController yd_controller;
    if(!yd_controller.loadLidar()) {
        exit(EXIT_FAILURE);
    };
    yd_controller.setBuadrate(B230400);
    if(!yd_controller.initialize()) {
        std::cout << "initialized failed " << std::endl;
    };
    yd_controller.startScan();
    sleep(10);
    yd_controller.stopScan();
    
    return 0;
    
}