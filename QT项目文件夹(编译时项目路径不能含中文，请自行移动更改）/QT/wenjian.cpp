#include "wenjian.h"
#include "ui_wenjian.h"

wenjian::wenjian(QWidget *parent) : QWidget(parent),
                                    ui(new Ui::wenjian)
{
    ui->setupUi(this);
    setWindowTitle("文件压缩");
    ui->jindu->setMaximum(100);
    ui->jindu->setValue(0);

    this->setAutoFillBackground(true); //设置背景
    QPalette palette = this->palette();
    QImage ImgAllbackground(":/resourse/room.jfif");
    QImage fitimgpic = ImgAllbackground.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio);
    palette.setBrush(QPalette::Window, QBrush(fitimgpic));
    this->setPalette(palette);
    this->setWindowIcon(QIcon(":/resourse/code2.jpg"));

    QMovie *pMovie = new QMovie(":/resourse/action.gif"); //动画显示，括号里面是图片来源路径
    ui->label_3->setMovie(pMovie);
    pMovie->start();

    QMovie *mMovie = new QMovie(":/resourse/action3.gif"); //动画显示，括号里面是图片来源路径
    ui->label_4->setMovie(mMovie);
    mMovie->start();

    setStyleSheet(
        //正常状态样式
        "QPushButton{"
        "background-color:rgba(100,225,100,30);" //背景色（也可以设置图片）
        "border-style:outset;"                   //边框样式（inset/outset）
        "border-width:4px;"                      //边框宽度像素
        "border-radius:20px;"                    //边框圆角半径像素
        "border-color:rgba(255,255,255,30);"     //边框颜色
        "font:bold 20px;"                        //字体，字体大小
        "color:rgba(0,0,0,100);"                 //字体颜色
        "padding:6px;"                           //填衬
        "}"
        //鼠标按下样式
        "QPushButton:pressed{"
        "background-color:rgba(100,255,100,200);"
        "border-color:rgba(255,255,255,30);"
        "border-style:inset;"
        "color:rgba(0,0,0,100);"
        "}"
        //鼠标悬停样式
        "QPushButton:hover{"
        "background-color:rgba(100,255,100,100);"
        "border-color:rgba(255,255,255,200);"
        "color:rgba(0,0,0,200);"
        "}");
}

wenjian::~wenjian()
{
    delete ui;
}

string wenjian::ToYuanma(unsigned long duma)
{
    string binarystring;
    while (duma > 0)
    {
        int r = duma % 2;
        if (r == 1)
            binarystring.insert(0, 1, '1');
        else
            binarystring.insert(0, 1, '0');
        duma = duma / 2;
    }
    if (binarystring.size() < 32)
    {
        binarystring.insert(0, 32 - binarystring.size(), '0');
    }
    return binarystring;
}

void wenjian::on_jishi_clicked()
{
    delete this;
    jishi *ji = new jishi;
    ji->show();
}

bool wenjian::on_bianmaBox_clicked()
{
    if (ui->bianmaBox->isChecked())
        return true;
    else
        return false;
}

void wenjian::on_YaSuo_clicked()
{
    QString filename = QFileDialog::getOpenFileName(NULL, "请选择待压缩文件", ".", "*.txt;*.bmp");

    if((filename.right(1)=="T")||filename.right(1)=="t")   //判断选择的是txt还是bmp
    {
        txtYaSuo(filename);
    }
    else
    {
        bmpYaSuo(filename);
    }
}

void wenjian::on_JieYa_clicked()
{
    QString filename = QFileDialog::getOpenFileName(NULL, "请选择待解压文件", ".", "*.txtYS;*.bmpYS");

    if((filename.right(3)=="TYS")||filename.right(3)=="tYS")   //判断选择的是txtYS还是bmpYS
    {
        txtJieYa(filename);
    }
    else
    {
        bmpJieYa(filename);
    }
}

