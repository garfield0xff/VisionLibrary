#include "ts/ts.hpp"
#include "logger.hpp"
#include "serial/serial.h"

using namespace vl::lidar::serial;


namespace vl_test {
    TEST(SERIAL_TEST, device_port) {
        std::vector<PortInfo> lst = list_ports();

        if(lst.empty()) VL_LOG_ERROR("ERROR", "DEVICE FS IS EMPTY");

        for (std::vector<PortInfo>::iterator it = lst.begin(); it != lst.end(); it++)
        {
            VL_LOG_DEBUG("SERIAL_TEST", (*it).port.c_str());
        }

    }
}
    