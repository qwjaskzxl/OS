#include<iostream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <fcntl.h>
using namespace std;

const int N = 26;
const int SIZE = 1024*1024*1024;		//文件大小 
const int MAXN = 128*1024*1024;   		//缓冲区大小 
const int threadCount = 4;				//计算线程数量 
const int Part= MAXN/threadCount;		//缓冲区每部分大小 
int numbers[N]; //字母统计 
char* loadedFile;					//双缓冲区 

//生成缓冲区的1个Part 
void char_gene(int index)
{
	int high = index + Part;
	for(int i=index; i<high; i++)
	{				
		loadedFile[i] = char((rand() % (26))+ 97);
	}
} 

//字母统计 
void char_count()
{
	for(int i=0; i<MAXN; i++)
	{
		numbers[int(loadedFile[i]-'a')]++;
	}
} 

//输出统计结果 
void output_counts()
{
	printf("The number of the 26 letters is as follows:\n");
	int total = 0; 
	for(int i=0; i<N; i++)
	{
		total += numbers[i];
		printf("%c %d\n", i+'a', numbers[i]);
	}
	printf("The total number letters is %d.\n", total);
//	printf("%d\n", total);
}

int main()
{
	int start = clock();
	memset(numbers, 0 ,sizeof(numbers));
	
	char filename[100];
	freopen("input.txt", "r", stdin);
	cin >> filename;
	fclose(stdin);
	
	FILE *fp = fopen(filename, "w");
	
	/*单缓冲*/ 
	loadedFile = new char[MAXN];
    
   /*声明threadCount个线程*/ 
    thread* threads = new thread[threadCount];
    
	for(int i=0; i<SIZE/MAXN; i++)
	{
		
		//多线程计算，将缓冲区分成threadCount个区域 
        for (int j=0; j<threadCount; j++)
        {
			int index = j*Part;			//每一块起始地址 
            threads[j] = thread(char_gene, index);
        }
        
        for (int j=0; j<threadCount; j++)
		{ 
			threads[j].join();
		} 
		
		char_count(); 
		fwrite(loadedFile, 1, MAXN, fp);		 
	} 
	
	//清理 ,关闭文件 
    delete loadedFile;
    fclose(fp);
	    
    //输出统计结果 
    output_counts();
    
    //计算并输出耗时 
    printf("It takes %.5lf s to randomly generate 1GB of letters.\n", double(clock()-start)/CLOCKS_PER_SEC);
    
	return 0;	
}


