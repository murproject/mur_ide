#include "Application.hxx"
#undef main
#include <SDL.h>

int main(int argc, char* argv[]) {
  return Ide::Ui::Application::execute(argc, argv);
}
