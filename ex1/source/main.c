#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <malloc.h>

//由于下面的三个数组太大，只能定义为全局变量
char str[(1<<17)+2][34];         //用于存储从文件中读入的数据
char L[1<<17][34],R[1<<17][34];  //MERGE_SORT中用于存储分解后的两部分

static LARGE_INTEGER freq;

int compare(char* str1,char* str2)
{//str1<str2 返回<0 str1==str2返回0 str1>str2返回>0
    if(strlen(str1)>strlen(str2))
        return 1;
    else if(strlen(str1)==strlen(str2))
        return strcmp(str1,str2);
    else return -1;
}

void exchange(char *str1,char *str2)
{//交换字符两个字符串的内容
    char temp[34];
    strcpy(temp,str1);
    strcpy(str1,str2);
    strcpy(str2,temp);
}

//直接插入排序
void INSERTION_SORT(char str[][34],int n)
{
    int i,j;
    char temp[34];
    for(i=2;i<=n;i++)
    {
        strcpy(temp,str[i]);  //temp为当前要插入的元素
        j = i-1;
        while(j>0&&compare(str[j],temp)>0)
        {
                strcpy(str[j+1],str[j]);
                j--;
        }
        strcpy(str[j+1],temp);
    }

}

void insert(int n)
{
    int i;
    FILE *input,*output,*time;
    char *out_file1 = "../output//insert_sort//result_";
    char *in_file = "../input//input_strings.txt";
    char *time_file = "../output//insert_sort//time.txt";
    char *out_file;
    char size[5];
    static LARGE_INTEGER t1,t2;
    float timeused;
    out_file =(char*)malloc(strlen(out_file1)+10);
    if(!out_file)
    {
        printf("Malloc Filed!");
        exit(0);
    }
    itoa(n,size,10);
    strcpy(out_file,out_file1);
    strcat(out_file,size);
    strcat(out_file,".txt");
    input=fopen(in_file,"r");
    if(!input)
    {
        printf("Failed to open the input file!");
        exit (0);
    }
    output=fopen(out_file,"w");
    time=fopen(time_file,"a");

    printf("\nsize=2^%-2d   Please wait...",n);
    for(i=1;i<(1<<n)+1;i++)
        fgets(str[i],34,input);
    QueryPerformanceCounter(&t1);
    INSERTION_SORT(str,1<<n);
    QueryPerformanceCounter(&t2);
    for(i=1;i<(1<<n)+1;i++)
        fputs(str[i],output);
    timeused=(float)(t2.QuadPart-t1.QuadPart)/(float)freq.QuadPart;
    printf("Finished    time=%f us   ",timeused*1000000);
    fseek(time,0,SEEK_END);
    fprintf(time,"size=2^%-2d    time=%f us\n",n,timeused*1000000);

    fclose(input);
    fclose(output);
    fclose(time);
}
//堆排序
void MAX_HEAP(char str[][34],int i,int n)
{//将除了第i个元素外满足大根堆的数组调整成大根堆
    int l,r,largest;
    l = 2*i;
    r = 2*i+1;
    if(l<=n&&compare(str[i],str[l])<0)
        largest = l;
    else largest = i;
    if(r<=n&&compare(str[r],str[largest])>0)
        largest = r;
    if(largest != i)
    {
        exchange(str[i],str[largest]);
        MAX_HEAP(str,largest,n);
    }
}

void BUILD_MAX_HEAP(char str[][34],int n)
{//建立大根堆
    int i;
    for(i=(int)n/2;i>0;i--)
        MAX_HEAP(str,i,n);
}

void HEAP_SORT(char str[][34],int n)
{
    int i;
    BUILD_MAX_HEAP(str,n);
    for(i=n;i>1;i--)
    {
        exchange(str[1],str[i]);
        MAX_HEAP(str,1,i-1);
    }
}
void heap(int n)
{
    int i;
    FILE *input,*output,*time;
    char *out_file1 = "../output\\heap_sort\\result_";
    char *in_file = "../input\\input_strings.txt";
    char *time_file = "../output\\heap_sort\\time.txt";
    char *out_file;
    char size[5];
    static LARGE_INTEGER t1,t2;
    float timeused;
    out_file =(char*)malloc(strlen(out_file1)+10);
    if(!out_file)
    {
        printf("Malloc Filed!");
        exit(0);
    }
    itoa(n,size,10);
    strcpy(out_file,out_file1);
    strcat(out_file,size);
    strcat(out_file,".txt");
    input=fopen(in_file,"r");
    if(!input)
    {
        printf("Failed to open the input file!");
        exit (0);
    }
    output=fopen(out_file,"w");
    time=fopen(time_file,"a");

    printf("\nsize=2^%-2d   Please wait...",n);
    for(i=1;i<(1<<n)+1;i++)
        fgets(str[i],34,input);
    QueryPerformanceCounter(&t1);
    HEAP_SORT(str,1<<n);
    QueryPerformanceCounter(&t2);
    for(i=1;i<(1<<n)+1;i++)
        fputs(str[i],output);
    timeused=(float)(t2.QuadPart-t1.QuadPart)/(float)freq.QuadPart;
    printf("Finished    time=%f us   ",timeused*1000000);
    fseek(time,0,SEEK_END);
    fprintf(time,"size=2^%-2d    time=%f us\n",n,timeused*1000000);

    fclose(input);
    fclose(output);
    fclose(time);
}

