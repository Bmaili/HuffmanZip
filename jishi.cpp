#include "jishi.h"
#include "ui_jishi.h"

jishi::jishi(QWidget *parent) : QWidget(parent),
                                ui(new Ui::jishi)
{
    ui->setupUi(this);
    setWindowTitle("即时编译码");

    ui->shuru->setPlaceholderText( "请在这里输入需要编码的文本(◍•ᴗ•◍)" );
    ui->shuru_2->setPlaceholderText( "请在这里输入需要译码的文本(◍•ᴗ•◍)" );
    ui->search2->setPlaceholderText( "该字符的编码为：" );

    connect(ui->change, &QPushButton::clicked, this, &jishi::switchpage);
    connect(ui->search, SIGNAL(textChanged()), this, SLOT(chaxun()));

    this->setAutoFillBackground(true);//设置背景
     QPalette palette=this->palette();
     QImage ImgAllbackground(":/resourse/cat4.jfif");
     QImage fitimgpic=ImgAllbackground.scaled(this->width(),this->height(), Qt::IgnoreAspectRatio);
     palette.setBrush(QPalette::Window, QBrush(fitimgpic));
     this->setPalette(palette);

    QMovie *pMovie = new QMovie(":/resourse/dongtu.gif"); //动画显示，括号里面是图片来源路径
    ui->label_7->setMovie(pMovie);
    ui->label_8->setMovie(pMovie);
    pMovie->start();

    this->setWindowIcon(QIcon(":/resourse/code2.jpg"));

    setStyleSheet(
                     //正常状态样式
                     "QPushButton{"
                     "background-color:rgba(100,225,100,30);"//背景色（也可以设置图片）
                     "border-style:outset;"                  //边框样式（inset/outset）
                     "border-width:4px;"                     //边框宽度像素
                     "border-radius:20px;"                   //边框圆角半径像素
                     "border-color:rgba(255,255,255,30);"    //边框颜色
                     "font:bold 20px;"                       //字体，字体大小
                     "color:rgba(0,0,0,100);"                //字体颜色
                     "padding:6px;"                          //填衬
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

jishi::~jishi()
{
    delete ui;
}

void jishi::on_zhuanhuan_clicked()
{
    int Ascii, weight[ascii]; //weight[]储存字符权重，其下标为字符值ascii码,Ascii为字符ASCII码
    QString qstr = ui->shuru->toPlainText(), sc;

    ui->shuchu->setText(sc); //输出框清空初始化
    string str = string((const char *)qstr.toLocal8Bit()), sc1;

    for (int j = 0; j < str.length(); j++)
    {
        Ascii = str[j];
        if (Ascii >= 0 && Ascii <= 127)
        {
            sc1 = sc1 + (str[j]);
        }
    }
    // ui->shuru->setPlainText(QString::fromStdString(sc1));    //过滤掉不能编码的字符
    if (!sc1.length())
    {
        return;
    }

    ui->search->setText(sc); //搜索框初始化
    ui->search2->setText(sc);
    ui->lba->setText(sc); //编码表框清空初始化
    haffman = NULL;
    memset(weight, 0, sizeof(weight)); //权重值数组初始化

    for (int k = 0; k < sc1.length(); k++)
    {
        Ascii = sc1[k]; //字符转化为其ascii码
        weight[Ascii]++;
    }

    haffman = createTree(weight, ascii); //开始建树
    weightSearch(haffman);

    for (int j = 0; j < sc1.length(); j++) //输出输入文本的二进制码
    {
        Ascii = sc1[j];
        sc.append(TreeNode[Ascii].bit);
    }
    ui->shuchu->setText(sc);
}

void jishi::on_zhuanhuan2_clicked()
{
    if (!haffman) //检查是否已编码
    {
        QMessageBox::about(this, "译码错误", "请先对文本进行编码！");
        return;
    }
    QString qstr = ui->shuru_2->toPlainText(), sc;
    ui->shuchu_2->setText(sc); //输出框清空初始化
    string str = string((const char *)qstr.toLocal8Bit()), sc1;

    for (int j = 0; j < str.length(); j++) //过滤0与1之外的字符
    {
        if (str[j] == '0' || str[j] == '1')
        {
            sc1 = sc1 + (str[j]);
        }
    }
    ui->shuru_2->setPlainText(QString::fromStdString(sc1)); //过滤0与1之外的字符
    if (!sc1.length())
    {
        return;
    }
    readCode(haffman, haffman, sc1, 0);
}

void jishi::switchpage()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender()); //得到按下的按钮的指针
    if (button == ui->change)
    {
        int i = ui->stackedWidget->currentIndex();
        ui->stackedWidget->setCurrentIndex((i + 1) % 2);
    }
}

