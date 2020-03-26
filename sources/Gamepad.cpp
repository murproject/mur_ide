#include "Gamepad.hxx"
#include <QDebug>
#include <QSettings>

namespace Ide::Ui {
Gamepad *Gamepad::instance = nullptr;
qml::RegisterType<Gamepad> Gamepad::Register;

Gamepad::Gamepad()
{
    if (instance != nullptr) {
        throw std::runtime_error{"Instance of gamepad already exists"};
    }
    m_gamepad = new QGamepad{};
    loadSettings();

    m_gamepadAxesNames[gamepadAxes::axisLeftX] = "Left  axis X";
    m_gamepadAxesNames[gamepadAxes::axisLeftY] = "Left  axis Y";
    m_gamepadAxesNames[gamepadAxes::axisRightX] = "Right axis X";
    m_gamepadAxesNames[gamepadAxes::axisRightY] = "Right axis Y";

    connect(m_gamepad, &QGamepad::axisLeftXChanged, this, &Gamepad::onLeftXChanged);
    connect(m_gamepad, &QGamepad::axisLeftYChanged, this, &Gamepad::onLeftYChanged);
    connect(m_gamepad, &QGamepad::axisRightXChanged, this, &Gamepad::onRightXChanged);
    connect(m_gamepad, &QGamepad::axisRightYChanged, this, &Gamepad::onRightYChanged);
}

void Gamepad::update()
{
    m_gamepadValues[gamepadAxes::axisLeftX] = m_gamepad->axisLeftX();
    m_gamepadValues[gamepadAxes::axisLeftY] = m_gamepad->axisLeftY();
    m_gamepadValues[gamepadAxes::axisRightX] = m_gamepad->axisRightX();
    m_gamepadValues[gamepadAxes::axisRightY] = m_gamepad->axisRightY();
}

void Gamepad::onLeftXChanged()
{
    emit axesValueChanged();
    if (!isRebindRequested()) {
        return;
    }
    if (std::abs(m_gamepad->axisLeftX()) < 0.6) {
        return;
    }
    rebind(gamepadAxes::axisLeftX);
    emit axisNameChanged();
}

void Gamepad::onLeftYChanged()
{
    emit axesValueChanged();
    if (!isRebindRequested()) {
        return;
    }
    if (std::abs(m_gamepad->axisLeftY()) < 0.6) {
        return;
    }
    rebind(gamepadAxes::axisLeftY);
    emit axisNameChanged();
}

void Gamepad::onRightXChanged()
{
    emit axesValueChanged();
    if (!isRebindRequested()) {
        return;
    }
    if (std::abs(m_gamepad->axisRightX()) < 0.6) {
        return;
    }
    rebind(gamepadAxes::axisRightX);
    emit axisNameChanged();
}

void Gamepad::onRightYChanged()
{
    emit axesValueChanged();
    if (!isRebindRequested()) {
        return;
    }
    if (std::abs(m_gamepad->axisRightY()) < 0.6) {
        return;
    }
    rebind(gamepadAxes::axisRightY);
    emit axisNameChanged();
}

void Gamepad::rebind(Gamepad::gamepadAxes axes)
{
    if (!isRebindRequested()) {
        return;
    }
    if (m_rebintXrequested) {
        m_gamepadBinding[powerAxes::axisX] = axes;
        m_rebintXrequested = false;
    }

    if (m_rebintYrequested) {
        m_gamepadBinding[powerAxes::axisY] = axes;
        m_rebintYrequested = false;
    }

    if (m_rebintZrequested) {
        m_gamepadBinding[powerAxes::axisZ] = axes;
        m_rebintZrequested = false;
    }
    saveSettings();
}

bool Gamepad::isRebindRequested()
{
    return m_rebintXrequested || m_rebintYrequested || m_rebintZrequested;
}

Gamepad *Gamepad::Create()
{
    instance = new Gamepad{};
    return instance;
}

QGamepad *Gamepad::getGamepad()
{
    return m_gamepad;
}

int Gamepad::getAxisXvalue()
{
    update();
    auto val = static_cast<int>((m_gamepadValues[m_gamepadBinding[powerAxes::axisX]]
                                 * (isInverseX() ? -100.0 : 100.0)));
    return val;
}

int Gamepad::getAxisYvalue()
{
    update();
    auto val = static_cast<int>((m_gamepadValues[m_gamepadBinding[powerAxes::axisY]]
                                 * (isInverseY() ? -100.0 : 100.0)));
    return val;
}

int Gamepad::getAxisZvalue()
{
    update();
    auto val = static_cast<int>((m_gamepadValues[m_gamepadBinding[powerAxes::axisZ]]
                                 * (isInverseZ() ? -100.0 : 100.0)));

    return val;
}

QString Gamepad::getAxisXname()
{
    return m_gamepadAxesNames[m_gamepadBinding[powerAxes::axisX]];
}

QString Gamepad::getAxisYname()
{
    return m_gamepadAxesNames[m_gamepadBinding[powerAxes::axisY]];
}

QString Gamepad::getAxisZname()
{
    return m_gamepadAxesNames[m_gamepadBinding[powerAxes::axisZ]];
}

void Gamepad::rebindAxisX()
{
    m_rebintXrequested = true;
    emit axisNameChanged();
}

void Gamepad::rebindAxisY()
{
    m_rebintYrequested = true;
    emit axisNameChanged();
}

void Gamepad::rebindAxisZ()
{
    m_rebintZrequested = true;
    emit axisNameChanged();
}

bool Gamepad::isRebindX()
{
    return m_rebintXrequested;
}

bool Gamepad::isRebindY()
{
    return m_rebintYrequested;
}

bool Gamepad::isRebindZ()
{
    return m_rebintZrequested;
}

bool Gamepad::isInverseX()
{
    return m_isXinverse;
}

bool Gamepad::isInverseY()
{
    return m_isYinverse;
}

bool Gamepad::isInverseZ()
{
    return m_isZinverse;
}

void Gamepad::setInverseX(bool val)
{
    m_isXinverse = val;
    emit inverseionChanged();
    saveSettings();
}

void Gamepad::setInverseY(bool val)
{
    m_isYinverse = val;
    emit inverseionChanged();
    saveSettings();
}

void Gamepad::setInverseZ(bool val)
{
    m_isZinverse = val;
    emit inverseionChanged();
    saveSettings();
}

void Gamepad::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("axisX", static_cast<int>(m_gamepadBinding[powerAxes::axisX]));
    settings.setValue("axisY", static_cast<int>(m_gamepadBinding[powerAxes::axisY]));
    settings.setValue("axisZ", static_cast<int>(m_gamepadBinding[powerAxes::axisZ]));

    settings.setValue("axisXInv", m_isXinverse);
    settings.setValue("axisYInv", m_isYinverse);
    settings.setValue("axisZInv", m_isZinverse);
}

void Gamepad::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    m_gamepadBinding[powerAxes::axisX] = static_cast<gamepadAxes>(
        settings.value("axisX", 0).toInt());

    m_gamepadBinding[powerAxes::axisY] = static_cast<gamepadAxes>(
        settings.value("axisY", 1).toInt());

    m_gamepadBinding[powerAxes::axisZ] = static_cast<gamepadAxes>(
        settings.value("axisZ", 2).toInt());

    m_isXinverse = settings.value("axisXInv", false).toBool();
    m_isYinverse = settings.value("axisYInv", false).toBool();
    m_isZinverse = settings.value("axisZInv", false).toBool();

    emit inverseionChanged();
    emit axisNameChanged();
}

} // namespace ide::ui
