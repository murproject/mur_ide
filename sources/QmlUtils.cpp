#include "QmlUtils.hxx"

namespace Ide::qml {

void InitializeControllers() {
  for (const auto &func : getControllersInitializersList()) {
    func();
  }
}

QList<std::function<void()>> &getControllersInitializersList() {
  static QList<std::function<void()>> List;
  return List;
}

} //ui
