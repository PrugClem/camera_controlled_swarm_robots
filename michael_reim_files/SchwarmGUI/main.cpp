#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glfw.h>
#include <GL/glstr.h>
#include <GL/Shader.h>
#include <GL/glc.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <CL/opencl.h>
#include <Event.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "includes/window_hanlder.h"
#include "includes/my_msg.h"
#include "GUI/gui_includes/gui.h"
#include "includes/my_event.h"

void init_OpenCL(const std::string& device_name_regex, cl_command_queue& cmd_queue, cl_context& context)
{
    cmd_queue = NULL;

    // get platforms
    unsigned int num_platforms;
    clGetPlatformIDs(0, NULL, &num_platforms);
    cl_platform_id platforms[num_platforms];
    int err = clGetPlatformIDs(num_platforms, platforms, NULL);

    if(err != 0)
    {
        std::cout << get_msg("ERROR / INIT OpenCL") << "Failed to load platforms. OpenCL error: " << err << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / INIT OpenCL") << "Platforms loaded (" << num_platforms << ")." << std::endl;

    cl_device_id device = NULL;
    cl_platform_id platform = NULL;
    bool found_matching_GPU = false;

    //get right device & platform
    for(unsigned int i=0; i<num_platforms && !found_matching_GPU; i++)
    {

        // get devices
        unsigned int num_devices;
        clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_devices);
        cl_device_id devices[num_devices];
        err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, num_devices, devices, NULL);

        if(err != 0)
        {
            std::cout << get_msg("ERROR / INIT OpenCL") << "Failed to load devices. OpenCL error: " << err << std::endl;
            exit(-1);
        }
        std::cout << get_msg("INFO / INIT OpenCL") << "Devices loaded (" << num_devices << ") for platform " << i << "." << std::endl;

        if(err == 0)
        {
            // search for right device
            for(unsigned int j=0; j<num_devices && !found_matching_GPU; j++)
            {
                // get device info
                char info[64];
                clGetDeviceInfo(devices[j], CL_DEVICE_NAME, sizeof(info), info, NULL);

                std::string str = info;
                int found = str.find(device_name_regex, 0);
                if(found != -1)
                {
                    found_matching_GPU = true;
                    platform = platforms[i];
                    device = devices[j];
                }
            }
        }
    }

    if(device == NULL)
    {
        std::cout << get_msg("ERROR / INIT OpenCL") << "No device found with regex: \"" + device_name_regex + "\"." << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / INIT OpenCL") <<  "Device found with regex: \"" + device_name_regex + "\"." << std::endl;

    char info[64];
    clGetPlatformInfo(platform, CL_PLATFORM_NAME, sizeof(info), info, NULL);
    std::cout << get_msg("INFO / INIT OpenCL") <<  "Used platform name: \"" << info << "\"."  << std::endl;

    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(info), info, NULL);
    std::cout << get_msg("INFO / INIT OpenCL") <<  "Used device name: \"" << info << "\"."  << std::endl;

    cl_context_properties properties[] = {
	    CL_GL_CONTEXT_KHR,      (cl_context_properties)wglGetCurrentContext(),
	    CL_WGL_HDC_KHR,         (cl_context_properties)wglGetCurrentDC(),
        CL_CONTEXT_PLATFORM,    (cl_context_properties)platform,
	    0
    };

    context = clCreateContext(properties, 1, &device, NULL, NULL, &err);
    if(err != 0)
    {
        std::cout << get_msg("ERROR / INIT OpenCL") << "Failed to create OpenCL context. OpenCL error: " << err << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / INIT OpenCL") << "OpenCL context created." << std::endl;

    cmd_queue = clCreateCommandQueue(context, device, 0, &err);
    if(err != 0)
    {
        std::cout << get_msg("ERROR / INIT OpenCL") << "Failed to create OpenCL command queue." << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / INIT OpenCL") << "OpenCL command queue created." << std::endl;
}

void init_OpenGL(int width, int height, int pos_x, int pos_y, int interval)
{
    std::cout << get_msg("INFO / WINDOW HANDLER") << "Started Window-Handler." << std::endl;

    glfwInit();
    std::cout << get_msg("INFO / OpenGL") << "GLEW initialized." << std::endl;
    if(!glfwOpenWindow(width, height, 24, 24, 24, 24, 24, 0, GLFW_WINDOW))
    {
        std::cout << get_msg("ERROR / OpenGL WINDOW") << "Failed to create OpenGL window." << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwSetWindowPos(pos_x, pos_y);
    glfwSetWindowTitle("Scharm GUI und Visualisierung - 0.0.0");
    std::cout << get_msg("INFO / OpenGL WINDOW") << "OpenGL window created." << std::endl;
    glewInit();
    std::cout << get_msg("INFO / OpenGL") << "GLFW initialized." << std::endl;
    glfwSwapInterval(interval);    // limit to 60 fps
}

void init_event_handler(EventHandler& event_handler)
{
    Listener* button_listener = new ButtonListener();
    event_handler.add_listener(button_listener);
    std::cout << get_msg("INFO / EVENT-HANDLER") << "Initialized Event-Handler." << std::endl;
}

void load_button_font(gl::Font& font)
{
    constexpr char path_font[32] = "Font/arial.png";
    constexpr char path_offset[32] = "Font/arial_offset.txt";

    std::cout << get_msg("INFO / LOADING FONT") << "Loading font offsets \"" << path_offset << "\"." << std::endl;
    std::fstream font_offset(path_offset, std::ios::in);
    std::vector<gl::CharacterOffset> offsets;

    if(!font_offset)
        std::cout << get_msg("INFO / LOADING FONT") << "Unable open file \"" << path_offset << "\"." << std::endl;

    // read offsets from file
    gl::CharacterOffset offset;
    while(!font_offset.eof())
    {
        font_offset >> offset.positive_x >> offset.positive_y >> offset.negative_x >> offset.negative_y;
        offsets.push_back(offset);
    }
    font_offset.close();

    std::cout << get_msg("INFO / LOADING FONT") << "Loading font \"" << path_font << "\"." << std::endl;
    gl::FontError err = font.load(path_font, true, offsets);
    if(err == gl::FontError::FONT_ERROR_INVALID_PATH)
    {
        std::cout << get_msg("ERROR / LOADING FONT") << "Unable to find path \"" << path_font << "\"." << std::endl;
        exit(-1);
    }
    else if(err == gl::FontError::FONT_ERROR_INVALID_IMAGE_SIZE)
    {
        std::cout << get_msg("ERROR / LOADING FONT") << "Button font has invalid image size." << std::endl;
        exit(-1);
    }
    else if(err == gl::FontError::FONT_ERROR_INVALID_NUM_CHARS)
    {
        std::cout << get_msg("ERROR / LOADING FONT") << "Button font has invalid number of characters." << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / LOADING FONT") << "Button font loaded." << std::endl;
}

void load_shader(gl::Shader& shader, const std::string& vertex_fname, const std::string& fragment_fname)
{
    static const std::string path("Shaders/");
    std::cout << get_msg("INFO / LOADING SHADER") << "Loading \"" << path + vertex_fname << "\" and \"" << path + fragment_fname << "\"." << std::endl;
    gl::ShaderLoadError err = shader.load(path + vertex_fname, path + fragment_fname);
    if(err & gl::ShaderLoadErrorType::INVALID_FILE_PATH)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Unable to find path \"" << path + vertex_fname << "\" or \"" << path + fragment_fname << "\"." << std::endl;
        exit(-1);
    }
    if(err & gl::ShaderLoadErrorType::SHADER_ALREADY_LOADED)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Shader has already been loaded." << std::endl;
        exit(-1);
    }
    if(err & gl::ShaderLoadErrorType::VERTEX_SHADER_ERROR)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Compile error occured in vertex shader." << std::endl;
        std::cout << get_msg("INFO / VERTEX SHADER") << shader.get_last_vertex_msg() << std::endl;
        exit(-1);
    }
    if(err & gl::ShaderLoadErrorType::FRAGMENT_SHADER_ERROR)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Compile error occured in fragment shader." << std::endl;
        std::cout << get_msg("INFO / FRAGMENT SHADER") << shader.get_last_fragment_msg() << std::endl;
        exit(-1);
    }
    if(err & gl::ShaderLoadErrorType::SHADER_LINK_ERROR)
    {
        std::cout << get_msg("ERROR / LOADING SHADER") << "Link error occured." << std::endl;
        std::cout << get_msg("INFO / LINK SHADER") << shader.get_last_link_msg() << std::endl;
        exit(-1);
    }
    std::cout << get_msg("INFO / LOADING SHADER") << "Shader \"" << path + vertex_fname << "\" and \"" << path + fragment_fname << "\" loaded." << std::endl;
}

