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
int numbers[N]; //��ĸͳ�� 
char* loadedFile;					//˫������ 

//���ɻ�������1��Part 
void char_gene(int index)
{
	int high = index + Part;
	for(int i=index; i<high; i++)
	{				
		loadedFile[i] = char((rand() % (26))+ 97);
	}
} 

//��ĸͳ�� 
void char_count()
{
	for(int i=0; i<MAXN; i++)
	{
		numbers[int(loadedFile[i]-'a')]++;
	}
} 

//���ͳ�ƽ�� 
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
	
	/*������*/ 
	loadedFile = new char[MAXN];
    
   /*����threadCount���߳�*/ 
    thread* threads = new thread[threadCount];
    
	for(int i=0; i<SIZE/MAXN; i++)
	{
		
		//���̼߳��㣬���������ֳ�threadCount������ 
        for (int j=0; j<threadCount; j++)
        {
			int index = j*Part;			//ÿһ����ʼ��ַ 
            threads[j] = thread(char_gene, index);
        }
        
        for (int j=0; j<threadCount; j++)
		{ 
			threads[j].join();
		} 
		
		char_count(); 
		fwrite(loadedFile, 1, MAXN, fp);		 
	} 
	
	//���� ,�ر��ļ� 
    delete loadedFile;
    fclose(fp);
	    
    //���ͳ�ƽ�� 
    output_counts();
    
    //���㲢�����ʱ 
    printf("It takes %.5lf s to randomly generate 1GB of letters.\n", double(clock()-start)/CLOCKS_PER_SEC);
    
	return 0;	
}


