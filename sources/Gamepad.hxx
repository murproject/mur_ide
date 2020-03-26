#pragma once

#include <QGamepad>
#include <QObject>

#include "QmlUtils.hxx"

namespace Ide::Ui {
class Gamepad : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QGamepad *Gamepad READ getGamepad CONSTANT)

    Q_PROPERTY(QString axisXName READ getAxisXname NOTIFY axisNameChanged)
    Q_PROPERTY(QString axisYName READ getAxisYname NOTIFY axisNameChanged)
    Q_PROPERTY(QString axisZName READ getAxisZname NOTIFY axisNameChanged)

    Q_PROPERTY(bool rebindX READ isRebindX NOTIFY axisNameChanged)
    Q_PROPERTY(bool rebindY READ isRebindY NOTIFY axisNameChanged)
    Q_PROPERTY(bool rebindZ READ isRebindZ NOTIFY axisNameChanged)

    Q_PROPERTY(bool inverseX READ isInverseX WRITE setInverseX NOTIFY inverseionChanged)
    Q_PROPERTY(bool inverseY READ isInverseY WRITE setInverseY NOTIFY inverseionChanged)
    Q_PROPERTY(bool inverseZ READ isInverseZ WRITE setInverseZ NOTIFY inverseionChanged)

    Q_PROPERTY(int axisXValue READ getAxisXvalue NOTIFY axesValueChanged)
    Q_PROPERTY(int axisYValue READ getAxisYvalue NOTIFY axesValueChanged)
    Q_PROPERTY(int axisZValue READ getAxisZvalue NOTIFY axesValueChanged)

public:
    static Gamepad *instance;
    static Gamepad *Create();

    QGamepad *getGamepad();

public slots:

    int getAxisXvalue();
    int getAxisYvalue();
    int getAxisZvalue();

    QString getAxisXname();
    QString getAxisYname();
    QString getAxisZname();

    void rebindAxisX();
    void rebindAxisY();
    void rebindAxisZ();

    bool isRebindX();
    bool isRebindY();
    bool isRebindZ();

    bool isInverseX();
    bool isInverseY();
    bool isInverseZ();

    void setInverseX(bool);
    void setInverseY(bool);
    void setInverseZ(bool);

    void saveSettings();
    void loadSettings();

signals:
    void axisNameChanged();
    void inverseionChanged();
    void axesValueChanged();

private:
    enum class gamepadAxes { axisLeftX, axisLeftY, axisRightX, axisRightY };

    enum class powerAxes { axisX, axisY, axisZ };

    Gamepad();
    void update();
    static qml::RegisterType<Gamepad> Register;

    QGamepad *m_gamepad;
    QMap<gamepadAxes, double> m_gamepadValues;
    QMap<powerAxes, gamepadAxes> m_gamepadBinding;
    QMap<gamepadAxes, QString> m_gamepadAxesNames;

    void onLeftXChanged();
    void onLeftYChanged();
    void onRightXChanged();
    void onRightYChanged();

    void rebind(gamepadAxes);

    bool isRebindRequested();

    bool m_rebintXrequested = false;
    bool m_rebintYrequested = false;
    bool m_rebintZrequested = false;

    bool m_isXinverse = false;
    bool m_isYinverse = false;
    bool m_isZinverse = false;
};

} // namespace ide::ui
