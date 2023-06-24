#include <stdlib.h>
#include <stdio.h>
#include "threadStatus.h"
#include <Windows.h>
#include <stdbool.h>


#pragma warning(disable:4996)

int* array;
int arraySize;
enum threadStatus* threadStatuses;
int threadCount;
CRITICAL_SECTION section;//���ƶ��̶߳�ȫ�ֵĹ���

void WINAPI marker(LPVOID params);

int CheckAllThreads()
{
	for (int i = 0; i < threadCount; ++i)
	{
		if (threadStatuses[i] == active)
		{
			return false;
		}
	}
	return true;
}


int main()
{
	HANDLE* threads;
	InitializeCriticalSection(&section);//ʹ��CRITICAL_SECTIONǰ�ĳ�ʼ������

	printf("д�������С ���ѧ�ڧ�� �� ��ѧ٧ާ֧� �ާѧ��ڧӧ�\n");
	scanf("%d", &arraySize);

	array = (int*)calloc(arraySize, sizeof(int));

	for (int i = 0; i < arraySize; ++i)
	{
		array[i] = 0;
	}

	printf("��¼�������� ����ݧڧ�֧��ӧ� �����ܧ�� �٧ѧ�ڧ��\n");
	scanf("%d", &threadCount);

	threads = (HANDLE*)calloc(threadCount, sizeof(HANDLE));
	threadStatuses = (enum threadStatus*)calloc(threadCount, sizeof(threadStatuses));

	for (int i = 0; i < threadCount; ++i)
	{
		int a = i + 1;
		threadStatuses[i] = pause;
		threads[i] = CreateThread(
			NULL,
			0,
			marker,
			(void*)a,
			0,//����ִ��
			NULL);
	}

	for (int i = 0; i < threadCount; ++i)
	{
		threadStatuses[i] = active;
	}

	while (true)
	{
		while (CheckAllThreads() != true) {}

		printf("����: ���ѧ��ڧ�: ");
		for (int i = 0; i < arraySize; ++i)
		{
			printf("%d ", array[i]);
		}
		printf("\n���������� ���ӧ֧էڧ�� �ߧ�ާ֧�� �ާѧ�ܧ֧���\n");
		int index = 0;
		
		bool flag = true;
		while(flag == true){
			scanf("%i", &index);

			flag = true;
			for (int i = 0; i < arraySize; i++)
			{
				if (index > array[i]) {
					flag = false;
				}
			}
			
		}

		threadStatuses[index - 1] = running;
		while (threadStatuses[index - 1] != runned) {}
		printf("�����ѱ��ı䣺���ѧ��ڧ� �ڧ٧ާ֧ߧ֧�: ");
		for (int i = 0; i < arraySize; ++i)
		{
			printf("%d ", array[i]);
		}
		printf("\n");

		for (int i = 0; i < threadCount; ++i)
		{
			if (threadStatuses[i] == pause) {
				threadStatuses[i] = active;
			}
		}

		int allFinished = 1;
		for (int i = 0; i < threadCount; ++i)
		{
			if (threadStatuses[i] != runned)
			{
				allFinished = 0;
				break;
			}
		}

		if (allFinished)
		{
			break;
		}
	}
	DeleteCriticalSection(&section);
}