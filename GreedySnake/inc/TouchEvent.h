#ifndef __TOUCHEVENT_H__
#define __TOUCHEVENT_H__

#include "Touch.h"

namespace tevent
{
    // 触摸状态
    typedef enum Touch_State
    {
        NUL = 0, // 空
        CLICK,   // 点击
        PRESS,   // 按压
        UP,      // 上滑
        DOWN,    // 下滑
        LEFT,    // 左滑
        RIGHT    // 右滑
    } Touch_State_t;

    // 坐标结构体
    typedef struct Pos
    {
        int x; // x轴
        int y; // y轴
    } pos_t;

    class TouchEvent
    {
    private:
        Touch_State_t s; // 触摸状态
        pos_t first_pos; // 触摸屏幕的开始坐标
        pos_t last_pos;  // 触摸屏幕的离开坐标
    public:
        explicit TouchEvent();

        /***
         * @brief 获取触摸屏状态
         *
         * @param ts 触摸屏对象
         */
        void event(const touch::Touch &ts);

    private:
        /***
         * @brief 初始化事件
         *
         */
        void init_event();

        /***
         * @brief 处理调用event(const touch::Touch &ts)后产生的数据
         *
         * @param num
         */
        void handle(const int &num);

        /***
         * @brief 求一个数的绝对值
         *
         * @param num 数字
         */
        uint32_t Abs(const int &num);
    };

};

#endif