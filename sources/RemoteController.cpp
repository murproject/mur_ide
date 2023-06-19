#include "RemoteController.hxx"
#include "Gamepad.hxx"
#include "NetworkController.hxx"
#include "TextIO.hxx"

#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <shared_mutex>
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QStandardPaths>

namespace Ide::Ui {
RemoteController *RemoteController::instance = nullptr;
qml::RegisterType<RemoteController> RemoteController::Register;

RemoteController::RemoteController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of image_view_controller already exists");
    }

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("Remote");

    m_watermark = settings.value("watermark_text", m_default_watermark).toString();
    m_watermarkOn = settings.value("watermark_on", true).toBool();

    m_speedLimits[static_cast<int>(SpeedModes::Low)] = settings.value("speed_low",  50).toInt();
    m_speedLimits[static_cast<int>(SpeedModes::Mid)] = settings.value("speed_mid",  75).toInt();
    m_speedLimits[static_cast<int>(SpeedModes::Max)] = settings.value("speed_max", 100).toInt();

    settings.endGroup();

    m_frontImage = QImage(":/images/video_placeholder.png");
    m_bottomImage = QImage(":/images/video_placeholder.png");

    if (!IO::directoryExists(m_imagesDir)) {
        QDir directory(m_imagesDir);
        directory.cdUp();
        directory.mkdir("murImages");
    }

    if (!IO::directoryExists(m_imagesDir + "video")) {
        QDir directory(m_imagesDir);
        directory.mkdir("video");
    }

    m_imageUpdateThreadFront = std::thread(&RemoteController::updateImagesFront, this);
    QThread::msleep(100);
    m_imageUpdateThreadBottom = std::thread(&RemoteController::updateImagesBottom, this);

    m_transmitThrustTimer = new QTimer{};

    connect(m_transmitThrustTimer, &QTimer::timeout, this, &RemoteController::updateRemoteThrust);
}

RemoteController *RemoteController::Create()
{
    instance = new RemoteController();
    return instance;
}

Gamepad *RemoteController::getGamepad()
{
    return Gamepad::instance;
}

QImage RemoteController::getFrontImage()
{
    std::shared_lock<std::shared_mutex> lock(m_imageMutex);
    return m_frontImage;
}

QImage RemoteController::getBottomImage()
{
    std::shared_lock<std::shared_mutex> lock(m_imageMutex);
    return m_bottomImage;
}

bool RemoteController::isReadingImages()
{
    return m_isReadingImages;
}

bool RemoteController::isWatermarkOn() {
    return m_watermarkOn;
}

void RemoteController::setWatermarkOn(bool state) {
    m_watermarkOn = state;

    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("Remote/watermark_on", state);

    emit watermarkOnStateChanged();
}

float RemoteController::getTargetYaw() {
    return m_targetYaw;
}

bool RemoteController::isAutoYaw()
{
    return m_isAutoYaw;
}

bool RemoteController::isAutoRoll()
{
    return m_isAutoRoll;
}

bool RemoteController::isAutoPitch()
{
    return m_isAutoPitch;
}

bool RemoteController::isAutoDepth()
{
    return m_isAutoDepth;
}

bool RemoteController::isRecordingVideo()
{
    return m_isRecordingVideo;
}

bool RemoteController::isAutoYawAltmode() {
    return m_autoYawAltmode;
}

void RemoteController::startImageCapture()
{
    if (m_isReadingImages) {
        return;
    }

    m_isReadingImages = true;
    m_transmitThrustTimer->start(50);

    emit readingStateChanged();
}

void RemoteController::stopImageCapture()
{
    if (!m_isReadingImages) {
        return;
    }

    m_transmitThrustTimer->stop();
    m_isReadingImages = false;

    emit readingStateChanged();
}

void RemoteController::setAutoYaw(bool val)
{
    m_isAutoYaw = val;
    emit autoModeChanged();
}

void RemoteController::setAutoDepth(bool val)
{
    m_isAutoDepth = val;
    emit autoModeChanged();
}

void RemoteController::setAutoRoll(bool val)
{
    m_isAutoRoll = val;
    emit autoModeChanged();
}

void RemoteController::setAutoPitch(bool val)
{
    m_isAutoPitch = val;
    emit autoModeChanged();
}

