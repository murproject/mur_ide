#pragma once

#include <QString>

namespace Ide::Ui {

class Application {
public:
  Application() = default;
  ~Application() = default;
  static Application *instance;
  static QString getResourcesDirectory();
  static int execute(int, char *[]);

  private:
  static void initialize();
  static void setupEnvironment();
  static void addFontDirectory();
  static QString m_resourceDirectory;
};
}
