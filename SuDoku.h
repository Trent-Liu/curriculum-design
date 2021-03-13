#include"SatDefine.h"


/*
在六阶数独棋盘内随机生成初始棋局
*/
void InitRandomboard(int board[6][6])
{

	srand((unsigned)time(nullptr));
	for (int j = 0; j <= 5; j++)
	{
		for (int i = 0; i <= 5; i++)
			board[j][i] = -1;                    //-1的位置表示当前位置无结果

	}
	for (int j = 0; j <= 10; j++)
	{
		int a = rand() % 36;

		board[a / 6][a % 6] = rand() % 2;

	}
}


/*
输出棋盘
*/
void checkboard(int board[6][6])
{
	for (int i = 0; i <= 5; i++)
	{
		printf("\t\t\t-------------\n");
		printf("\t\t\t|");
		for (int j = 0; j <= 5; j++) {
			if (board[i][j] != -1)
				printf("%d|", board[i][j]);
			else
				printf("%c|", '*');
		}
		printf("\n");

	}
}

/*
比较棋盘的某两个位置是否相等
*/
int  ifsame(int row1, int col1, int row2, int col2)
{
	if (pan[row1][col1] == 0 && pan[row2][col2] == 0 || pan[row1][col1] == 1 && pan[row2][col2] == 1)
		return 1;
	else
		return 0;
}

/*
数独棋盘转化为cnf文件
*/
void TransToCnf(int board[6][6])
{


	int numvar = 0;
	for (int i = 0; i <= 5; i++) {
		for (int j = 0; j <= 5; j++)
		{
			if (board[i][j] != -1)
				numvar++;

		}
	}

	FILE* fp;
	strcpy(filename, filepath);
	strcat(filename, "SuDoku.cnf");
	fp = fopen(filename, "w");
	if (fp == NULL)
	{
		printf("棋盘写入失败");
	}
	fprintf(fp, "p cnf 216 %d\n", numvar + 1206);
	/*fprintf(fp, "p cnf 36 96\n");*/


	HeadNode* List = new HeadNode;

	//已知的棋盘变元作为单子句存入cnf
	for (int i = 0; i <= 5; i++) {
		for (int j = 0; j <= 5; j++)
		{
			if (board[i][j] == 1)
				fprintf(fp, "%d 0\n", i * 6 + j + 1);
			else if (board[i][j] == 0)
				fprintf(fp, "%d 0\n", -(i * 6 + j + 1));
		}
	}

	//约束1：行与列中无三个连续的0或1
	for (int i = 0; i <= 5; i++)           //行
	{
		for (int j = 1; j <= 4; j++)
		{
			fprintf(fp, "%d %d %d 0\n", i * 6 + j, i * 6 + j + 1, i * 6 + j + 2);
			fprintf(fp, "%d %d %d 0\n", -(i * 6 + j), -(i * 6 + j + 1), -(i * 6 + j + 2));
		}
	}
	for (int i = 1; i <= 6; i++)           //列
	{
		for (int j = 0; j <= 3; j++)
		{
			fprintf(fp, "%d %d %d 0\n", j * 6 + i, (j + 1) * 6 + i, (j + 2) * 6 + i);
			fprintf(fp, "%d %d %d 0\n", -(j * 6 + i), -((j + 1) * 6 + i), -((j + 2) * 6 + i));
		}
	}




	//约束二
	for (int i = 0; i <= 5; i++)   //每行的任意4个数不能全1或全0
		for (int i1 = 1; i1 <= 3; i1++)
			for (int i2 = i1 + 1; i2 <= 4; i2++)
				for (int i3 = i2 + 1; i3 <= 5; i3++)
					for (int i4 = i3 + 1; i4 <= 6; i4++)
					{
						fprintf(fp, "%d %d %d %d 0\n", i * 6 + i1, i * 6 + i2, i * 6 + i3, i * 6 + i4);
						fprintf(fp, "%d %d %d %d 0\n", -(i * 6 + i1), -(i * 6 + i2), -(i * 6 + i3), -(i * 6 + i4));
					}
	for (int i = 1; i <= 6; i++)   //每列的任意4个数不能全1或全0
		for (int i1 = 0; i1 <= 2; i1++)
			for (int i2 = i1 + 1; i2 <= 3; i2++)
				for (int i3 = i2 + 1; i3 <= 4; i3++)
					for (int i4 = i3 + 1; i4 <= 5; i4++)
					{
						fprintf(fp, "%d %d %d %d 0\n", i1 * 6 + i, i2 * 6 + i, i3 * 6 + i, i4 * 6 + i);
						fprintf(fp, "%d %d %d %d 0\n", -(i1 * 6 + i), -(i2 * 6 + i), -(i3 * 6 + i), -(i4 * 6 + i));
					}

	//约束3
	int n = 36;
	for (int i = 0; i <= 4; i++)  //行
		for (int j = i + 1; j <= 5; j++)
		{
			for (int k = 1; k <= 6; k++)
			{

				fprintf(fp, "%d %d %d 0\n", -(i * 6 + k), j * 6 + k, n + k);
				fprintf(fp, "%d %d %d 0\n", i * 6 + k, -(j * 6 + k), n + k);
				fprintf(fp, "%d %d %d 0\n", i * 6 + k, j * 6 + k, -(n + k));
				fprintf(fp, "%d %d %d 0\n", -(i * 6 + k), -(j * 6 + k), -(n + k));


			}
			fprintf(fp, "%d %d %d %d %d %d 0\n", n + 1, n + 2, n + 3, n + 4, n + 5, n + 6);
			n += 6;
		}
	for (int i = 1; i <= 5; i++)  //行
		for (int j = i + 1; j <= 6; j++)
		{
			for (int k = 0; k <= 5; k++)
			{
				fprintf(fp, "%d %d %d 0\n", -(k * 6 + i), k * 6 + j, n + k + 1);
				fprintf(fp, "%d %d %d 0\n", k * 6 + i, -(k * 6 + j), n + k + 1);
				fprintf(fp, "%d %d %d 0\n", k * 6 + i, k * 6 + j, -(n + k + 1));
				fprintf(fp, "%d %d %d 0\n", -(k * 6 + i), -(k * 6 + j), -(n + k + 1));


			}
			fprintf(fp, "%d %d %d %d %d %d 0\n", n + 1, n + 2, n + 3, n + 4, n + 5, n + 6);
			n += 6;

		}
	fclose(fp);
}