int min_time(const std::vector<int>& vec)
{
    int min = vec[0];
    for(int i : vec)
    {
        if(min > i)
            min = i;
    }
    return min;
}

int max_time(const std::vector<int>& vec)
{
    int max = vec[0];
    for(int i : vec)
    {
        if(max < i)
            max = i;
    }
    return max;
}

double avg_time(const std::vector<int>& vec)
{
    double sum = 0.0f;
    for(int i : vec)
        sum += i;
    return sum / vec.size();
}

int main()  // its showtime
{
    /* declaration */
    // programm running valriable
    bool main_running = true;

    // shader filenamen
    const std::string button_vertex_shader_fname("button_vertex.txt");
    const std::string button_fragment_shader_fname("button_fragment.txt");
    const std::string text_vertex_shader_fname("text_vertex.txt");
    const std::string text_fragment_shader_fname("text_fragment.txt");

    // OpenGL render matrices
    glm::mat4 projection2D;

    /* INIT */
    // init event handler
    EventHandler event_handler(ListenerType::DYNAMIC_LISTENER);
    init_event_handler(event_handler);
    event_handler.start();
    std::cout << get_msg("INFO / EVENT-HANDLER") << "Started Event-Handler." << std::endl;

    // start window size
    WindowHandler::init(1920 / 2, 1080 / 2);
    int win_pos_x = 1920 / 4;
    int win_pos_y = 1080 / 4;

    // init OpenGL
    init_OpenGL(WindowHandler::get_width(), WindowHandler::get_height(), win_pos_x, win_pos_y, 1);

    // init OpenCL
    cl_command_queue cmd_queue;
    cl_context context;
    init_OpenCL("GeForce", cmd_queue, context);

    // load fonts
    gl::Font button_font;
    load_button_font(button_font);

    // load shaders
    gl::Shader button_shader(256), text_shader(256);
    load_shader(button_shader, button_vertex_shader_fname, button_fragment_shader_fname);
    load_shader(text_shader, text_vertex_shader_fname, text_fragment_shader_fname);

    // create buttons
    GUI::Element::set_font(&button_font);
    float button_color[] = {0.1f, 0.1f, 0.1f, 1.0f,
                            0.1f, 0.1f, 0.1f, 1.0f,
                            0.1f, 0.1f, 0.1f, 1.0f,
                            0.1f, 0.1f, 0.1f, 1.0f};
    float button_text_color[] = {0.0f, 1.0f, 1.0f, 1.0f};

    GUI::Button test_button(WindowHandler::get_width_ptr(), WindowHandler::get_height_ptr(), WindowHandler::get_aspect_ptr());
    test_button.set_pos(0.2f, 0.2f);
    test_button.set_size(0.5f, 0.5f);
    test_button.set_button_color(button_color);
    test_button.set_text_value("Button");
    test_button.set_text_color(button_text_color);
    test_button.set_text_size(gl::convert::from_pixels_size(50, WindowHandler::get_height()));

    GUI::Button test_button2(WindowHandler::get_width_ptr(), WindowHandler::get_height_ptr(), WindowHandler::get_aspect_ptr());
    test_button2.set_pos(-0.7f, 0.2f);
    test_button2.set_size(0.5f, 0.5f);
    test_button2.set_button_color(button_color);
    test_button2.set_text_value("Arial");
    test_button2.set_text_color(button_text_color);
    test_button2.set_text_size(gl::convert::from_pixels_size(50, WindowHandler::get_height()));

    // init button handler
    GUI::ElementHandler button_handler;
    button_handler.attach_element(test_button);
    button_handler.attach_element(test_button2);
    button_handler.start(std::chrono::milliseconds(5));
    std::cout << get_msg("INFO / BUTTON-HANDLER") << "Started Button-Handler." << std::endl;

    // init button renderer
    GUI::ElementRenderer button_renderer(context, cmd_queue);
    button_renderer.attach_handler(button_handler);
    button_renderer.init();
    button_renderer.start(std::chrono::milliseconds(5));
    std::cout << get_msg("INFO / BUTTON-RENDERER") << "Started Button-Renderer." << std::endl;

    /* setup frame time measurement */
    unsigned int timer_query;
    glGenQueries(1, &timer_query);
    std::vector<int> time_results;

    while(main_running)
    {
        button_renderer.update();

        glBeginQuery(GL_TIME_ELAPSED, timer_query);

        WindowHandler::update();
        projection2D = glm::ortho(-WindowHandler::get_aspect(), WindowHandler::get_aspect(), -1.0f, 1.0f, -1.0f, 1.0f);
        text_shader.uniform_matrix_4x4f("projection2D", 1, true, glm::value_ptr(projection2D));
        text_shader.uniform_1f("th0", 0.40f);
        text_shader.uniform_1f("th1", 0.55f);

        glViewport(0, 0, WindowHandler::get_width(), WindowHandler::get_height());
        glClearColor(0.15f, 0.15f, 0.15f, 1.0f); // darkmode
        glClear(GL_COLOR_BUFFER_BIT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_TEXTURE_2D);

        button_shader.use();
        glBindVertexArray(button_renderer.get_element_vertex_array());
        glDrawArrays(GL_QUADS, 0, button_renderer.get_element_vertex_count());

        text_shader.use();
        glBindVertexArray(button_renderer.get_text_vertex_array());
        glBindTexture(GL_TEXTURE_2D, button_font.texture_id());
        glMultiDrawArraysIndirect(GL_QUADS, button_renderer.get_text_command(), button_renderer.get_text_count(), button_renderer.get_text_command_stride());
        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        glfwSwapBuffers();
        main_running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);

        glEndQuery(GL_TIME_ELAPSED);
        int time;
        glGetQueryObjectiv(timer_query, GL_QUERY_RESULT, &time);
        time_results.push_back(time);
    }

    glDeleteQueries(1, &timer_query);
    std::cout << get_msg("INFO / OpenGL QUERY") << "All queries deleted." << std::endl;

    button_renderer.stop();
    std::cout << get_msg("INFO / BUTTON RENDERER") << "Stopped Button-Renderer." << std::endl;

    glfwTerminate();
    std::cout << get_msg("INFO / OpenGL WINDOW") << "OpenGL window Closed." << std::endl;
    std::cout << get_msg("INFO / OpenGL") << "OpenGL content successfully terminated." << std::endl;

    button_handler.stop();
    std::cout << get_msg("INFO / BUTTON HANDLER") << "Stopped Button-Handler." << std::endl;

    event_handler.stop();
    event_handler.cleanup();
    std::cout << get_msg("INFO / EVENT HANDLER") << "Stopped Event-Handler." << std::endl;

    std::cout <<  get_msg("INFO") << "MIN-Frame-Time: " << min_time(time_results) / 1000.0f / 1000.0f << "ms / frame" << std::endl;
    std::cout <<  get_msg("INFO") << "MAX-Frame-Time: " << max_time(time_results) / 1000.0f / 1000.0f << "ms / frame" << std::endl;
    std::cout <<  get_msg("INFO") << "AVG-Frame-Time: " << avg_time(time_results) / 1000.0f / 1000.0f << "ms / frame" << std::endl;

    std::cout << get_msg("INFO / EXIT") << "Exit status 0." << std::endl;
    return 0;   // you have been terminated
}
