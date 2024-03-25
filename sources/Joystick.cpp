#include "Joystick.hxx"
#include <QDebug>

namespace Ide::Ui{
Joystick *Joystick::instance = nullptr;
qml::RegisterType<Joystick> Joystick::Register;

QList<QString> JoystickAxes::JoystickAxesNames = {
    "···",
    "Left X",
    "Left Y",
    "Right X",
    "Right Y",
    "L2",
    "R2",
    "A",
    "B",
    "X",
    "Y",
    "L1",
    "R1",
    "Select",
    "Start",
    "L3",
    "R3",
    "Up",
    "Right",
    "Down",
    "Left",
    "Center",
    "Guide",
};

QList<QString> JoystickAxes::MovementAxesNames = {
    "Empty",
    "Axis X (yaw)",
    "Axis Y (forward)",
    "Axis W (side)",
    "Axis Z (depth)",
    "L2",
    "R2",
    "Slower",
    "Faster",
    "Axis count",
};

Joystick::Joystick() {

    if (instance != nullptr) {
        throw std::runtime_error{"Instance of gamepad already exists"};
    }
    m_joystick = QJoysticks::getInstance();
    loadSettings();

    qmlRegisterUncreatableType<JoystickAxes>("mur.GamepadAxes", 1, 0, "GamepadAxes", "");

    m_updateTimer = new QTimer{};
    m_updateTimer->setInterval(25);
    connect(m_updateTimer, &QTimer::timeout, this, &Joystick::onUpdateTimeout);
    m_updateTimer->start();

    this->connectJoystick();
    connect(QJoysticks::getInstance(), &QJoysticks::axisChanged, this, &Joystick::onAxisEvent);
    connect(QJoysticks::getInstance(), &QJoysticks::buttonChanged, this, &Joystick::onButtonEvent);
    connect(QJoysticks::getInstance(), &QJoysticks::povChanged, this, &Joystick::onPOVEvent);
    connect(QJoysticks::getInstance(), &QJoysticks::countChanged, this, &Joystick::connectJoystick);
}

void Joystick::onUpdateTimeout() {
    if (m_axisChanged) {
        m_axisChanged = false;
        emit axesValueChanged();
    }
}

void Joystick::onAxisEvent(int device, int axis, double value){
    m_gamepadAllValues[axis + 1] = value;

    if (m_isRebinding && qAbs(value) > 0.6) {
        rebindAxis(axis + 1);
    }

    m_axisChanged = true;
}

void Joystick::onButtonEvent(int device, int button, bool pressed){
    double value = (pressed ? 1 : -1);
    m_gamepadAllValues[button + 7] = value;

    if (m_isRebinding && qAbs(value) > 0.6) {
        rebindAxis(button + 7);
    }

    emit axesValueChanged();
}

void Joystick::onPOVEvent(int device, int pov, int angle){
    auto value = angle / 90 + 1;

    if (angle > -1) {
        m_gamepadAllValues[16 + value] = 1;
    } else {
        for(int i = povStartIndex; i < povEndIndex; i++){
            m_gamepadAllValues[i] = -1;
        }
    }

    if (m_isRebinding && qAbs(value) > 0.6) {
        rebindAxis(16 + value);
    }

    emit axesValueChanged();
}

int Joystick::getAxisValue(JoystickAxes::MovementAxes axis){
    return calcAxisValue(m_gamepadAllValues[m_gamepadAxesBindings[axis]] * (m_gamepadAxesInversions[axis] ? -100 : 100));
}

bool Joystick::getButtonValue(JoystickAxes::MovementAxes axis) {
    return calcAxisValue(m_gamepadAllValues[m_gamepadAxesBindings[axis]] * (m_gamepadAxesInversions[axis] ? -100 : 100)) > 50;
}

bool Joystick::getAxisInversion(JoystickAxes::MovementAxes axis) {
    return m_gamepadAxesInversions[axis];
}

void Joystick::setAxisInversion(int axis, bool inversed) {
    m_gamepadAxesInversions[static_cast<JoystickAxes::MovementAxes>(axis)] = inversed;
    emit rebinded();
}

void Joystick::clearAxis(int axis) {
    m_currentlyRebindingAxis = static_cast<JoystickAxes::MovementAxes>(axis);
    setAxisInversion(axis, false);
    rebindAxis(0);
}

void Joystick::rebindAxis(int axis) {
    m_gamepadAxesBindings[m_currentlyRebindingAxis] = axis;
    m_currentlyRebindingAxis = JoystickAxes::AxisZero;
    m_isRebinding = false;
    emit rebindingChanged();
    emit rebinded();
}

void Joystick::swapAxes(int axis1, int axis2) {
    auto gamepadAxis1 = m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(axis1)];
    auto gamepadAxis2 = m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(axis2)];

    m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(axis2)] = gamepadAxis1;
    m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(axis1)] = gamepadAxis2;

    m_isRebinding = false;
    emit rebindingChanged();
    emit rebinded();
}

