#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>


extern int* array;
extern int arraySize;
extern enum threadStatus* threadStatuses;
extern int threadCount;
extern  LPCRITICAL_SECTION section;

enum threadStatus
{
	active,
	pause,
	runnimg,
	runned
};


void WINAPI marker(LPVOID params)
{
	int threadIndex = (int)params - 1;
	srand(threadIndex + 1);
	printf("�������ֵ��߳� ���ڧ�� �� �ߧ�ާ֧��� %d ��ʼ ���ѧ�ѧݧ�\n", threadIndex + 1);
	while (1)
	{
		while (threadStatuses[threadIndex] != active) {}
		int markedElementsCount = 0;
		while (1)
		{
			int digit = rand();
			digit %= arraySize;
			EnterCriticalSection(&section);//��
			if (array[digit] == 0)
			{
				markedElementsCount++;
				Sleep(5);
				array[digit] = threadIndex + 1;
				Sleep(5);
			}
			else
			{
				printf(" ��: ��ڧ�ݧ�: %d\n ��ǵ�Ԫ�ؼ���: ����ݧڧ�֧��ӧ� ���ާ֧�֧ߧߧ�� ��ݧ֧ާ֧ߧ���:%d\n �޷������������Ԫ�أ����֧ӧ�٧ާ�اߧ� ���ާ֧�ڧ�� ��ݧ֧ާ֧ߧ� ��� �ڧߧէ֧ܧ��: %d\n",
					threadIndex + 1, markedElementsCount, digit);
				threadStatuses[threadIndex] = pause;
				LeaveCriticalSection(&section);//����
				break;
			}
			LeaveCriticalSection(&section);
		}
		while (threadStatuses[threadIndex] == pause) {}

		if (threadStatuses[threadIndex] == runnimg)
		{
			for (int i = 0; i < arraySize; ++i)
			{ 
				if (array[i] == threadIndex + 1)
				{
					array[i] = 0;
				}
			}
			threadStatuses[threadIndex] = runned;
			break;
		}
	}

}