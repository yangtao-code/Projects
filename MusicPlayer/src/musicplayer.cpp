#include "musicplayer.h"
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>

string fifo_cmd = (string) "file=" + FIFONAME;

void *MusicPlayer::GetState(void *arg)
{
    MusicPlayer *m = (MusicPlayer *)arg;
    while (1)
    {

        m->get_state();
    }
}

void MusicPlayer::mplayer()
{
    while (1)
    {
        // 显示音乐选择菜单
        this->display_jpg(0, 0, "./pic/mplayermenu.jpg");
        while (1)
        {
            // this->get_state();
            //  播放MP3音乐接口
            if (s.way == CLICK && s.x0 >= 153 && s.x0 <= 360 && s.y0 >= 155 && s.y0 <= 305)
            {
                this->play_mp3();
                break;
            }
            // 播放MP4视频接口
            else if (s.way == CLICK && s.x0 >= 450 && s.x0 <= 660 && s.y0 >= 155 && s.y0 <= 305)
            {
                this->play_mp4();
                break;
            }
            // 退出接口
            else if (s.way == CLICK && s.x0 >= 740 && s.x0 <= 800 && s.y0 >= 0 && s.y0 <= 40)
            {
                exit_mplayer();
            }
        }
    }
}

void MusicPlayer::play_mp3()
{
    if (this->filenames_mp3.size == 0)
    {
        cout << "无MP3音乐." << endl;
        return;
    }
    // 设置音乐播放状态为正在播放
    this->mplayer_state = STAT_ON;
    // 设置当前播放的音乐为mp3音乐
    this->Current_music_player = 1;

    this->display_jpg(0, 0, "./pic/mp3_menu.jpg");
    this->display_jpg(0, 0, "./pic/1.jpg");
    while (1)
    {
        if (this->mplayer_state == STAT_ON)
        {
            this->display_jpg(350, 410, "./pic/true.jpg");
        }
        else
        {
            this->display_jpg(350, 410, "./pic/false.jpg");
        }
        // 创建一个子进程播放mp3音乐
        pid_t pid = fork();
        if (-1 == pid)
        {
            perror("fork failed");
            return;
        }
        // 子进程播放音乐
        else if (0 == pid)
        {
            execlp("mplayer", "mplayer", "-slave", "-quiet",
                   "-input", fifo_cmd.c_str(), work_mp3->date.c_str(), NULL);

            exit(0);
        }
        // 父进程触摸选择
        else
        {
            // 打开管道
            fifo_fd = open(FIFONAME, O_RDWR);
            if (fifo_fd == -1)
            {
                perror("open fifo failed");
                exit(0);
            }

            int ret = this->option_mp3();

            // 关闭管道
            close(fifo_fd);
            // 返回值为-1为返回上一级目录
            if (ret == -1)
                return;
        }
    }
}

int MusicPlayer::option_mp3()
{
    while (1)
    {
        //  如果按上一首按钮播放上一首歌曲
        if (s.is_used == false && s.way == CLICK && s.x0 >= 245 && s.x0 <= 300 && s.y0 >= 420 && s.y0 <= 470)
        {
            s.is_used = true;
            this->play_prev_music(this->Current_music_player);
            break;
        }
        // 如果按下一首按钮就播放下一首歌曲
        else if (s.is_used == false && s.way == CLICK && s.x0 >= 507 && s.x0 <= 560 && s.y0 >= 420 && s.y0 <= 470)
        {
            s.is_used = true;
            this->play_next_music(this->Current_music_player);
            break;
        }
        // 暂停/开始播放接口
        else if (s.is_used == false && s.way == CLICK && s.x0 >= 385 && s.x0 <= 420 && s.y0 >= 425 && s.y0 <= 480)
        {
            s.is_used = true;
            this->stop_or_play_music(this->Current_music_player);
        }
        // 后退播放接口
        else if (s.is_used == false && s.way == LEFT)
        {
            s.is_used = true;
            this->back_music();
        }
        // 前进播放接口
        else if (s.is_used == false && s.way == RIGHT)
        {
            s.is_used = true;
            this->forwark_music();
        }
        // 返回接口
        else if (s.is_used == false && s.way == CLICK &&
                 s.x0 >= 620 && s.x0 <= 700 && s.y0 >= 420 && s.y0 <= 460)
        {
            s.is_used = true;
            this->return_music();
            return -1;
        }
        // 退出接口
        else if (s.is_used == false && s.way == CLICK &&
                 s.x0 >= 720 && s.x0 <= 800 && s.y0 >= 420 && s.y0 <= 460)
        {
            s.is_used = true;
            this->quit_mplayer();
        }
    }
    return 0;
}