void Joystick::requestRebind(int axis) {
    m_isRebinding = true;
    m_currentlyRebindingAxis = static_cast<JoystickAxes::MovementAxes>(axis);
    emit rebindingChanged();
}

QList<qreal> Joystick::getAllAxes() {
    QList<qreal> axes;
    for (int i = 0; i < JoystickAxes::AxisCount; i++) {
        axes.append(getAxisValue(static_cast<JoystickAxes::MovementAxes>(i)));
    }
    return axes;
}

QList<bool> Joystick::getAllAxesInversions() {
    QList<bool> axes;

    for (int i = 0; i < JoystickAxes::AxisCount; i++) {
        axes.append(getAxisInversion(static_cast<JoystickAxes::MovementAxes>(i)));
    }

    return axes;
}

QList<QString> Joystick::getAllAxesBindings() {
    QList<QString> axes;

    for (int i = 0; i < JoystickAxes::AxisCount; i++) {
        axes.append(getGamepadAxisName(i));
    }

    return axes;
}

QString Joystick::getGamepadAxisName(int axis)
{
    return JoystickAxes::JoystickAxesNames[m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(axis)]];
}

QString Joystick::getMovementAxisName(int axis)
{
    return JoystickAxes::MovementAxesNames[static_cast<JoystickAxes::MovementAxes>(axis)];
}

bool Joystick::isRebinding() {
    return m_isRebinding;
}

int Joystick::getRebindingAxis() {
    return m_currentlyRebindingAxis;
}

void Joystick::setForceAxisValue(int axis, int value) {
    m_gamepadAllValues[axis] = value / 100.0f;
    emit axesValueChanged();
}

void Joystick::addForceAxisValue(int axis, int value) {
    m_gamepadAllValues[axis] = m_gamepadAllValues[axis] + (value / 100.0f);
    emit axesValueChanged();
}

bool Joystick::isJoystickConnected() {
    return m_joystick->joystickExists(0);
}

Joystick *Joystick::Create(){
    instance = new Joystick{};
    return instance;
}

QJoysticks *Joystick::getJoystick()
{
    return m_joystick;
}

int Joystick::calcAxisValue(int val) {
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

int Joystick::getDeadzone() {
    return m_deadzone;
}

void Joystick::setDeadzone(int val)
{
    m_deadzone = val;
    emit deadzoneChanged();
}

double Joystick::getExpFactor() {
    return m_expFactor;
}

void Joystick::setExpFactor(double value) {
    m_expFactor = value;
    emit expFactorChanged();
}

void Joystick::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginWriteArray("Gamepad/axes");
    for (int i = 0; i < JoystickAxes::AxisCount; i++) {
        settings.setArrayIndex(i);
        settings.setValue("binding", m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(i)]);
        settings.setValue("inversion", m_gamepadAxesInversions[static_cast<JoystickAxes::MovementAxes>(i)]);
    }
    settings.endArray();

    settings.setValue("Gamepad/deadzone", m_deadzone);
    settings.setValue("Gamepad/exp_factor", m_expFactor);
    settings.endArray();
}

void Joystick::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    int size = settings.beginReadArray("Gamepad/axes");
    for (int i = 0; i < size; i++) {
        settings.setArrayIndex(i);
        m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(i)] = settings.value("binding", 0).toInt();
        m_gamepadAxesInversions[static_cast<JoystickAxes::MovementAxes>(i)] = settings.value("inversion", false).toBool();
        m_gamepadAxesBindings[JoystickAxes::AxisW] = 0;
    }
    settings.endArray();

    if (m_gamepadAxesBindings[JoystickAxes::AxisX] == 0) {
        m_gamepadAxesBindings[JoystickAxes::AxisX] = 1;
    }

    if (m_gamepadAxesBindings[JoystickAxes::AxisY] == 0) {
        m_gamepadAxesBindings[JoystickAxes::AxisY] = 2;
    }

    if (m_gamepadAxesBindings[JoystickAxes::AxisZ] == 0) {
        m_gamepadAxesBindings[JoystickAxes::AxisZ] = 4;
    }

    if (m_gamepadAxesBindings[JoystickAxes::SpeedSlow] == 0) {
        m_gamepadAxesBindings[JoystickAxes::SpeedSlow] = 7;
    }

    if (m_gamepadAxesBindings[JoystickAxes::SpeedFast] == 0) {
        m_gamepadAxesBindings[JoystickAxes::SpeedFast] = 8;
    }

    m_deadzone = settings.value("Gamepad/deadzone", 0).toInt();
    m_expFactor = settings.value("Gamepad/exp_factor", 1.0).toDouble();

    settings.endArray();

    emit deadzoneChanged();
    emit expFactorChanged();
    emit rebinded();
}

void Joystick::connectJoystick()
{
    auto joy = QJoysticks::getInstance()->joystickExists(0);

    if (!qApp->closingDown()) {
        emit onJoystickConnectedChanged(joy);
    }
}
}

