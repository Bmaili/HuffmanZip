# HuffmanZip



## 简介

HuffmanZip是一款基于Huffman算法的文本、位图压缩与解压软件，同时还支持对ascii文本的即时Huffman编码与译码。



## 相关图片

![](http://bmalimarkdown.oss-cn-beijing.aliyuncs.com/img/gJfYDK.png)
![](https://z3.ax1x.com/2021/05/09/gJfUED.png)
![](http://bmalimarkdown.oss-cn-beijing.aliyuncs.com/img/gJfaUe.png)
![](http://bmalimarkdown.oss-cn-beijing.aliyuncs.com/img/gJfJu6.png)



## 软件说明

1. 大二上学期的数据结构课设。

2. 利用数据结构中的二叉树的哈夫曼树对字符进行编码译码、位图进行压缩和解压并生成对应的文本格式，并采用UI设计，对压缩软件中的文件导入、随机输入、复制编码、字符查询、文件的压缩解压等功能进行模拟。

3. 主要功能有二：

- 能即时地对输入的文本进行Huffman编码、解码，显示各字符的编码结果，利于使用者对Huffman的直观理解、学习。

- 利用Huffman编码对文本或位图文件进行数据压缩，同时将压缩信息也写入压缩文件内，故用户也可使用该软件解压之。



## 使用\开发说明
- 开发平台：Desktop QT 5.9.9 MinGW 32bit

- 开发语言：C\C++

- 编译时项目路径不能含中文，编译后可使用Enigma Virtual Box之类的软件打包封装。


- .exe可执行文件 于64位win10测试后通过。
- 可使用提供的文本、图片样例进行测试。
