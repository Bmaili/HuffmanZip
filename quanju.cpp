#include "quanju.h"

struct HFTree *haffman = NULL;
struct HFTree TreeNode[(RGB * 2)];

quanju::quanju()
{

}

HFTree *createTree(int w[], int ASCorRGB)
{
    int i, j, x1, x2, n = 0;
    unsigned long m1, m2;//m1,m2为最小的权重与次小的权重，x1,x2标记之，
    //初始化                         //n为叶子结点) //w[ascii]储存字符的权值{数（输入文本的字符种类数）
    for (i = 0; i < (RGB * 2); i++) //因为TreeNode[(RGB*2)],有(RGB*2)个元素
    {
        TreeNode[i].parent = -1;
        TreeNode[i].lchild = -1;
        TreeNode[i].rchild = -1;
        TreeNode[i].date = i;
        TreeNode[i].weight = 0;
        memset(TreeNode[i].bit, 0, sizeof(TreeNode[i].bit));
        if (i < ASCorRGB && w[i] != 0)
        {
            TreeNode[i].weight = w[i];
            n++;
        }
    }

    if (n >= 2) //当叶子结点有两个以上时
    {
        for (i = 0; i < n - 1; i++) //创建n-1个非叶子结点
        {
            m1 = m2 = 4000000000;   //接近unsigned long 最大值
            x1 = x2 = 0;
            for (j = 0; j < ASCorRGB + i; j++) //挑出权重最小与次小的结点
            {
                if (((TreeNode[j]).weight != 0) && ((TreeNode[j]).weight < m1) && ((TreeNode[j]).parent == -1))
                {
                    m2 = m1;
                    x2 = x1;
                    m1 = (TreeNode[j]).weight;
                    x1 = j;
                }
                else if (((TreeNode[j]).weight != 0) && ((TreeNode[j]).weight < m2) && ((TreeNode[j]).parent == -1))
                {
                    m2 = (TreeNode[j]).weight;
                    x2 = j;
                }
            }

            //将找出两颗权值最小和次小的子树合并为一棵树
            (TreeNode[x1]).parent = ASCorRGB + i;
            (TreeNode[x2]).parent = ASCorRGB + i;
            (TreeNode[ASCorRGB + i]).weight = (TreeNode[x1]).weight + (TreeNode[x2]).weight;
            (TreeNode[ASCorRGB + i]).rchild = x1;
            (TreeNode[ASCorRGB + i]).lchild = x2;
        }
        char ch[100] = {'\0'};
        treeCode(ASCorRGB + n - 2, ch); //这东西是根节点别问我咋知道的，几小时DeBug都栽在这上面
        return &TreeNode[ASCorRGB + n - 2];
    }
    else //特殊情形处理：只有一个叶子结点
    {
        for (i = 0; i < ASCorRGB; i++)
        {
            if (TreeNode[i].weight)
            {
                strcat(TreeNode[i].bit, "0");
                return &TreeNode[i];
            }
        }
    }
    TreeNode[i].weight = 0;
}

void treeCode(int Treeptr, char ch[]) //创建树后，对叶子结点的char bit[100]和int date赋值
{                                     //Treeptr为数组TreeNode下标
    char ch2[100] = "\0";
    strcat(ch2, ch);
    if (TreeNode[Treeptr].lchild != -1)
    {
        strcat(ch, "0"); //递归，将0或1加到字符串bit后
        treeCode(TreeNode[Treeptr].lchild, ch);
    }
    if (TreeNode[Treeptr].rchild != -1)
    {
        strcat(ch2, "1");
        treeCode(TreeNode[Treeptr].rchild, ch2);
    }
    else
    {
        strcat(TreeNode[Treeptr].bit, ch);
    }
}
