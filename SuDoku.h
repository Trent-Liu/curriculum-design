#include"SatDefine.h"


/*
����������������������ɳ�ʼ���
*/
void InitRandomboard(int board[6][6])
{

	srand((unsigned)time(nullptr));
	for (int j = 0; j <= 5; j++)
	{
		for (int i = 0; i <= 5; i++)
			board[j][i] = -1;                    //-1��λ�ñ�ʾ��ǰλ���޽��

	}
	for (int j = 0; j <= 10; j++)
	{
		int a = rand() % 36;

		board[a / 6][a % 6] = rand() % 2;

	}
}


/*
�������
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
�Ƚ����̵�ĳ����λ���Ƿ����
*/
int  ifsame(int row1, int col1, int row2, int col2)
{
	if (pan[row1][col1] == 0 && pan[row2][col2] == 0 || pan[row1][col1] == 1 && pan[row2][col2] == 1)
		return 1;
	else
		return 0;
}

/*
��������ת��Ϊcnf�ļ�
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
		printf("����д��ʧ��");
	}
	fprintf(fp, "p cnf 216 %d\n", numvar + 1206);
	/*fprintf(fp, "p cnf 36 96\n");*/


	HeadNode* List = new HeadNode;

	//��֪�����̱�Ԫ��Ϊ���Ӿ����cnf
	for (int i = 0; i <= 5; i++) {
		for (int j = 0; j <= 5; j++)
		{
			if (board[i][j] == 1)
				fprintf(fp, "%d 0\n", i * 6 + j + 1);
			else if (board[i][j] == 0)
				fprintf(fp, "%d 0\n", -(i * 6 + j + 1));
		}
	}

	//Լ��1����������������������0��1
	for (int i = 0; i <= 5; i++)           //��
	{
		for (int j = 1; j <= 4; j++)
		{
			fprintf(fp, "%d %d %d 0\n", i * 6 + j, i * 6 + j + 1, i * 6 + j + 2);
			fprintf(fp, "%d %d %d 0\n", -(i * 6 + j), -(i * 6 + j + 1), -(i * 6 + j + 2));
		}
	}
	for (int i = 1; i <= 6; i++)           //��
	{
		for (int j = 0; j <= 3; j++)
		{
			fprintf(fp, "%d %d %d 0\n", j * 6 + i, (j + 1) * 6 + i, (j + 2) * 6 + i);
			fprintf(fp, "%d %d %d 0\n", -(j * 6 + i), -((j + 1) * 6 + i), -((j + 2) * 6 + i));
		}
	}




	//Լ����
	for (int i = 0; i <= 5; i++)   //ÿ�е�����4��������ȫ1��ȫ0
		for (int i1 = 1; i1 <= 3; i1++)
			for (int i2 = i1 + 1; i2 <= 4; i2++)
				for (int i3 = i2 + 1; i3 <= 5; i3++)
					for (int i4 = i3 + 1; i4 <= 6; i4++)
					{
						fprintf(fp, "%d %d %d %d 0\n", i * 6 + i1, i * 6 + i2, i * 6 + i3, i * 6 + i4);
						fprintf(fp, "%d %d %d %d 0\n", -(i * 6 + i1), -(i * 6 + i2), -(i * 6 + i3), -(i * 6 + i4));
					}
	for (int i = 1; i <= 6; i++)   //ÿ�е�����4��������ȫ1��ȫ0
		for (int i1 = 0; i1 <= 2; i1++)
			for (int i2 = i1 + 1; i2 <= 3; i2++)
				for (int i3 = i2 + 1; i3 <= 4; i3++)
					for (int i4 = i3 + 1; i4 <= 5; i4++)
					{
						fprintf(fp, "%d %d %d %d 0\n", i1 * 6 + i, i2 * 6 + i, i3 * 6 + i, i4 * 6 + i);
						fprintf(fp, "%d %d %d %d 0\n", -(i1 * 6 + i), -(i2 * 6 + i), -(i3 * 6 + i), -(i4 * 6 + i));
					}

	//Լ��3
	int n = 36;
	for (int i = 0; i <= 4; i++)  //��
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
	for (int i = 1; i <= 5; i++)  //��
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
��������
*/
void arraycopy(int a[6][6], int b[6][6])
{
	for (int i = 0; i <= 5; i++)
		for (int j = 0; j <= 5; j++)
			b[i][j] = a[i][j];
}
/*������Ϸ*/
int SuDokuGame(int qipan[6][6]) {
	int qipan1[6][6];

	arraycopy(qipan, qipan1);
	printf("\t\t\t��ǰ���̣�\n");
	checkboard(qipan1);
	printf("\t\t\t��ֱ�����Ҫ��д���У��У�ʹ��1��0�����ȱ��\n");
	printf("\t\t\t������������0�����˳����;\n\t\t\t");
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
				printf("\t\t\t��λ���ǳ�ʼλ�ã�������д���޸�\n");
				mark = 1;
			}
			if (mark != 1)
				qipan1[answer[0] - 1][answer[1] - 1] = answer[2];
		}
		putchar('\n');
		checkboard(qipan1);
		printf("\n\t\t\t��������!\n\t\t\t");
	}
}