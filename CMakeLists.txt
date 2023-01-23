cmake_minimum_required(VERSION 3.12)
project(Progetto_PO)

set(CMAKE_CXX_STANDARD 14)

include_directories(/opt/homebrew/opt/allegro/include)
/opt/homebrew/opt/allegro/include
link_directories(/opt/homebrew/opt/allegro/lib)
link_libraries(allegro allegro_main allegro_primitives allegro_font allegro_ttf)

add_executable(Progetto_PO main.cpp)
