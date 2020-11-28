#pragma once
#ifndef MINEMAP_H
#define MINEMAP_H

class CMineMap
{
public:
    CMineMap();
    ~CMineMap();
    void Create();
    void Create(int,int,int);
    void Restart();
private:
    int ctnflag(int m, int n);

public:
    int **Map;  //保存雷区信息(指针方便动态分配空间)
    int nMime;//雷数
    int mx;  //行数
    int my;   //列数

    bool Onlbtup(int m, int n);  //左击
    bool Onrbtdown(int m, int n);  //右击
    bool Onlrbtdown(int m, int n); //全击

    bool iswin();

    int winf;  //判断胜利或否
    int rMine;  //实时雷数
    //bool first;
    int timer;
};

#endif // MINEMAP_H
