#include "Gamepad.hxx"
#include <QDebug>
#include <QSettings>

namespace Ide::Ui {
Gamepad *Gamepad::instance = nullptr;
qml::RegisterType<Gamepad> Gamepad::Register;

QList<QString> GamepadAxes::GamepadAxesNames = {
    "···",
    "Left X",
    "Left Y",
    "Right X",
    "Right Y",
    "A",
    "B",
    "X",
    "Y",
    "L1",
    "R1",
    "L2",
    "R2",
    "Select",
    "Start",
    "L3",
    "R3",
    "Up",
    "Down",
    "Right",
    "Left",
    "Center",
    "Guide",
};

QList<QString> GamepadAxes::MovementAxesNames = {
    "Empty",
    "Axis X (yaw)",
    "Axis Y (forward)",
    "Axis W (side)",
    "Axis Z (depth)",
    "Slower",
    "Faster",
    "Axis count",
};

Gamepad::Gamepad()
{
    if (instance != nullptr) {
        throw std::runtime_error{"Instance of gamepad already exists"};
    }
    m_gamepad = new QGamepad{};
    loadSettings();

    qmlRegisterUncreatableType<GamepadAxes>("mur.GamepadAxes", 1, 0, "GamepadAxes", "");

    m_updateTimer = new QTimer{};
    m_updateTimer->setInterval(25);
    connect(m_updateTimer, &QTimer::timeout, this, &Gamepad::onUpdateTimeout);
    m_updateTimer->start();

    this->connectGamepad();
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadAxisEvent, this, &Gamepad::axisEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonPressEvent, this, &Gamepad::buttonEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::gamepadButtonReleaseEvent, this, &Gamepad::buttonReleaseEvent);
    connect(QGamepadManager::instance(), &QGamepadManager::connectedGamepadsChanged, this, &Gamepad::connectGamepad);
}

void Gamepad::onUpdateTimeout() {
    if (m_axisChanged) {
        m_axisChanged = false;
        emit axesValueChanged();
    }
}

void Gamepad::axisEvent(int deviceId, QGamepadManager::GamepadAxis axis, double value) {
    m_gamepadAllValues[axis + 1] = value;

    if (m_isRebinding && qAbs(value) > 0.6) {
        rebindAxis(axis + 1);
    }

    m_axisChanged = true;
}

void Gamepad::buttonReleaseEvent(int deviceId, QGamepadManager::GamepadButton button) {
    buttonEvent(deviceId, button, 0.0);
}

void Gamepad::buttonEvent(int deviceId, QGamepadManager::GamepadButton button, double value) {
    value = value * 2 -1;
    m_gamepadAllValues[button + 5] = value;

    if (m_isRebinding && qAbs(value) > 0.6) {
        rebindAxis(button + 5);
    }

    emit axesValueChanged();
}

int Gamepad::getAxisValue(GamepadAxes::MovementAxes axis) {
    return calcAxisValue(m_gamepadAllValues[m_gamepadAxesBindings[axis]] * (m_gamepadAxesInversions[axis] ? -100 : 100));
}

int Gamepad::getAxisValueRaw(int axis) {
    auto movAxis = static_cast<GamepadAxes::MovementAxes>(axis);
    return m_gamepadAllValues[m_gamepadAxesBindings[movAxis]] * (m_gamepadAxesInversions[movAxis] ? -100 : 100);
}

bool Gamepad::getButtonValue(GamepadAxes::MovementAxes axis) {
    return calcAxisValue(m_gamepadAllValues[m_gamepadAxesBindings[axis]] * (m_gamepadAxesInversions[axis] ? -100 : 100)) > 50;
}


bool Gamepad::getAxisInversion(GamepadAxes::MovementAxes axis) {
    return m_gamepadAxesInversions[axis];
}

void Gamepad::setAxisInversion(int axis, bool inversed) {
    m_gamepadAxesInversions[static_cast<GamepadAxes::MovementAxes>(axis)] = inversed;
    emit rebinded();
}

void Gamepad::rebindAxis(int axis) {
    m_gamepadAxesBindings[m_currentlyRebindingAxis] = axis;
    m_currentlyRebindingAxis = GamepadAxes::AxisZero;
    m_isRebinding = false;
    emit rebindingChanged();
    emit rebinded();
}

void Gamepad::clearAxis(int axis) {
    m_currentlyRebindingAxis = static_cast<GamepadAxes::MovementAxes>(axis);
    setAxisInversion(axis, false);
    rebindAxis(0);
}

void Gamepad::swapAxes(int axis1, int axis2) {
    auto gamepadAxis1 = m_gamepadAxesBindings[static_cast<GamepadAxes::MovementAxes>(axis1)];
    auto gamepadAxis2 = m_gamepadAxesBindings[static_cast<GamepadAxes::MovementAxes>(axis2)];

    m_gamepadAxesBindings[static_cast<GamepadAxes::MovementAxes>(axis2)] = gamepadAxis1;
    m_gamepadAxesBindings[static_cast<GamepadAxes::MovementAxes>(axis1)] = gamepadAxis2;

    m_isRebinding = false;
    emit rebindingChanged();
    emit rebinded();
}

void Gamepad::requestRebind(int axis) {
    m_isRebinding = true;
    m_currentlyRebindingAxis = static_cast<GamepadAxes::MovementAxes>(axis);
    emit rebindingChanged();
}

