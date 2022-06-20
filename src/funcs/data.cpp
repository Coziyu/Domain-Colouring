#include <vector>
#include "glm/glm.hpp"

std::vector<float> vertices = {
   0.5f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f,
  -0.5f, -0.5f,  0.0f,
  -0.5f,  0.5f,  0.0f
};

  glm::mat4 model(1.0f);
  glm::mat4 view(1.0f);


double mouse_scroll_y_offset = 0;
bool mouse_left_pressed = false;
double mouse_left_press_x_pos = 0;
double mouse_left_press_y_pos = 0;
double mouse_left_current_x_pos = 0;
double mouse_left_current_y_pos = 0;

double camera_x_pos = 0;
double camera_y_pos = 0;
double camera_zoom = 1;