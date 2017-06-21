#include "h264manager.h"

H264Manager::H264Manager(QWidget *parent) : CodecManager(parent, "libx264") {
    addParameter("Scale", "s", "");
    addParameter("Bitrate", "b:v", "");
    addParameter("Minrate", "minrate", "");
    addParameter("Maxrate", "maxrate", "");
    addParameter("Buffer size", "bufsize", "");
    addParameter("Aspect ratio", "aspect", "");
    addParameter("Costant Rate Factor", "crf", "");
}

H264Manager::~H264Manager() {}
