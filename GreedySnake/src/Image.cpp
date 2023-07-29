#include <iostream>

#include "Screen.h"
#include "Image.h"

image::Image::Image(const std::string &_image_path)
    : image_path(_image_path), width(0), height(0),
      bits_per_pixel(0), bytes_per_pixel(0), image_size(0)
{
}

void image::Image::set_image_path(const std::string &_image_path)
{
    this->image_path = _image_path;
}

// void image::Image::set_width(const uint32_t &_width)
// {
//     this->width = _width;
// }
// void image::Image::set_height(const uint32_t &_height)
// {
//     this->height = _height;
// }
// void image::Image::set_bits_per_pixel(const uint32_t &_bits_per_pixel)
// {
//     this->height = set_height;
// }

int image::Image::get_width() const
{
    return this->width;
}

int image::Image::get_height() const
{
    return this->height;
}

uint32_t image::Image::get_bits_per_pixel() const
{
    return this->bits_per_pixel;
}

std::string image::Image::get_image_path() const
{
    return this->image_path;
}

uint32_t image::Image::get_image_size() const
{
    return this->image_size;
}