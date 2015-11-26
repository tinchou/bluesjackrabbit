#ifndef BLUESJACKRABBIT_CLIENT_CHARACTERRENDERER_H
#define BLUESJACKRABBIT_CLIENT_CHARACTERRENDERER_H


#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>
#include "Renderer.h"

class CharacterRenderer : public Renderer {
 public:
  explicit CharacterRenderer(const Vector &position, double radius);
  virtual ~CharacterRenderer();
  virtual void render(const Cairo::RefPtr<Cairo::Context> &cr);
 private:
  double radius_;
  unsigned int sprite_step_;
  double last_x_;
  bool first_render_;
  // Cairo::RefPtr<Cairo::ImageSurface> image_;
  Glib::RefPtr<Gdk::Pixbuf> image_;
  std::vector<Glib::RefPtr<Gdk::Pixbuf> > images_right_;
  std::vector<Glib::RefPtr<Gdk::Pixbuf> > images_left_;
};


#endif  // BLUESJACKRABBIT_CLIENT_CHARACTERRENDERER_H
