#include "SatDefine.h"
/*
读取cnf文件时能依次读取非0的整数值，不论正负
*/
int GetNum(FILE* fp)
{
    char a;
    int mark = 1;   //符号判断，是否为负数
    int sum = 0;
    fscanf(fp, "%c", &a);
    if (a == EOF)
        return 0;
    switch (a)
    {
    case'-':
        mark = -1;                  //若为负号开头则继续读取
        fscanf(fp, "%c", &a);
        break;
    case '0':
        fscanf(fp, "%c", &a);
        if (a == '\r')
            fscanf(fp, "%c", &a);

        return 0;
    }
    if (a == ' ' || a == '\n')
        fscanf(fp, "%c", &a);
    if (a == '0')
        return 0;
    while (a != ' ' && a != '\n' && a != '\r') {
        sum = sum * 10 + a - '0'; //得到文字的值，直到读到非数字为止
        if (sum == 0)
            return 0;
        fscanf(fp, "%c", &a);
    }
    if (a == '\r')
        fscanf(fp, "%c", &a);
    return mark * sum;                      //用来得到文字（包括正负和值）
}

/*
读取cnf文件，将文件内容转换为结构
*/
HeadNode* CreateClause(int& VARNUM, FILE* fp) {
    int var;
    HeadNode* HEAD = (HeadNode*)malloc(sizeof(HeadNode));
    HeadNode* headRear = HEAD;
    HeadNode* END = (HeadNode*)malloc(sizeof(HeadNode));
    for (int i = 0; i < numClause; i++) {
        headRear->Num = 0;
        var = GetNum(fp);//获取值
        LiteralNode* front = (LiteralNode*)malloc(sizeof(LiteralNode));;
        front->data = var;
        headRear->right = front;
        headRear->Num++;//记录每个子句中变元的数目
        var = GetNum(fp);
        while (var != 0) {
            LiteralNode* rear = (LiteralNode*)malloc(sizeof(LiteralNode));
            front->next = rear;
            rear->data = var;
            front = front->next;
            headRear->Num++;
            var = GetNum(fp);
        }
        front->next = nullptr;   //最后指向nullptr
        HeadNode* tp = (HeadNode*)malloc(sizeof(HeadNode));;
        headRear->down = tp;
        END = headRear;
        headRear = headRear->down;
    }
    END->down = nullptr;

    //output link lists
    HeadNode* Phead = HEAD;
    LiteralNode* front;

    VARNUM = numVar;
    fclose(fp);
    return HEAD;

}
/*
查找结构中是否有单变元子句，优先消除
*/
HeadNode* SearchSingle(HeadNode* Pfind) {

    while (Pfind != nullptr && Pfind->Num != 1)
    {
        Pfind = Pfind->down;

    }
    return Pfind;


}

/*
删除整行子句
*/
void DelWholeRaw(HeadNode* Clause, HeadNode*& LIST) {

    HeadNode* front;
    if (Clause == nullptr)
        return;
    if (Clause == LIST)
        LIST = Clause->down;
    else {
        for (HeadNode* front = LIST; front != nullptr; front = front->down)
            if (front->down == Clause) {
                front->down = Clause->down;

            }
    }
    return;
}
/*
删除含有temp的整行，或者单独删除-temp。
*/
void DeleteVar(int temp, HeadNode*& LIST) {

    HeadNode* pHeadNode = LIST;
    if (LIST == nullptr)
        return;

    for (pHeadNode; pHeadNode != nullptr; pHeadNode = pHeadNode->down)
        for (LiteralNode* rear = pHeadNode->right; rear != nullptr; rear = rear->next) {
            if (rear->data == temp)     //某个子句中有该正变元，则直接删除整个子句
            {
                DelWholeRaw(pHeadNode, LIST);
            }

            else if (rear->data == -temp)
            {
                if (rear == pHeadNode->right)
                {
                    pHeadNode->right = rear->next;
                    pHeadNode->Num--;
                }
                else {
                    for (LiteralNode* front = pHeadNode->right; front != nullptr; front = front->next)
                        if (front->next == rear) {
                            front->next = rear->next;
                            pHeadNode->Num--;
                        }
                }
            }
        }
}
/*
复制结构，做备份保存
*/
HeadNode* CopyAllClause(HeadNode* LIST) { //此处检验传参正常，开始检查复制有无逻辑错误
    HeadNode* A = LIST;


    HeadNode* C = (HeadNode*)malloc(sizeof(HeadNode));
    HeadNode* NewHead = C;
    HeadNode* B;
    LiteralNode* A1;
    LiteralNode* C1 = (LiteralNode*)malloc(sizeof(LiteralNode));
    LiteralNode* B1;

    while (A != nullptr)    //一一复制
    {
        C->Num = A->Num;
        A1 = A->right;
        C->right = C1;
        while (A1 != nullptr)
        {

            C1->data = A1->data;
            A1 = A1->next;
            B1 = C1;
            C1 = (LiteralNode*)malloc(sizeof(LiteralNode));
            B1->next = C1;
        }
        B1->next = nullptr;
        A = A->down;
        B = C;
        C = (HeadNode*)malloc(sizeof(HeadNode));
        B->down = C;
    }
    B->down = nullptr;

    return NewHead;
}
/*
将选出的Var，添加到结构的头部当作单子句
*/
HeadNode* AddnewClause(HeadNode* LIST, int Var) { //所加的单子句位于链表的头
    HeadNode* AddHead = new HeadNode;
    LiteralNode* AddData = new LiteralNode;
    AddData->data = Var;
    AddData->next = nullptr;
    AddHead->right = AddData;
    AddHead->Num = 1;
    AddHead->down = LIST;
    LIST = AddHead;
    return LIST;
}

