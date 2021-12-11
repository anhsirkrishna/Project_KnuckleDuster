#pragma once

#include "GL\glew.h"
#include <iostream>

//Helper fuction to convert textures from pixel coords to 0-1 range
//Also swaps the coordinates for use
void ConvertTextureCoords(GLfloat* tex_coords, float tex_width, float tex_height);

void DEBUG_LOG(std::string const log_text);

template <typename T, size_t TSize>
struct RingBuffer {
    enum { array_size = TSize + 1 };
    T array[array_size];
    size_t read;
    size_t write;
    RingBuffer() :read(0), write(0) {}

    T pop() {
        T p;
        if (read != write) {
            p = array + read;
            read = (read + 1) % array_size;
        }
        return p;
    }
    bool is_empty() { return write == read; }
    bool is_full() { return (write + 1) % array_size == read; }
    void push(T v) {
        array[write] = v;
        write = (write + 1) % array_size;
        //Gracefully handle write overflow
        if (write == read)read = (read + 1) % array_size;

    }
};