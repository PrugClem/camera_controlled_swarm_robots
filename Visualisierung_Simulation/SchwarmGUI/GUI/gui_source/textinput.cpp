#include "../gui_includes/textinput.h"

using namespace GUI;

TextInput::TextInput(std::atomic_int* width_ptr, std::atomic_int* height_ptr, atomic_float* aspect_ptr)
: Element(width_ptr, height_ptr, aspect_ptr)
{
    this->is_active = false;
    this->is_cursor_active = false;

    this->pos_x = 0.0f;
    this->pos_y = 0.0f;
    this->size_x = 0.0f;
    this->size_y = 0.0f;

    for(size_t i=0; i < 16; i++)
        this->text_input_color[i] = 0.0f;
    
    for(size_t i=0; i < 4; i++)
    {
        this->text_color[i] = 0.0f;
        this->cursor_color[i] = 0.0f;
    }

    this->text.set_font(*Element::get_font());

    this->cursor_interval = std::chrono::seconds(1);
    this->mesh_buff = nullptr;

    this->set_mesh_changed(true);
    this->set_text_mesh_changed(true);
}

void TextInput::__destruct(void) noexcept
{
    delete(this->mesh_buff);
    this->mesh_buff = nullptr;
}

void TextInput::calc_cursor_pos(float mouse_x) noexcept
{
    
}

const float* TextInput::generate_mesh(void)
{

}

const float* TextInput::generate_text_mesh(void)
{
    
}

std::vector<int> TextInput::handle(void)
{

}

void TextInput::set_pos_x(float x) noexcept
{
    this->pos_x = x;
    this->set_mesh_changed(true);
}

void TextInput::set_pos_y(float y) noexcept
{
    this->pos_y = y;
    this->set_mesh_changed(true);
}

void TextInput::set_pos(float x, float y) noexcept
{
    this->pos_x = x;
    this->pos_y = y;
    this->set_mesh_changed(true);
}

void TextInput::set_size_x(float x) noexcept
{
    this->size_x = x;
    this->set_mesh_changed(true);
}

void TextInput::set_size_y(float y) noexcept
{
    this->size_y = y;
    this->set_mesh_changed(true);
}

void TextInput::set_size(float x, float y) noexcept
{
    this->size_x = x;
    this->size_y = y;
    this->set_mesh_changed(true);
}

void TextInput::set_text_input_color(const float* color) noexcept
{
    this->text_input_mutex.lock();
    memcpy(this->text_input_color, color, 16 * sizeof(float));
    this->text_input_mutex.unlock();
    this->set_mesh_changed(true);
}

void TextInput::set_text_color(const float* color) noexcept
{
    this->text_input_mutex.lock();
    memcpy(this->text_color, color, 4 * sizeof(float));
    this->text_input_mutex.unlock();
    this->text.set_color_all(color[0], color[1], color[2], color[3]);
    this->set_text_mesh_changed(true);
}

void TextInput::set_cursor_color(const float* color) noexcept
{
    this->text_input_mutex.lock();
    memcpy(this->cursor_color, color, 4 * sizeof(float));
    this->text_input_mutex.unlock();
    this->set_mesh_changed(true);
}

void TextInput::set_col_space(float space) noexcept
{
    this->text.set_col_space(space);
    this->set_text_mesh_changed(true);
}

void TextInput::activate_cursor(bool b) noexcept
{   
    // if cursor gets disabled, set opacity (alpha value) to 0
    this->cursor_color[3] = (b) ? this->cursor_color[3] : 0.0f;
    this->set_mesh_changed(true);
}