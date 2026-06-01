#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <mutex>
#include "../object/object.h"

class window {
    SDL_Window* window_data = nullptr;
    SDL_Renderer* renderer_data = nullptr;
    SDL_Event event;

    mutable std::mutex render_mutex;
    bool resize = false;
    int width = 800, height = 600;
    int base_color[3] = {255, 255, 255};

    std::vector<const object*> v_objects;

public:
    window(int width, int height);
    ~window();
    void create_window();
    void destroy_window();
    void set_size(int width, int height);
    std::pair<int, int> get_size() const;
    void set_foreground(uint8_t r, uint8_t g, uint8_t b);
    void add_object(const object *obj);
    void add_object(const shape *obj);
    // std::vector<const object *> ger_objects();
    std::mutex& get_mutex() const;
    bool update();
    void clear();
    // bool del(size_t id);
};