/*
拷贝棋盘
*/
void arraycopy(int a[6][6], int b[6][6])
{
	for (int i = 0; i <= 5; i++)
		for (int j = 0; j <= 5; j++)
			b[i][j] = a[i][j];
}
/*数独游戏*/
int SuDokuGame(int qipan[6][6]) {
	int qipan1[6][6];

	arraycopy(qipan, qipan1);
	printf("\t\t\t当前棋盘：\n");
	checkboard(qipan1);
	printf("\t\t\t请分别输入要填写的行，列，使用1或0来填补空缺：\n");
	printf("\t\t\t连续输入三个0，则退出求解;\n\t\t\t");
	int answer[3] = { -1 };
	while (scanf("%d", &answer[0]) != EOF) {
		int mark = 0;
		for (int i = 1; i < 3; i++) {
			scanf("%d", &answer[i]);
		}
		if (answer[0] == 0 && answer[1] == 0 && answer[2] == 0) {
			return 0;
		}
		else {

			if (qipan[answer[0] - 1][answer[1] - 1] == 1 || qipan[answer[0] - 1][answer[1] - 1] == 0) {
				printf("\t\t\t该位置是初始位置，不可填写或修改\n");
				mark = 1;
			}
			if (mark != 1)
				qipan1[answer[0] - 1][answer[1] - 1] = answer[2];
		}
		putchar('\n');
		checkboard(qipan1);
		printf("\n\t\t\t继续输入!\n\t\t\t");
	}
}