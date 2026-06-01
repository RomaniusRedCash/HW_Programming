#include "Header.h"
#include "window/window.h"
// #include "object/rectangle/rectangle.h"
// #include "object/circle/circle.h"
// #include "space/space.h"
#include "space/world.h"
// #include "route/route.h"
// #include "route/planer/planer.h"
#include "rrt_lang/parser/parser.h"
#include "rrt_lang/parser/words/words.h"
#include "rrt_lang/lexer/lexer.h"
#include "rrt_lang/runtime/runtime.h"
#include <iostream>
#include <sstream>
// #include <vector>
#include <thread>
#include <unistd.h>

bool is_running = true;

int main() {
    // box_word basebox_word;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "ERROR! init SDL failed: " << SDL_GetError() << std::endl;
        return -1;
    }

    world wrld(700,700);

    std::thread th([&wrld](){
        lexer lx;
        // std::string str = "b 300 100 100 500\nb 400 100 200 100\nb 100 500 200 100\nb 100 300 500 100\nb 100 100 100 200\nb 500 400 100 200";
        // std::string str ="map 100 100 ";
        // std::stringstream sin(str);
        std::string buffer_data;
        while (is_running) {
            auto start_time = std::chrono::steady_clock::now();
            bool show_prompt = true;
            std::thread th([&start_time,&show_prompt](){
                while (show_prompt &&
                    std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start_time).count() < 50
                ) std::this_thread::sleep_for(std::chrono::milliseconds(10));
                if (show_prompt) std::cout << "<RRT:" << std::to_string(lexer::n_line) << ">: " << std::flush;
            });
            if (!std::getline(std::cin, buffer_data)) {
                th.join();
                break;
            }
            show_prompt = false;
            th.join();

            std::stringstream ss(buffer_data);
            ss >> lx;
            runtime::run(parser::parce(lx.get_tokens()), wrld);
            lx.clear();
        }
    });

    while (wrld.win.update() && is_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000/60));
    }
    SDL_Quit();
    // th.join();
    std::quick_exit(0);
    return 0;
}

double get_distance2(float x1, float y1, float x2, float y2) {
    double x=x1-x2;
    double y=y1-y2;
    return x*x+y*y;
}
