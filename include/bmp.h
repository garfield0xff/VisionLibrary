#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// BMP 파일 헤더 구조체
#pragma pack(push, 1)
struct BITMAPFILEHEADER {
    uint16_t bfType;      // 파일 타입, BM (0x4D42)
    uint32_t bfSize;      // 파일 크기
    uint16_t bfReserved1; // 예약 필드
    uint16_t bfReserved2; // 예약 필드
    uint32_t bfOffBits;   // 이미지 데이터까지의 오프셋
};

struct BITMAPINFOHEADER {
    uint32_t biSize;          // 헤더 크기
    int32_t biWidth;          // 이미지 폭
    int32_t biHeight;         // 이미지 높이
    uint16_t biPlanes;        // 플레인 수 (항상 1)
    uint16_t biBitCount;      // 비트 수 (24비트 BMP의 경우 24)
    uint32_t biCompression;   // 압축 유형 (0인 경우 비압축)
    uint32_t biSizeImage;     // 이미지 크기
    int32_t biXPelsPerMeter;  // 가로 해상도
    int32_t biYPelsPerMeter;  // 세로 해상도
    uint32_t biClrUsed;       // 색상 테이블에 사용된 색상 수
    uint32_t biClrImportant;  // 중요한 색상 수
};

bool readBMP(const std::string &filename, cv::Mat &image);
cv::Mat bilinearResize(const cv::Mat& inputImage, int newWidth, int newHeight);

#pragma pack(pop)