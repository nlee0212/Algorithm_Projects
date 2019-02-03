#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NAMELEN 15
#define NUM 1000

char filename[NAMELEN] = "input2.txt";
char output[NAMELEN] = "output.txt";
int i;
int* string;

int checking(int firstnum, int secondnum, int strlen, int* first, int* second, int* str);

int main()
{
	FILE*fp = fopen(filename, "r");
	FILE*op = fopen(output, "w");
	int firstnum, secondnum;
	int strlen;
	int* first, *second;

	fscanf(fp, "%d", &firstnum);
	first = (int*)malloc(sizeof(int)*firstnum);
	for (i = 0;i < firstnum;i++)
		fscanf(fp, "%d", &first[i]);
	first[i] = '\0';
	fscanf(fp, "%d", &secondnum);
	second = (int*)malloc(sizeof(int)*secondnum);
	for (i = 0;i < secondnum;i++)
		fscanf(fp, "%d", &second[i]);
	second[i] = '\0';
	string = (int*)malloc(sizeof(int)*(firstnum + secondnum));
	
	while (1)
	{
		if (firstnum + secondnum != strlen)
		{
			printf("ERROR\n");
			return 0;
		}
		if (fscanf(fp, "%d", &strlen) == EOF)
			break;
		string = (int*)realloc(string, sizeof(int)*strlen);
		memset(string, 0, strlen);
		for (i = 0;i < strlen;i++)
			fscanf(fp, "%d", &string[i]);
		fprintf(op,"%d", checking(firstnum, secondnum, strlen, first, second,string));
	}

	fprintf(op,"\n");
	fclose(fp);
	fclose(op);

	return 0;
}

int checking(int firstnum, int secondnum, int strlen, int* first, int* second, int* str)
{
	int check = -1;
	int i;
	int a = 0, b = 0;
	int ifa, ifb;

	if (strlen == 0)
		return 1;

	for (i = 0;i < strlen;i++)
	{
		if (check == 0)
			break;
		
		if (str[i] == first[a] && a < firstnum)
		{
			if (str[i] == second[b] && b < secondnum)
			{
				ifa = checking(firstnum - a - 1, secondnum - b, strlen - a - b - 1, first + a + 1, second + b, str + a + b + 1);
				ifb = checking(firstnum - a, secondnum - b - 1, strlen - a - b - 1, first + a, second + b + 1, str + a + b + 1);
				if (ifa)
				{
					a++;
					check = 1;
				}
				else if (ifb)
				{
					b++;
					check = 1;
				}
				else
					check = 0;
			}

			else
			{
				a++;
				check = 1;
			}
		}

		else if (str[i] == second[b] && b < secondnum)
		{
			b++;
			check = 1;
		}

		else
			check = 0;
	}

	if (!check)
		return 0;
	else
		return 1;
}