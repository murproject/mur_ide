#include "Application.hxx"
#undef main

int main(int argc, char* argv[]) {
  return Ide::Ui::Application::execute(argc, argv);
}
