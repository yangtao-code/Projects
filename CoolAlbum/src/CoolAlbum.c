#include "CoolAlbum.h"

void CoolAlbum(Linklist *list)
{
    // 如果链表为空,退出函数
    if (list->n == 0)
        return;
    Node *p = list->first;
    while (1)
    {
        if(NULL != strstr(p->date,".bmp"))
        {
            display_bmp_byc(0, 0, p->date);
        }  
        else if (NULL != strstr(p->date,".jpg"))
        {
            display_jpg(0, 0, p->date);
        }
        else
        {
            printf("这不是一个图片.\n");
        }

        while (1)
        {
            // 获取滑动方向
            int way = getway();
            if (way == SHUT)
            {
                Close_software();
            }
            else if (way == RIGHT)
            {
                p = p->next;
                break;
            }
            else if (way == LEFT)
            {
                p = p->pre;
                break;
            }
        }
    }
}

Linklist *create_piclist(const char *dirpath)
{
    Linklist *list = Create_Linklist(); // 创建链表
    // 打开目录
    DIR *dir = opendir(dirpath);
    if (dir == NULL)
    {
        perror("opendir failed");
        exit(1);
    }
    struct dirent *d;
    // 读目录里面的内容,将文件名增加到链表中
    while (d = readdir(dir))
    {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;
        int l = strlen(d->d_name);
        // 判断是否为bmp文件或者jpg文件,是就吧这个文件名存入文件名链表里面
        if (strcmp(d->d_name + l - 4, ".bmp") == 0 || strcmp(d->d_name + l - 4, ".jpg") == 0)
        {
            char name_buf[50] = "";
            sprintf(name_buf, "%s/%s",dirpath, d->d_name);
            Push_Linklist(list, name_buf);
        }
    }
    return list;
}