void RemoteController::setAutoYawAltmode(bool val) {
    m_autoYawAltmode = val;
    m_targetYaw = static_cast<float>(NetworkController::instance->getYaw());
    emit autoModeChanged();
    emit targetYawChanged();
}

QString RemoteController::getTimestamp()
{
    return QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss-zzz");
}

void RemoteController::setRecordingVideo(bool val)
{
    m_isRecordingVideo = val;

    if (m_recordFront.isOpened()) {
        m_recordFront.release();
    }

    if (m_recordBottom.isOpened()) {
        m_recordBottom.release();
    }

    if (m_isRecordingVideo) {
        QString path_prefix = m_imagesDir + "video/" + getTimestamp();

        std::string pathFront = QString(m_pipeline_record).replace("%path%", path_prefix + "_front").toStdString();
        std::string pathBottom = QString(m_pipeline_record).replace("%path%", path_prefix + "_bottom").toStdString();

        cv::Size size_front(m_captureFront.get(cv::CAP_PROP_FRAME_WIDTH),
                            m_captureFront.get(cv::CAP_PROP_FRAME_HEIGHT));

        cv::Size size_bottom(m_captureBottom.get(cv::CAP_PROP_FRAME_WIDTH),
                             m_captureBottom.get(cv::CAP_PROP_FRAME_HEIGHT));

        if (size_front.width <= 0) size_front.width = 640;
        if (size_front.height <= 0) size_front.height = 480;

        if (size_bottom.width <= 0) size_bottom.width = 640;
        if (size_bottom.height <= 0) size_bottom.height = 480;

        m_recordFront.open(pathFront, 0, 30, size_front, true);
        m_recordBottom.open(pathBottom, 0, 30, size_bottom, true);
    }

    emit recordingVideoChanged();
}

void RemoteController::saveImageFront()
{
    saveImage(m_frontImage, "_front");
}

void RemoteController::saveImageBottom()
{
    saveImage(m_bottomImage, "_bottom");
}

void RemoteController::setImage(QImage &destination, QImage target)
{
    {
        std::unique_lock<std::shared_mutex> lock(m_imageMutex);
        destination = target;
    }
    emit imageChanged();
}

