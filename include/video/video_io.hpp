#ifndef VIDEO_IO_
#define VIDEO_IO_

#include <gst/gst.h>
#include <thread>

namespace vl {
namespace video_io {

enum VideoDevice {
    MacVideo = 1 << 0,
};


class VideoBaseController {

public:

VideoBaseController(VideoDevice device) : m_device(device) {};
void initializeStreamer();
void renderWindow();
bool stopRenderWindow();
const char* getLaunchPipeLine();


protected:
    GstElement* m_pipeline;
    VideoDevice m_device;
    const char *m_launch_pipeLine;
    bool m_is_streaming;

};
} //  namespace video_io
} //  namespace vl



#endif // VIDEO_IO_