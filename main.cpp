#include"SatDefine.h"
#include"SuDoku.h"
#include"SATsolve.h"
int main() {
    int op1 = 1;
    while (1) {
        printf("\n\n");
        printf("\t\t\t\t\t      ��ʼ�˵� \n");
        printf("\t\t-------------------------------------------------\n");
        printf("\t\t\t\t\t1. ���SAT\n");
        printf("\t\t\t\t\t2. ������Ϸ\n");
        printf("\t\t\t\t\t3. �˳�\n");
        printf("\t\t-------------------------------------------------\n");
        printf("\t\t\t���������ѡ�����루1����3��:");
        scanf("%d", &op1);
        system("cls");
        switch (op1) {
        case 1: {

            printf("\n\n\n\n\t\t\t\t������Ҫ��ȡ��cnf�ļ���·��:\n\t\t\t\t");   //��cnf�ļ�
            scanf("%s", filename);
            strcpy(filename1, filename);
            
            
            HeadNode*LIST=cnfToList();

            result = (consequence*)malloc(sizeof(consequence) * (numVar + 1));//��¼���յ����ֵ
            for (int i = 1; i <= numVar; i++)
                result[i].value = -1;
            clock_t StartTime, EndTime;
            
            StartTime = clock();
            int value = DPLL(LIST, result);
            EndTime = clock();


            if (value)   //�õ����
            {
                time_cost = (EndTime - StartTime);//�õ��������ĺ�ʱ
                printf("\n\t\t\t��������㣬��ʱ%d���롣\n", time_cost);
                printf("\n\t\t\tSAT����Ľ�Ϊ\n\t\t\t");
                for (int i = 1; i <= numVar; i++)
                {
                    int b = result[i].value ? 1 : -1;
                    printf("  %d  ", b * i);

                    if (i % 10 == 0)
                        printf("\n\t\t\t");
                }

                SaveFile1(result);
                if (checkanswer()) {

                    printf("\t\t\t�����飬�����ȷ�����ѱ��浽�ļ��С�\n");
                }
                else {
                    printf("�����飬������ڴ���\n");
                }
                system("pause");
                system("cls");
                break;
            }
            else  //�ò������
            {
                printf("SAT�޽�\n");
                SaveFile2;
                break;
            }
        }

        case 2:
            int value = 0;
            clock_t StartTime, EndTime;
            while (!value)
            {
                StartTime = clock();
                InitRandomboard(pan);
                //checkboard(pan);
                TransToCnf(pan);

                
                HeadNode* LIST = cnfToList();

                result = (consequence*)malloc(sizeof(consequence) * (numVar + 1));//��¼���յ����ֵ
                for (int i = 1; i <= numVar; i++)
                    result[i].value = -1;


                
                value = DPLL(LIST, result);
                EndTime = clock();

            }
            if (value)   //�õ����
            {
                for (int i = 1; i <= 36; i++)
                    pan[((i - 1) / 6)][(i - 1) % 6] = result[i].value;
                
                checkboard(pan);
                for (int i = 0; i <= 5; i++)
                    for (int j = 0; j <= 5; j++)
                        if (i > 1 && ifsame(i - 2, j, i - 1, j)) pan[i][j] = -1;//���ж��ϱ߽�
                        else if (j > 1 && ifsame(i, j - 2, i, j - 1)) pan[i][j] = -1;
                        else if (i < 4 && ifsame(i + 2, j, i + 1, j)) pan[i][j] = -1;
                        else if (j < 4 && ifsame(i, j + 2, i, j + 1)) pan[i][j] = -1;
                        else if (i > 0 && i < 5 && ifsame(i + 1, j, i - 1, j)) pan[i][j] = -1;
                        else if (j > 0 && j < 5 && ifsame(i, j + 1, i, j - 1)) pan[i][j] == -1;
                EndTime = clock();
                time_cost = (EndTime - StartTime);//�õ��������ĺ�ʱ
                printf("\n\t\t\t������zֻ��Ψһ��������о֣���ʱ%d���롣\n", time_cost);
                printf("\n\t\t\t������������\n\t\t\t");
               checkboard(pan);
               int pan1[6][6];
               arraycopy(pan, pan1);
               TransToCnf(pan);
               StartTime = clock();
               HeadNode* LIST = cnfToList();
               result = (consequence*)malloc(sizeof(consequence) * (numVar + 1));//��¼���յ����ֵ
               for (int i = 1; i <= numVar; i++)
                   result[i].value = -1;



               value = DPLL(LIST, result);
               EndTime = clock();
               time_cost = (EndTime - StartTime);
               for (int i = 1; i <= 36; i++)
                       pan[((i - 1) / 6)][(i - 1) % 6] = result[i].value;

               
               
               int op2;
               printf("\t\t\t��ѡ�� 1.ֱ�Ӳ鿴������\n\t\t\t       2.���Խ������\n\t\t\t");
               scanf("%d", &op2);
               if (op2 == 1)
               {
                   printf("\n\n\n\t\t\t����õ������ʱ%d���룬����Ϊ��ȷ���\n", time_cost);
                   checkboard(pan);
               }
               else if (op2 == 2)
               {
                   SuDokuGame(pan1);
                   printf("���������ȷ��Ϊ:");
                   checkboard(pan);
               }



            }
        }
    }
}