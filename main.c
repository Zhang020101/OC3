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
CRITICAL_SECTION section;//限制多线程对全局的共享

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
	InitializeCriticalSection(&section);//使用CRITICAL_SECTION前的初始化对象

	printf("写入数组大小 Запись в размер массива\n");
	scanf("%d", &arraySize);

	array = (int*)calloc(arraySize, sizeof(int));

	for (int i = 0; i < arraySize; ++i)
	{
		array[i] = 0;
	}

	printf("记录流的数量 Количество потоков записи\n");
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
			0,//立即执行
			NULL);
	}

	for (int i = 0; i < threadCount; ++i)
	{
		threadStatuses[i] = active;
	}

	while (true)
	{
		while (CheckAllThreads() != true) {}

		printf("排列: Массив: ");
		for (int i = 0; i < arraySize; ++i)
		{
			printf("%d ", array[i]);
		}
		printf("\n输入标记数字 Введите номера маркеров\n");
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
		printf("阵列已被改变：Массив изменен: ");
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