QList<qreal> Gamepad::getAllAxes() {
    QList<qreal> axes;

    for (int i = 0; i < GamepadAxes::AxisCount; i++) {
        axes.append(getAxisValue(static_cast<GamepadAxes::MovementAxes>(i)));
    }

    return axes;
}

QList<bool> Gamepad::getAllAxesInversions() {
    QList<bool> axes;

    for (int i = 0; i < GamepadAxes::AxisCount; i++) {
        axes.append(getAxisInversion(static_cast<GamepadAxes::MovementAxes>(i)));
    }

    return axes;
}

QList<QString> Gamepad::getAllAxesBindings() {
    QList<QString> axes;

    for (int i = 0; i < GamepadAxes::AxisCount; i++) {
        axes.append(getGamepadAxisName(i));
    }

    return axes;
}

QString Gamepad::getGamepadAxisName(int axis)
{
    return GamepadAxes::GamepadAxesNames[m_gamepadAxesBindings[static_cast<GamepadAxes::MovementAxes>(axis)]];
}

QString Gamepad::getMovementAxisName(int axis)
{
    return GamepadAxes::MovementAxesNames[static_cast<GamepadAxes::MovementAxes>(axis)];
}

bool Gamepad::isRebinding() {
    return m_isRebinding;
}

int Gamepad::getRebindingAxis() {
    return m_currentlyRebindingAxis;
}

void Gamepad::setForceAxisValue(int axis, int value) {
    m_gamepadAllValues[axis] = value / 100.0f;
    emit axesValueChanged();
}

void Gamepad::addForceAxisValue(int axis, int value) {
    m_gamepadAllValues[axis] = m_gamepadAllValues[axis] + (value / 100.0f);
    emit axesValueChanged();
}

bool Gamepad::isGamepadConnected() {
    return m_gamepad->isConnected();
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

int Gamepad::calcAxisValue(int val) {
    if (abs(val) < m_deadzone) {
        return 0;
    }

    bool invert = val < 0;

    val = (abs(static_cast<double>(val)) - m_deadzone) / (100.0f - m_deadzone) * 100.0f;
    val = qPow(static_cast<double>(val), m_expFactor) / qPow(100.0f, m_expFactor - 1.0f);

    val *= (invert ? -1 : 1);
    val = qMax(qMin(val, 100), -100);

    return val;
}

int Gamepad::getDeadzone() {
    return m_deadzone;
}

void Gamepad::setDeadzone(int val)
{
    m_deadzone = val;
    emit deadzoneChanged();
}

double Gamepad::getExpFactor() {
    return m_expFactor;
}

void Gamepad::setExpFactor(double value) {
    m_expFactor = value;
    emit expFactorChanged();
}

void Gamepad::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginWriteArray("Gamepad/axes");
    for (int i = 0; i < GamepadAxes::AxisCount; i++) {
        settings.setArrayIndex(i);
        settings.setValue("binding", m_gamepadAxesBindings[static_cast<GamepadAxes::MovementAxes>(i)]);
        settings.setValue("inversion", m_gamepadAxesInversions[static_cast<GamepadAxes::MovementAxes>(i)]);
    }
    settings.endArray();

    settings.setValue("Gamepad/deadzone", m_deadzone);
    settings.setValue("Gamepad/exp_factor", m_expFactor);
    settings.endArray();
}

void Gamepad::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    int size = settings.beginReadArray("Gamepad/axes");
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        m_gamepadAxesBindings[static_cast<GamepadAxes::MovementAxes>(i)] = settings.value("binding", 0).toInt();
        m_gamepadAxesInversions[static_cast<GamepadAxes::MovementAxes>(i)] = settings.value("inversion", false).toBool();
        m_gamepadAxesBindings[GamepadAxes::AxisW] = 0;
    }
    settings.endArray();

    if (m_gamepadAxesBindings[GamepadAxes::AxisX] == 0) {
        m_gamepadAxesBindings[GamepadAxes::AxisX] = 1;
    }

    if (m_gamepadAxesBindings[GamepadAxes::AxisY] == 0) {
        m_gamepadAxesBindings[GamepadAxes::AxisY] = 2;
    }

    if (m_gamepadAxesBindings[GamepadAxes::AxisZ] == 0) {
        m_gamepadAxesBindings[GamepadAxes::AxisZ] = 4;
    }

    if (m_gamepadAxesBindings[GamepadAxes::SpeedSlow] == 0) {
        m_gamepadAxesBindings[GamepadAxes::SpeedSlow] = 5;
    }

    if (m_gamepadAxesBindings[GamepadAxes::SpeedFast] == 0) {
        m_gamepadAxesBindings[GamepadAxes::SpeedFast] = 6;
    }

    m_deadzone = settings.value("Gamepad/deadzone", 0).toInt();
    m_expFactor = settings.value("Gamepad/exp_factor", 1.0).toDouble();

    settings.endArray();

    emit deadzoneChanged();
    emit expFactorChanged();
    emit rebinded();
}

void Gamepad::connectGamepad()
{
    auto gamepads = QGamepadManager::instance()->connectedGamepads();

    if (!gamepads.isEmpty()) {
        m_gamepad->setDeviceId(*gamepads.begin());
    }

    if (!qApp->closingDown()) {
        emit onGamepadConnectedChanged(!gamepads.isEmpty());
    }
}


} 
