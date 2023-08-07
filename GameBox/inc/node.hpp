#ifndef __NODE_H__
#define __NODE_H__

// 结点类
template <class T>
class Node
{
public:
    Node(T& data)
    {
        this->data = new T(data);
        this->pre = NULL;
        this->next = NULL;
    }

    T data;     //数据
    Node *next; //后继结点
    Node *pre;  //前结点
};


#endif // !__NODE_H__