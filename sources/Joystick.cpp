#include "Joystick.hxx"
#include <QDebug>
#include <unordered_map>

namespace Ide::Ui{
Joystick *Joystick::instance = nullptr;
qml::RegisterType<Joystick> Joystick::Register;

QList<QString> JoystickAxes::JoystickAxesNames = {
    "···",
    "Left -X",
    "Left +X",
    "Left -Y",
    "Left +Y",
    "Right -X",
    "Right +X",
    "Right -Y",
    "Right +Y",
    "-L2",
    "L2",
    "-R2",
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
    "mAxis X (yaw)",
    "pAxis X (yaw)",
    "mAxis Y (forward)",
    "pAxis Y (forward)",
    "mAxis W (side)",
    "pAxis W (side)",
    "mAxis Z (depth)",
    "pAxis Z (depth)",
    "-L2",
    "L2",
    "-R2",
    "R2",
    "Slower",
    "Faster",
    "Axis count",
};

Joystick::Joystick() {
    if (!glfwInit()) {
        throw std::runtime_error{"Failed to initialize GLFW"};
    }

    if (instance != nullptr) {
        throw std::runtime_error{"Instance of gamepad already exists"};
    }

    loadSettings();

    qmlRegisterUncreatableType<JoystickAxes>("mur.GamepadAxes", 1, 0, "GamepadAxes", "");

    m_updateTimer = new QTimer{};
    m_updateTimer->setInterval(25);
    connect(m_updateTimer, &QTimer::timeout, this, &Joystick::onUpdateTimeout);
    m_updateTimer->start();

    this->connectJoystick();
    glfwSetJoystickCallback(joystickConnectionCallback);
}

void Joystick::joystickConnectionCallback(int jid, int event)
{
    if (instance) {
        instance->connectJoystick();
    }
}

void Joystick::onUpdateTimeout() {
    glfwPollEvents();

    if (isJoystickConnected() and !m_isKeyboardMode) {
        int axisCount, buttonCount;
        const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axisCount);
        const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

        for (int i = 0; i < axisCount; i++) {
            onAxisEvent(i * 2, axisCount, axes[i]);
        }

        for (int i = 0; i < buttonCount; i++) {
            onButtonEvent(i, axisCount * 2 + 1, buttons[i]);
        }
    }
}

void Joystick::onAxisEvent(int axis, int count, double value){
    int current_index = (value < 0.0) ? axis + 1 : axis + 2;
    int another_index = current_index % 2 == 0 ? axis + 1 : axis + 2;

    double previousValue = m_gamepadAllValues[current_index];

    if (value == 0.0) {
        m_gamepadAllValues[current_index] = 0.0;
        m_gamepadAllValues[another_index] = 0.0;
    } else {
        m_gamepadAllValues[current_index] = qAbs(value);
        m_gamepadAllValues[another_index] = 0.0;
    }

    if (previousValue != value) {
        emit axesValueChanged();
    }

    if (m_isRebinding && qAbs(qAbs(value) - m_calibrationValues[current_index]) > rebindThreshold) {
        rebindAxis(current_index);
    }
}

void Joystick::onButtonEvent(int button, int count, bool pressed){
    int index = button + count;
    double previousValue = m_gamepadAllValues[index];
    double value = (pressed ? 1 : 0);
    m_gamepadAllValues[index] = value;

    if (previousValue != value){
        emit axesValueChanged();
    }

    if (m_isRebinding && (qAbs(value - m_calibrationValues[index])) > rebindThreshold) {
        rebindAxis(index);
    }
}

int Joystick::getAxisValue(JoystickAxes::MovementAxes axis){
    double value = qAbs(m_gamepadAllValues[m_gamepadAxesBindings[axis]] - m_calibrationValues[m_gamepadAxesBindings[axis]]);
    double range = value * (1 + m_calibrationValues[m_gamepadAxesBindings[axis]]);

    return calcAxisValue(std::ceil(range * 100));
}

int Joystick::getAxesValue(JoystickAxes::MovementAxes negative_axis, JoystickAxes::MovementAxes positive_axis){
    int m_value = -1 * getAxisValue(negative_axis);
    int p_value = getAxisValue(positive_axis);
    return qAbs(m_value) > p_value ? m_value : p_value;
}

bool Joystick::getButtonValue(JoystickAxes::MovementAxes axis) {
    double value = m_gamepadAllValues[m_gamepadAxesBindings[axis]] - m_calibrationValues[m_gamepadAxesBindings[axis]];
    double range = value * (1 + m_calibrationValues[m_gamepadAxesBindings[axis]]);

    return calcAxisValue(std::ceil(range * 100)) > m_buttonThreshold;
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
    if (axis == JoystickAxes::AxisZero) {
        m_isRebinding = false;
        emit rebinded();
    } else {
        m_isRebinding = true;
    }
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

QVariantList Joystick::getAllAxesBindings() {
    QVariantList axes;

    for (int i = 0; i < JoystickAxes::AxisCount; i++) {
        QVariantMap value;
        auto[name, index] = getGamepadAxisName(i);
        value["axis_index"] = index;
        value["axis_name"] = name;
        axes.append(value);
    }

    return axes;
}

std::pair<QString, QString> Joystick::getGamepadAxisName(int axis)
{
    int index = m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(axis)];

    QString axis_name = (index < 0 || index >= JoystickAxes::JoystickAxesNames.size())
                            ? QString::number(index)
                            : JoystickAxes::JoystickAxesNames[m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(axis)]];

    QString axis_index = index == 0 ? "···" : QString::number(index);

    return {axis_name, axis_index};
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
    return glfwJoystickPresent(GLFW_JOYSTICK_1);
}

