#include "videoinfowidget.h"
#include "ui_videoinfowidget.h"

VideoInfoWidget::VideoInfoWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::VideoInfoWidget) {
    ui->setupUi(this);

    // redirect stream probe output to file (cannot read it in any other way
    // unfortunately)
    streamProbeProcess.setStandardOutputFile(PROBE_FILE_NAME);

    // react to frame probe output with parseFrameProbeOutput
    connect(&frameProbeProcess, &QProcess::readyRead, this,
            &VideoInfoWidget::parseFrameProbeOutput);

    // react to stream probe output with parseStreamProbeOutput
    connect(&streamProbeProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(parseStreamProbeOutput(int, QProcess::ExitStatus)));
}

VideoInfoWidget::~VideoInfoWidget() { delete ui; }

void VideoInfoWidget::stopProbe() {
    frameProbeProcess.kill();
    streamProbeProcess.kill();
    frameProbeProcess.waitForFinished();
    streamProbeProcess.waitForFinished();
}

void VideoInfoWidget::parseFrameProbeOutput() {

    // read output line by line
    while (frameProbeProcess.canReadLine()) {
        QString output = frameProbeProcess.readLine();

        // find frame type
        if (output.startsWith("pict_type=")) {
            // put single character describing frame type into queue
            framesQueue.enqueue(output.toUtf8().constData()[10]);

            // throw out oldest frame types if over queue size limit
            if (framesQueue.size() > 16)
                stopProbe();
                //framesQueue.dequeue();

            // create snapshot of queue to display in widget
            QString framesQueueSnapshot;

            QListIterator<char> i(framesQueue);

            while (i.hasNext()) {
                framesQueueSnapshot.append(i.next());
            }

            ui->frameTypes->setText(framesQueueSnapshot);
        }

        // find frame width
        if (output.startsWith("width=")) {
            ui->frameWidth->setText(output.mid(6, output.length()));
        }

        // find frame height
        if (output.startsWith("height=")) {
            ui->frameHeight->setText(output.mid(7, output.length()));
        }
    }
}

void VideoInfoWidget::clearFrameQueue() {
    framesQueue.clear();
    ui->frameTypes->setText("");
}

void VideoInfoWidget::startFrameProbe(QString command) {
    frameProbeProcess.start(command);
}

void VideoInfoWidget::startStreamProbe(QString command) {
    streamProbeProcess.start(command);
}

void VideoInfoWidget::parseStreamProbeOutput(int a, QProcess::ExitStatus b) {
    // to silence unused warning
    (void)a;
    (void)b;

    // read from data dumped into file
    std::ifstream myReadFile;
    myReadFile.open(PROBE_FILE_NAME.toUtf8().data());
    char output[100];
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {

            myReadFile >> output;

            QString fileOutput = QString(output);

            // find frame rate
            if (fileOutput.startsWith("avg_frame_rate=")) {
                ui->frameRate->setText(fileOutput.mid(15, fileOutput.length()));
            }

            // find codec name
            //if (fileOutput.startsWith("codec_name=")) {
                //ui->codecName->setText(fileOutput.mid(11, fileOutput.length()));
            //}

            // find bit rate
            if (fileOutput.startsWith("bit_rate=")) {
                ui->bitRate->setText(fileOutput.mid(9, fileOutput.length()));
            }

            // find aspect ratio
            if (fileOutput.startsWith("display_aspect_ratio=")) {
                ui->aspectRatio->setText(
                    fileOutput.mid(21, fileOutput.length()));
            }
        }
    } else {
        qDebug() << "ERROR: cannot access file";
    }
    myReadFile.close();
}
