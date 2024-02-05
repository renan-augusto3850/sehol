#include <gtkmm.h>

int main(int argc, char *argv[]) {
  auto app = Gtk::Application::create(argc, argv);

  Gtk::Window window;
  window.set_title("Minha Janela");
  window.set_default_size(200, 200);

  Gtk::Button button("Clique aqui!");
  window.add(button);

  button.show();
  window.show_all();

  return app->run(window);
}