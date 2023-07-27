#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Touch.h"



touch::Touch *touch::Touch::instance = nullptr;

touch::Touch::Touch(const std::string &file)
    : dev_node(file)
{
    this->fd = open(dev_node.c_str(), O_RDWR | O_NONBLOCK);
    if (this->fd == -1)
    {
        perror("open touch dev error");
        throw -1;
    }
}

touch::Touch::~Touch()
{
    int ret = close(this->fd);
    if (ret < 0)
    {
        perror("close touch dev error");
        exit(-1);
    }
    instance = nullptr;
}

touch::Touch *touch::Touch::getInstance()
{
    if (instance == nullptr)
    {
        instance = new touch::Touch;
    }
    return instance;
}

int touch::Touch::get_fd() const
{
    return this->fd;
}
