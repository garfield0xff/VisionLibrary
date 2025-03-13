#ifndef _SERIAL_H_
#define _SERIAL_H_

#include "vlstd.hpp"

#include <string>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <format>
#include <termios.h>


using namespace std;

namespace vl {
namespace lidar {
namespace serial {

class Serial  {
public:
  Serial(const string &port, int baudrate) 
  : m_port(port), m_baudrate(baudrate) {};
  ~Serial();

  bool open();
  bool close();
  bool isOpen();
  bool write(const uint8_t *data, size_t length);
  bool setBaudrate();
  size_t read(uint8_t *buffer, size_t size);
  size_t readline(string& buffer, size_t size);

private:
  int m_fd;
  int m_baudrate;
  string m_port;
}; 

struct PortInfo {
  string friendly_name;
  string port;
  string device_id;
};
  
std::vector<PortInfo> list_ports();

} // namespace serial
} // namespace lidar
} // namespace vl

#endif // _SERIAL_H_