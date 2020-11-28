#include "MineMap.h"
#include <stdlib.h>
#include <time.h>

CMineMap::CMineMap(): nMime(0), winf(0), rMine(0)
{
    mx = 20; my = 15;
    nMime = 50;
    Map=nullptr;  //保存雷区信息
}  //默认初始值,构造函数


CMineMap::~CMineMap()
{
}  //析构函数

//重新布雷
void CMineMap::Restart()
{
    int i,j;
    for (i = 0; i < mx; i++)
        for (j = 0; j < my; j++)
            Map[i][j] = 100;  //初始赋值(无雷)
    srand(time(nullptr));
    for (i = 0; i < nMime; i++)
    {
        int m = rand() % mx;  //生成(伪)随机数布置雷的分布
        int n = rand() % my;
        if (Map[m][n] != 99)
            Map[m][n] = 99;
        else
            i--;  //随机数重复了,返回去
    }
    for (i = 0; i < mx; i++)
        for (j = 0; j < my; j++)
        {
            if (Map[i][j] == 99)
            {
                int m, n;
                for (m = -1; m < 2; m++)
                    for (n = -1; n < 2; n++)
                    {
                        if ((i + m >= mx) || (j + n >= my) || (i + m<0) || (j + n<0) || (Map[i + m][j + n] == 99))  //出界 或者 是雷(就成为99不变)
                            continue;
                        Map[i + m][j + n]++;  //原始数据100，但之后必定>100
                    }
            }
        }  //雷周围其它框内的数

    winf = 0;  //初始状态。-1为输，1为赢
    rMine = nMime;
    //first = 1;
    timer = 0;
}

//动态数组分配内存并布雷
void CMineMap::Create()
{
    int i;
    if(Map!=nullptr)  //雷区非空
    {
        for(i=0;i<mx;i++)
        {
            delete Map[i];
        }
        delete Map;
        Map=nullptr;
    }  //整到空
    Map = new int*[mx];
    for (i = 0; i < mx; i++)
    {
        Map[i] = new int[my];
    }
    Restart();
}

//动态数组分配内存并布雷，传入参数：　列数　行数　雷数
void CMineMap::Create(int cx,int cy,int ms)
{
    int i;
    if(Map!=nullptr)
    {
        for(i=0;i<mx;i++)
        {
            delete Map[i];
        }
        delete Map;
        Map=nullptr;
        mx=cx;
        my=cy;
        nMime=ms;
    }
    Map = new int*[mx];
    for (i = 0; i < mx; i++)
    {
        Map[i] = new int[my];
    }
    Restart();
}

bool CMineMap::Onlbtup(int m, int n) //实现是否中雷(左击实现)，且旗子无法左击
{ //点击成功返回true，否则返回false
    if(winf==-1||winf==1)
        return false;
    if (m >= mx || n >= my || m < 0 || n < 0 || Map[m][n] < 99) //界外或点击出现数不正常
        return false;
    if (Map[m][n] >= 101 && Map[m][n] <= 108)
    {
        Map[m][n] -= 100;  //-100后的数字表示该点周围雷个数
        //first = 0;
        iswin(); //判断是否赢
        return true;
    }
    if (Map[m][n] == 100)
    {   //点击的地放周围都没雷，则递归点击周围８个，炸出一大片
        Map[m][n] -= 100;
        Onlbtup(m - 1, n);
        Onlbtup(m + 1, n);
        Onlbtup(m, n - 1);
        Onlbtup(m, n + 1);
        Onlbtup(m - 1, n - 1);
        Onlbtup(m + 1, n - 1);
        Onlbtup(m - 1, n + 1);
        Onlbtup(m + 1, n + 1);
    }
    if (Map[m][n] == 99)
    {   //中雷了

        /*
        if (first)  //if(first)==if(first!=0)
        { //检测雷区周围，做一些善后工作
            rMine--;  //在布雷时有“rMine = nMime;”
            int a, b, t = 0;
            for (a = -1; a < 2; a++)
                for (b = -1; b < 2; b++)
                {
                    if ((a + m < mx) && (b + n < my) && (a + m >= 0) && (b + n >= 0) && (a || b)) //a||b排除了它自己
                    {
                        if (Map[a + m][b + n] > 99)
                            Map[a + m][b + n]--;  //显示雷区周围有雷数的数字-1
                        if (Map[a + m][b + n] == 99)
                            t++;  //统计该雷周围的雷数
                    }
                }

            first = 0;
            Map[m][n] = 100 + t;
            Onlbtup(m, n);

            return true;
        }*/

        /*当点击的格子是99代表你中雷了，把所有雷都显示出来。就是把所有99都置为-1，如果插小红旗的地方，插错了的置为-2*/
        int i, j;
        for (i = 0; i < mx; i++)
            for (j = 0; j < my; j++)
            {
                if (Map[i][j] == 99)
                    Map[i][j] = -1;
                if (Map[i][j]>49 && Map[i][j] < 60)
                    Map[i][j] = -2;
            }
        winf = -1;
    }
    return true;
}

//右键插小红旗
bool CMineMap::Onrbtdown(int m, int n)
{
    if(winf==-1||winf==1)
        return false;
    if (m >= mx || m < 0 || n >= my || n < 0||Map[m][n]<49)
        return false;
    if (Map[m][n]>=99&&Map[m][n]<109)
    {
        Map[m][n] -= 50;
        rMine--;
    }  //点一下-50
    else if (Map[m][n] >= 49 && Map[m][n] < 59)
    {
        rMine++;
        Map[m][n] += 50;
    }  //再点一下+50
    return true;
}


bool CMineMap::Onlrbtdown(int m, int n) //双击实现
{
    if (m >= mx || m < 0 || n >= my || n < 0||Map[m][n]>49)
        return false;
    if (Map[m][n] == ctnflag(m, n))  //Map[m][n]代表的周围雷数与实际雷数t已经匹配
    {//递归点击其周围几个
        Onlbtup(m - 1, n);
        Onlbtup(m + 1, n);
        Onlbtup(m, n - 1);
        Onlbtup(m, n + 1);
        Onlbtup(m - 1, n - 1);
        Onlbtup(m + 1, n - 1);
        Onlbtup(m - 1, n + 1);
        Onlbtup(m + 1, n + 1);
    }
    return true;
}

int CMineMap::ctnflag(int m, int n)
{
    int t = 0;
    if (m >= mx || m < 0 || n >= my || n < 0)
        return -1;
    int a, b;
    for (a = -1; a < 2; a++)
        for (b = -1; b < 2; b++)
        {
            if ((m + a >= mx) || (n + b >= my) || (m + a<0) || (n + b<0) || (Map[m + a][n + b] >= 59) || (Map[m + a][n + b] < 49))
                continue;
            t++;  //统计标记了的旗子数
        }
    return t;
}  //Map[m][n]代表的周围雷数与实际雷数t已经匹配

//判断是否赢了
bool CMineMap::iswin()
{
    int i, j;
    for (i = 0; i < mx; i++)
    {
        for (j = 0; j < my; j++)
        {
            if (Map[i][j]>99) //非雷的还未全部点开
                return false;
            if (Map[i][j]>49 && Map[i][j] < 59) //错标旗子的
                return false;
        }
    }
    winf = 1;
    return true;
}