Joystick *Joystick::Create(){
    instance = new Joystick{};
    return instance;
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

bool Joystick::getKeyboardMode() {
    return m_isKeyboardMode;
}

void Joystick::setKeyboardMode(bool val)
{
    m_isKeyboardMode = val;
    emit keyboardModeChanged();
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

int Joystick::getButtonThreshold() {
    return m_buttonThreshold;
}

void Joystick::setButtonThreshold(int value) {
    m_buttonThreshold = value;
    rebindThreshold = static_cast<double>(value) / 100;
    emit buttonThresholdChanged();
}

void Joystick::calibrateJoystick() {
    const int numSamples = 10;
    QMap<int, double> samples;

    for (int i = 0; i < numSamples; i++) {
        for (int k = 0; k < m_gamepadAllValues.size(); k++) {
            samples[k] += qAbs(m_gamepadAllValues[k]);
        }
    }

    for (int i = 0; i < m_gamepadAllValues.size(); i++) {
        m_calibrationValues[i] = samples[i] / numSamples;
    }
}

void Joystick::resetCalibration() {
    for(int i = 0; i < m_calibrationValues.size(); i++){
        m_calibrationValues[i] = 0;
    }
}

void Joystick::setCalibration(bool calibrated){
    if (calibrated) {
        calibrateJoystick();
    } else {
        resetCalibration();
    }
    m_isCalibrated = calibrated;
}

bool Joystick::getCalibration(){
    return m_isCalibrated;
}

QVariantList Joystick::getPresetNames()
{
    QSettings settings("settings.ini", QSettings::IniFormat);

    QStringList groupsNames = settings.childGroups();
    QVariantList presetNames;

    for(int i=0 ; i < groupsNames.length() ; i++)
    {
        QString some_name = groupsNames.at(i);
        if (some_name.startsWith("Preset")){
            presetNames.append(some_name.remove(0, 7));
        }
    }

    return presetNames;
}

void Joystick::deletePreset(QString preset_name){
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.remove(preset_name);
}

void Joystick::setLastPresetName(QString name){
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginGroup("Gamepad");
    settings.setValue("preset_name", name);
    settings.endGroup();
}

QString Joystick::getLastPresetName(){
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginGroup("Gamepad");
    QString name = settings.value("preset_name", 0).toString();
    settings.endGroup();

    return name == "0" ? "Default" : name;
}

void Joystick::saveSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    QString name = "Preset_" + getLastPresetName();

    settings.beginWriteArray(name + "/axes");
    for (int i = 0; i < JoystickAxes::AxisCount; i++) {
        settings.beginGroup(metaEnum.valueToKey(i));
        settings.setValue("binding", m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(i)]);
        settings.setValue("calibration", m_calibrationValues[static_cast<JoystickAxes::MovementAxes>(i)]);
        settings.endGroup();
    }
    settings.endArray();

    settings.setValue(name + "/deadzone", m_deadzone);
    settings.setValue(name + "/exp_factor", m_expFactor);
    settings.setValue(name + "/button_threshold", m_buttonThreshold);
    settings.setValue(name + "/calibrated", m_isCalibrated);

    emit presetSaved();
}

void Joystick::loadSettings()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    QString name = "Preset_" + getLastPresetName();

    settings.beginReadArray(name + "/axes");

    for (int i = 0; i < JoystickAxes::AxisCount ; i++) {
        settings.beginGroup(metaEnum.valueToKey(i));
        m_calibrationValues[static_cast<JoystickAxes::MovementAxes>(i)] = settings.value("calibration", 0.0).toDouble();
        m_gamepadAxesBindings[static_cast<JoystickAxes::MovementAxes>(i)] = settings.value("binding", 0).toUInt();
        settings.endGroup();
    }

    settings.endArray();

    m_gamepadAxesBindings[JoystickAxes::AxisWm] = 0;
    m_gamepadAxesBindings[JoystickAxes::AxisWp] = 0;

    toDefaultSettings(false);

    m_deadzone = settings.value(name + "/deadzone", 0).toInt();
    m_expFactor = settings.value(name + "/exp_factor", 1.0).toDouble();
    m_buttonThreshold = settings.value(name + "/button_threshold", 50).toInt();
    m_isCalibrated = settings.value(name + "/calibrated", false).toBool();

    emit deadzoneChanged();
    emit expFactorChanged();
    emit buttonThresholdChanged();
    emit calibrationChanged();
    emit rebinded();
}

void Joystick::toDefaultSettings(bool force) {
    std::unordered_map<JoystickAxes::MovementAxes, int> defaultAxes = {
        {JoystickAxes::AxisXm, 1},
        {JoystickAxes::AxisXp, 2},
        {JoystickAxes::AxisYm, 3},
        {JoystickAxes::AxisYp, 4},
        {JoystickAxes::AxisZm, 7},
        {JoystickAxes::AxisZp, 8},
        {JoystickAxes::SpeedSlow, 17},
        {JoystickAxes::SpeedFast, 18},
    };

    for (const auto& [axis, defaultValue] : defaultAxes) {
        auto currentValue = m_gamepadAxesBindings[axis];
        m_gamepadAxesBindings[axis] = force || currentValue == 0 ? defaultValue : currentValue;
    }

    if (force) {
        setDeadzone(0);
        setExpFactor(1.0);
        setButtonThreshold(50);
    }

    emit rebinded();
}

void Joystick::connectJoystick()
{
    auto joy = glfwJoystickPresent(GLFW_JOYSTICK_1);

    if (!qApp->closingDown()) {
        emit onJoystickConnectedChanged(joy);
    }
}
}

