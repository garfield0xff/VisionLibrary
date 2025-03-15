#include "video/video_io.hpp"


namespace vl {
namespace video_io{

void VideoBaseController::initializeStreamer() {
    gst_init(nullptr, nullptr);
    m_pipeline = gst_parse_launch(getLaunchPipeLine(), nullptr);
    gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
}

bool VideoBaseController::stopRenderWindow() {
    m_is_streaming = false;
    gst_element_set_state(m_pipeline, GST_STATE_NULL);
    gst_object_unref(m_pipeline);
    return true;
}

const char* VideoBaseController::getLaunchPipeLine() {
    if(m_device == MacVideo) return "avfvideosrc ! videoconvert ! glupload ! glcolorconvert ! glimagesink";
    return nullptr;
}

}
}