#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <string>
#include <vector>

namespace vl {
namespace lidar {
namespace serial {

struct PortInfo {
    std::string port;
    std::string description;
    std::string hardware_id;
    std::string device_id;
};

std::vector<PortInfo> list_ports();

}
}
}

#endif // _SERIAL_H_
