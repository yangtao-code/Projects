#ifndef __SCREEN_H__
#define __SCREEN_H__
// 屏幕默认的文件地址
#define SCREEN_DEV_PATH "/dev/fb0"

namespace screen
{
    // 屏幕类
    class Screen
    {
    private:
        static Screen *instance; //单例对象指针
        /* 数据成员: 描述 对象的 属性的 */
        const char *dev_node;    // 屏幕文件的文件名
        int fd;                  // 文件描述符
        uint32_t lcd_width;      // 显示屏每一行有多少个像素点
        uint32_t lcd_height;     // 显示屏总共有多少行
        uint32_t bits_per_pixel; // 每个像素点占多少bits
        int32_t *plcd;           // 指向屏幕 帧缓冲区的首地址

    public:
        //// 获取获取屏幕类的唯一实例
        static Screen* getInstance();

        /***
         * @brief 在屏幕上画一个点
         *
         * @param x 点在屏幕上的x轴坐标
         * @param y 点在屏幕上的y轴坐标
         * @param color 颜色值
         */
        void draw_Point(const int &x, const int &y, const uint32_t &color = 0x00)const;

        /****   下面函数用于获取屏幕的信息    ****/

        uint32_t get_lcd_width() const;

        uint32_t get_lcd_height() const;

        uint32_t get_bits_per_pixel() const;

        /***************************************/
    private:
        /***
         * @brief 构造函数,用来打开屏幕文件和映射地址
         *
         * @param file 屏幕文件地址
         */
        Screen(const char *file = SCREEN_DEV_PATH);

        Screen(const Screen&) = delete;
        Screen& operator=(const Screen&) = delete;

        /***
         * @brief 析构函数,解映射、关闭屏幕文件描述符,回收资源
         *
         */
        ~Screen();

    };

   
}

#endif
