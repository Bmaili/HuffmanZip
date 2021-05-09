#ifndef WENJIAN_H
#define WENJIAN_H

#include "quanju.h"
#include  "jishi.h"

namespace Ui {
class wenjian;
}

class wenjian : public QWidget
{
    Q_OBJECT

public:
    explicit wenjian(QWidget *parent = nullptr);
    ~wenjian();

    string ToYuanma(unsigned long duma);    //将压缩文件里的信息转换成01编码
    void txtYaSuo(QString filename);    //文本压缩
    void bmpYaSuo(QString filename);    //位图压缩
    void txtJieYa(QString filename);    //文本解压
    void bmpJieYa(QString filename);    //位图解压

private:
    Ui::wenjian *ui;
    //分别为源文件文本，对应的哈夫曼编码,每次从原文截取一段的字符串
    string YuanMa,BianMa,Jiequ;

private slots:

    void on_jishi_clicked();       //即时编译码操作界面
    bool on_bianmaBox_clicked();   //是否勾选了“生成编码文件”选项
    void on_YaSuo_clicked();       //点击“压缩”
    void on_JieYa_clicked();       //点击“解压”
};

#endif // WENJIAN_H
