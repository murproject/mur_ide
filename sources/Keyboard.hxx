#pragma once
#include <QObject>
#include "QmlUtils.hxx"
#include <QQmlApplicationEngine>
#include <QTimer>

#include <QObject>

namespace Ide::Ui{

class KeyboardAxes : public QObject
{
    Q_OBJECT

public:
    enum MovementAxes {
        AxisZero,

        AxisXm,
        AxisXp,
        AxisYm,
        AxisYp,
        AxisWm,
        AxisWp,
        AxisZm,
        AxisZp,

        SpeedSlow,
        SpeedFast,

        AxisCount,
    };
    Q_ENUM(MovementAxes)
};

class Keyboard : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<qreal> allAxes READ getAllAxes NOTIFY axesValueChanged)
    Q_PROPERTY(QList<QString> allAxesBindings READ getAllAxesBindings NOTIFY rebinded)
    Q_PROPERTY(int rebindingAxis READ getRebindingAxis NOTIFY rebindingChanged)

public:
    static Keyboard *instance;
    static Keyboard *Create();

    int getAxesValue(KeyboardAxes::MovementAxes, KeyboardAxes::MovementAxes);

public slots:
    int getAxisBinding(int);
    void requestRebind(int);
    int getRebindingAxis();
    void rebindAxis(int axis);
    QList<QString> getAllAxesBindings();

    void swapAxes(int, int);
    void clearAxis(int);

    QString getKeyName(int);

    int getAxisValue(KeyboardAxes::MovementAxes axis);
    QList<qreal> getAllAxes();
    void setAxisValue(int id_key, int value);

    void saveSettings();
    void toDefaultSettings(bool force = true);

signals:
    void rebinded();
    void axesValueChanged();
    void rebindingChanged();

private:
    Keyboard();
    static qml::RegisterType<Keyboard> Register;

    void loadSettings();

    QMap<KeyboardAxes::MovementAxes, int> m_keyboardAxesBindings;

    bool m_isRebinding = false;
    KeyboardAxes::MovementAxes m_currentlyRebindingAxis;
    QMap<int, double> m_keyboardAllValues;
    QMap<QString, int> keyboardCodeSymbol;

    QMetaEnum metaEnum = QMetaEnum::fromType<KeyboardAxes::MovementAxes>();
};
}
