#ifndef __TOUCH_H__
#define __TOUCH_H__

#include <iostream>
#define TOUCH_DEV_PATH "/dev/input/event0"

namespace touch
{
    // 触摸屏
    class Touch
    {
    private:
        static Touch *instance;     // 单例类的指针
        const std::string dev_node; // 设备文件路径
        int fd;                     // 打开的触摸设备文件描述符

    public:
        // 获取获取屏幕类的唯一实例
        static Touch *getInstance();

        /***
         * @brief 获取触摸屏文件描述符
         *
         * @return int
         */
        int get_fd() const;

        // 析构函数,回收触摸屏资源
        ~Touch();

    private:
        // 构造函数,打开触摸屏
        Touch(const std::string &file = TOUCH_DEV_PATH);

        // 删除拷贝构造函数和赋值操作
        Touch(const Touch &) = delete;
        Touch &operator=(const Touch &) = delete;
    };
};

#endif