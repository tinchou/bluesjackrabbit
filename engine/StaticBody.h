#ifndef BLUESJACKRABBIT_STATICBODY_H
#define BLUESJACKRABBIT_STATICBODY_H


#include "Body.h"
#include "Vector.h"

/**
 * Actual implementation of the Body class. Provides static body physics, useful for objects like walls and floors.
 */
class StaticBody : public Body {
 public:
  explicit StaticBody(Vector *position);
  virtual ~StaticBody();
  virtual const Vector &velocity() const;
  virtual void apply_force(const Vector &vector);
  virtual void set_velocity(const Vector &velocity);
  virtual void apply_jump_force();
  virtual Vector position() const;
  virtual void reposition(const Vector &reposition);
  virtual Vector next_position() const;
  virtual void update_fixed();
  virtual void stop();
  virtual void stop_x();
  virtual void stop_y();
  virtual bool stopped();
  virtual bool stopped_x();
  virtual void handle_line_hit(Line line);

 private:
  static const Vector velocity_;
  Vector *position_;
};


#endif  // BLUESJACKRABBIT_STATICBODY_H
