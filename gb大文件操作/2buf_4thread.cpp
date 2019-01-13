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
int numbers[SIZE/MAXN][threadCount][N]; //字母统计 
char* loadedFile[2];					//双缓冲区 

//对缓冲区的1个Part进行字母统计 
void char_count(int step, int index, int bufferNum, int threadNum)
{
	int high = index + Part;
	for(int i=index; i<high; i++)
	{		
		numbers[bufferNum][threadNum][int(loadedFile[step][i]-'a')]++;
	}
} 

//输出统计结果 
void output_counts()
{
	printf("The number of the 26 letters is as follows:\n");
	int total = 0; 
	for(int i=0; i<N; i++)
	{
		int t = 0;
		for(int k=0; k<SIZE/MAXN; k++)
		{
			for(int j=0; j<threadCount; j++)
				{
					t += numbers[k][j][i];
				}
		}
		printf("%c %d\n", i+'a', t);
		total += t;
	}
	printf("The total number of letters is %d.\n", total);
}


//读入到step号缓冲区 
void freads(int step)
{
	fread(loadedFile[step], 1, MAXN, stdin);
}

int main()
{
	int start = clock();
	memset(numbers, 0 ,sizeof(numbers));
	
	//获取待分析文件名 
	char filename[100];
	freopen("input.txt", "r", stdin);
	cin >> filename;
	fclose(stdin);
	freopen(filename,"rb",stdin);
//	FILE *fp = fopen("chars.txt", "r");
	
	/*双缓冲*/ 
	loadedFile[0] = new char[MAXN];
    loadedFile[1] = new char[MAXN];
    
   /*声明threadCount个线程*/ 
    thread* threads = new thread[threadCount];
    bool step = 0, needWait = false;
    
	for(int i=0; i<SIZE/MAXN; i++)
	{
		//读入缓冲区 
		freads(step);
//		fread(loadedFile[step], 1, MAXN, fp);
		
		//等待前一个缓冲区所有线程计算完 
		if(needWait)
		{
			for (int j=0; j<threadCount; j++)
			{ 
				threads[j].join();
			} 
		}
        else
        {
		    needWait = true;
        } 
            
        //多线程计算，将缓冲区分成threadCount个区域 
        for (int j=0; j<threadCount; j++)
        {
			int index = j*Part;			//每一块起始地址 
            threads[j] = thread(char_count, step, index, i, j);
        }
        //切换到另一个缓冲区 
        step = !step;     
	} 
    
    //计算完读入的最后一个缓冲区 
    for (int i=0; i<threadCount; i++)
	{
		threads[i].join();
	} 
	
	//清理 ,关闭文件 
    delete loadedFile[0];
    delete loadedFile[1];
    fclose(stdin);
//	fclose(fp);
    
    //输出统计结果 
    output_counts();
    
    //计算并输出耗时 
    printf("It takes %.5lf s to read and count 1GB of letters.\n", double(clock()-start)/CLOCKS_PER_SEC);
    
	return 0;	
}


