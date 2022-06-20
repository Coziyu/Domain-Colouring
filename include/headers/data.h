#pragma once
#include <vector>
#include "glm/glm.hpp"
//data
extern std::vector<float> vertices;

extern glm::mat4 model;
extern glm::mat4 view;


extern double mouse_scroll_y_offset;
extern bool mouse_left_pressed;
extern double mouse_left_press_x_pos;
extern double mouse_left_press_y_pos;
extern double mouse_left_current_x_pos;
extern double mouse_left_current_y_pos;

extern double camera_x_pos;
extern double camera_y_pos;
extern double camera_zoom;

