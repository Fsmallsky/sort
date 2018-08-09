#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>

//�������������̫��ֻ�ܶ���Ϊȫ�ֱ���
int num[(1<<17)+2];         //���ڴ洢���ļ��ж��������
int result[(1<<17)+2];

static LARGE_INTEGER freq;  //��ʱʱ�õ��Ļ���Ƶ��

void exchange(int *num1,int *num2)  //�����ַ��������ֵ�λ��
{
    int temp;
    temp = *(num1);
    *(num1) = *(num2);
    *(num2) = temp;
}

//ð������
void BUBBLE_SORT(int* num,int n)
{
    int i,j;
    for(i=n;i>1;i--)
    {
        for(j=1;j<n;j++)
        {
            if(num[j]>num[j+1])
                exchange(num+j,num+j+1);
        }
    }
}
void bubble(int n)
{
    int i;
    FILE *input,*output,*time;
    char *out_file1 = "../output//bubble_sort//result_";
    char *in_file = "../input//input_numbers.txt";
    char *time_file = "../output//bubble_sort//time.txt";
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
        fscanf(input,"%d",num+i);
    QueryPerformanceCounter(&t1);
    BUBBLE_SORT(num,1<<n);
    QueryPerformanceCounter(&t2);
    for(i=1;i<(1<<n)+1;i++)
        fprintf(output,"%d\n",num[i]);
    timeused=(float)(t2.QuadPart-t1.QuadPart)/(float)freq.QuadPart;
    printf("    Finished   time=%f us   ",timeused*1000000);
    fprintf(time,"size=2^%-2d   time=%f us\n",n,timeused*1000000);

    fclose(input);
    fclose(output);
    fclose(time);
}

//��������
int PARTITION(int *num,int p,int r)
{
    int temp;
    int i=p-1,j;
    temp = num[r];
    for(j=p;j<r;j++)
        if(num[j]<temp)
            exchange(num+(++i),num+j);
    exchange(num+i+1,num+r);
    return i+1;
}

void QUICK_SORT(int *num,int p,int r)
{
    int q;
    if(p<r)
    {
        q = PARTITION(num,p,r);
        QUICK_SORT(num,p,q-1);
        QUICK_SORT(num,q+1,r);
    }
}

void quick(int n)
{
    int i;
    FILE *input,*output,*time;
    char *out_file1 = "../output//quick_sort//result_";
    char *in_file = "../input//input_numbers.txt";
    char *time_file = "../output//quick_sort//time.txt";
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
        fscanf(input,"%d",num+i);
    QueryPerformanceCounter(&t1);
    QUICK_SORT(num,1,1<<n);
    QueryPerformanceCounter(&t2);
    for(i=1;i<(1<<n)+1;i++)
        fprintf(output,"%d\n",num[i]);
    timeused=(float)(t2.QuadPart-t1.QuadPart)/(float)freq.QuadPart;
    printf("    Finished   time=%f us   ",timeused*1000000);
    fprintf(time,"size=2^%-2d   time=%f us\n",n,timeused*1000000);

    fclose(input);
    fclose(output);
    fclose(time);
}

//��������
void RADIX_SORT(int *num,int d,int n)
{
    //����Ĵ��������Լ��źú�ķ������鶼Ϊnum
    //���ڻ���������ԭ�������㷨��������Ҫ��������result
    //dΪ����Ļ�����������ȡ10��nΪ����Ĺ�ģ
    int i,j,k,temp,m,C[10];
    float t;
    for(i=1;i<=d;i++)
    {//��Ҫ����һ���ȶ��������㷨���˴�ʹ�ü�������
        for(k=1;k<=n;k++)
            result[k] = num[k]; //�Ƚ��������ݿ�����ressult��num���ڷ���
        for(j=0;j<=9;j++)
            C[j] = 0;
        for(k=1;k<=n;k++)
        {
            for(t=result[k],m=i-1;m>0;m--)   t/=10;
            temp = (int)t%10;
            //�������ݲ������ݱ����������ݵ������iλ��10���ƣ���ͬ��
            //tempΪȡ�������ݵ������iλ
            C[temp] +=1;
        }
        for(j=1;j<=9;j++)
            C[j] += C[j-1];
        for(k=n;k>0;k--)
        {
            for(t=result[k],m=i-1;m>0;m--)   t/=10;
            temp = (int)t%10;
            num[C[temp]--] = result[k];
        }
    }
}

