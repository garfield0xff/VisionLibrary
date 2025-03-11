#include "serial/serial.h"

#include <glob.h>
#include <termios.h>
#include <logger.hpp>


using namespace vl::lidar::serial;

static vector<string> glob(const vector<string> &patterns);
static string basename(const string &path, char seperator);

vector<string> glob(const vector<string> &patterns)
{
    vector<string> paths_found;

    if(patterns.size() == 0) {
        return paths_found;
    }

    glob_t glob_results;

    int glob_retval = glob(patterns[0].c_str(), 0, NULL, &glob_results);

    vector<string>::const_iterator iter = patterns.begin();

    while(++iter != patterns.end()) {
        glob(iter->c_str(), GLOB_APPEND, NULL, &glob_results);
    }

    for(size_t path_index = 0; path_index < glob_results.gl_pathc; path_index++) {
        paths_found.push_back(glob_results.gl_pathv[path_index]);
    }

    globfree(&glob_results);

    return paths_found;
};

string basename(const string &path, char seperator) {
    size_t pos = path.rfind(seperator);

    if(pos == std::string::npos) {
        return path;
    }

    return string(path, pos + 1, string::npos);
}

Serial::~Serial() {
    close();
}


bool Serial::open() {
    m_fd = ::open(m_port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (m_fd < 0) {
        return false;
    }
    return true;
}

bool Serial::close() {
    if(m_fd >= 0) {
        ::close(m_fd);
        m_fd = -1;
    }

    return true;
}

bool Serial::isOpen() {
    return m_fd >= 0;
}

bool Serial::write(const uint8_t *data, size_t length) {
    ssize_t writen = ::write(m_fd, data, length);
    if(writen < 0) return false;  
    return true;
}

string Serial::read() {
    uint8_t* buffer;

}

bool Serial::setBaudrate() {
    struct termios tio;
    memset(&tio, 0, sizeof(tio));
    /**
     * 8N1 Mode
     * 8bit data frame
     * not use Parity bit
     * 1 STOP BIT
     * RX, LOCAL
     */
    tio.c_cflag = (tio.c_lflag & ~CSIZE) | CS8;
    tio.c_cflag &= ~PARENB;
    tio.c_cflag &= ~CSTOPB;
    tio.c_cflag |= CREAD | CLOCAL;
    tio.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tio.c_iflag &= ~(IXON | IXOFF | IXANY);
    tio.c_oflag &= ~OPOST;

    cfsetispeed(&tio, m_baudrate);
    cfsetospeed(&tio, m_baudrate);

    tcflush(m_fd, TCIFLUSH);
    if(tcsetattr(m_fd, TCSANOW, &tio) != 0) {
        return false;
    }
    return true;
}



vector<PortInfo> vl::lidar::serial::list_ports()  {
    vector<string> search_globs;
    vector<PortInfo> results;

    search_globs.push_back("/dev/tty.usbserial*");
    search_globs.push_back("/dev/cu.usbserial*");

    vector<string> device_found = glob(search_globs);
    vector<string>::iterator iter = device_found.begin();

    while(iter != device_found.end()) {
        string port = *iter++;
        
        {
            PortInfo device_entry;
            device_entry.port = port;
            device_entry.friendly_name = basename(port, '/');
            device_entry.device_id = basename(port, '-');
            results.push_back(device_entry);
        }
    }
    
    return results;
}