/*
检验结构是否为空
*/
status IfClauseEmpty(HeadNode* LIST) {
    HeadNode* PHead = LIST;
    while (PHead != nullptr) {
        if (PHead->Num == 0)
            return TRUE;
        PHead = PHead->down;
    }
    return FALSE;
}

//int Getmax(records* Order)    /* 自定义函数 shsort()*/
//{
//    int a = 1;
//    int max = Order[1].order;
//    for (int i = 2; i < 2 * numVar; i++) {
//        if (Order[i].order > max) {
//            max = Order[i].order;
//            a = i;
//        }
//    }
//
//    return Order[a].var;
//}
//
//
//int SortVar(HeadNode* LIST) {
// 
//    for (int i = 1; i <= numVar; i++)
//    {
//
//        Order[i].var = i;
//        Order[i + numVar].var = -i;
//        Order[i].order = 0;
//        Order[i + numVar].order = 0;
//    }
//    for (HeadNode* begin = LIST; begin != nullptr; begin = begin->down)    //标记所有元素的出现次数
//    {
//        for (DataNode* begin2 = begin->right; begin2 != nullptr; begin2 = begin2->next)
//        {
//            if (begin2->data > 0)
//                Order[begin2->data].order++;
//            else
//                Order[-(begin2->data) + numVar].order++;
//        }
//    }
//    /* for (int i = 1; i <= 2*numVar; i++)
//     {
//         printf("\t%d %d", Order[i].var, Order[i].order);
//     }*/
//    return  Getmax(Order);
//}
//void Delete(HeadNode* LIST)
//{
//    HeadNode* Head1 = LIST;
//    DataNode* Head2 = Head1->right;
//    while (Head1 != nullptr)
//    {
//        
//            while (Head2 != nullptr)
//            {
//                DataNode* Head22 = Head2->next;
//                free(Head2);
//                Head2 = Head22;
//
//            }
//        
//        
//        HeadNode* Head11 = Head1->down;
//        if (Head11 == nullptr) return;
//        free(Head1);
//        Head1 = Head11;
//        if(Head1!=nullptr)
//        Head2 = Head1->right;
//    }
//}


 /*
 DPLL算法，求解SAT问题。
 */
