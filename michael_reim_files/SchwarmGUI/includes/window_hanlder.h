#ifndef WINDOW_HANLDER_H_INCLUDED
#define WINDOW_HANLDER_H_INCLUDED

#include <GL/glfw.h>
#include <atomic>

class WindowHandler
{
    using atomic_float = std::atomic<float>;

private:
    inline static std::atomic_int win_width = 0, win_height = 1, start_height = 1;
    inline static atomic_float win_aspect = 1.0f, size_aspect = 1.0f;

public:
    static void init(int width, int height)
    {
        win_width = width;
        win_height = height;
        start_height = height;

        win_aspect = (float)width / (float)height;
        size_aspect = 1.0f;
    }

    static int get_width(void)                      {return win_width;}
    static std::atomic_int* get_width_ptr(void)     {return &win_width;}
    static int get_height(void)                     {return win_height;}
    static std::atomic_int* get_height_ptr(void)    {return &win_height;}
    static float get_aspect(void)                   {return win_aspect;}
    static atomic_float* get_aspect_ptr(void)       {return &win_aspect;}
    static float get_size_aspect(void)              {return size_aspect;}
    static atomic_float* get_size_aspect_ptr(void)  {return &size_aspect;}

    static void update(void)
    {
        int width, height;
        glfwGetWindowSize(&width, &height);
        if(width != win_width || height != win_height)
        {
            height = (height < 1) ? 1 : height;
            win_aspect = (float)width / (float)height;
            size_aspect = (float)start_height / (float)height;
            win_width = width;
            win_height = height;
            glfwSetWindowSize(width, height);
        }
    }
};

#endif // WINDOW_HANLDER_H_INCLUDED
