// Este include no iria. Deberiamos lanzar excepcion en caso de no encontrar archivo.
#include <iostream>
#include <glibmm/fileutils.h>
#include <glibmm/markup.h>
#include <gtkmm/builder.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include <gtkmm/paned.h>

#include "EventBus.h"
#include "MainWindow.h"


MainWindow::MainWindow(SceneRenderer *scene)
  : main_frame(),
    initial_screen(),
    new_game_screen(),
    scene(scene) {
  set_title("Blues Jackrabbit");
  set_resizable(false);
  set_size_request(640, 480);
  set_position(Gtk::WIN_POS_CENTER);

  init_main_game_screen();
  init_new_game_screen();
  init_join_game_screen();

  main_frame.pack_start(*scene);
  main_frame.pack_start(initial_screen);
  main_frame.pack_start(new_game_screen);
  main_frame.pack_start(join_game_screen);

  add(main_frame);
  show_all();
  scene->hide();
  new_game_screen.hide();
  join_game_screen.hide();
}

MainWindow::~MainWindow() {
}

void MainWindow::new_game_click() {
  initial_screen.hide();
  new_game_screen.show();
}

void MainWindow::join_game_click() {
  initial_screen.hide();
  join_game_screen.show();
}

void MainWindow::init_click() {
  new_game_screen.hide();
  scene->show();
}

Glib::RefPtr<Gtk::Builder> MainWindow::load_from_glade(std::string file_name, Gtk::Widget *parent) {
  Glib::RefPtr<Gtk::Builder> refBuilder = Gtk::Builder::create();
  try {
    refBuilder->add_from_file(file_name);
  } catch (const Glib::FileError &ex) {
    std::cerr << "FileError: " << ex.what() << std::endl;
  } catch (const Glib::MarkupError &ex) {
    std::cerr << "MarkupError: " << ex.what() << std::endl;
  } catch (const Gtk::BuilderError &ex) {
    std::cerr << "BuilderError: " << ex.what() << std::endl;
  }
  // FRAME HARDCODE. AL menos que todos los glade lo llamen frame y listo.
  Gtk::Widget *other;
  refBuilder->get_widget("frame", other);
  other->reparent(*parent);
  return refBuilder;
}

void MainWindow::init_main_game_screen() {
  Glib::RefPtr<Gtk::Builder> builder = load_from_glade("main_frame.glade", &initial_screen);
  Gtk::Button *button = NULL;
  builder->get_widget("buttonNewGame", button);
  if (button) {
    button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::new_game_click));
  }
  button = NULL;
  builder->get_widget("buttonJoinGame", button);
  if (button) {
      button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::join_game_click));
  }
  button = NULL;
  builder->get_widget("buttonExit", button);
  if (button) {
    button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::hide));
  }
}

void MainWindow::init_new_game_screen() {
  Glib::RefPtr<Gtk::Builder> builder = load_from_glade("new_game.glade", &new_game_screen);
  Gtk::Button *button = NULL;
  builder->get_widget("start", button);
  if (button) {
    button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::init_click));
  }
}

void MainWindow::init_join_game_screen() {
  Glib::RefPtr<Gtk::Builder> builder = load_from_glade("join_game.glade", &join_game_screen);
}