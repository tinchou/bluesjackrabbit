#include "Surface.h"
#include <iostream>

Surface::Surface() {
    centro_x = 0.5;
    centro_y = 0.5;
}

Surface::~Surface() {
}

bool Surface::on_draw(const Cairo::RefPtr<Cairo::Context> &cr) {
    cr->set_source_rgb(0.86, 0.85, 0.47);
    cr->paint();  // fill image with the color
    cr->restore();  // color is back to black now

    cr->save();
    // draw a border around the image
    cr->set_line_width(20.0);  // make the line wider
    cr->rectangle(0.0, 0.0, get_width(), get_height());
    cr->stroke();

    cr->set_source_rgba(0.0, 0.0, 0.0, 0.7);
    // draw a circle in the center of the image
    cr->arc(centro_x * get_width(),
            centro_y * get_height(),
            get_height() / 4,
            0.0,
            2.0 * M_PI);
    cr->stroke();

    // draw a diagonal line
    cr->move_to(get_width() / 4.0, get_height() / 4.0);
    cr->line_to(get_width() * 3.0 / 4.0, get_height() * 3.0 / 4.0);
    cr->stroke();

    return true;
}

void Surface::moverArriba() {
    centro_y -= 0.01;
    queue_draw();
}

void Surface::moverAbajo() {
    centro_y += 0.01;
    queue_draw();
}

void Surface::moverIzq() {
    centro_x -= 0.01;
    queue_draw();
}

void Surface::moverDer() {
    centro_x += 0.01;
    queue_draw();
}