void wenjian::txtYaSuo(QString filename)
{
    YuanMa = "";BianMa = "";
    int Ascii, weight[ascii];
    string writetreema="";
    haffman = NULL;
    memset(weight, 0, sizeof(weight));

    QFile aFile(filename);
    if (!aFile.exists()) //文件不存在
        return;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    filename.chop(4); //把".txt"这四个字符去掉
    //压缩文件名、路径设置
    QFile yasuofile;
    yasuofile.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), filename, QString("(*.txtYS)")));
    if (!yasuofile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    ui->jindu->setValue(5);
    clock_t begin = clock(); //记录压缩开始时间

    QByteArray a;
    while (!aFile.atEnd())
    {
        a = aFile.read(1024);
        //一次性读取1024个字节，不足1024个字节则读取全部
        string b = a.toStdString();
        //转换为string
        for (int i = 0; i < b.size(); i++)
        {
            Ascii = b[i];
            if (Ascii >= 0 && Ascii <= 127)
            { //字符转化为其ascii码
                YuanMa += b[i];
                weight[Ascii]++;
            }
        }
    }
    aFile.close();

    ui->jindu->setValue(25);
    if (!YuanMa.length()) //当文件为空或者不含ascii码内容时
    {
        ui->jindu->setValue(0);
        QMessageBox::about(this, "压缩失败", "该文件无可压缩内容！！！");
        return;
    }

    //开始建树
    haffman = createTree(weight, ascii);

    for (int j = 0; j < YuanMa.length(); j++) //将文本的二进制码保存在BianMa中
    {
        ui->jindu->setValue(30 + 20 * j / YuanMa.length());
        Ascii = YuanMa[j];
        BianMa.append(TreeNode[Ascii].bit);
    }
    ui->jindu->setValue(50);

    if (on_bianmaBox_clicked()) //是否生成编码文件，这时间也算在压缩里的
    {
        QFile bianmafile(filename + ".BianMa.txt");
        bianmafile.open(QIODevice::WriteOnly | QIODevice::Text);
        bianmafile.write(QString::fromStdString(BianMa).toUtf8());
        bianmafile.close();
    }

    ui->jindu->setValue(60);
    unsigned long yuanchang = BianMa.length(); //原文本对应的二进制编码长度
    unsigned long chang = BianMa.length() / 32 + 1;
    //每32位压缩成一个4字节的int，末尾不够32个就补“0”
    for (int k = 0; k < 32 * chang - yuanchang; k++)
        BianMa.append("0");

    FILE *fper = fdopen(yasuofile.handle(), "wb");

    for(int i=0;i<ascii;i++)           //将树的叶子结点信息写进压缩文件中
    {
        writetreema.append(ToYuanma(weight[i]));
    }
    for (int i=0;i<ascii;i++) {
        bitset<32> tree_byte(writetreema.substr(32 * i, 32));
        unsigned long tmp = tree_byte.to_ulong();
        fwrite(&tmp, sizeof(int), 1, fper);
    }

    ui->jindu->setValue(65);
    fwrite(&yuanchang, sizeof(int), 1, fper); //写数据的长度
    for (unsigned long i = 0; i < chang; i++)
    {
        ui->jindu->setValue(70 + 30 * i / chang);
        bitset<32> byte(BianMa.substr(32 * i, 32));
        //每次取total的32位，并以i为计数器向后推移
        unsigned long tmp = byte.to_ulong();
        fwrite(&tmp, sizeof(int), 1, fper);
        //写入，写入……
    }
    fclose(fper);
    yasuofile.close();

    clock_t end = clock(); //结束时间
    ui->jindu->setValue(100);

    QFileInfo fileInfo(filename + ".txt");
    //---获取原来的文件大小（字节）
    qint64 yuansize = fileInfo.size();
    QFileInfo fileInfo2(yasuofile.fileName());
    //---获取后来的文件大小（字节）
    qint64 yssize = fileInfo2.size();

    //弹窗提示本次压缩情况
    QString tip("压缩比:");
    tip += QString::number(double(yssize * 100) / yuansize);
    tip += "%    压缩用时：";
    tip += QString::number(double(end - begin) / CLOCKS_PER_SEC);
    tip += "s   ";
    QMessageBox::about(this, "压缩成功！！！", tip);
    //收尾工作
    ui->jindu->setValue(0);
    haffman = NULL;
    YuanMa = "";BianMa = "";
}

