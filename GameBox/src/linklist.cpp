#include "linklist.h"

void Linklist::push(const char *str)
{
    Node *node = new Node(str);
    this->size++;
    if (1 == this->size)
    {
        this->first = node;
        this->last = node;
        node->pre = this->last;
        node->next = this->first;
    }
    else
    {
        node->pre = this->last;
        node->next = this->first;
        this->first->pre = node;
        this->last->next = node;
        this->last = node;
    }
}

void Linklist::push(const string &str)
{
    Node *node = new Node(str);
    this->size++;
    if (1 == this->size)
    {
        this->first = node;
        this->last = node;
        node->pre = this->last;
        node->next = this->first;
    }
    else
    {
        node->pre = this->last;
        node->next = this->first;
        this->first->pre = node;
        this->last->next = node;
        this->last = node;
    }
}

int Linklist::pop()
{
    //链表大小为0,就返回
    if (this->size == 0)
    {
        cout << "当前链表大小为0,不可以删除" << endl;
        return -1;
    }
    //工作结点
    Node *p = this->last;
    if (this->size == 1)
    {
        this->first = NULL;
        this->last = NULL;
    }
    else
    {
        p->pre->next = this->first;
        this->first->pre = p->pre; 
    }
    delete p;
    this->size--;
    return 0;
}

void Linklist::print_list()
{
    Node *p = this->first;
    for (int i = 0; i < this->size; i++)
    {
        cout << p->date << endl;
        p = p->next;
    }
}

Linklist::Linklist()
{
    this->first = NULL;
    this->last = NULL;
    this->size = 0;
}

Linklist::~Linklist()
{
    //释放开辟的空间
    Node *p = this->first;
    for (int i = 0; i < this->size; i++)
    {
        Node *tmp = p;
        p = p->next;
        delete tmp;
    }
    this->first = NULL;
    this->last = NULL;
    this->size = 0;
}