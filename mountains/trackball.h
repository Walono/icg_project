#pragma once
#include "icg_common.h"

class Trackball {
public:
    Trackball() : _radius(1.0f) {}

    // This function is called when the user presses the left mouse button down.
    // x, and y are in [-1, 1]. (-1, -1) is the bottom left corner while (1, 1)
    // is the top right corner.
    void begin_drag(float x, float y) {
      _anchor_pos = vec3(x, y, 0.0f);
      project_onto_surface(_anchor_pos);
    }

    // This fucntion is called while the user moves the curser around while the
    // left mouse button is still pressed.
    // x, and y are in [-1, 1]. (-1, -1) is the bottom left corner while (1, 1)
    // is the top right corner.
    // Returns the rotation of the trackball in matrix form.
    mat4 drag(float x, float y) {
      vec3 current_pos = vec3(x, y, 0.0f);
      project_onto_surface(current_pos);

      mat4 rotation = mat4::Identity();
      if (current_pos == _anchor_pos) {
        return rotation;
      }

      const float angle_boost = 6.0f;
      vec3 axis = _anchor_pos.cross(current_pos);
      float angle = angle_boost * atan2(axis.norm(), _anchor_pos.dot(current_pos));
      rotation = Eigen::Affine3f(Eigen::AngleAxisf(angle, axis.normalized())).matrix();

      return rotation;
    }

private:
    // Projects the point p (whose z coordiante is still empty/zero) onto the
    // trackball surface. If the position at the mouse cursor is outside the
    // trackball, use a hyberbolic sheet as explained in:
    // https://www.opengl.org/wiki/Object_Mouse_Trackball.
    // The trackball radius is given by '_radius'.
    void project_onto_surface(vec3& p) const {
      const float rad2 = _radius * _radius;
      const float length2 = p.x() * p.x() + p.y() * p.y();

      if (length2 <= rad2 * 0.5f) {
        p.z() = std::sqrt(rad2 - length2);
      } else {
        p.z() = rad2 / (2.0f * std::sqrt(length2));
      }
      float length = std::sqrt(length2 + p.z() * p.z());
      p /= length;
    }

    float _radius;
    vec3 _anchor_pos;
    mat4 _rotation;
};
