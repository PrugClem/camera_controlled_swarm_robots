mkdir obj
g++ -Wall -O3 -std=c++17 -IE:\Privat\Programmieren\Klassen\cppsock -IE:/Privat/Programmieren/Klassen/mingw-std-threads-master -c main.cpp -o obj/main.o
g++ -Wall -O3 -std=c++17 -IE:\Privat\Programmieren\Klassen\cppsock -IE:/Privat/Programmieren/Klassen/mingw-std-threads-master -c msh_src/sockethandler.cpp -o obj/sockethandler.o
g++ -Wall -O3 -std=c++17 -IE:\Privat\Programmieren\Klassen\cppsock -IE:/Privat/Programmieren/Klassen/mingw-std-threads-master -c msh_src/server.cpp -o obj/server.o
g++ -Wall -O3 -std=c++17 -IE:\Privat\Programmieren\Klassen\cppsock -IE:/Privat/Programmieren/Klassen/mingw-std-threads-master -c msh_src/client.cpp -o obj/client.o
g++ -o SockenHandler.exe obj/main.o obj/sockethandler.o obj/server.o obj/client.o E:\Privat\Programmieren\Klassen\cppsock\libcppsock.a E:\Privat\Programmieren\Klassen\cppsock\cppsock_winonly.cpp C:\CodeBlocks\gcc-8.2-32\i686-pc-mingw32\lib\libws2_32.a -s
ar -r libsockethandler.a obj/sockethandler.o obj/server.o obj/client.o E:\Privat\Programmieren\Klassen\cppsock\libcppsock.a E:\Privat\Programmieren\Klassen\cppsock\cppsock_winonly.cpp C:\CodeBlocks\gcc-8.2-32\i686-pc-mingw32\lib\libws2_32.a -s