#pragma once

#include <QImage>
#include <QQuickPaintedItem>

namespace Ide::Ui {

class QMLImageItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ getImage WRITE setImage NOTIFY imageChanged)

public:
    explicit QMLImageItem(QQuickItem *parent = nullptr);

    Q_INVOKABLE void setImage(const QImage &image);
    QImage getImage();

    void paint(QPainter *painter) override;

signals:
    void imageChanged();

private:
    QImage m_image;
};

} 
