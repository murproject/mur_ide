#include "Keyboard.hxx"

namespace Ide::Ui{

Keyboard *Keyboard::instance = nullptr;
qml::RegisterType<Keyboard> Keyboard::Register;

Keyboard::Keyboard() {
    if(QSysInfo::productType() == "windows"){
        keyboardCodeSymbol = {
            {"None", -1}, {"`", 41}, {"1", 2}, {"2", 3}, {"3", 4}, {"4", 5}, {"5", 6}, {"6", 7}, {"7", 8}, {"8", 9}, {"9", 10}, {"0", 11}, {"-", 12}, {"=", 13}, {"Backspace", 14},
            {"Tab", 15}, {"Q", 16}, {"W", 17}, {"E", 18}, {"R", 19}, {"T", 20}, {"Y", 21}, {"U", 22}, {"I", 23}, {"O", 24}, {"P", 25}, {"[", 26}, {"]", 27}, {"Enter", 28},
            {"Caps Lock", 58}, {"A", 30}, {"S", 31}, {"D", 32}, {"F", 33}, {"G", 34}, {"H", 35}, {"J", 36}, {"K", 37}, {"L", 38}, {";", 39}, {"'", 40}, {"\\", 43},
            {"Shift", 42}, {"Z", 44}, {"X", 45}, {"C", 46}, {"V", 47}, {"B", 48}, {"N", 49}, {"M", 50}, {",", 51}, {".", 52}, {"/", 53}, {"Shift R", 54},
            {"Ctrl", 29}, {"Space", 57}, {"Ctrl R", 285}, {"↑", 328}, {"→", 333}, {"↓", 336}, {"←", 331}
        };
    } else {
        keyboardCodeSymbol = {
            {"None", -1}, {"`", 49}, {"1", 10}, {"2", 11}, {"3", 12}, {"4", 13}, {"5", 14}, {"6", 15}, {"7", 16}, {"8", 17}, {"9", 18}, {"0", 19}, {"-", 20}, {"=", 21}, {"Backspace", 22},
            {"Tab", 23}, {"Q", 24}, {"W", 25}, {"E", 26}, {"R", 27}, {"T", 28}, {"Y", 29}, {"U", 30}, {"I", 31}, {"O", 32}, {"P", 33}, {"[", 34}, {"]", 35}, {"Enter", 36},
            {"Caps Lock", 66}, {"A", 38}, {"S", 39}, {"D", 40}, {"F", 41}, {"G", 42}, {"H", 43}, {"J", 44}, {"K", 45}, {"L", 46}, {";", 47}, {"'", 48}, {"\\", 51},
            {"Shift", 50}, {"Z", 52}, {"X", 53}, {"C", 54}, {"V", 55}, {"B", 56}, {"N", 57}, {"M", 58}, {",", 59}, {".", 60}, {"/", 61}, {"Shift R", 62},
            {"Ctrl", 37}, {"Space", 65}, {"Ctrl R", 105}, {"↑", 111}, {"→", 114}, {"↓", 116}, {"←", 113}
        };
    }

    loadSettings();
    qmlRegisterUncreatableType<KeyboardAxes>("input.KeyboardAxes", 1, 0, "KeyboardAxes", "");
}

Keyboard *Keyboard::Create(){
    instance = new Keyboard{};
    return instance;
}

void Keyboard::requestRebind(int axis) {
    m_isRebinding = true;
    m_currentlyRebindingAxis = static_cast<KeyboardAxes::MovementAxes>(axis);
    emit rebindingChanged();
}

void Keyboard::rebindAxis(int axis) {
    m_keyboardAxesBindings[m_currentlyRebindingAxis] = axis;

    m_currentlyRebindingAxis = KeyboardAxes::AxisZero;
    m_isRebinding = false;
    emit rebindingChanged();
    emit rebinded();
}

int Keyboard::getAxisBinding(int axis) {
    return m_keyboardAxesBindings[static_cast<KeyboardAxes::MovementAxes>(axis)];
}

QList<QString> Keyboard::getAllAxesBindings() {
    QList<QString> axes;

    for (int i = 0; i < KeyboardAxes::AxisCount; i++) {
        axes.append(getKeyName(m_keyboardAxesBindings[static_cast<KeyboardAxes::MovementAxes>(i)]));
    }
    return axes;
}

int Keyboard::getRebindingAxis() {
    return m_currentlyRebindingAxis;
}

QString Keyboard::getKeyName(int nativeCode){
    QString keyName = keyboardCodeSymbol.key(nativeCode, "");
    return keyName == "" ? "None" : keyName;
}

void Keyboard::setAxisValue(int id_key, int value) {
    for (int i = 0; i < KeyboardAxes::AxisCount; ++i) {
        KeyboardAxes::MovementAxes axis = static_cast<KeyboardAxes::MovementAxes>(i);

        if (m_keyboardAxesBindings[axis] == id_key) {
            m_keyboardAllValues[axis] = value;
            emit axesValueChanged();
        }
    }
}

int Keyboard::getAxisValue(KeyboardAxes::MovementAxes axis){
    return m_keyboardAllValues[axis];
}

QList<qreal> Keyboard::getAllAxes() {
    QList<qreal> axes;
    for (int i = 0; i < KeyboardAxes::AxisCount; i++) {
        axes.append(getAxisValue(static_cast<KeyboardAxes::MovementAxes>(i)));
    }
    return axes;
}

int Keyboard::getAxesValue(KeyboardAxes::MovementAxes negative_axis, KeyboardAxes::MovementAxes positive_axis){
    int m_value = getAxisValue(negative_axis);
    int p_value = getAxisValue(positive_axis);
    return qAbs(m_value) > p_value ? m_value : p_value;
}

void Keyboard::swapAxes(int axis1, int axis2){
    auto keyboardAxis1 = m_keyboardAxesBindings[static_cast<KeyboardAxes::MovementAxes>(axis1)];
    auto keyboardAxis2 = m_keyboardAxesBindings[static_cast<KeyboardAxes::MovementAxes>(axis2)];

    m_keyboardAxesBindings[static_cast<KeyboardAxes::MovementAxes>(axis2)] = keyboardAxis1;
    m_keyboardAxesBindings[static_cast<KeyboardAxes::MovementAxes>(axis1)] = keyboardAxis2;

    m_isRebinding = false;
    emit rebindingChanged();
    emit rebinded();
}

void Keyboard::clearAxis(int axis){
    m_currentlyRebindingAxis = static_cast<KeyboardAxes::MovementAxes>(axis);
    rebindAxis(0);
}

void Keyboard::saveSettings(){
    QSettings settings("settings.ini", QSettings::IniFormat);

    settings.beginWriteArray("Keyboard/axes");

    for (int i = 0; i < KeyboardAxes::AxisCount; i++) {
        settings.beginGroup(metaEnum.valueToKey(i));
        settings.setValue("binding", getKeyName(m_keyboardAxesBindings[static_cast<KeyboardAxes::MovementAxes>(i)]));
        settings.endGroup();
    }
    settings.endArray();
}

void Keyboard::loadSettings(){
    QSettings settings("settings.ini", QSettings::IniFormat);
    QStringList groupsNames = settings.childGroups();
    bool settingsExists = false;

    for(int i=0 ; i < groupsNames.length() ; i++)
    {
        if (groupsNames.at(i) == "Keyboard"){
            settingsExists = true;
        }
    }

    settings.beginReadArray("Keyboard/axes");

    for (int i = 0; i < KeyboardAxes::AxisCount ; i++) {
        settings.beginGroup(metaEnum.valueToKey(i));

        QString value = settingsExists ? settings.value("binding", 0).toString() : "None";
        m_keyboardAxesBindings[static_cast<KeyboardAxes::MovementAxes>(i)] = keyboardCodeSymbol.value(value, 0);

        settings.endGroup();
    }
    settings.endArray();

    m_keyboardAxesBindings[KeyboardAxes::AxisWm] = keyboardCodeSymbol.value("None", 0);
    m_keyboardAxesBindings[KeyboardAxes::AxisWp] = keyboardCodeSymbol.value("None", 0);

    toDefaultSettings(false);

    emit rebinded();
}

void Keyboard::toDefaultSettings(bool force) {
    std::unordered_map<KeyboardAxes::MovementAxes, int> defaultAxes = {
       {KeyboardAxes::AxisXm, keyboardCodeSymbol.value("A", 0)},
       {KeyboardAxes::AxisXp, keyboardCodeSymbol.value("D", 0)},
       {KeyboardAxes::AxisYm, keyboardCodeSymbol.value("W", 0)},
       {KeyboardAxes::AxisYp, keyboardCodeSymbol.value("S", 0)},
       {KeyboardAxes::AxisZm, keyboardCodeSymbol.value("Q", 0)},
       {KeyboardAxes::AxisZp, keyboardCodeSymbol.value("E", 0)},
       {KeyboardAxes::SpeedSlow, keyboardCodeSymbol.value("L", 0)},
       {KeyboardAxes::SpeedFast, keyboardCodeSymbol.value("K", 0)},
       };

    for (const auto& [axis, defaultValue] : defaultAxes) {
        auto currentValue = m_keyboardAxesBindings[axis];
        m_keyboardAxesBindings[axis] = force || currentValue == -1 ? defaultValue : currentValue;
    }

    emit rebinded();
}
}
