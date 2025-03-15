#include "lidar/lidar_tminiplus.hpp"
#include "video/video_io.hpp"

#include <string>
#include <iostream>


int main(int argc, char** argv) {
    vl::lidar::YDLidarController yd_controller;
    WindowFrameBuffer w_fb("Test", 800, 800);

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

    vl::video_io::VideoBaseController video_controller(vl::video_io::MacVideo);

    std::thread timer_thread([&]() {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        yd_controller.stopScan();
        // video_controller.stopRenderWindow();
    });

    // video_controller.initializeStreamer();
    std::vector<float> verticies;

    float camera_fov_deg = 90.0f;

    while(yd_controller.isScanning() && !w_fb.isCloseWindow()) {
        std::vector<vl::lidar::PointCloud> latest_cloud = yd_controller.getLatestPointCloud();
        verticies.clear();

        for(int i = 0; i < latest_cloud.size(); i++) {
            float x = latest_cloud[i].x;
            float y = latest_cloud[i].y;
            float dist = latest_cloud[i].distance;
            verticies.push_back(latest_cloud[i].x);
            verticies.push_back(latest_cloud[i].y);
            verticies.push_back(0.0f);
        }        
        w_fb.render2dPoint(verticies, camera_fov_deg);
        glfwPollEvents();
        std::this_thread::sleep_for(std::chrono::milliseconds(30)); 
    }
    w_fb.closeWindow();
    
    if (timer_thread.joinable())
        timer_thread.join();

    return 0;
}