void radix(int n)
{
    int i;
    FILE *input,*output,*time;
    char *out_file1 = "../output//radix_sort//result_";
    char *in_file = "../input//input_numbers.txt";
    char *time_file = "../output//radix_sort//time.txt";
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
        fscanf(input,"%d",num+i);
    QueryPerformanceCounter(&t1);
    RADIX_SORT(num,5,1<<n);
    QueryPerformanceCounter(&t2);
    for(i=1;i<(1<<n)+1;i++)
        fprintf(output,"%d\n",num[i]);
    timeused=(float)(t2.QuadPart-t1.QuadPart)/(float)freq.QuadPart;
    printf("    Finished   time=%f us   ",timeused*1000000);
    fprintf(time,"size=2^%-2d   time=%f us\n",n,timeused*1000000);

    fclose(input);
    fclose(output);
    fclose(time);
}

//��������
void COUNTING_SORT(int *num,int *result,int k,int n)
{
    int i,j;
    int C[k+1];
    for(i=0;i<=k;i++)
        C[i]=0;
    for(j=1;j<=n;j++)
        C[num[j]] += 1;
    for(i=1;i<=k;i++)
        C[i] += C[i-1];
    for(j=n;j>0;j--)
        result[C[num[j]]--]=num[j];
}
void counting(int n)
{
    int i;
    FILE *input,*output,*time;
    char *out_file1 = "../output//counting_sort//result_";
    char *in_file = "../input//input_numbers.txt";
    char *time_file = "../output//counting_sort//time.txt";
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
        fscanf(input,"%d",num+i);
    QueryPerformanceCounter(&t1);
    COUNTING_SORT(num,result,65535,1<<n);
    QueryPerformanceCounter(&t2);
    for(i=1;i<(1<<n)+1;i++)
        fprintf(output,"%d\n",result[i]);
    timeused=(float)(t2.QuadPart-t1.QuadPart)/(float)freq.QuadPart;
    printf("   Finished   time=%f us   ",timeused*1000000);
    fprintf(time,"size=2^%-2d   time=%f us\n",n,timeused*1000000);

    fclose(input);
    fclose(output);
    fclose(time);
}

int main()
{
    FILE *time_out;
    QueryPerformanceFrequency(&freq);

    printf("BUBBLE_SORT");
    time_out=fopen("../output\\bubble_sort\\time.txt","w");
    fputs("BUBBLE_SORT\n",time_out);
    fclose(time_out);
    bubble(2);
    bubble(5);
    bubble(8);
    bubble(11);
    bubble(14);
    //bubble(17);

    printf("\nQUICK_SORT");
    time_out=fopen("../output\\quick_sort\\time.txt","w");
    fputs("QUICK_SORT\n",time_out);
    fclose(time_out);
    quick(2);
    quick(5);
    quick(8);
    quick(11);
    quick(14);
    quick(17);

    printf("\nRADIX_SORT");
    time_out=fopen("../output\\radix_sort\\time.txt","w");
    fputs("RADIX_SORT\n",time_out);
    fclose(time_out);
    radix(2);
    radix(5);
    radix(8);
    radix(11);
    radix(14);
    radix(17);

    printf("\nCOUNTING_SORT");
    time_out=fopen("../output\\counting_sort\\time.txt","w");
    fputs("COUNTING_SORT\n",time_out);
    fclose(time_out);
    counting(2);
    counting(5);
    counting(8);
    counting(11);
    counting(14);
    counting(17);
bubble(17);
    return 0;
}
