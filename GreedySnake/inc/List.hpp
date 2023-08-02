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
    void set_data(const DataType &data) { this->data = data; }
    void set_front(Node *_front) { this->front = _front; }
    void set_next(Node *_next) { this->next = _next; }

    DataType get_data() const { return this->data; }

    Node *get_front() const { return this->front; }

    Node *get_next() const { return this->next; }

    /**************************************/
    template <class T>
    friend std::ostream &operator<<(std::ostream &os, const Node<T> &node);

    Node(const DataType &_data = 0)
        : data(_data), front(nullptr), next(nullptr) {}
    Node() = delete;
    Node(const Node &) = delete;
    Node(Node &&) = delete;

    ~Node() {}
};

template <class T>
std::ostream &operator<<(std::ostream &os, const Node<T> &node)
{
    std::cout << node.data;
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
    uint32_t size() const
    {
        return this->length;
    }

    void push_back(const DataType &_data);
    void push_front(const DataType &_data);

    bool empty() const
    {
        return !length ? true : false;
    }

    /******************************************/

    /****   运算符重载函数   ****/
    template <class T>
    friend std::ostream &operator<<(std::ostream &os, const List<T> &list);

    /**************************/

    List() : first(nullptr), last(nullptr), length(0) {}

    List(const DataType &_data);

    List(const List<DataType> &) = delete;

private:
    // 当链表中结点个数为零时的增加结点操作
    void zero_push(Node<DataType> *_node);
};

template <class T>
std::ostream &operator<<(std::ostream &os, const List<T> &list)
{
    if (!list.empty())
    {
        Node<T> *p = list.first;
        do
        {
            os << *p << " ";
            p = p->get_next();
        } while (p != list.first);
    }
    return os;
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

#endif