// void MusicPlayer::signal_handler(int signo)
//  {
//      if (signo == SIGCHLD)
//      {
//          work_mp3 = work_mp3->next;
//      }
//  }

void MusicPlayer::play_mp4()
{
    if (this->filenames_mp4.size == 0)
    {
        cout << "无MP4视频." << endl;
        return;
    }
    // 设置音乐播放状态为正在播放
    this->mplayer_state = STAT_ON;
    // 设置当前播放的音乐为mp4音乐
    this->Current_music_player = 2;
    while (1)
    {
        pid_t pid = fork();
        if (-1 == pid)
        {
            perror("fork failed:");
            return;
        }
        else if (pid == 0)
        {
            execlp("mplayer", "-slave", "-quiet", "-zoom", "-x", "800", "-y", " 480",
                   "-input", fifo_cmd.c_str(), work_mp4->date.c_str(), NULL);
            exit(0);
        }
        else
        {
            // 打开管道
            fifo_fd = open(FIFONAME, O_RDWR);
            if (fifo_fd == -1)
            {
                perror("open fifo failed");
                exit(0);
            }

            int ret = this->option_mp4();

            // 关闭管道
            close(fifo_fd);
            // 返回值为-1为返回上一级目录
            if (ret == -1)
                return;
        }
    }
}

int MusicPlayer::option_mp4()
{
    while (1)
    { //  上滑播放上一个视频
        if (s.is_used == false && s.way == UP)
        {
            s.is_used = true;
            this->play_prev_music(this->Current_music_player);
            break;
        }
        // 下滑播放下一个视频
        else if (s.is_used == false && s.way == DOWN)
        {
            s.is_used = true;
            this->play_next_music(this->Current_music_player);
            break;
        }
        // 暂停/开始播放接口
        else if (s.is_used == false && s.way == CLICK && s.x0 >= 0 && s.x0 <= 800 && s.y0 >= 100 && s.y0 <= 400)
        {
            s.is_used = true;
            this->stop_or_play_music(this->Current_music_player);
        }
        // 左滑后退播放
        else if (s.is_used == false && s.way == LEFT)
        {
            s.is_used = true;
            this->back_music();
            return -1;
        }
        // 右滑快进播放
        else if (s.is_used == false && s.way == RIGHT)
        {
            s.is_used = true;
            this->forwark_music();
        }
        // 返回接口
        else if (s.is_used == false && s.way == CLICK &&
                 s.x0 >= 700 && s.x0 <= 800 && s.y0 >= 420 && s.y0 <= 480)
        {
            s.is_used = true;
            this->return_music();
            return -1;
        }
        // 退出接口
        else if (s.is_used == false && s.way == CLICK &&
                 s.x0 >= 700 && s.x0 <= 800 && s.y0 >= 0 && s.y0 <= 80)
        {
            s.is_used = true;
            this->quit_mplayer();
        }
    }
    return 0;
}

void MusicPlayer::play_prev_music(int m)
{
    this->write_cmd("quit\n");
    wait(NULL);
    // 链表指向当前结点的前一个结点
    this->mplayer_state = STAT_ON;

    if (1 == m)
        work_mp3 = work_mp3->pre;
    else if (2 == m)
        work_mp4 = work_mp4->pre;
}

void MusicPlayer::play_next_music(int m)
{
    this->write_cmd("quit\n");
    wait(NULL);
    // 链表指向当前结点的前一个结点
    if (1 == m)
        work_mp3 = work_mp3->next;
    else if (2 == m)
        work_mp4 = work_mp4->next;

    this->mplayer_state = STAT_ON;
}

