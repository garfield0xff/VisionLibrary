#include "ts/ts.hpp"
#include "serial/serial.h"

// 

namespace vl_test {
    TEST(SERIAL_TEST, device_port) {
        std::vector<PortInfo> lst = list_ports();


        std::map<std::string, std::string> ports;
    
        for (std::vector<PortInfo>::iterator it = lst.begin(); it != lst.end(); it++)
        {
            std::string port = "ydlidar" + (*it).device_id;
            ports[port] = (*it).port;
        }

        for(const auto &[device_name, port] : ports) {
            cout << "device name : " << device_name << endl;
            cout << "ports name : " << port << endl;
        }
    }
}
    