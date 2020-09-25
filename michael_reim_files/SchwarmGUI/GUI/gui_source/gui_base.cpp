#include "../gui_includes/gui_base.h"

using namespace GUI;

Element::Element(std::atomic_int* width_ptr, std::atomic_int* height_ptr, atomic_float* aspect_ptr)
{
    this->__width_ptr = width_ptr;
    this->__height_ptr = height_ptr;
    this->__aspect_ptr = aspect_ptr,

    this->__mesh_changed = true;
    this->__text_mesh_changed = true;

    this->__last_aspect = this->aspect_value();
    
    this->__last_cursor_x = 0.0f;
    this->__last_cursor_y = 0.0f;
}