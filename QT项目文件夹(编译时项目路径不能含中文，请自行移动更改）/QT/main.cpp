#include "jishi.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   jishi *jis=new jishi;
   jis->show();
   return a.exec();
}
