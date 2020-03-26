#include "RemoteController.hxx"
#include "Gamepad.hxx"
#include "NetworkController.hxx"
#include "TextIO.hxx"

#include <algorithm>
#include <opencv2/opencv.hpp>
#include <shared_mutex>
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPixmap>
#include <QStandardPaths>

//! TODO: Refactor this
//!
namespace Ide::Ui {
RemoteController *RemoteController::instance = nullptr;
qml::RegisterType<RemoteController> RemoteController::Register;

RemoteController::RemoteController()
{
    if (instance != nullptr) {
        throw std::runtime_error("Instance of image_view_controller already exists");
    }

    m_frontImage = QImage(":/images/video_placeholder.png");
    m_bottomImage = QImage(":/images/video_placeholder.png");
    m_imageUpdateThreadFront = std::thread(&RemoteController::updateImagesFront, this);
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

bool RemoteController::isAutoYaw()
{
    return m_isAutoYaw;
}

bool RemoteController::isAutoDepth()
{
    return m_isAutoDepth;
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

    auto image = QImage(":/images/video_placeholder.png");
    setImageFront(image);
    setImageBottom(image);
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

void RemoteController::saveImageFront()
{
    saveImage(m_frontImage);
}

void RemoteController::saveImageBottom()
{
    saveImage(m_bottomImage);
}

void RemoteController::setImageFront(QImage &front)
{
    {
        std::unique_lock<std::shared_mutex> lock(m_imageMutex);
        m_frontImage = front;
    }
    emit imageChanged();
}

void RemoteController::setImageBottom(QImage &bottom)
{
    {
        std::unique_lock<std::shared_mutex> lock(m_imageMutex);
        m_bottomImage = bottom;
    }

    emit imageChanged();
}

void RemoteController::updateImagesFront()
{
    cv::Mat frame_front;

    m_captureFront.open("udpsrc port=5000 ! "
                        "application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/"
                        "1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink drop=true sync=false");

    while (m_threadFlag) {
        if (m_captureFront.read(frame_front)) {
            if (!frame_front.empty()) {
                QImage img = QImage((uchar *) frame_front.data,
                                    frame_front.cols,
                                    frame_front.rows,
                                    QImage::Format_RGB888)
                                 .rgbSwapped();
                setImageFront(img);
            }
        }
    }

    m_captureFront.release();
}

void RemoteController::updateImagesBottom()
{
    cv::Mat frame_bottom;

    m_captureBottom.open("udpsrc port=5001 ! "
                         "application/x-rtp,media=video,payload=26,clock-rate=90000,encoding-name=JPEG,framerate=30/"
                         "1 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink drop=true sync=false");

    while (m_threadFlag) {
        if (m_captureBottom.read(frame_bottom)) {
            if (!frame_bottom.empty()) {
                QImage img = QImage((uchar *) frame_bottom.data, frame_bottom.cols, frame_bottom.rows, QImage::Format_RGB888).rgbSwapped();
                setImageBottom(img);
            }
        }
    }

    m_captureBottom.release();
}

void RemoteController::updateRemoteThrust()
{
    if (!Gamepad::instance->getGamepad()->isConnected()) {
        return;
    }

    auto x = Gamepad::instance->getAxisXvalue();
    auto y = Gamepad::instance->getAxisYvalue();
    auto z = Gamepad::instance->getAxisZvalue();

    if (!isAutoYaw() && !isAutoDepth()) {
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

        QJsonArray array = {x, y, z, int()};

        json["type"] = "remote_control";
        json["axes"] = array;
        json["yaw"] = m_isAutoYaw;
        json["depth"] = m_isAutoDepth;

        QJsonDocument doc(json);
        QString str(doc.toJson(QJsonDocument::Compact));
        NetworkController::instance->setRemoteThrust(str);
    }
}

void RemoteController::saveImage(const QImage &image)
{
    auto dir = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation) + "/"
               + "murImages";
    if (!IO::directoryExists(dir)) {
        QDir direcotory(dir);
        direcotory.cdUp();
        direcotory.mkdir("murImages");
    }

    auto file_name = QDateTime::currentDateTime().toString("yyyymmddhhmmsszz") + ".png";
    qDebug() << dir + "/" + file_name;
    image.save(dir + "/" + file_name);
}

} // namespace ide::ui