void wenjian::bmpYaSuo(QString filename)
{
    BMPHeader mybmp;
    unsigned char **data;
    BianMa="";
    string writetreema;
    int weight[RGB]; //256种点的权重
    haffman = NULL;
    memset(weight, 0, sizeof(weight)); //初始化

    QFile aFile(filename);
    if (!aFile.exists()) //文件不存在
        return;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    filename.chop(4); //把".txt"这四个字符去掉
    QFile yasuofile;
    yasuofile.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), filename, QString("BMP(*.bmpYS)")));
    if (!yasuofile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    ui->jindu->setValue(5);
    clock_t begin = clock(); //记录开始时间
    FILE *fp = fdopen(aFile.handle(), "rb");
    FILE *fper = fdopen(yasuofile.handle(), "wb");

    //为所需信息赋值：
    fread(&mybmp.BF, sizeof(BITMAPFILEHEADER), 1, fp); //如果没有读取头文件会导致 biBitCount直接成为40—— 位图信息头大小（40byte）
    fread(&mybmp.BI, sizeof(BITMAPINFOHEADER), 1, fp);
    fread(&mybmp.rgb, sizeof(int), 256, fp);
    //只能打开256色位BMP图像：
    if (mybmp.BI.biBitCount != 8) //文件打开失败，请选择正确文件格式!
    {
        ui->jindu->setValue(0);
        QMessageBox::about(this, "失败", "文件打开失败，请选择正确文件格式！！！");
        return;
    }

    fwrite(&mybmp.BF, sizeof(BITMAPFILEHEADER), 1, fper);
    fwrite(&mybmp.BI, sizeof(BITMAPINFOHEADER), 1, fper);
    fwrite(&mybmp.rgb, sizeof(int), 256, fper);

    ui->jindu->setValue(10);
    data = new unsigned char *[mybmp.BI.biHeight];

    for (int i = 0; i < mybmp.BI.biHeight; i++)
        data[i] = new unsigned char[mybmp.BI.biWidth];

    for (int i = 0; i < mybmp.BI.biHeight; i++)
    {
        ui->jindu->setValue(10 + 20 * i / mybmp.BI.biHeight);
        for (int j = 0; j < mybmp.BI.biWidth; j++)
        {
            fread(&data[i][j], 1, 1, fp); //data赋值——将位图中每一个像素点的信息提取保存起来，作为数据的一部分。
        }
        if (mybmp.BI.biWidth % 4 > 0)
        { //偏移量定位
            fseek(fp, 4 - mybmp.BI.biWidth % 4, SEEK_CUR);
        }
    }
    fclose(fp);

    ui->jindu->setValue(30);
    for (int i = 0; i < mybmp.BI.biHeight; i++)
    {
        ui->jindu->setValue(30 + 10 * i / mybmp.BI.biHeight);
        for (int j = 0; j < mybmp.BI.biWidth; j++)
        {
            weight[data[i][j]]++; //循环求每一种点的权重
        }
    }

    ui->jindu->setValue(40);
    //返回根点值——确定查找范围
    haffman = createTree(weight, RGB);

    ui->jindu->setValue(45);
    //开辟缓冲区
    unsigned long BuffLength = 0;
    for (int i = 0; i < 256; i++)
    {
        BuffLength += weight[i] * strlen(TreeNode[i].bit);
        //计算所有哈夫曼编码的总长度以确定缓冲区的大小
    }
    char *Buff = new char[BuffLength];

    //将压缩后的文件读入缓冲区
    unsigned long cur = 0;
    for (int i = 0; i < mybmp.BI.biHeight; i++)
    {
        ui->jindu->setValue(50 + 25 * i / mybmp.BI.biHeight);
        for (int j = 0; j < mybmp.BI.biWidth; j++)
        {
            for (int k = 0; k < strlen(TreeNode[data[i][j]].bit); k++)
            {
                Buff[cur] = TreeNode[data[i][j]].bit[k]; //data的数据是像素点的信息
                cur++;
            }
        }
    }

    ui->jindu->setValue(75);
    if (on_bianmaBox_clicked())
    {
        QFile bianmafile(filename + ".BianMa.txt");
        bianmafile.open(QIODevice::WriteOnly | QIODevice::Text);
        bianmafile.write(QString::fromStdString(Buff).toUtf8());
        bianmafile.close();
    }

    ui->jindu->setValue(80);
    BianMa = Buff;
    unsigned long yuanchang = BianMa.length();
    unsigned long chang = BianMa.length() / 32 + 1;

    for (int k = 0; k < 32 * chang - yuanchang; k++)
        BianMa.append("0");

    for(int i=0;i<RGB;i++)
    {
        writetreema.append(ToYuanma(weight[i]));
    }
    for (int i=0;i<RGB;i++) {
        bitset<32> tree_byte(writetreema.substr(32 * i, 32));
        unsigned long tmp = tree_byte.to_ulong();
        fwrite(&tmp, sizeof(int), 1, fper);
    }

    fwrite(&yuanchang, sizeof(int), 1, fper); //写数据的长度
    for (unsigned long i = 0; i < chang; i++)
    {
        ui->jindu->setValue(80 + 20 * i / chang);
        bitset<32> byte(BianMa.substr(32 * i, 32));
        //每次取total的32位，并以i为计数器向后推移
        unsigned long tmp = byte.to_ulong();
        fwrite(&tmp, sizeof(int), 1, fper);
        //写入，写入……
    }
    fclose(fper);
    yasuofile.close();

    clock_t end = clock(); //记录结束时间
    ui->jindu->setValue(100);

    QString fileStr = filename + ".bmp";
    QFileInfo fileInfo(fileStr);
    //---获取原来的文件大小（字节）
    qint64 yuansize = fileInfo.size();

    QString fileStr2 = yasuofile.fileName();
    QFileInfo fileInfo2(fileStr2);
    //---获取后来的文件大小（字节）
    qint64 yssize = fileInfo2.size();

    //弹窗提示本次压缩情况
    QString tip("压缩比:");
    tip += QString::number(double(yssize * 100) / yuansize);
    tip += "%    压缩用时：";
    tip += QString::number(double(end - begin) / CLOCKS_PER_SEC);
    tip += "s   ";
    QMessageBox::about(this, "压缩成功！！！", tip);

    //收尾工作
    ui->jindu->setValue(0);
    haffman = NULL;
    YuanMa = "";BianMa = "";
}