void RemoteController::updateImages(cv::VideoCapture *cap, cv::VideoWriter *rec, int port, QImage *targetImg)
{
    std::unique_lock lock(m_capMutex);

    cv::Mat frame;
    QString pipe = m_pipeline_receive;
    pipe.replace("%port%", QString::number(port));
    cap->open(pipe.toStdString(), cv::CAP_GSTREAMER);

    qDebug() << "opened video on port " << port;

    lock.unlock();
    lock.release();

    while (m_threadFlag) {
        if (cap->read(frame)) {
            if (!frame.empty()) {
                if (m_watermarkOn) {
                    QString textFormat = m_watermark;
                    textFormat.replace("%device%",  NetworkController::instance->getVehicleFancyName(false));
                    textFormat.replace("%depth%",   QString::number(NetworkController::instance->getDepth(), 2, 2));
                    textFormat.replace("%temp%",    QString::number(NetworkController::instance->getTemperature(), 2, 2));
                    textFormat.replace("%yaw%",     QString::number(NetworkController::instance->getYaw(), 2, 2));
                    textFormat.replace("%roll%",    QString::number(NetworkController::instance->getRoll(), 2, 2));
                    textFormat.replace("%pitch%",   QString::number(NetworkController::instance->getPitch(), 2, 2));
                    textFormat.replace("%date%",    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
                    std::string text = textFormat.toStdString();

                    static cv::Point watermarkPos(4, 14);
                    static float watermarkScale = 0.35;

                    cv::putText(frame, text, watermarkPos, cv::FONT_HERSHEY_SIMPLEX, watermarkScale, cv::Scalar::all( 50), 3, cv::LINE_AA);
                    cv::putText(frame, text, watermarkPos, cv::FONT_HERSHEY_SIMPLEX, watermarkScale, cv::Scalar::all(255), 1, cv::LINE_AA);
                }

                QImage img = QImage((uchar *)frame.data,frame.cols, frame.rows, QImage::Format_RGB888).rgbSwapped();
                setImage(*targetImg, img);

                if (m_isRecordingVideo) {
                    rec->write(frame);
                }
            }
        }
    }

    cap->release();
}

void RemoteController::updateImagesFront() {
    updateImages(&m_captureFront, &m_recordFront, 5000, &m_frontImage);
}

void RemoteController::updateImagesBottom() {
    updateImages(&m_captureBottom, &m_recordBottom, 5001, &m_bottomImage);
}

void RemoteController::setServoPower(int device, int index, int val) {
    int *currentValue;

    if (device == 0) {
        currentValue = &m_rovControls.servos_fcu0[index];
    } else if (device == 1) {
        currentValue = &m_rovControls.servos_fcu1[index];
    } else if (device == 2) {
        currentValue = &m_rovControls.servos_onboard[index];
    } else {
        return;
    }

    if (val != *currentValue) {
        emit servoValueChanged(device, index, val);
        *currentValue = std::clamp(val, -100, 100);
    }
}

void RemoteController::setServoPower(int device, int index, int speed, bool button1) {
    static qint64 lastTimetamp = 0;
    qint64 curTimestamp = QDateTime::currentDateTime().currentMSecsSinceEpoch();

    if (curTimestamp - lastTimetamp < 100) {
        return;
    }

    if (!button1) {
        return;
    }

    lastTimetamp = curTimestamp;

    int *currentValue;

    if (device == 0) {
        currentValue = &m_rovControls.servos_fcu0[index];
    } else if (device == 1) {
        currentValue = &m_rovControls.servos_fcu1[index];
    } else if (device == 2) {
        currentValue = &m_rovControls.servos_onboard[index];
    } else {
        return;
    }

    int val = *currentValue;
    if (button1) {
        val += speed;
        if (val > 100) {
            val = -100;
        }
    }

    if (val != *currentValue) {
        emit servoValueChanged(device, index, val);
        *currentValue = std::clamp(val, -100, 100);
    }
}

void RemoteController::setServoPower(int device, int index, int speed, bool button1, bool button2) {
    if (button1 && button2) {
        return;
    }

    if (!button1 && !button2) {
        return;
    }

    int *currentValue;

    if (device == 0) {
        currentValue = &m_rovControls.servos_fcu0[index];
    } else if (device == 1) {
        currentValue = &m_rovControls.servos_fcu1[index];
    } else if (device == 2) {
        currentValue = &m_rovControls.servos_onboard[index];
    } else {
        return;
    }

    int val = *currentValue;
    if (button1) {
        val -= speed;
    }

    if (button2) {
        val += speed;
    }

    if (val != *currentValue) {
        emit servoValueChanged(device, index, val);
        *currentValue = std::clamp(val, -100, 100);
    }
}

void RemoteController::setServoPower(int device, int index, int valMinus, int valNeutral, int valPlus, bool button1, bool button2) {
    int *currentValue;

    if (device == 0) {
        currentValue = &m_rovControls.servos_fcu0[index];
    } else if (device == 1) {
        currentValue = &m_rovControls.servos_fcu1[index];
    } else if (device == 2) {
        currentValue = &m_rovControls.servos_onboard[index];
    } else {
        return;
    }

    int val = *currentValue;

    if (button1) {
        val = valMinus;
    }

    if (button2) {
        val = valPlus;
    }

    if (button1 == button2) {
        val = valNeutral;
    }

    if (val != *currentValue) {
        emit servoValueChanged(device, index, val);
        *currentValue = std::clamp(val, -100, 100);
    }
}


void RemoteController::setServoValue(int device, int servo, int value) {
    if (device == 0) {
        m_rovControls.servos_fcu0[servo] = value;
    } else if (device == 1) {
        m_rovControls.servos_fcu1[servo] = value;
    }else if (device == 2) {
        m_rovControls.servos_onboard[servo] = value;
    }
}

void RemoteController::sendRovControl(int device, int motor, int power) {

    QJsonObject json;
    json["type"] = "rov_control";

    QJsonArray main  = {0, 0, 0, 0, 0, 0, 0, 0};
    QJsonArray fcu_0 = {0, 0, 0, 0, 0, 0, 0, 0};
    QJsonArray fcu_1 = {0, 0, 0, 0, 0, 0, 0, 0};

    if (device == 0) {
        main[motor] = power;
    }

    if (device == 1) {
        fcu_0[motor] = power;
    }

    if (device == 2) {
        fcu_1[motor] = power;
    }

    QJsonArray fcu = {fcu_0, fcu_1};

    json["main"] = main;
    json["fcu"] = fcu;

    QJsonDocument doc(json);
    QString str(doc.toJson(QJsonDocument::Compact));
    NetworkController::instance->setRemoteThrust(str);
}


QStringList RemoteController::getPipelines() {
    QStringList pipelines;
    pipelines << m_pipeline_stream << m_pipeline_receive << m_pipeline_record << m_watermark;

    return pipelines;
}

void RemoteController::setPipelines(QString watermark) {
    QSettings settings("settings.ini", QSettings::IniFormat);

    m_watermark = watermark;

    settings.beginGroup("Remote");
    settings.setValue("gst_pipeline_stream", m_pipeline_stream);
    settings.setValue("gst_pipeline_receive", m_pipeline_receive);
    settings.setValue("gst_pipeline_record", m_pipeline_record);
    settings.setValue("watermark_text", watermark);
    settings.endGroup();
}

int RemoteController::getSpeedMode() {
    int speed = static_cast<int>(SpeedModes::Mid);

    return static_cast<int>(speed);
}

void RemoteController::saveSpeedLimits() {
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginGroup("Remote");
    settings.setValue("speed_low", m_speedLimits[static_cast<int>(SpeedModes::Low)]);
    settings.setValue("speed_mid", m_speedLimits[static_cast<int>(SpeedModes::Mid)]);
    settings.setValue("speed_max", m_speedLimits[static_cast<int>(SpeedModes::Max)]);
    settings.endGroup();
}


int RemoteController::axis_treshold(int value) {
    float speed = m_speedLimits[getSpeedMode()] / 100.0f;

    value *= speed;

    return value;
}

void RemoteController::updateRemoteThrust()
{
    auto gamepad = Gamepad::instance;

    auto x = axis_treshold(gamepad->getAxisValue(GamepadAxes::AxisY));
    auto y = axis_treshold(gamepad->getAxisValue(GamepadAxes::AxisX));
    auto z = axis_treshold(gamepad->getAxisValue(GamepadAxes::AxisZ));
    auto w = axis_treshold(gamepad->getAxisValue(GamepadAxes::AxisW));

    if (!isAutoYaw() && !isAutoDepth() && !NetworkController::instance->isRov()) {
        auto first_forward_motor = static_cast<int>(std::clamp(x + y, -100, 100));
        auto second_forward_motor = static_cast<int>(std::clamp(x - y, -100, 100));
        auto first_upward_motor = static_cast<int>(std::clamp(z, -100, 100));
        auto second_upward_motor = static_cast<int>(std::clamp(z, -100, 100));

        QJsonObject json;

        QJsonArray array = {first_upward_motor,
                            first_forward_motor,
                            second_forward_motor,
                            second_upward_motor,
                            0,
                            0,
                            0,
                            0,
                            int()};

        json["type"] = "thrust";
        json["power"] = array;

        QJsonDocument doc(json);
        QString str(doc.toJson(QJsonDocument::Compact));
        NetworkController::instance->setRemoteThrust(str);
    } else {
        QJsonObject json;

        QJsonArray array = {x, y, z, w};

        json["type"] = "remote_control";

        if (isAutoYawAltmode() && NetworkController::instance->isFeatureSupported("yaw_altmode")) {
            if (x != 0) {
                m_targetYaw += x * 0.1;
                if (m_targetYaw >  180) m_targetYaw -= 360;
                if (m_targetYaw < -180) m_targetYaw += 360;
                emit targetYawChanged();
            }
            json["yaw_altmode"] = true;
            array[0] = std::round(m_targetYaw);
        }

        json["axes"] = array;
        json["yaw"] = m_isAutoYaw;
        json["depth"] = m_isAutoDepth;

        if (NetworkController::instance->isRov() || true) {
            json["roll"] = m_isAutoRoll;
            json["pitch"] = m_isAutoPitch;


        }

        QJsonDocument doc(json);
        QString str(doc.toJson(QJsonDocument::Compact));
        NetworkController::instance->setRemoteThrust(str);
    }

    emit speedModeChanged();
}

void RemoteController::saveImage(const QImage &image, const QString name)
{
    auto file_name = getTimestamp() + name + ".png";
    qDebug() << m_imagesDir + file_name;
    image.save(m_imagesDir + file_name);
}

QList<int> RemoteController::getSpeedLimits() {
    return m_speedLimits;
}

void RemoteController::setSpeedLimit(int mode, int value) {
    m_speedLimits[mode] = value;
    emit speedModeChanged();
}

} 
