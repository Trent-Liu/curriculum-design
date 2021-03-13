#ifndef Sat_Define
#define Sat_Define
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>
//�������ڱ��ÿ����Ԫ�Ľ������ΪTRUE ȡ�� FALSE ȡ�� UNKNOWN δ֪ UNDEF ��Ԫ������

#define TRUE 1
#define FALSE 0
#define ROW 9
#define COL 9
#define NoAnwser -1
typedef int status;

typedef struct LiteralNode {
    int data;
    LiteralNode* next;
}DataNode;

typedef struct HeadNode {
    int Num;
    LiteralNode* right;
    HeadNode* down;
}HeadNode;

struct consequence {
    int value;//����ֵ ��ʱΪtrue-1����ʱΪfalse-0
};

struct records {
    int order;//���Ԫ�ĳ��ִ���
    int var;//��Ԫ
};





char filename[200];
char filename1[200];
FILE* fp;
FILE* fp1;
int numVar;
int numClause;
int time_cost;
char filepath[40] = "./Save/";

records* Order = (records*)malloc(sizeof(records) * (2 * numVar + 1));



consequence* result;



int pan[6][6];

int GetNum(FILE* fp);
HeadNode* CreateClause(int& VARNUM, FILE* fp);
HeadNode* SearchSingle(HeadNode* Pfind);
void DelWholeRaw(HeadNode* Clause, HeadNode*& LIST);
void DeleteDataNode(int temp, HeadNode*& LIST);
HeadNode* Duplication(HeadNode* LIST);
HeadNode* ADDSingleClause(HeadNode* LIST, int Var);
status IsEmptyClause(HeadNode* LIST);
status DPLL(HeadNode* LIST, consequence* result);
void SaveFile1(consequence* result);
void SaveFile2(void);
int checkanswer(void);
HeadNode* cnfToList(void);
void InitRandomboard(int board[6][6]);
void checkboard(int board[6][6]);
int  ifsame(int row1, int col1, int row2, int col2);
void TransToCnf(int board[6][6]);
void arraycopy(int a[6][6], int b[6][6]);
int PlaySuDoKu(int qipan[6][6]);



#endif