#include "lidar/lidar_tminiplus.hpp"
#include <string>
#include <iostream>

int main(int argc, char** argv) {
    vl::lidar::YDLidarController yd_controller;
    WindowFrameBuffer w_fb("LidarTest", 800, 800);

    if (!yd_controller.loadLidar()) {
        exit(EXIT_FAILURE);
    }

    w_fb.initializeWindow();
    w_fb.loadShader("asset/shader/SimplePclTransform.vertexshader",
                    "asset/shader/SimplePclColor.fragmentshader");

    yd_controller.setWindowFrameBuffer(&w_fb);
    yd_controller.setBuadrate(B230400);

    if (!yd_controller.initialize()) {
        std::cerr << "Initialize failed\n";
        exit(EXIT_FAILURE);
    }

    if (!yd_controller.startScan()) {
        std::cerr << "Scan failed\n";
        exit(EXIT_FAILURE);
    }

    std::thread timer_thread([&yd_controller]() {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        yd_controller.stopScan();  
    });

    yd_controller.showPointCloud(); 

    if (timer_thread.joinable())
        timer_thread.join();

    return 0;
}
