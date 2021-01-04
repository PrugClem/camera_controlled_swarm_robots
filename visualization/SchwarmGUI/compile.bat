mkdir obj
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -c main.cpp -o obj/main.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -c GUI/gui_source/button.cpp -o obj/button.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -c GUI/gui_source/gui_base.cpp -o obj/gui_base.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -c GUI/gui_source/gui_handler.cpp -o obj/gui_handler.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -c GUI/gui_source/gui_renderer.cpp -o obj/gui_renderer.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -c GUI/gui_source/textbox.cpp -o obj/textbox.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -c Vehicle/source/vehicle_src.cpp -o obj/vehicle.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -c Vehicle/source/vehicle_buffer_src.cpp -o obj/vehicle_buffer.o
g++ -Wall -O3 -std=c++17 -I../../library/glew-2.1.0/include -I../../library/GLFW-2.7/include -I../../library/gl_shader-1.0.2/include -I../../library/glstr-2.1.0/include -I../../library/glm -I../../library/glc-1.0.0 -I../../library/model_loader-1.0.0/include -I../../library/event_handler-1.1.0/include -I../../library/mingw-std-threads-master -I../../library/stb_master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -I"../../library/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/inc" -I../../library/high_resolution_timer -c Vehicle/source/vehicle_processor_src.cpp -o obj/vehicle_processor.o
g++ -Wall -O3 -std=c++17 -c SchwarmPacket/packet.cpp -o obj/packet.o
g++ -Wall -O3 -std=c++17 -c SchwarmPacket/otherpacket.cpp -o obj/otherpacket.o
g++ -Wall -O3 -std=c++17 -I../../library/mingw-std-threads-master -I../../library/sockethandler-1.0.0/include -I../../library/cppsock -c client/client.cpp -o obj/client.o
g++ -L../../library/GLFW-2.7/lib -L../../library/gl_shader-1.0.2/lib -L../../library/glstr-2.1.0/lib -L../../library/model_loader-1.0.0/lib -L../../library/ws2_32 -L../../library/cppsock -L../../library/sockethandler-1.0.0/lib -L../../library/event_handler-1.1.0/lib -o SchwarmGUI.exe obj/main.o obj/button.o obj/gui_base.o obj/gui_handler.o obj/gui_renderer.o obj/textbox.o obj/vehicle.o obj/vehicle_buffer.o obj/vehicle_processor.o obj/packet.o obj/otherpacket.o obj/client.o E:/Privat/Programmieren/Klassen/cppsock/cppsock_winonly.cpp "E:/Privat/Programmieren/Klassen/OpenCL/NVIDIA GPU Computing SDK/OpenCL/common/lib/Win32/OpenCL.lib" -l_glShader -l_GLstring ../../library/glew-2.1.0/lib/Release/Win32/glew32.lib ../../library/glew-2.1.0/lib/Release/Win32/glew32s.lib -lglfw -lopengl32 -lglu32 -lgdi32 -lModelLoader -lsockethandler -lcppsock -lws2_32 -l_event_handler -s
