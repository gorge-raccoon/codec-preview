#ifndef CODECTABSWIDGET_H
#define CODECTABSWIDGET_H

#include <QProcess>
#include <QRegularExpression>
#include <QVector>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

//#include "ffmpegcommand.h"


#include "CodecComparisonWindow.h"

namespace Ui {
class CodecTabsWidget;
}

class CodecTabsWidget : public QWidget {
    Q_OBJECT

  private:
    Ui::CodecTabsWidget *ui;
    QVector<CodecParametersWidget *> codecWidgets;
    QProcess streamingProcess;
    QProcess cameraNameGetterProcess;
    QProcess singleFrameProcess;
    QString inputParameters;
    QString inputLocation;
    bool selectedCodecs[6];
    CodecComparisonWindow compareWindow;
    int previousFirst;
    int previousSecond;
    int previousThird;

    QGraphicsScene* scene;
    QGraphicsView* view;
    QGraphicsPixmapItem *item;




  public:
    explicit CodecTabsWidget(QWidget *parent = 0);
    ~CodecTabsWidget();
    void compareWindowStream(int first, int second, int third);
    void stopStreaming();
    static QString buildStreamingCommand(QString inputParameters,
                                         QString inputLocation,
                                         QString outputPrameters,
                                         QString rawLocation,
                                         QString encodedLocation);

    static QString buildMultipleStreamingCommands(
        QString inputParameters, QString inputLocation,
        QVector<QString> outputPrameters, QVector<QString> outputLocations);

    static QString buildProbeCommand(QString location, QString params);
    static QString parametersToString(QMap<QString, QString> *parameters);
    void openFromFile(QString filePath);
    void openFromCamera();
    void selectMultipleCodecs();
    QString getStreamingParameters();
    void startStreaming(QString streamingParameters);
    void setCRF(QString value);
    void insertParameter(QVBoxLayout *layout);
    void getSingleFrame();


  signals:
    void currentTabChanged();
    void settingsChanged();

  private slots:
    void onTabChange();
    void parseCameraNameProbeOutput(int a, QProcess::ExitStatus b);
    void compareWindowStream();
    void onSingleFrameGotten(int a, QProcess::ExitStatus b);

};

#endif // CODECTABSWIDGET_H