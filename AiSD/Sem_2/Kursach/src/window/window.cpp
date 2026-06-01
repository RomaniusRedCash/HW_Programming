#include "window.h"
#include <iostream>
#include <cmath>

extern bool is_running;

window::window(int width, int height) : width(width), height(height) {
    create_window();
    std::cout<<"Create window"<<std::endl;
}

window::~window() {
    destroy_window();
}

void window::create_window() {
    if (!SDL_CreateWindowAndRenderer("RRT", width, height, 0, &window_data, &renderer_data)) {
        std::cerr << "ERROR! failed create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(-1);
    }
}

void window::destroy_window() {
    if (renderer_data) {
        SDL_DestroyRenderer(renderer_data);
        renderer_data = nullptr;
    }
    if (window_data) {
        SDL_DestroyWindow(window_data);
        window_data = nullptr;
    }
}

void window::set_size(int width, int height) {
    // std::lock_guard<std::mutex> lock(render_mutex);
    std::cout<<"resize with "<<width<<"x"<<height<<std::endl;
    this->height=height;
    this->width=width;
    resize = true;
}

std::pair<int, int> window::get_size() const {
    return {width, height};
}

void window::set_foreground(uint8_t r, uint8_t g, uint8_t b) {}

void window::add_object(const object *obj) {
    if (!obj) return;
    std::lock_guard<std::mutex> lock(get_mutex());
    v_objects.push_back(obj);
}

void window::add_object(const shape *obj){
    if (!obj) return;
    std::visit([this](const auto& concrete_shape){this->add_object(&concrete_shape);}, *obj);
};

// void window::add_object(const rectangle *obj){add_object(obj);};
// void window::add_object(const circle *obj){add_object(obj);};
// void window::add_object(const line *obj){add_object(obj);};

std::mutex &window::get_mutex() const {
    return render_mutex;
}

// std::vector<const object *> window::ger_objects() {
//     return v_objects;
// }

bool window::update() {
    const std::lock_guard<std::mutex> lock(get_mutex());
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            is_running = false;
            return false;
        }
    }

    if (resize){
        destroy_window();
        create_window();
        resize = false;
    }

    SDL_SetRenderDrawColor(renderer_data, base_color[0], base_color[1], base_color[2], 255);
    SDL_RenderClear(renderer_data);

    for (const object* obj : v_objects){
        if (obj)
            obj->render(renderer_data);
    }

    SDL_RenderPresent(renderer_data);

    return true;
}

void window::clear() {
    const std::lock_guard<std::mutex> lock(get_mutex());
    v_objects.clear();
}

// bool window::del(size_t id) {
//     const std::lock_guard<std::mutex> lock(get_mutex());
//     if (id < v_objects.size()) {
//         v_objects[id] = nullptr;
//         return true;
//     }
//     return false;
// }
