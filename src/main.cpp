#include "application/app.hpp"

int main(int argc, char **argv) {
  t_app_settings settings = {1000, 800, "Render"};

  App app(settings);
  if (app.init() != 0) {
    return -1;
  }

  app.run();

  return 0;
}
