#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>

static bool setSerialAttributes(int fd, speed_t baudrate) {
    // termios 구조체 초기화
    struct termios tty;
    memset(&tty, 0, sizeof(tty));

    if (tcgetattr(fd, &tty) != 0) {
        std::cerr << "[Error] tcgetattr failed: " << strerror(errno) << std::endl;
        return false;
    }

    // 입력, 출력 속도 설정
    cfsetispeed(&tty, baudrate);
    cfsetospeed(&tty, baudrate);

    // c_cflag: 8N1 모드
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;    // 데이터 비트 8
    tty.c_cflag &= ~PARENB;                       // 패리티 없음
    tty.c_cflag &= ~CSTOPB;                       // 스탑 비트 1
    tty.c_cflag |= CREAD | CLOCAL;                // 수신 가능, 로컬 모드

    // c_lflag: 원시 모드(Raw)
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    // c_iflag: 소프트웨어 플로우 제어 등 끄기
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);

    // c_oflag: 출력 처리 없음
    tty.c_oflag &= ~OPOST;

    // 버퍼 비우고 속성 적용
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        std::cerr << "[Error] tcsetattr failed: " << strerror(errno) << std::endl;
        return false;
    }

    return true;
}

int main() {
    // 1) 열고자 하는 시리얼 포트 경로 (MacOS 예시)
    const char* portName = "/dev/cu.usbserial-0001";

    std::cout << "1" << std::endl;
    // 2) 포트 열기
    int fd = open(portName, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        std::cerr << "[Error] Failed to open " << portName << ": " << strerror(errno) << std::endl;
        return 1;
    }

    std::cout << "2" << std::endl;

    // 3) Baudrate 설정 (115200) - cfsetispeed 에 대응하는 상수 사용
    //    B115200은 POSIX 표준 상수
    if(!setSerialAttributes(fd, B230400)) {
        close(fd);
        return 1;
    }

    std::cout << "[Info] Serial port opened: " << portName << " (115200 8N1)" << std::endl;

    // 4) 라이다 스캔 시작 명령 전송 (A5 60)
    //    바이트 배열: 0xA5 0x60
    uint8_t startScanCmd[2];
    startScanCmd[0] = 0xA5;
    startScanCmd[1] = 0x65;

    ssize_t written = write(fd, startScanCmd, 2);
    if (written < 0) {
        std::cerr << "[Error] Failed to write startScanCmd: " << strerror(errno) << std::endl;
        close(fd);
        return 1;
    }
    std::cout << "[Info] Sent Start Scan command (A5 60)" << std::endl;

    // 5) 잠시 대기 (모터가 도는지 확인)
    //    실제로 라이다를 보면, 모터가 회전하는지 눈으로 확인
    sleep(30);

    // 6) 프로그램 종료 전, 스캔 중단 명령(A5 65)도 보내볼 수 있음
    //    (정말 간단히 디버깅만 할 거라면 생략 가능)
    // uint8_t stopScanCmd[2] = {0xA5, 0x65};
    // write(fd, stopScanCmd, 2);
    // std::cout << "[Info] Sent Stop Scan command (A5 65)" << std::endl;

    // 7) 포트 닫기
    close(fd);
    std::cout << "[Info] Closed serial port. Exiting." << std::endl;

    return 0;
}