status DPLL(HeadNode* LIST, consequence* result) {

    HeadNode* Pfind = LIST;
    HeadNode* SingleClause = SearchSingle(Pfind);   //找到邻接表（范式）中的第一个单子句

    while (SingleClause != nullptr)    //若发现单子句，则对此单子句的变元赋值并修改邻接表
    {
        int temp = SingleClause->right->data;
        if (temp > 0)   //单子句变元大于零，则应该取真
            result[temp].value = TRUE;
        else
            result[-temp].value = FALSE;
        DelWholeRaw(SingleClause, LIST);    //删除这一单子句
        DeleteVar(temp, LIST);  //存在相等的结点，则删除整个子句，若存在值相反的结点，删除该结点。

        if (!LIST) return TRUE;
        else if (IfClauseEmpty(LIST)) return FALSE;
        Pfind = LIST;
        SingleClause = SearchSingle(Pfind);//回到头节点继续进行检测是否有单子句
    }

    int Var = LIST->right->data;   //人为选择变元，用第一个，并将其提出放在头部
    HeadNode* replica = CopyAllClause(LIST);   //复制原先的邻接表，为递归做准备
    HeadNode* temp1 = AddnewClause(LIST, Var);//装载变元成为单子句


    int i = DPLL(temp1, result);
    if (i) {
        /* Delete(temp1);*/
        return TRUE;
    }
    else {
        HeadNode* temp2 = AddnewClause(replica, -Var);
        i = DPLL(temp2, result);
        /*Delete(temp2);*/
        return i;
    }

}
/*
当得到结果，将结果按格式存入res文件
*/
void SaveFile1(consequence* result) {
    FILE* fp1;
    strcpy(filename, filepath);
    strcat(filename, "result.res");
    if ((fp1 = fopen(filename, "w")) == NULL)
        system("mkdir data");
    if ((fp1 = fopen(filename, "w")) != NULL)
    {
        fprintf(fp1, "s 1\nv");
        fflush(fp);
        for (int i = 1; i <= numVar; i++)
        {
            int b = result[i].value ? 1 : -1;
            fprintf(fp1, " %d", b * i);

        }
        fprintf(fp1, "\nt %d", time_cost);
        fflush(fp);
        fclose(fp1);

    }
}

/*
得不到正确结果，按格式填写res文件
*/
void SaveFile2(void)
{
    FILE* fp1;
    strcpy(filename, filepath);
    strcat(filename, "result.res");
    if ((fp1 = fopen(filename, "w")) == NULL)
        system("mkdir data");
    if ((fp1 = fopen(filename, "w")) != NULL)
    {
        fprintf(fp1, "s 0\nt %d", time_cost);
        fflush(fp);

        fclose(fp1);

    }
}
/*
检查结果是否正确
*/
int checkanswer(void)
{
    int i = 0;
    printf("\n\n");
    printf("\t\t\t以下验证结果的正确性\n");

    fp = fopen(filename, "r"); //res结果文件
    fp1 = fopen(filename1, "r"); //cnf文件

    char c;
    fscanf(fp, "%c", &c);
    while (c != '\n')
        fscanf(fp, "%c", &c);

    fscanf(fp, "%c", &c);
    fscanf(fp, "%c", &c);
    for (int k = 1; k <= numVar; k++)
        result[k].value = GetNum(fp);

    //for (int i = 1; i <= numVar; i++)
    //{
    //    
    //    printf("  %d  ", result[i].value);

    //    if (i % 10 == 0)
    //        printf("\n\t\t\t");
    //}


    fclose(fp);


    fscanf(fp1, "%c", &c);

    while (c == 'c')                    //若开头为c，则为注释段，若读到p则停止读取
    {
        while (c != '\n' && c != '\r')  //
            fscanf(fp1, "%c", &c);       //自动读取下一字符
        fscanf(fp1, "%c", &c);
        if (c == '\n')                  //有连续换行符的情况
            fscanf(fp1, "%c", &c);
    }

    fscanf(fp1, " cnf ");                //读到cnf，即开始读到文件的信息段
    GetNum(fp1);
    GetNum(fp1);

    int check = 0;
    int a;
    while (i < numClause)
    {
        a = GetNum(fp1);
        while (a != 0)
        {
            printf("\t%d\t", a);
            int b = a > 0 ? a : -a;
            if (a * result[b].value > 0)
                check++;
            a = GetNum(fp1);
        }
        if (check == 0)
        {
            printf("第%d行出错\n", i);
            return 0;
        }
        check = 0;
        i++;
        printf("第%d行正确\n", i);
    }
    return 1;
}

/*cnf文件转为结构*/
HeadNode* cnfToList(void)
{
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("fail to open the file!\n ");

    }

    char c;
    fscanf(fp, "%c", &c);

    while (c == 'c')                    //若开头为c，则为注释段，若读到p则停止读取
    {
        while (c != '\n' && c != '\r')  //
            fscanf(fp, "%c", &c);       //自动读取下一字符
        fscanf(fp, "%c", &c);
        if (c == '\n')                  //有连续换行符的情况
            fscanf(fp, "%c", &c);
    }

    fscanf(fp, " cnf ");                //读到cnf，即开始读到文件的信息段

    numVar = GetNum(fp);              //读取变量数，并赋值给numVar
    numClause = GetNum(fp);                        //再读取子句数
    HeadNode* LIST = CreateClause(numVar, fp);   //创造出了邻接表
    return LIST;
}