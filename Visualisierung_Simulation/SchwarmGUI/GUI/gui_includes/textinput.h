#ifndef TEXTINPUT_H_INCLUDED
#define TEXTINPUT_H_INCLUDED

#include "gui_base.h"
#include <chrono>

#if defined(_GLIBCXX_HAS_GTHREADS) && defined(_GLIBCXX_USE_C99_STDINT_TR1)
    #include <mutex>
    using std::mutex;
#else
    #include <mingw.mutex.h>
    using mingw_stdthread::mutex;
#endif

namespace GUI
{
    class TextInput : public Element
    {
        using atomic_float = std::atomic<float>;

    private:
        inline static constexpr size_t  MSG_BOX_VERTEX_COUNT = 4,
                                        CURSOR_VERTEX_COUNT = 4;

        std::atomic_bool is_active, is_cursor_active;
        atomic_float pos_x, pos_y,
                     size_x, size_y,
                     cursor_pos_x, cursor_pos_y;
        std::atomic_uint cursor_char_pos;   // Position at which character the cursor stands.
        float text_input_color[16], text_color[4], cursor_color[4];
        gl::string text;
        std::chrono::milliseconds cursor_interval;
        float* mesh_buff;
        mutex text_input_mutex;

        void __destruct(void) noexcept;
        void calc_cursor_pos(float mouse_x) noexcept;

    protected:
        virtual bool has_text(void) const noexcept              {return true;}

        virtual const float* generate_mesh(void);
        virtual const float* generate_text_mesh(void);
        virtual const float* mesh(void) const noexcept          {return this->mesh_buff;}
        virtual const float* text_mesh(void) const noexcept     {return this->text.mesh();}

        virtual size_t vertex_count(void) const noexcept        {return MSG_BOX_VERTEX_COUNT + CURSOR_VERTEX_COUNT;}
        virtual size_t mesh_length(void) const noexcept         {return this->vertex_count() * (Element::color_component_count() + Element::vertex_component_count());}
        virtual size_t mesh_buffer_size(void) const noexcept    {return this->mesh_length() * sizeof(float);}
        virtual size_t text_length(void) const noexcept         {return this->text.length();}
        virtual size_t text_buff_size(void) const noexcept      {return this->text.length() * this->text.vertex_count() * this->text.component_count();}

        virtual std::vector<int> handle(void);

    public:
        TextInput(void) : TextInput(nullptr, nullptr, nullptr) {}
        TextInput(std::atomic_int* width_ptr, std::atomic_int* height_ptr, atomic_float* aspect_ptr);
        TextInput(const TextInput& text_input) = delete;
        virtual ~TextInput(void) {this->__destruct();}

        virtual Type get_type(void) noexcept                    {return Element::Type::TEXT_INPUT;}

        void set_pos_x(float x) noexcept;
        void set_pos_y(float y) noexcept;
        void set_pos(float x, float y) noexcept;
        void set_size_x(float x) noexcept;
        void set_size_y(float y) noexcept;
        void set_size(float x, float y) noexcept;
        void set_text_input_color(const float* color) noexcept;
        void set_text_color(const float* color) noexcept;
        void set_cursor_color(const float* color) noexcept;
        void set_col_space(float space) noexcept;
        void set_cursor_interval(std::chrono::milliseconds interval)    {this->cursor_interval = interval;}

        float get_pos_x(void) const noexcept                    {return this->pos_x;}
        float get_pos_y(void) const noexcept                    {return this->pos_y;}
        void get_pos(float& x, float& y) const noexcept         {x = this->pos_x, y = this->pos_y;}
        float get_size_x(void) const noexcept                   {return this->size_x;}
        float get_size_y(void) const noexcept                   {return this->size_y;}
        void get_size(float& x, float& y) const noexcept        {x = this->size_x; y = this->size_y;}
        float get_col_space(void) const noexcept                {return this->text.col_space();}
        const float* get_text_input_color(void) const noexcept  {return this->text_input_color;}
        const float* get_text_color(void) const noexcept        {return this->text_color;}
        const float* get_cursor_color(void) const noexcept      {return this->cursor_color;}

        void activate(bool b) noexcept                          {this->is_active = b;}
        bool active(void) const noexcept                        {return this->is_active;}
        void activate_cursor(bool b) noexcept;
        bool cursor_active(void) noexcept                       {return this->is_cursor_active;}
    };

    enum TextInputAction : int
    {
        TEXT_WRITE          = 0x101,
        TEXT_DELETE         = 0x102,
        TEXT_ENTER          = 0x103,
        TEXT_BOX_ACTIVATE   = 0x104,
        TEXT_BOX_DEACTIVATE = 0x105
    };
};

#endif // TEXTINPUT_H_INCLUDED
