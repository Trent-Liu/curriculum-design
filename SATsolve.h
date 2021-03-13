#include "SatDefine.h"
/*
��ȡcnf�ļ�ʱ�����ζ�ȡ��0������ֵ����������
*/
int GetNum(FILE* fp)
{
    char a;
    int mark = 1;   //�����жϣ��Ƿ�Ϊ����
    int sum = 0;
    fscanf(fp, "%c", &a);
    if (a == EOF)
        return 0;
    switch (a)
    {
    case'-':
        mark = -1;                  //��Ϊ���ſ�ͷ�������ȡ
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
        sum = sum * 10 + a - '0'; //�õ����ֵ�ֵ��ֱ������������Ϊֹ
        if (sum == 0)
            return 0;
        fscanf(fp, "%c", &a);
    }
    if (a == '\r')
        fscanf(fp, "%c", &a);
    return mark * sum;                      //�����õ����֣�����������ֵ��
}

/*
��ȡcnf�ļ������ļ�����ת��Ϊ�ṹ
*/
HeadNode* CreateClause(int& VARNUM, FILE* fp) {
    int var;
    HeadNode* HEAD = (HeadNode*)malloc(sizeof(HeadNode));
    HeadNode* headRear = HEAD;
    HeadNode* END = (HeadNode*)malloc(sizeof(HeadNode));
    for (int i = 0; i < numClause; i++) {
        headRear->Num = 0;
        var = GetNum(fp);//��ȡֵ
        LiteralNode* front = (LiteralNode*)malloc(sizeof(LiteralNode));;
        front->data = var;
        headRear->right = front;
        headRear->Num++;//��¼ÿ���Ӿ��б�Ԫ����Ŀ
        var = GetNum(fp);
        while (var != 0) {
            LiteralNode* rear = (LiteralNode*)malloc(sizeof(LiteralNode));
            front->next = rear;
            rear->data = var;
            front = front->next;
            headRear->Num++;
            var = GetNum(fp);
        }
        front->next = nullptr;   //���ָ��nullptr
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
���ҽṹ���Ƿ��е���Ԫ�Ӿ䣬��������
*/
HeadNode* SearchSingle(HeadNode* Pfind) {

    while (Pfind != nullptr && Pfind->Num != 1)
    {
        Pfind = Pfind->down;

    }
    return Pfind;


}

/*
ɾ�������Ӿ�
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
ɾ������temp�����У����ߵ���ɾ��-temp��
*/
void DeleteVar(int temp, HeadNode*& LIST) {

    HeadNode* pHeadNode = LIST;
    if (LIST == nullptr)
        return;

    for (pHeadNode; pHeadNode != nullptr; pHeadNode = pHeadNode->down)
        for (LiteralNode* rear = pHeadNode->right; rear != nullptr; rear = rear->next) {
            if (rear->data == temp)     //ĳ���Ӿ����и�����Ԫ����ֱ��ɾ�������Ӿ�
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
���ƽṹ�������ݱ���
*/
HeadNode* CopyAllClause(HeadNode* LIST) { //�˴����鴫����������ʼ��鸴�������߼�����
    HeadNode* A = LIST;


    HeadNode* C = (HeadNode*)malloc(sizeof(HeadNode));
    HeadNode* NewHead = C;
    HeadNode* B;
    LiteralNode* A1;
    LiteralNode* C1 = (LiteralNode*)malloc(sizeof(LiteralNode));
    LiteralNode* B1;

    while (A != nullptr)    //һһ����
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
��ѡ����Var����ӵ��ṹ��ͷ���������Ӿ�
*/
HeadNode* AddnewClause(HeadNode* LIST, int Var) { //���ӵĵ��Ӿ�λ�������ͷ
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
����ṹ�Ƿ�Ϊ��
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

//int Getmax(records* Order)    /* �Զ��庯�� shsort()*/
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
//    for (HeadNode* begin = LIST; begin != nullptr; begin = begin->down)    //�������Ԫ�صĳ��ִ���
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
 DPLL�㷨�����SAT���⡣
 */
status DPLL(HeadNode* LIST, consequence* result) {

    HeadNode* Pfind = LIST;
    HeadNode* SingleClause = SearchSingle(Pfind);   //�ҵ��ڽӱ���ʽ���еĵ�һ�����Ӿ�

    while (SingleClause != nullptr)    //�����ֵ��Ӿ䣬��Դ˵��Ӿ�ı�Ԫ��ֵ���޸��ڽӱ�
    {
        int temp = SingleClause->right->data;
        if (temp > 0)   //���Ӿ��Ԫ�����㣬��Ӧ��ȡ��
            result[temp].value = TRUE;
        else
            result[-temp].value = FALSE;
        DelWholeRaw(SingleClause, LIST);    //ɾ����һ���Ӿ�
        DeleteVar(temp, LIST);  //������ȵĽ�㣬��ɾ�������Ӿ䣬������ֵ�෴�Ľ�㣬ɾ���ý�㡣

        if (!LIST) return TRUE;
        else if (IfClauseEmpty(LIST)) return FALSE;
        Pfind = LIST;
        SingleClause = SearchSingle(Pfind);//�ص�ͷ�ڵ�������м���Ƿ��е��Ӿ�
    }

    int Var = LIST->right->data;   //��Ϊѡ���Ԫ���õ�һ�����������������ͷ��
    HeadNode* replica = CopyAllClause(LIST);   //����ԭ�ȵ��ڽӱ�Ϊ�ݹ���׼��
    HeadNode* temp1 = AddnewClause(LIST, Var);//װ�ر�Ԫ��Ϊ���Ӿ�


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
���õ���������������ʽ����res�ļ�
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
�ò�����ȷ���������ʽ��дres�ļ�
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
������Ƿ���ȷ
*/
int checkanswer(void)
{
    int i = 0;
    printf("\n\n");
    printf("\t\t\t������֤�������ȷ��\n");

    fp = fopen(filename, "r"); //res����ļ�
    fp1 = fopen(filename1, "r"); //cnf�ļ�

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

    while (c == 'c')                    //����ͷΪc����Ϊע�ͶΣ�������p��ֹͣ��ȡ
    {
        while (c != '\n' && c != '\r')  //
            fscanf(fp1, "%c", &c);       //�Զ���ȡ��һ�ַ�
        fscanf(fp1, "%c", &c);
        if (c == '\n')                  //���������з������
            fscanf(fp1, "%c", &c);
    }

    fscanf(fp1, " cnf ");                //����cnf������ʼ�����ļ�����Ϣ��
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
            printf("��%d�г���\n", i);
            return 0;
        }
        check = 0;
        i++;
        printf("��%d����ȷ\n", i);
    }
    return 1;
}

/*cnf�ļ�תΪ�ṹ*/
HeadNode* cnfToList(void)
{
    fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("fail to open the file!\n ");

    }

    char c;
    fscanf(fp, "%c", &c);

    while (c == 'c')                    //����ͷΪc����Ϊע�ͶΣ�������p��ֹͣ��ȡ
    {
        while (c != '\n' && c != '\r')  //
            fscanf(fp, "%c", &c);       //�Զ���ȡ��һ�ַ�
        fscanf(fp, "%c", &c);
        if (c == '\n')                  //���������з������
            fscanf(fp, "%c", &c);
    }

    fscanf(fp, " cnf ");                //����cnf������ʼ�����ļ�����Ϣ��

    numVar = GetNum(fp);              //��ȡ������������ֵ��numVar
    numClause = GetNum(fp);                        //�ٶ�ȡ�Ӿ���
    HeadNode* LIST = CreateClause(numVar, fp);   //��������ڽӱ�
    return LIST;
}