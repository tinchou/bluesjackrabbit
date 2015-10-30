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
#include "ServerProxy.h"

MainWindow::MainWindow(SceneRenderer *scene, const ServerProxy &server_proxy)
  : main_frame(),
    initial_screen(),
    new_game_screen(),
    map_id(0),
    scene(scene),
    map_combo(NULL) {
  this->server_proxy = server_proxy;
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

  connected = this->server_proxy.connect();
}

MainWindow::~MainWindow() {
}

void MainWindow::new_game_click() {
  if (connected) {
    std::map<size_t, std::string> maps = server_proxy.list_maps();
    load_combo(maps);
    initial_screen.hide();
    new_game_screen.show();
  }
}

void MainWindow::join_game_click() {
  initial_screen.hide();
  join_game_screen.show();
}

void MainWindow::init_click() {
  server_proxy.start_game(map_id);
  new_game_screen.hide();
  scene->show();
}

Glib::RefPtr<Gtk::Builder> MainWindow::load_from_glade(std::string file_name, Gtk::Box *parent) {
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
  Glib::RefPtr<Gtk::Builder> builder = load_from_glade("static/main_frame.glade", &initial_screen);
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
  Glib::RefPtr<Gtk::Builder> builder = load_from_glade("static/new_game.glade", &new_game_screen);
  Gtk::Button *button = NULL;
  builder->get_widget("start", button);
  if (button) {
    button->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::init_click));
  }
  builder->get_widget("map", map_combo);
  // Create the Combobox Tree model:
  combo_model = Gtk::ListStore::create(columns);
  map_combo->set_model(combo_model);
  map_combo->signal_changed().connect(sigc::mem_fun(*this, &MainWindow::combo_map_changed));
  map_combo->pack_start(columns.id);
  map_combo->pack_start(columns.map_name);
}

void MainWindow::combo_map_changed() {
  // std::cout << ((*map_combo->get_active())[columns.id]) << std::endl;
  map_id = ((*map_combo->get_active())[columns.id]);
}

void MainWindow::init_join_game_screen() {
  Glib::RefPtr<Gtk::Builder> builder = load_from_glade("static/join_game.glade", &join_game_screen);
}


void MainWindow::load_combo(const std::map<size_t, std::string> &names) {
  for (std::map<size_t, std::string>::const_iterator it = names.begin(); it != names.end(); it++) {
    Gtk::TreeModel::Row row = *(combo_model->append());
    row[columns.id] = it->first;
    row[columns.map_name] = it->second;
    // map_combo->set_active(row);
  }
}