void wenjian::txtJieYa(QString filename)
{
    YuanMa = "", BianMa = "", Jiequ = "";
    int weight[ascii];
    haffman = NULL;
    memset(weight, 0, sizeof(weight));

    QFile aFile(filename);
    if (!aFile.exists()) //文件不存在
        return;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    FILE *fp = fdopen(aFile.handle(), "rb");

    filename.chop(6);   //将.txtYS这6个字符去掉
    QFile jieyafile;
    jieyafile.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), filename, QString("(*.NEW.txt)")));
    if (!jieyafile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    ui->jindu->setValue(5);
    clock_t begin = clock(); //记录解压开始时间

    for (int i = 0; i < ascii; i++)   //读取树的各叶子结点信息
    {
        fread(&weight[i], sizeof(int), 1, fp);
    }

    ui->jindu->setValue(10);
    haffman = createTree(weight, ascii); //开始建树

    ui->jindu->setValue(20);
    unsigned long duma;
    unsigned long yuanchang;
    fread(&yuanchang, sizeof(int), 1, fp); //读数据的长度
    //开始读取压缩文件里的压缩编码，转换成01编码形式
    while (!feof(fp))
    {
        fread(&duma, sizeof(int), 1, fp);
        BianMa.append(ToYuanma(duma));
    }
    BianMa.erase(yuanchang);
    fclose(fp);

    //将01编码根据树翻译成原文，算法为译码
    ui->jindu->setValue(40);
    HFTree *nhaf = haffman;
    unsigned char ch;
    if((haffman->lchild==-1)&&(haffman->rchild==-1))   //特殊情况，如果只有一个叶子结点
    {
        ch=haffman->date;
        YuanMa.insert(0,BianMa.length(),ch);
    }
    else
    {
        for (int i = 0; i * 3200 <= BianMa.length(); i++)
        {
            ui->jindu->setValue(40+50*i*3200/BianMa.length());
            Jiequ = BianMa.substr(i * 3200, 3200);
            int I = 0;
            while (I < Jiequ.length())
            {
                if ((nhaf->lchild == -1) && (nhaf->rchild == -1))
                {
                    ch = nhaf->date;
                    YuanMa.insert(YuanMa.length(), 1, ch);
                    nhaf = haffman;
                }
                switch (Jiequ[I])
                {
                case '0':
                    I++;
                    nhaf = &TreeNode[nhaf->lchild];
                    break;
                case '1':
                    I++;
                    nhaf = &TreeNode[nhaf->rchild];
                    break;
                default:
                    break;
                }
            }//while
        } //for
        ch = nhaf->date;
        YuanMa.insert(YuanMa.length(), 1, ch);
    }//else

    ui->jindu->setValue(90);

    jieyafile.write(QString::fromStdString(YuanMa).toUtf8());
    jieyafile.close();

    ui->jindu->setValue(100);
    clock_t end = clock(); //记录解压结束时间

    //弹窗提示本次解压情况
    QString tip("   解压用时：");
    tip += QString::number(double(end - begin) / CLOCKS_PER_SEC);
    tip += "s   ";
    QMessageBox::about(this, "解压成功！！！", tip);

    ui->jindu->setValue(0);
    YuanMa = "", BianMa = "", Jiequ = "";
    haffman = NULL;
}

