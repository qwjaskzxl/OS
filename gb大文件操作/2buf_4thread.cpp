#include<iostream>
#include <ctime>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <fcntl.h>
using namespace std;

const int N = 26;
const int SIZE = 1024*1024*1024;		//�ļ���С 
const int MAXN = 128*1024*1024;   		//��������С 
const int threadCount = 4;				//�����߳����� 
const int Part= MAXN/threadCount;		//������ÿ���ִ�С 
int numbers[SIZE/MAXN][threadCount][N]; //��ĸͳ�� 
char* loadedFile[2];					//˫������ 

//�Ի�������1��Part������ĸͳ�� 
void char_count(int step, int index, int bufferNum, int threadNum)
{
	int high = index + Part;
	for(int i=index; i<high; i++)
	{		
		numbers[bufferNum][threadNum][int(loadedFile[step][i]-'a')]++;
	}
} 

//���ͳ�ƽ�� 
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


//���뵽step�Ż����� 
void freads(int step)
{
	fread(loadedFile[step], 1, MAXN, stdin);
}

int main()
{
	int start = clock();
	memset(numbers, 0 ,sizeof(numbers));
	
	//��ȡ�������ļ��� 
	char filename[100];
	freopen("input.txt", "r", stdin);
	cin >> filename;
	fclose(stdin);
	freopen(filename,"rb",stdin);
//	FILE *fp = fopen("chars.txt", "r");
	
	/*˫����*/ 
	loadedFile[0] = new char[MAXN];
    loadedFile[1] = new char[MAXN];
    
   /*����threadCount���߳�*/ 
    thread* threads = new thread[threadCount];
    bool step = 0, needWait = false;
    
	for(int i=0; i<SIZE/MAXN; i++)
	{
		//���뻺���� 
		freads(step);
//		fread(loadedFile[step], 1, MAXN, fp);
		
		//�ȴ�ǰһ�������������̼߳����� 
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
            
        //���̼߳��㣬���������ֳ�threadCount������ 
        for (int j=0; j<threadCount; j++)
        {
			int index = j*Part;			//ÿһ����ʼ��ַ 
            threads[j] = thread(char_count, step, index, i, j);
        }
        //�л�����һ�������� 
        step = !step;     
	} 
    
    //�������������һ�������� 
    for (int i=0; i<threadCount; i++)
	{
		threads[i].join();
	} 
	
	//���� ,�ر��ļ� 
    delete loadedFile[0];
    delete loadedFile[1];
    fclose(stdin);
//	fclose(fp);
    
    //���ͳ�ƽ�� 
    output_counts();
    
    //���㲢�����ʱ 
    printf("It takes %.5lf s to read and count 1GB of letters.\n", double(clock()-start)/CLOCKS_PER_SEC);
    
	return 0;	
}