void jishi::on_putf_clicked()
{
    //括号里的参数分别是：指定父类、标题、默认打开后显示的目录、右下角的文件过滤器。
    QString filename = QFileDialog::getOpenFileName(NULL, "标题", ".", "*.txt");
    QFile aFile(filename);
    if (!aFile.exists()) //文件不存在
        return;
    if (!aFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QByteArray a;
    while (!aFile.atEnd())
    {
        a = aFile.read(1024); //每次读取1024字节
        ui->shuru->insertPlainText(a);
    }
    aFile.close();
}

void jishi::on_copy_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->shuchu->toPlainText());
}

void jishi::on_copy2_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->shuchu_2->toPlainText());
}

void jishi::on_clear_clicked()
{
    QString cl;
    ui->shuru->setPlainText(cl);
    ui->shuchu->setText(cl);
}

void jishi::on_clear2_clicked()
{
    QString cl;
    ui->shuru_2->setPlainText(cl);
    ui->shuchu_2->setText(cl);
}

void jishi::on_suiji_clicked()
{
    QString str;
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    int b = qrand() % 11 + 20; //随机生成20到30次
    for (int i = 0; i < b; i++)
    {
        int a = qrand() % 95 + 32; //随机生成32到126的随机数
        str = char(a);
        ui->shuru->insertPlainText(str);
    }
}

void jishi::on_wenjian_clicked()
{
    delete this;
    wenjian *wen = new wenjian;
    wen->show();
}

void jishi::chaxun()
{
    QString qstr = ui->search->toPlainText();
    if (qstr.length() > 1) //始终保持一个字符
    {
        qstr.chop(1);
        ui->search->setText(qstr);
        return;
    }
    string aa = string((const char *)qstr.toLocal8Bit());
    int As = aa[0];
    if (!(As >= 0 && As <= 127)) //使得输入的字符必须合法
    {
        qstr.chop(1);
        ui->search->setText(qstr);
        return;
    }
    if (TreeNode[As].weight == 0)
    {
        ui->search2->setText("无此字符的编码！");
    }
    else
    {
        ui->search2->setText(TreeNode[As].bit);
    }
}

void jishi::weightSearch(HFTree *tree) //按权重值排序检索编码，算法为树的层次遍历
{
    QString str;
    QString ch;
    int queue[(ascii * 2)]; //仿队列结构的数组，数组quene里储存TreeNode的下标
    int top = 0, rear = 0;  //头，尾指针
    queue[0] = tree->date;
    while (top <= rear)
    {
        if ((TreeNode[queue[top]].lchild == -1) && (TreeNode[queue[top]].rchild == -1))
        {
            ch = char(TreeNode[queue[top]].date);
            if (TreeNode[queue[top]].date == 10)
                ch = "'换行'";
            if (TreeNode[queue[top]].date == 9)
                ch = "'Tab'";
            if (TreeNode[queue[top]].date == 32)
                ch = "'空格'";
            str = ch + "的哈夫曼编码是：\n" + TreeNode[queue[top]].bit;
            ui->lba->append(str);
            ++top;
        }
        else
        {
            queue[++rear] = TreeNode[queue[top]].lchild;
            queue[++rear] = TreeNode[queue[top]].rchild;
            ++top;
        }
    }
    ui->lba->moveCursor(QTextCursor::Start); //将光标移到文本开始处
}

void jishi::readCode(HFTree *tree, HFTree *roottree, string bianma, int I) //将二进制码翻译成文本
{
    QString sc;
    if ((tree->lchild == -1) && (tree->rchild == -1))
    {
        if (tree->parent != -1)
        {
            sc = ui->shuchu_2->toPlainText();
            sc += tree->date;
            ui->shuchu_2->setPlainText(sc);
            tree = roottree;
        }
        else //特殊情况，只有一个结点时
        {
            for (int i = 0; i < bianma.length(); i++)
            {
                if (bianma[i] == '0')
                    sc += tree->date;
            }
            ui->shuchu_2->setPlainText(sc);
            return;
        }
    }
    switch (bianma[I])
    {
    case '0':
        readCode(&TreeNode[tree->lchild], roottree, bianma, I + 1);
        return;
    case '1':
        readCode(&TreeNode[tree->rchild], roottree, bianma, I + 1);
        return;
    default:
        return;
    }
}
