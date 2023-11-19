#include<stdio.h>
#include<stdlib.h>

int global_var_data = 10;//全局变量存储在数据段
int global_var_bss;
int main(){
    static int local_static_var = 20;//初始化的静态局部变量存储在data段上
    static int local_var_bss;//未初始化的静态局部变量存储在bss段上
    int local_var=40;
    int *ptr;
    ptr =(int*)malloc(sizeof(int));//动态分配内存
    if(ptr==NULL)
    {
        printf("Memory not allocated.\n");
        exit(0);
    }
    *ptr = 30;//为堆上的数据赋值

    //打印变量
    printf("Value of local_var_bss is (bss sector): %d\n",local_var_bss);
    printf("Value of local_var is (stack): %d\n",local_var);
    printf("Value of local_static_var is (data sector): %d\n",local_static_var);
    printf("Value of global_var is (data sector): %d\n",global_var_data);
    printf("Value of global_var is (bss): %d\n",global_var_bss);
    printf("Value of *ptr (heap sector)is : %d\n",*ptr);
  
    //释放内存
    free(ptr);
    return 0;
}
