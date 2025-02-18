#ifndef _WINDOW_IO_H_
#define _WINDOW_IO_H_

#include <termios.h>
#include <vlstd.hpp>

namespace vl
{

namespace window_io
{

class WindowBackend
{
public:

    WindowBackend();
    ~WindowBackend() {};

    

    // bool initTermios();
    virtual bool initTermios(FileDescriptor fd) = 0;
    // virtual bool setBuadrate(Bps baudrate) = 0;

protected:
    struct termios m_tty;
    FileDescriptor m_fd;

}; // class WindowBackend


class SerialWindowBackend : public WindowBackend
{
public:
    SerialWindowBackend() {};
    ~SerialWindowBackend() {};

    bool initTermios(FileDescriptor fd) override;
    // bool setBuadrate(Bps buadrate) override;
    // bool setSerialPort(FileDescriptor fd);
    bool setSerial8N1Mode();

}; // class SerialWindowBackend



} // namespace window_io

} // namespace vl

#endif /*_WINDOW_IO_H_*/