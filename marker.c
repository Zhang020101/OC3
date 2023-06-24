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
	printf("带有数字的线程 Нить с номером %d 开始 Начало\n", threadIndex + 1);
	while (1)
	{
		while (threadStatuses[threadIndex] != active) {}
		int markedElementsCount = 0;
		while (1)
		{
			int digit = rand();
			digit %= arraySize;
			EnterCriticalSection(&section);//锁
			if (array[digit] == 0)
			{
				markedElementsCount++;
				Sleep(5);
				array[digit] = threadIndex + 1;
				Sleep(5);
			}
			else
			{
				printf(" 数: число: %d\n 标记的元素计数: Количество отмеченных элементов:%d\n 无法标记索引处的元素：Невозможно отметить элемент по индексу: %d\n",
					threadIndex + 1, markedElementsCount, digit);
				threadStatuses[threadIndex] = pause;
				LeaveCriticalSection(&section);//解锁
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