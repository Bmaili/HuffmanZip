#ifndef QUANJU_H
#define QUANJU_H

#include <QWidget>
#include <QApplication>
#include <QFileDialog>
#include <QString>
#include <QFile>
#include <QListWidget>
#include <QTime>
#include <QtGlobal>
#include <QClipboard>
#include <QMessageBox>
#include <QWidget>
#include <QPalette>
#include <QPixmap>
#include <QMovie>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <windows.h>
#include <bitset>
#include <string>



#define ascii 130 //ascii码的数据128，取个整吧
#define RGB 256   //256色位图

using namespace std;

//haffman 树的结构
struct HFTree
{
    int date;                   //改为int型，既代表字符对应的ascii码值，又代表数组下标
    unsigned long weight;       //权重（字符次数）
    int parent, lchild, rchild; //仿链表结构的数组
    char bit[100];              //叶子结点对应的二进制码
};

//BMP文件结构
struct BMPHeader
{
    BITMAPFILEHEADER BF; //文件头   这俩前缀在<windows.h>中有定义
    BITMAPINFOHEADER BI; //信息头
    int rgb[256];        //BMP调色板
};

extern struct HFTree *haffman;
extern struct HFTree TreeNode[(RGB * 2)];
extern HFTree *createTree(int w[], int asc);       //创建树的函数
extern void treeCode(int Treeptr, char ch[]);      //创建树后，给树结点bit赋值函数

class quanju
{
public:
    quanju();
};

#endif // QUANJU_H
