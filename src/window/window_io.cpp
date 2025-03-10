#include "window/window_io.hpp"

namespace vl
{

namespace window_io
{

WindowBackend::WindowBackend()
{
    memset(&m_tty, 0, sizeof(m_tty));
}

bool SerialWindowBackend::initTermios(FileDescriptor fd)
{   
    m_fd = fd;

    if (tcgetattr(m_fd, &m_tty) != 0)
    {
        std::cerr << "[Error] tcgetattr failed: " << std::endl;
        return false; 
    }

    cfsetispeed(&m_tty, B230400);
    cfsetospeed(&m_tty, B230400);

    setSerial8N1Mode();
    

    m_tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    m_tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    m_tty.c_oflag &= ~OPOST;

    tcflush(m_fd, TCIFLUSH);
    if (tcsetattr(m_fd, TCSANOW, &m_tty) != 0) {
        std::cerr << "[Error] tcsetattr failed: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

bool SerialWindowBackend::setSerial8N1Mode()
{
    m_tty.c_cflag = (m_tty.c_lflag & ~CSIZE) | CS8;
    m_tty.c_cflag &= ~PARENB;
    m_tty.c_cflag &= ~CSTOPB;
    m_tty.c_cflag |= CREAD | CLOCAL;

    return true;
}



}

}