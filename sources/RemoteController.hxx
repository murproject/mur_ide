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

class Gamepad;

class RemoteController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage front READ getFrontImage NOTIFY imageChanged)
    Q_PROPERTY(QImage bottom READ getBottomImage NOTIFY imageChanged)
    Q_PROPERTY(bool remote READ isReadingImages NOTIFY readingStateChanged)

    Q_PROPERTY(bool autoDepth READ isAutoDepth WRITE setAutoDepth NOTIFY autoModeChanged)
    Q_PROPERTY(bool autoYaw READ isAutoYaw WRITE setAutoYaw NOTIFY autoModeChanged)

    Q_PROPERTY(Ide::Ui::Gamepad *Gamepad READ getGamepad CONSTANT)

public:
    static RemoteController *instance;
    static RemoteController *Create();

    Gamepad *getGamepad();

    QImage getFrontImage();
    QImage getBottomImage();

    bool isReadingImages();
    bool isAutoYaw();
    bool isAutoDepth();
    void startImageCapture();
    void stopImageCapture();
    void setAutoYaw(bool);
    void setAutoDepth(bool);
signals:
    void imageChanged();
    void readingStateChanged();
    void autoModeChanged();
public slots:
    void saveImageFront();
    void saveImageBottom();

private:
    void setImageFront(QImage &);
    void setImageBottom(QImage &);
    void updateImagesFront();
    void updateImagesBottom();

    void updateRemoteThrust();

    void saveImage(const QImage &);

    RemoteController();
    static qml::RegisterType<RemoteController> Register;

    QImage m_frontImage;
    QImage m_bottomImage;

    QTimer *m_transmitThrustTimer;

    cv::VideoCapture m_captureFront;
    cv::VideoCapture m_captureBottom;
    cv::VideoCapture m_recordFront;
    cv::VideoCapture m_recordBottom;

    std::shared_mutex m_imageMutex;
    std::thread m_imageUpdateThreadFront;
    std::thread m_imageUpdateThreadBottom;

    std::atomic<bool> m_threadFlag = true;
    bool m_isReadingImages = false;

    bool m_isAutoYaw = false;
    bool m_isAutoDepth = false;
};

} // namespace ide::ui
