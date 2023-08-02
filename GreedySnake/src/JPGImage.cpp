#include <iostream>

#include <jerror.h>
#include <jpeglib.h>
#include "JPGImage.h"

void JPGImage::draw(const screen::Screen &s, int x0, int y0) const
{
    // 第一步:分配并初始化应该jpeg解压缩对象
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo); // 初始化cinfo这个解压缩对象
    // 第二步:指定要解压的JPEG文件
    // 1.打开jpg文件
    FILE *jpg_fd = fopen(image_path.c_str(), "rb");
    if (jpg_fd == nullptr)
    {
        perror("fopen failed");
        return;
    }
    // 2. 指定要解压的jpg文件
    jpeg_stdio_src(&cinfo, jpg_fd);

    // 第三步:调用jpeg_read_header获取图像信息(比如:宽/高等)

    if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK)
    {
        printf("Invalid JPEG file format\n");
        jpeg_destroy_decompress(&cinfo);
        fclose(jpg_fd);
        return;
    }
    // 第四步:设置参数,一般采用默认参数
   
    // 第五步:开始解压缩
    jpeg_start_decompress(&cinfo);


    // 行字节数
    int rowbyte = cinfo.output_width * cinfo.output_components;
    // 总字节数
    int byte = this->image_size;
    // cinfo.output_scanline表示现在已经解压扫描读取了多少行的数据,初始值为0
    uint8_t *buf = new uint8_t[rowbyte];
    while (cinfo.output_scanline < cinfo.output_height)
    {

        // 第一个参数表示解压对象
        // 第二个参数表示保存解压后的数据的空间(二级指针)
        // 第三个参数表示读取多少行数据进行解压
        jpeg_read_scanlines(&cinfo, &buf, 1);

        // cinfo.output_scanline 自动+1
        // 将读取出来并解压缩的JPEG的一行显示到屏幕的对应位置上去
        uint32_t color;
        int x;
        int i = 0;
        uint8_t a, r, g, b;
        for (x = 0; x < cinfo.output_width; x++)
        {
            if (cinfo.output_components == 3)
            {
                a = 0x00;
            }
            else
            {
                a = buf[i++];
            }
            r = buf[i++];
            g = buf[i++];
            b = buf[i++];
            color = (a << 24) | (r << 16) | (g << 8) | b;
            s.draw_Point(x0 + x, y0 + cinfo.output_scanline - 1, color);
        }
    }

    /* Step 7: 调用jpeg_finish_decompress完成解压 */
    jpeg_finish_decompress(&cinfo);
    /* Step 8: 调用jpeg_destroy_decompress销毁解压缩对象 */
    jpeg_destroy_decompress(&cinfo);

    delete[] buf;
    fclose(jpg_fd);
}

bool JPGImage::init_Image()
{
    // 第一步:分配并初始化应该jpeg解压缩对象
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo); // 初始化cinfo这个解压缩对象
    // 第二步:指定要解压的JPEG文件
    // 1.打开jpg文件
    FILE *jpg_fd = fopen(image_path.c_str(), "rb");
    if (jpg_fd == nullptr)
    {
        perror("fopen failed");
        return false;
    }
    // 2. 指定要解压的jpg文件
    jpeg_stdio_src(&cinfo, jpg_fd);

    // 第三步:调用jpeg_read_header获取图像信息(比如:宽/高等)
    // 读取 JPEG 文件头部信息并检查文件格式
    if (jpeg_read_header(&cinfo, TRUE) != JPEG_HEADER_OK)
    {
        printf("Invalid JPEG file format\n");
        jpeg_destroy_decompress(&cinfo);
        fclose(jpg_fd);
        return false;
    }

    // 图片的宽、高、色深、大小
    this->width = cinfo.image_width;
    this->height = cinfo.image_height;
    this->bytes_per_pixel = cinfo.num_components;
    this->bits_per_pixel = cinfo.output_components * 8;

    std::cout << image_path << ": "
              << "宽 = " << width << ",高 = " << height << ",色深 = " << bytes_per_pixel
              << std::endl;
    //总字节数
    this->image_size = cinfo.output_width * cinfo.output_height * cinfo.output_components;

    /* Step 8: 调用jpeg_destroy_decompress销毁解压缩对象 */
    jpeg_destroy_decompress(&cinfo);
    fclose(jpg_fd);

    return true;
}

JPGImage::JPGImage(const std::string &_image_path)
    : Image(_image_path)
{
    bool ret = this->init_Image();
    if (ret == false)
        throw -1;
}

bool JPGImage::updateImage(const std::string &_image_path)
{
    this->image_path = _image_path;
    bool ret = this->init_Image();
    if (ret == false)
        return false;
}