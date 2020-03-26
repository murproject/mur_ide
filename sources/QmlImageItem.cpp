#include "QmlImageItem.hxx"
#include <QPainter>

namespace Ide::Ui {

QMLImageItem::QMLImageItem(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_image(":/images/mur_logo256.png")
{}

void QMLImageItem::setImage(const QImage &image)
{
    m_image = image;
    update();
    emit imageChanged();
}

QImage QMLImageItem::getImage()
{
    return m_image;
}

void QMLImageItem::paint(QPainter *painter)
{
    QRectF bounding_rect = boundingRect();
    painter->fillRect(bounding_rect, QColor("#282C34"));
    QImage scaled = m_image.scaledToHeight(bounding_rect.height());

    QPointF center = bounding_rect.center() - scaled.rect().center();

    if (center.x() < 0) {
        center.setX(0);
    }
    if (center.y() < 0) {
        center.setY(0);
    }

    painter->drawImage(center, scaled);
}

} // namespace ide::ui
