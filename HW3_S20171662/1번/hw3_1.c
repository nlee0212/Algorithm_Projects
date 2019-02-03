#include <stdio.h>

#define NAMELEN 15

char filename[NAMELEN] = "input.txt";
char textfile[NAMELEN];
char outputfile[NAMELEN] = "output.txt";
int s, f, p;
int m, n;
int i, j;
void gapped_alignment(int, int);
int maximum(int a, int b, int c);
int check(char a, char b);
int max;
int**score;
char *X, *Y;
int *X_gap, *Y_gap;
int xgap=0, ygap=0,len=0;
FILE* op;

int main()
{
	FILE* fp = fopen(filename, "r");
	FILE* tp;
	
	fscanf(fp, "%s", textfile);
	fscanf(fp, "%d %d %d", &s, &f, &p);
	fclose(fp);

	tp = fopen(textfile, "rb");

	fread(&m, sizeof(int), 1, tp);
	//printf("m=%d\n", m);
	fread(&n, sizeof(int), 1, tp);
	//n--;//for input1, 1191을 1190으로 바꾸기 위해!!!!!!!!!!!!
	//printf("n=%d\n", n);

	X = (char*)malloc(sizeof(char)*m);
	Y = (char*)malloc(sizeof(char)*n);
	X_gap = (int*)malloc(sizeof(int)*n);
	Y_gap = (int*)malloc(sizeof(int)*m);

	score = (int**)malloc(sizeof(int*)*(n + 2));
	for (i = 0;i < n + 2;i++)
		score[i] = (int*)malloc(sizeof(int)*(m + 2));

	for (i = 0;i < m;i++)
		fread(&X[i], sizeof(char), 1, tp);
	X[i] = '\0';

	for (i = 0;i < n;i++)
		fread(&Y[i], sizeof(char), 1, tp);
	Y[i] = '\0';

	fclose(tp);

	op = fopen(outputfile, "w");

	//printf("X: %s\n", X);
	//printf("Y: %s\n", Y);

	score[0][0] = 0;
	score[0][1] = 0;
	score[1][0] = 0;
	score[1][1] = 0;

	for (i = 2;i <= m + 1;i++)
	{
		score[0][i] = X[i - 2];
		score[1][i] = 0 - (p * (i - 1));
	}
	for (i = 2;i <= n + 1;i++)
	{
		score[i][0] = Y[i - 2];
		score[i][1] = 0 - (p * (i - 1));
	}
	
	if (m == 0)
	{
		fp = fopen(outputfile, "w");
		fprintf(fp, "%d\n", (0 - (p * n)));
		fprintf(fp, "%d\n", n);
		fprintf(fp, "%d\n", n);
		for (i = 0;i < n;i++)
			fprintf(fp, "%d\n", i);
		return 0;
	}

	else if (n == 0)
	{
		fp = fopen(outputfile, "w");
		fprintf(fp, "%d\n", (0 - (p * m)));
		fprintf(fp, "%d\n", m);
		fprintf(fp, "%d\n", m);
		for (i = 0;i < m;i++)
			fprintf(fp, "%d\n", i);
		return 0;
	}

	else
	{
		gapped_alignment(m,n);
	}

	return 0;
}

void gapped_alignment(int a, int b)
{
	int first, second, third;

	for (i = 2;i <= n + 1;i++)
	{
		for (j = 2;j <= m+1;j++)
		{
			first = score[i - 1][j - 1] + check(score[0][j], score[i][0]);
			second = score[i - 1][j] - p;
			third = score[i][j - 1] - p;
			max = maximum(first, second, third);
			score[i][j] = max;
			//printf("%d ", score[i][j]);
		}
//		printf("\n");
	}
	i = n + 1;
	j = m + 1;
	while (1)
	{
		if (i == 1 && j==1)
			break;
	
		first = score[i - 1][j - 1];
		second = score[i - 1][j];
		third = score[i][j - 1];

		if (score[i][j] == (first + s) && (score[i][0]==score[0][j]))
		{
			i--;
			j--;
			len++;
		}

		else if (score[i][j] == (first - f))
		{
			i--;
			j--;
			len++;
		}

		else if (score[i][j] == (second - p))
		{
			i--;
			len++;
			X_gap[xgap] = len;
			xgap++;
		}

		else if (score[i][j] == (third - p))
		{
			j--;
			len++;
			Y_gap[ygap] = len;
			ygap++;
		}

	}

	X_gap[xgap] = '\0';
	Y_gap[ygap] = '\0';

	//printf("%d\n%d\n", score[n + 1][m + 1], len);
	fprintf(op,"%d\n%d\n", score[n + 1][m + 1], len);
	//printf("%d\n", xgap);
	fprintf(op,"%d\n", xgap);
	for (i = 0;i < xgap;i++)
	{
		//printf("%d\n", len - X_gap[xgap - i - 1] + 1);
		fprintf(op,"%d\n", len - X_gap[xgap - i - 1] + 1);
	}
	//printf("%d\n", ygap);
	fprintf(op,"%d\n", ygap);
	for (i = 0;i < ygap;i++)
	{
		//printf("%d\n", len - Y_gap[ygap - i - 1] + 1);
		fprintf(op,"%d\n", len - Y_gap[ygap - i - 1] + 1);
	}
}

int maximum(int a, int b, int c)
{
	if (a >= b && a >= c)
		return a;
	else if (b >= a && b >= c)
		return b;
	else
		return c;
}

int check(char a, char b)
{
	if (a == b)
		return s;
	else
		return (0 - f);
}