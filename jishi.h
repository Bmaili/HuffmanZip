#ifndef JISHI_H
#define JISHI_H

#include "quanju.h"
#include"wenjian.h"

namespace Ui {
class jishi;
}

class jishi : public QWidget
{
    Q_OBJECT

public:
    explicit jishi(QWidget *parent = nullptr);
    ~jishi();
    void readCode(HFTree *tree, HFTree *roottree, string bianma, int i);   //译码函数
    void weightSearch(HFTree *tree);     //字符编码排列
    void switchpage();              //模式切换

private:
    Ui::jishi *ui;

private slots:
    void on_zhuanhuan_clicked();     //  编码
    void on_putf_clicked();          //文件导入
    void on_copy_clicked();          //复制编码
    void on_zhuanhuan2_clicked();    //  译码
    void on_copy2_clicked();         //复制文本
    void on_clear_clicked();         // 清空编码框
    void on_clear2_clicked();        //清空译码框
    void on_suiji_clicked();         //输入随机字符串
    void chaxun();                   //查询字符对应编码
    void on_wenjian_clicked();       //打开文件压缩界面
};

#endif // JISHI_H