void wenjian::bmpJieYa(QString filename)
{
    BMPHeader mybmp;
    YuanMa = "", BianMa = "", Jiequ = "";
    int weight[RGB];
    haffman = NULL;
    memset(weight, 0, sizeof(weight));

    QFile aFile(filename);
    if (!aFile.exists()) //文件不存在
        return;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    FILE *fp = fdopen(aFile.handle(), "rb");

    filename.chop(6);        //删掉".bmpYS"六个字符
    QFile jieyafile;
    jieyafile.setFileName(QFileDialog::getSaveFileName(this, QString("保存路径"), filename, QString("(*.NEW.bmp)")));
    if (!jieyafile.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    FILE *fper = fdopen(jieyafile.handle(), "wb");

    ui->jindu->setValue(5);
    clock_t begin = clock(); //记录解压开始时间

    //从压缩文件里读取图片信息
    fread(&mybmp.BF, sizeof(BITMAPFILEHEADER), 1, fp); //如果没有读取头文件会导致 biBitCount直接成为40—— 位图信息头大小（40byte）
    fread(&mybmp.BI, sizeof(BITMAPINFOHEADER), 1, fp);
    fread(&mybmp.rgb, sizeof(int), 256, fp);
    ui->jindu->setValue(10);

    //将图片信息写进解压文件
    fwrite(&mybmp.BF, sizeof(BITMAPFILEHEADER), 1, fper);
    fwrite(&mybmp.BI, sizeof(BITMAPINFOHEADER), 1, fper);
    fwrite(&mybmp.rgb, sizeof(int), 256, fper);
    ui->jindu->setValue(15);

    for (int i = 0; i < RGB; i++)   //读取树的各叶子结点信息
    {
        fread(&weight[i], sizeof(int), 1, fp);
    }

    ui->jindu->setValue(20);
    haffman = createTree(weight, RGB); //开始建树

    ui->jindu->setValue(25);
    unsigned long duma;
    unsigned long yuanchang;
    fread(&yuanchang, sizeof(int), 1, fp); //读数据的长度
    //开始读取压缩文件里的压缩编码，转换成01编码形式
    while (!feof(fp))
    {
        fread(&duma, sizeof(int), 1, fp);
        BianMa.append(ToYuanma(duma));
    }
    BianMa.erase(yuanchang);
    fclose(fp);

    //将01编码根据树翻译成原文，算法为译码
    ui->jindu->setValue(50);
    HFTree *nhaf = haffman;
    unsigned char ch;
    if((haffman->lchild==-1)&&(haffman->rchild==-1))   //如果只有一个叶子结点
    {
        ch=haffman->date;
        YuanMa.insert(0,BianMa.length(),ch);
    }
    else
    {
        for (int i = 0; i * 3200 <= BianMa.length(); i++)
        {
            ui->jindu->setValue(50+30*i*3200/BianMa.length());
            Jiequ = BianMa.substr(i * 3200, 3200);
            int I = 0;
            while (I < Jiequ.length())
            {
                if ((nhaf->lchild == -1) && (nhaf->rchild == -1))
                {
                    ch = nhaf->date;
                    YuanMa.insert(YuanMa.length(), 1, ch);
                    nhaf = haffman;
                }
                switch (Jiequ[I])
                {
                case '0':
                    I++;
                    nhaf = &TreeNode[nhaf->lchild];
                    break;
                case '1':
                    I++;
                    nhaf = &TreeNode[nhaf->rchild];
                    break;
                default:
                    break;
                }
            }//while
        } //for
        ch = nhaf->date;
        YuanMa.insert(YuanMa.length(), 1, ch);
    }//else


    unsigned long xiangsu=0;
    for (int i = 0; i < mybmp.BI.biHeight; i++)
    {
        ui->jindu->setValue(80 + 20 * i / mybmp.BI.biHeight);
        for (int j = 0; j < mybmp.BI.biWidth; j++)
        {
            fwrite(&YuanMa[xiangsu], 1, 1, fper); //data赋值——将位图中每一个像素点的信息提取保存起来，作为数据的一部分。
            xiangsu++;
        }
        if (mybmp.BI.biWidth % 4 > 0)
        { //偏移量定位
            fseek(fper, 4 - mybmp.BI.biWidth % 4, SEEK_CUR);
        }
    }
    fclose(fper);

    ui->jindu->setValue(100);
    clock_t end = clock(); //记录解压结束时间

    //弹窗提示本次解压情况
    QString tip("   解压用时：");
    tip += QString::number(double(end - begin) / CLOCKS_PER_SEC);
    tip += "s   ";
    QMessageBox::about(this, "解压成功！！！", tip);

    ui->jindu->setValue(0);
    YuanMa = "", BianMa = "", Jiequ = "";
    haffman = NULL;
}


