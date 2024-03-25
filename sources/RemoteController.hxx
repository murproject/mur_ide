#pragma once

#include "QmlUtils.hxx"

#include <atomic>
#include <opencv2/opencv.hpp>
#include <shared_mutex>
#include <thread>
#include <QImage>
#include <QObject>
#include <QQuickImageProvider>
#include <QTimer>

namespace Ide::Ui {

struct RovControls {
    int servos_fcu0[4] = {0};
    int servos_fcu1[4] = {0};
    int servos_onboard[6] = {0};
};

class Joystick;

class RemoteController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage front READ getFrontImage NOTIFY imageChanged)
    Q_PROPERTY(QImage bottom READ getBottomImage NOTIFY imageChanged)
    Q_PROPERTY(bool remote READ isReadingImages NOTIFY readingStateChanged)
    Q_PROPERTY(bool watermarkOn READ isWatermarkOn WRITE setWatermarkOn NOTIFY watermarkOnStateChanged)

    Q_PROPERTY(bool autoDepth READ isAutoDepth WRITE setAutoDepth NOTIFY autoModeChanged)
    Q_PROPERTY(bool autoYaw READ isAutoYaw WRITE setAutoYaw NOTIFY autoModeChanged)
    Q_PROPERTY(bool autoRoll READ isAutoRoll WRITE setAutoRoll NOTIFY autoModeChanged)
    Q_PROPERTY(bool autoPitch READ isAutoPitch WRITE setAutoPitch NOTIFY autoModeChanged)
    Q_PROPERTY(bool autoYawAltmode READ isAutoYawAltmode WRITE setAutoYawAltmode NOTIFY autoModeChanged)
    Q_PROPERTY(float targetYaw READ getTargetYaw NOTIFY targetYawChanged)

    Q_PROPERTY(int speedMode READ getSpeedMode NOTIFY speedModeChanged)
    Q_PROPERTY(QList<int> speedLimits READ getSpeedLimits NOTIFY speedModeChanged)

    Q_PROPERTY(bool recordingVideo READ isRecordingVideo WRITE setRecordingVideo NOTIFY recordingVideoChanged)
    Q_PROPERTY(Ide::Ui::Joystick *Joystick READ getJoystick CONSTANT)

public:
    static RemoteController *instance;
    static RemoteController *Create();
    Joystick *getJoystick();

    QImage getFrontImage();
    QImage getBottomImage();

    bool isReadingImages();
    bool isAutoYaw();
    bool isAutoYawAltmode();
    bool isAutoDepth();
    bool isAutoRoll();
    bool isAutoPitch();
    bool isRecordingVideo();
    bool isWatermarkOn();
    float getTargetYaw();
    int getSpeedMode();
    void setWatermarkOn(bool);
    void startImageCapture();
    void stopImageCapture();
    void setAutoYaw(bool);
    void setAutoYawAltmode(bool);
    void setAutoDepth(bool);
    void setAutoRoll(bool);
    void setAutoPitch(bool);
    void setRecordingVideo(bool);

    enum class SpeedModes {Low, Mid, Max};

signals:
    void imageChanged();
    void readingStateChanged();
    void autoModeChanged();
    void targetYawChanged();
    void speedModeChanged();
    void recordingVideoChanged();
    void watermarkOnStateChanged();
    void servoValueChanged(int device, int servo, int value);
public slots:
    void saveImageFront();
    void saveImageBottom();
    void setServoValue(int device, int servo, int value);
    void sendRovControl(int device, int motor, int power = 50);
    void setPipelines(QString watermark);
    QStringList getPipelines();

    void setSpeedLimit(int, int);
    void saveSpeedLimits();
    QList<int> getSpeedLimits();

private:
    void setImage(QImage &destination, QImage target);
    void updateImages(cv::VideoCapture *cap, cv::VideoWriter *rec, int port, QImage *targetImg);
    void updateImagesFront();
    void updateImagesBottom();

    void updateRemoteThrust();
    void setServoPower(int device, int index, int val);
    void setServoPower(int device, int index, int speed, bool button1);
    void setServoPower(int device, int index, int speed, bool button1, bool button2);
    void setServoPower(int device, int index, int valMinus, int valNeutral, int valPlus, bool button1, bool button2);

    void saveImage(const QImage &, const QString name = "");

    RemoteController();
    static qml::RegisterType<RemoteController> Register;

    QImage m_frontImage;
    QImage m_bottomImage;

    QTimer *m_transmitThrustTimer;

    cv::VideoCapture m_captureFront;
    cv::VideoCapture m_captureBottom;
    cv::VideoWriter m_recordFront;
    cv::VideoWriter m_recordBottom;

    std::shared_mutex m_imageMutex;
    std::mutex m_capMutex;

    std::thread m_imageUpdateThreadFront;
    std::thread m_imageUpdateThreadBottom;

    std::atomic<bool> m_threadFlag = true;

    bool m_watermarkOn;

    bool m_isReadingImages = false;
    std::atomic<bool> m_isRecordingVideo = false;

    bool m_isAutoYaw = false;
    bool m_isAutoRoll = false;
    bool m_isAutoPitch = false;
    bool m_isAutoDepth = false;

    float m_targetYaw = 0.0;
    bool m_autoYawAltmode = false;

    QList<int> m_speedLimits = {50, 75, 100};

    RovControls m_rovControls;
    int axis_treshold(int value);

    QString getTimestamp();
    QString m_imagesDir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/murImages/";

    QString m_watermark;

    QString m_pipeline_stream = "/usr/bin/gst-launch-1.0 -v v4l2src device=/dev/video%idx% ! videorate "
                                        "! video/x-raw, format=YUY2, width=640, height=480, framerate=15/1 ! queue ! jpegenc quality=35 "
                                        "! rtpjpegpay ! udpsink async=true send-duplicates=false sync=false host=%host% port=%port%";

    QString m_pipeline_receive = "udpsrc port=%port% ! application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/1 "
                                         "! rtpjpegdepay ! jpegdec ! videoconvert ! appsink drop=true sync=false";


    QString m_pipeline_record = "appsrc ! videoconvert ! video/x-raw,format=YUY2 ! queue ! jpegenc ! matroskamux ! filesink location=%path%.mkv sync=false";

    QString m_default_watermark = "%device% / %date% / Depth: %depth% M / Temp: %temp% C / Yaw: %yaw%";
};

} 