void MusicPlayer::stop_or_play_music(int m)
{
    this->write_cmd("pause\n");
    if (this->mplayer_state == STAT_ON)
    {

        this->mplayer_state = STAT_STOP;
        if (m == 1)
            this->display_jpg(350, 410, "./pic/false.jpg");
    }
    else
    {
        this->mplayer_state = STAT_ON;
        if (m == 1)
            this->display_jpg(350, 410, "./pic/true.jpg");
    }
}

void MusicPlayer::forwark_music()
{
    this->write_cmd("seek 5\n");
}

void MusicPlayer::back_music()
{
    this->write_cmd("seek -5\n");
}

void MusicPlayer::return_music()
{
    this->write_cmd("quit\n");
    wait(NULL);
    this->mplayer_state = STAT_OFF;
    this->Current_music_player = 0;
    cout << "已返回" << endl;
}

void MusicPlayer::quit_mplayer()
{
    this->write_cmd("quit\n");
    wait(NULL);
    this->mplayer_state = STAT_OFF;
    this->Current_music_player = 0;

    // 关闭管道
    close(fifo_fd);
    exit_mplayer();
}

void MusicPlayer::init_fifo(string fifo_name)
{
    unlink(fifo_name.c_str());

    int ret = mkfifo(fifo_name.c_str(), 0666);
    if (ret == -1)
    {
        perror("mkfifo failed");
        exit(-1);
    }
}

void MusicPlayer::readdir_mp3()
{
    // 打开MP3目录
    DIR *mp3_dir = opendir("./MP3");
    if (NULL == mp3_dir)
    {
        perror("opendir failed");
        exit(-1);
    }
    struct dirent *d;
    // 读目录里面的内容,将文件名增加到链表中
    while (d = readdir(mp3_dir))
    {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;
        int l = strlen(d->d_name);
        // 判断是否为mp3文件,是就吧这个文件名存入文件名链表里面
        if (strcmp(d->d_name + l - 4, ".mp3") == 0)
        {
            string filepath = "./MP3/";
            filepath = filepath + d->d_name;
            filenames_mp3.push(filepath);
        }
    }
    closedir(mp3_dir);
}

void MusicPlayer::readdir_mp4()
{
    DIR *mp4_dir = opendir("./MP4");
    if (NULL == mp4_dir)
    {
        perror("opendir failed");
        exit(-1);
    }
    struct dirent *d;
    // 读目录里面的内容,将文件名增加到链表中
    while (d = readdir(mp4_dir))
    {
        if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0)
            continue;
        int l = strlen(d->d_name);
        // 判断是否为mp3文件,是就吧这个文件名存入文件名链表里面
        if (strcmp(d->d_name + l - 4, ".mp4") == 0)
        {
            string filepath = "./MP4/";
            filepath = filepath + d->d_name;
            filenames_mp4.push(filepath);
        }
    }
    closedir(mp4_dir);
}

void MusicPlayer::exit_mplayer()
{
    cout << "音乐播放器已关闭" << endl;
    this->display_Square(0, 0, 800, 480, 0x00000f);
    exit(0);
}

int MusicPlayer::write_cmd(string cmd)
{
    if (this->mplayer_state != STAT_OFF)
    {
        int ret = write(fifo_fd, cmd.c_str(), cmd.size());
        if (ret != cmd.size())
        {
            perror("write fifo failed");
            return -1;
        }
    }
    return 0;
}

MusicPlayer::MusicPlayer()
{
    // 创建线程来不断获取触摸状态
    int ret = pthread_create(&state_pid, NULL, MusicPlayer::GetState, (void *)this);
    if (ret != 0)
    {
        perror("thread create failed");
        exit(1);
    }
    this->mplayer_state = STAT_OFF;
    readdir_mp3();
    readdir_mp4();
    cout << "mp3音乐有: " << filenames_mp3.size << endl;
    this->work_mp3 = this->filenames_mp3.first;
    filenames_mp3.print_list();
    cout << "mp4视频有: " << filenames_mp4.size << endl;
    this->work_mp4 = this->filenames_mp4.first;
    filenames_mp4.print_list();
    this->Current_music_player = 0;
    // 初始化管道
    this->init_fifo(FIFONAME);
}

MusicPlayer::~MusicPlayer()
{
    // 回收线程资源
    pthread_cancel(state_pid);
}
