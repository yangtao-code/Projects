#pragma once
#include <iostream>
#include <string>
using namespace std;



// 双向循环链表类
class Linklist
{
public:
    Linklist();

    ~Linklist();

    // 尾插数据
    void push(const char *str);
    void push(const string &str);
    // 尾删数据,成功返回0,失败返回-1
    int pop();
    // 打印链表数据内容
    void print_list();
    Node *first; // 第一个结点
    Node *last;  // 最后一个结点
    int size;    // 链表大小
};