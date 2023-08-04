#ifndef __LIST_HPP__
#define __LIST_HPP__

#include <iostream>

// 结点类
template <class DataType>
class Node
{
private:
    DataType data; // 数据
    Node *front;   // 上一个结点
    Node *next;    // 下一个结点

public:
    /****   以下函数用于设置结点的属性信息   ****/

    // 设置数据
    void set_data(const DataType &data) { this->data = data; }
    // 设置前置结点
    void set_front(Node *_front) { this->front = _front; }
    // 设置后置结点
    void set_next(Node *_next) { this->next = _next; }
    // 获得数据域,对象变量用
    DataType &get_data() { return this->data; }
    // 获得数据域,常对象可用
    const DataType &get_data() const { return this->data; }
    // 获取当前结点的前置结点的地址
    Node *get_front() const { return this->front; }
    // 获取当前结点的后置结点的地址
    Node *get_next() const { return this->next; }

    /**************************************/

    template <class T>
    friend std::ostream &operator<<(std::ostream &os, const Node<T> &node);

    Node(const DataType &_data = 0)
        : data(_data), front(nullptr), next(nullptr) {}
    Node() = delete;
    Node(const Node &) = delete;
    Node(Node &&) = delete;

    ~Node()
    {
        this->front = nullptr;
        this->next = nullptr;
    }
};

// 运算符<<重载,打印结点数据内容
template <class T>
std::ostream &operator<<(std::ostream &os, const Node<T> &node)
{
    os << node.data;
    return os;
}

// 双向循环链表
template <class DataType>
class List
{
private:
    Node<DataType> *first; // 链表的第一个结点
    Node<DataType> *last;  // 链表的最后一个结点
    uint32_t length;       // 链表的结点数量

public:
    /****   以下函数用于获取或者设置链表的属性   */

    // 获取链表的结点个数
    uint32_t size() const { return this->length; }
    // 尾插数据
    void push_back(const DataType &_data);
    // 头插数据
    void push_front(const DataType &_data);
    // 尾删数据
    void pop_back();
    // 头删数据
    void pop_front();

    // 检测链表是否为空,为空返回true,假返回false
    bool empty() const { return !length ? true : false; }

    /******************************************/

    /****   运算符重载函数   ****/

    template <class T>
    friend std::ostream &operator<<(std::ostream &os, const List<T> &list);

    /***
     * @brief 返回链表第index个结点的数据,对象变量可用
     *
     * @param index  索引结点
     * @return DataType&
     */
    DataType &operator[](const int &index);

    /***
     * @brief 返回链表第index个结点的数据,常对象可用
     *
     * @param index 索引结点
     * @return const DataType&
     */
    const DataType &operator[](const int &index) const;

    /**************************/

    List() : first(nullptr), last(nullptr), length(0) {}
    List(const DataType &_data);
    List(const List &) = delete;
    void clear();
    ~List();

private:
    // 当链表中结点个数为零时的增加结点操作
    void zero_push(Node<DataType> *_node);
};

// 打印链表数据
template <class T>
std::ostream &operator<<(std::ostream &os, const List<T> &list)
{
    if (!list.empty())
    {
        Node<T> *p = list.first;
        int n = list.size();
        while (n--)
        {
            os << *p << " ";
            p = p->get_next();
        }
    }

    return os;
}

template <class DataType>
DataType &List<DataType>::operator[](const int &index)
{
    if (index < 0 || index >= length)
        throw -1;
    Node<DataType> *p = this->first;
    for (int i = 0; i < index; i++)
    {
        p = p->get_next();
    }
    return p->get_data();
}

template <class DataType>
const DataType &List<DataType>::operator[](const int &index) const
{
    if (index < 0 || index >= length)
        throw -1;
    const Node<DataType> *p = this->first;
    for (int i = 0; i < index; i++)
    {
        p = p->get_next();
    }
    return p->get_data();
}

template <class DataType>
void List<DataType>::push_front(const DataType &_data)
{
    Node<DataType> *node = new Node<DataType>(_data);
    if (empty())
    {
        zero_push(node);
        return;
    }
    node->set_next(this->first);
    node->set_front(this->last);
    this->last->set_next(node);
    this->first->set_front(node);
    this->first = node;
    this->length++;
}

template <class DataType>
void List<DataType>::push_back(const DataType &_data)
{
    Node<DataType> *node = new Node<DataType>(_data);
    if (empty())
    {
        zero_push(node);
        return;
    }

    this->first->set_front(node);
    this->last->set_next(node);
    node->set_front(this->last);
    node->set_next(this->first);

    this->last = this->last->get_next();
    this->length++;
}

template <class DataType>
void List<DataType>::pop_back()
{
    if (empty())
        return;
    Node<DataType> *p = this->last;
    if (length == 1)
    {
        this->first = nullptr;
        this->last = nullptr;
    }
    else
    {
        p->front->set_next(p->next);
        p->next->set_front(p->front);
    }
    this->length--;
    delete p;
}

template <class DataType>
void List<DataType>::pop_front()
{
    if (empty())
        return;
    Node<DataType> *p = this->first;
    if (length == 1)
    {
        this->first = nullptr;
        this->last = nullptr;
    }
    else
    {
        p->front->set_next(p->next);
        p->next->set_front(p->front);
    }
    this->length--;
    delete p;
}

template <class DataType>
void List<DataType>::zero_push(Node<DataType> *node)
{
    this->first = node;
    this->last = node;
    this->first->set_front(node);
    this->first->set_next(node);
    this->last->set_front(node);
    this->last->set_next(node);
    this->length = 1;
}

template <class DataType>
List<DataType>::List(const DataType &_data)
    : first(nullptr), last(nullptr), length(0)
{
    Node<DataType> *node = new Node<DataType>(_data);
    zero_push(node);
}

template <class DataType>
void List<DataType>::clear()
{
    if (empty())
        return;
    Node<DataType> *p = this->first;
    while (length--)
    {
        Node<DataType> *p1 = p;
        p = p->get_next();
        delete p1;
    }
    this->first = nullptr;
    this->last = nullptr;
}

template <class DataType>
List<DataType>::~List()
{
    this->clear();
}

#endif