//归并排序
void MERGE(char str[][34],int p,int q,int r)
{//将字符串数组str的前后两部分有序序列合并
    int n1,n2,i,j,k;
    n1 = q-p+1;
    n2 = r-q;
    for(i=1;i<=n1;i++)
        strcpy(L[i],str[p+i-1]);
    for(j=1;j<=n2;j++)
        strcpy(R[j],str[q+j]);
    strcpy(L[n1+1],"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");  //哨兵元素
    strcpy(R[n2+1],"zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");  //哨兵元素
    i = 1;
    j = 1;
    for(k=p;k<=r;k++)
        if(compare(L[i],R[j])<0)
            strcpy(str[k],L[i++]);
        else strcpy(str[k],R[j++]);
}

void MERGE_SORT(char str[][34],int p,int r)
{
    int q;
    if(p<r)
    {
        q = (int)(p+r)/2;
        MERGE_SORT(str,p,q);
        MERGE_SORT(str,q+1,r);
        MERGE(str,p,q,r);
    }
}

void merge(int n)
{
    int i;
    FILE *input,*output,*time;
    char *out_file1 = "../output\\merge_sort\\result_";
    char *in_file = "../input\\input_strings.txt";
    char *time_file = "../output\\merge_sort\\time.txt";
    char *out_file;
    char size[5];
    static LARGE_INTEGER t1,t2;
    float timeused;
    out_file =(char*)malloc(strlen(out_file1)+10);
    if(!out_file)
    {
        printf("Malloc Filed!");
        exit(0);
    }
    itoa(n,size,10);
    strcpy(out_file,out_file1);
    strcat(out_file,size);
    strcat(out_file,".txt");
    input=fopen(in_file,"r");
    if(!input)
    {
        printf("Failed to open the input file!");
        exit (0);
    }
    output=fopen(out_file,"w");
    time=fopen(time_file,"a");

    printf("\nsize=2^%-2d   Please wait...",n);
    for(i=1;i<(1<<n)+1;i++)
        fgets(str[i],34,input);
    QueryPerformanceCounter(&t1);
    MERGE_SORT(str,1,1<<n);
    QueryPerformanceCounter(&t2);
    for(i=1;i<(1<<n)+1;i++)
        fputs(str[i],output);
    timeused=(float)(t2.QuadPart-t1.QuadPart)/(float)freq.QuadPart;
    printf("Finished    time=%f us   ",timeused*1000000);
    fseek(time,0,SEEK_END);
    fprintf(time,"size=2^%-2d    time=%f us\n",n,timeused*1000000);

    fclose(input);
    fclose(output);
    fclose(time);
}

//快速排序
int PARTITION(char str[][34],int p,int r)
{//以最后一个元素为支点进行划分
    char temp[34];
    int i=p-1,j;
    strcpy(temp,str[r]);
    for(j=p;j<r;j++)
        if(compare(str[j],temp)<0)
            exchange(str[++i],str[j]);
    exchange(str[i+1],str[r]);
    return i+1;
}

void QUICK_SORT(char str[][34],int p,int r)
{//对str数组的p到r部分排序
    int q;
    if(p<r)
    {
        q = PARTITION(str,p,r);
        QUICK_SORT(str,p,q-1);
        QUICK_SORT(str,q+1,r);
    }
}

void quick(int n)
{
    int i;
    FILE *input,*output,*time;
    char *out_file1 = "../output\\quick_sort\\result_";
    char *in_file = "../input\\input_strings.txt";
    char *time_file = "../output\\quick_sort\\time.txt";
    char *out_file;
    char size[5];
    LARGE_INTEGER t1,t2;
    float timeused;
    out_file =(char*)malloc(strlen(out_file1)+10);
    if(!out_file)
    {
        printf("Malloc Filed!");
        exit(0);
    }
    itoa(n,size,10);
    strcpy(out_file,out_file1);
    strcat(out_file,size);
    strcat(out_file,".txt");
    input=fopen(in_file,"r");
    if(!input)
    {
        printf("Failed to open the input file!");
        exit (0);
    }
    output=fopen(out_file,"w");
    time=fopen(time_file,"a");

    printf("\nsize=2^%-2d   Please wait...",n);
    for(i=1;i<(1<<n)+1;i++)
        fgets(str[i],34,input);
    QueryPerformanceCounter(&t1);
    QUICK_SORT(str,1,1<<n);
    QueryPerformanceCounter(&t2);
    for(i=1;i<(1<<n)+1;i++)
        fputs(str[i],output);
    timeused=(float)(t2.QuadPart-t1.QuadPart)/(float)freq.QuadPart;
    printf("Finished    time=%f us   ",timeused*1000000);
    fseek(time,0,SEEK_END);
    fprintf(time,"size=2^%-2d    time=%f us\n",n,timeused*1000000);

    fclose(input);
    fclose(output);
    fclose(time);
}

int main()
{
    FILE *time_out;
    QueryPerformanceFrequency(&freq);

    printf("INSERTION_SORT");
    time_out=fopen("../output//insert_sort//time.txt","w");
    fputs("INSERTION_SORT\n",time_out);
    fclose(time_out);
    insert(2);
    insert(5);
    insert(8);
    insert(11);
    insert(14);
  //  insert(17);

    printf("\nHEAP_SORT");
    time_out=fopen("../output//heap_sort//time.txt","w");
    fputs("HEAP_SORT\n",time_out);
    fclose(time_out);
    heap(2);
    heap(5);
    heap(8);
    heap(11);
    heap(14);
    heap(17);

    printf("\nMERGE_SORT");
    time_out=fopen("../output//merge_sort//time.txt","w");
    fputs("MERGE_SORT\n",time_out);
    fclose(time_out);
    merge(2);
    merge(5);
    merge(8);
    merge(11);
    merge(14);
    merge(17);

    printf("\nQUICK_SORT");
    time_out=fopen("../output//quick_sort//time.txt","w");
    fputs("QUICK_SORT\n",time_out);
    fclose(time_out);
    quick(2);
    quick(5);
    quick(8);
    quick(11);
    quick(14);
    quick(17);

    return 0;
}
