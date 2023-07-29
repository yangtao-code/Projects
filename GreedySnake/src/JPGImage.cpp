#include <iostream>

#include <jconfig.h>
#include <jerror.h>
#include <jmorecfg.h>
#include <jpeglib.h>
#include "JPGImage.h"

void JPGImage::draw(const screen::Screen &s, int x0, int y0) const
{
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
    FILE *infile = fopen(jpg_name, "rb");
    if (infile == NULL)
    {
        perror("fopen failed");
        return -1;
    }
    // 2. 指定要解压的jpg文件
    jpeg_stdio_src(&cinfo, infile);

    // 第三步:调用jpeg_read_header获取图像信息(比如:宽/高等)

    jpeg_read_header(&cinfo, TRUE);
    // 第四步:设置参数,一般采用默认参数

    // 第五步:开始解压缩
    jpeg_start_decompress(&cinfo);
    printf("%s: 宽 = %d,高 = %d,分度值 = %d\n",
           jpg_name, cinfo.output_width, cinfo.output_height, cinfo.output_components);
    // 行字节数
    int rowbyte = cinfo.output_width * cinfo.output_components;
    // 总字节数
    int byte = cinfo.output_width * cinfo.output_height * cinfo.output_components;

    // cinfo.output_scanline表示现在已经解压扫描读取了多少行的数据,初始值为0
    unsigned char *buf = (unsigned char *)malloc(rowbyte);
    while (cinfo.output_scanline < cinfo.output_height)
    {

        // 第一个参数表示解压对象
        // 第二个参数表示保存解压后的数据的空间(二级指针)
        // 第三个参数表示读取多少行数据进行解压
        jpeg_read_scanlines(&cinfo, &buf, 1);

        // cinfo.output_scanline 自动+1
        // 将读取出来并解压缩的JPEG的一行显示到屏幕的对应位置上去
        unsigned int color;
        int x;
        int i = 0;
        unsigned char a, r, g, b;
        for (x = 0; x < cinfo.output_width; x++)
        {

            if (cinfo.output_components == 3)
            {
                a = 0xff;
            }
            else
            {
                a = buf[i++];
            }
            r = buf[i++];
            g = buf[i++];
            b = buf[i++];
            color = (r << 16) | (g << 8) | b;
            display_Point(x0 + x, y0 + cinfo.output_scanline - 1, color);
        }
    }

    /* Step 7: 调用jpeg_finish_decompress完成解压 */
    jpeg_finish_decompress(&cinfo);

    /* Step 8: 调用jpeg_destroy_decompress销毁解压缩对象 */
    jpeg_destroy_decompress(&cinfo);

    free(buf);
    fclose(infile);
}

JPGImage::JPGImage(const std::string &_image_path)
    : Image(_image_path)
{
}

bool JPGImage::updateImage(const std::string &_image_path)
{
}