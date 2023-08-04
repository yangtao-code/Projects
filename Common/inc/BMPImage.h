#ifndef __BMPIMAGE_H__
#define __BMPIMAGE_H__

#include "Image.h"

// bmp图片类
class BMPImage
    : public image::Image
{
private:
    /***
     * @brief 初始化图片信息
     *
     * @return true 成功返回true
     * @return false 失败返回false
     */
    bool init_Image();

public:
    BMPImage(const std::string &_image_path);

    /***
     * @brief 在屏幕s上显示图片,坐标为(x0,y0)
     *
     * @param s 屏幕
     * @param x0    x轴坐标
     * @param y0    y轴坐标
     */
    void draw(const screen::Screen &s, int x0, int y0) const;

    /***
     * @brief 更好图片并更新图片信息
     *
     * @param _image_path 图片路径
     * @return true 成功返回true
     * @return false 失败返回false
     */
    bool updateImage(const std::string &_image_path);
};

#endif