#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include <Windows.h>

/*
Algorithm Asignment2 ���� �ڵ��Դϴ�.
���� ���࿡ �־� Release��带 �ݵ�� Ȯ���� �ּ���. Debug�� �ӵ��� ��û ���̳��ϴ�.
a��� ���������� ������� �����ּ���. CHECK_TIME_END �ÿ� ���װ� �߻��մϴ�.

�Ʒ��� ���� �ڵ带 �⺻���� �ڵ��� ���ֽø� �˴ϴ�.
*/

//�ð� ���� �ڵ��Դϴ�. 
#define CHECK_TIME_START QueryPerformanceFrequency((LARGE_INTEGER*) &freq); QueryPerformanceCounter((LARGE_INTEGER*)&start);
#define CHECK_TIME_END(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float) (end - start)/freq);
// a��� ���������� ���� ������� ������. �ð����� ������ �߻��մϴ�.

//SWAP�� �����ϴ� ��ũ�� �Դϴ�.
#define SWAP(a, b) {\
	swap_temp = a; a = b; b = swap_temp;\
}

#define CSWAP(a,b) {\
	if(a>b) SWAP(a,b);\
}


//���� �̸��� ���� ��ũ�� �Դϴ�.
#define FILE_NAME "inputdata.bin"
//������
__int64 start, freq, end;
float resultTime;
int swap_temp;
//������!
int N;

int Validation(int* data, int left, int right);
int Selection_Sort(int *data, int left, int right);
void Quick_Sort_PSS(int *data, int left, int right);
int partition(int *data, int left, int right);



int main() {

	int temp;
	int* Arr;
	FILE* fa = fopen(FILE_NAME, "rb");
	fread(&N, sizeof(int), 1, fa);

	Arr = (int*)malloc(sizeof(int) * N);
	printf("[ %d ] Numbers Of Input Data\n", N);
	for (int i = 0; i < N; i++) {
		fread(&temp, sizeof(int), 1, fa);
		Arr[i] = temp;
	}

	Quick_Sort_PSS(Arr, 0, N - 1);
	if (Validation(Arr, 0, N - 1) == 0) {
		printf("Well Done\n");
	}
	else { // Validation ����� �ùٸ��� ������ �����޽����� ����մϴ�.
		printf("Try Again\n");
	}

	//�ð����
	printf("Total Time is [ %f (ms)]\n", resultTime*1000.0f);


	//for data check
	// �ּ� ��ü �Ͻð� �����͸� Ȯ���غ��ø� �˴ϴ�.

	/*for (int i = 0; i < N; i++) {
	printf("\t[%d]'s data is %d\n", i, Arr[i]);
	}*/

	fclose(fa);
	return 0;
}

int Validation(int* data, int left, int right) {
	for (int i = left; i < right - 1; i++) {
		if (data[i] > data[i + 1])
			return -1;
	}
	return 0;
}

int Selection_Sort(int *data, int left, int right)
{
	int i, j, idx;

	for (i = left;i < right;i++)
	{
		idx = i;
		for (j = i + 1;j <= right;j++)
		{
			if (data[j] < data[idx])
				idx = j;
		}
		SWAP(data[idx], data[i]);
	}
	return 0;
}

void Quick_Sort_PSS(int *data, int left, int right)
{
	if (left >= right)
		return 0;

	if (left == 0 && right == N - 1)
	{
		CHECK_TIME_START;
	}

	int pivot;

	if ((right - left) >= 16) {
		pivot = partition(data, left, right);
		Quick_Sort_PSS(data, left, pivot - 1);
		Quick_Sort_PSS(data, pivot + 1, right);
	}

	else {
		Selection_Sort(data, left, right);
	}

	if (left == 0 && right == N - 1)
	{
		CHECK_TIME_END(resultTime);
	}
}

int partition(int *data, int left, int right)
{
	int pivot, temp;
	int i, j;

	i = left;
	temp = data[left];
	for (j = left + 1;j <= right;j++)
	{
		if (data[j] < temp)
		{
			i++;
			SWAP(data[j], data[i]);
		}
	}
	pivot = i;
	SWAP(data[left], data[pivot]);

	return pivot;
}