#ifndef _WINDOW_IO_H_
#define _WINDOW_IO_H_

#include <termios.h>
#include <vlstd.hpp>


/**
 * @brief 
 * 
 */
namespace vl
{
namespace window_io
{

/**
 * @brief  A class that control io interface 
 * 
 * 
 * The WindowBackend class provides to control io interface
 * It defines to control fileDescriptor and set to communicate with device
 */
class WindowBackend
{
public:

    WindowBackend();
    ~WindowBackend() {};

    /**
     * @brief initialize interface io window frame buffer
     * 
     * @param fd Device FileDescriptor
     * @return true is successfully set, false otherwise
     */
    virtual bool initTermios(FileDescriptor fd) = 0;

protected:
    struct termios m_tty; ///< Struct termios to store option
    FileDescriptor m_fd;  ///< Device FileDescriptor

}; // class WindowBackend


/**
 * @brief A class that control serial communication with device
 * 
 */
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