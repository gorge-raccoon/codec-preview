#include "h264tab.h"

H264::H264()
{

}

void H264::start(QProcess &process, QString file)
{
    QString command = "ffmpeg -re -i \"" + file + "\" -c:v libx264 -preset ultrafast -an -f matroska udp://localhost:" + QString::number(ENCODED_VIDEO_PORT) + " -c:v libx264 -preset ultrafast -an -f matroska udp://localhost:"  + QString::number(VIDEO_PROBE_PORT);
    process.start(command.toUtf8().constData());
}
