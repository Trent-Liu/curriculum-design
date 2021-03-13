#include"SatDefine.h"
#include"SuDoku.h"
#include"SATsolve.h"
int main() {
    int op1 = 1;
    while (1) {
        printf("\n\n");
        printf("\t\t\t\t\t      初始菜单 \n");
        printf("\t\t-------------------------------------------------\n");
        printf("\t\t\t\t\t1. 求解SAT\n");
        printf("\t\t\t\t\t2. 数独游戏\n");
        printf("\t\t\t\t\t3. 退出\n");
        printf("\t\t-------------------------------------------------\n");
        printf("\t\t\t请做出你的选择并输入（1——3）:");
        scanf("%d", &op1);
        system("cls");
        switch (op1) {
        case 1: {

            printf("\n\n\n\n\t\t\t\t请输入要读取的cnf文件的路径:\n\t\t\t\t");   //打开cnf文件
            scanf("%s", filename);
            strcpy(filename1, filename);
            
            
            HeadNode*LIST=cnfToList();

            result = (consequence*)malloc(sizeof(consequence) * (numVar + 1));//记录最终的真假值
            for (int i = 1; i <= numVar; i++)
                result[i].value = -1;
            clock_t StartTime, EndTime;
            
            StartTime = clock();
            int value = DPLL(LIST, result);
            EndTime = clock();


            if (value)   //得到结果
            {
                time_cost = (EndTime - StartTime);//得到解决问题的耗时
                printf("\n\t\t\t已完成运算，用时%d毫秒。\n", time_cost);
                printf("\n\t\t\tSAT问题的解为\n\t\t\t");
                for (int i = 1; i <= numVar; i++)
                {
                    int b = result[i].value ? 1 : -1;
                    printf("  %d  ", b * i);

                    if (i % 10 == 0)
                        printf("\n\t\t\t");
                }

                SaveFile1(result);
                if (checkanswer()) {

                    printf("\t\t\t经检验，结果正确，并已保存到文件中。\n");
                }
                else {
                    printf("经检验，结果存在错误。\n");
                }
                system("pause");
                system("cls");
                break;
            }
            else  //得不到结果
            {
                printf("SAT无解\n");
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

                result = (consequence*)malloc(sizeof(consequence) * (numVar + 1));//记录最终的真假值
                for (int i = 1; i <= numVar; i++)
                    result[i].value = -1;


                
                value = DPLL(LIST, result);
                EndTime = clock();

            }
            if (value)   //得到结果
            {
                for (int i = 1; i <= 36; i++)
                    pan[((i - 1) / 6)][(i - 1) % 6] = result[i].value;
                
                checkboard(pan);
                for (int i = 0; i <= 5; i++)
                    for (int j = 0; j <= 5; j++)
                        if (i > 1 && ifsame(i - 2, j, i - 1, j)) pan[i][j] = -1;//可判断上边界
                        else if (j > 1 && ifsame(i, j - 2, i, j - 1)) pan[i][j] = -1;
                        else if (i < 4 && ifsame(i + 2, j, i + 1, j)) pan[i][j] = -1;
                        else if (j < 4 && ifsame(i, j + 2, i, j + 1)) pan[i][j] = -1;
                        else if (i > 0 && i < 5 && ifsame(i + 1, j, i - 1, j)) pan[i][j] = -1;
                        else if (j > 0 && j < 5 && ifsame(i, j + 1, i, j - 1)) pan[i][j] == -1;
                EndTime = clock();
                time_cost = (EndTime - StartTime);//得到解决问题的耗时
                printf("\n\t\t\t已生成z只有唯一解的数独残局，用时%d毫秒。\n", time_cost);
                printf("\n\t\t\t数独问题如下\n\t\t\t");
               checkboard(pan);
               int pan1[6][6];
               arraycopy(pan, pan1);
               TransToCnf(pan);
               StartTime = clock();
               HeadNode* LIST = cnfToList();
               result = (consequence*)malloc(sizeof(consequence) * (numVar + 1));//记录最终的真假值
               for (int i = 1; i <= numVar; i++)
                   result[i].value = -1;



               value = DPLL(LIST, result);
               EndTime = clock();
               time_cost = (EndTime - StartTime);
               for (int i = 1; i <= 36; i++)
                       pan[((i - 1) / 6)][(i - 1) % 6] = result[i].value;

               
               
               int op2;
               printf("\t\t\t请选择 1.直接查看数独答案\n\t\t\t       2.尝试解决数独\n\t\t\t");
               scanf("%d", &op2);
               if (op2 == 1)
               {
                   printf("\n\n\n\t\t\t解决得到结果用时%d毫秒，下面为正确结果\n", time_cost);
                   checkboard(pan);
               }
               else if (op2 == 2)
               {
                   SuDokuGame(pan1);
                   printf("该问题的正确答案为:");
                   checkboard(pan);
               }



            }
        }
    }
}