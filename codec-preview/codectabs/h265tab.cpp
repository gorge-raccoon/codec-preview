#include "h265tab.h"

H265::H265()
{

}

void H265::start(QProcess &process, QString file)
{
    QString command = "ffmpeg -re -i \"" + file + "\" -c:v libx265 -preset ultrafast -an -f matroska udp://localhost:" + ENCODED_VIDEO_PORT + " -c:v libx265 -preset ultrafast -an -f matroska udp://localhost:"  + VIDEO_PROBE_PORT;
    process.start(command.toUtf8().constData());
}
