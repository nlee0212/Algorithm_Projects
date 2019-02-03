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
int Insertion_Sort(int *data, int left, int right);

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

	if (Insertion_Sort(Arr, 0, N - 1) == 0) {
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

int Insertion_Sort(int *data, int left, int right)
{
	CHECK_TIME_START;
	int x, i = left+1, j;
	while (i < right + 1) {
		x = data[i];
		j = i - 1;
		while (j >= left && data[j] > x) {
			data[j + 1] = data[j];
			j--;
		}
		data[j + 1] = x;
		i++;
	}
	CHECK_TIME_END(resultTime);
	return Validation(data, left, right);
}