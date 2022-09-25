//
// Created by ASUS on 2022/9/18.
//

#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYPRIORITYQUEUE_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYPRIORITYQUEUE_H
#include "MyType.h"

typedef struct priorityQueue{
    //实现CommonStl接口，墙砖为CommonStl后，方法地址与CommonStl对齐
    Version* version;//记录容器版本
    TYPE_OF_PRIORITY_QUEUE*(*toArr)(struct priorityQueue*);
    int(*getSize)(struct priorityQueue*);
    int(*getEleSize)();

    //属性
    int n;//队列元素个数
    int capacity;//队列数组容量
    TYPE_OF_PRIORITY_QUEUE* objs;//存储队列的数组

    //方法
    int(*cmp)(TYPE_OF_PRIORITY_QUEUE, TYPE_OF_PRIORITY_QUEUE);//自定义类型比较器

    void(*offer)(struct priorityQueue*, TYPE_OF_PRIORITY_QUEUE);//添加元素入队列
    TYPE_OF_PRIORITY_QUEUE(*peek)(struct priorityQueue*);//访问但不弹出队头元素
    TYPE_OF_PRIORITY_QUEUE(*poll)(struct priorityQueue*);//弹出并返回队头元素
    int(*contains)(struct priorityQueue*, TYPE_OF_PRIORITY_QUEUE);//判断是否包含此元素
    void(*del)(struct priorityQueue*, TYPE_OF_PRIORITY_QUEUE);//删除元素
    void(*change)(struct priorityQueue*, TYPE_OF_PRIORITY_QUEUE oldEle, TYPE_OF_PRIORITY_QUEUE newEle);//改变元素
    void(*addAll)(struct priorityQueue*, TYPE_OF_PRIORITY_QUEUE* arr, int size);//添加全部元素
    void(*delAll)(struct priorityQueue*, TYPE_OF_PRIORITY_QUEUE);

    //打印函数
    void(*printFun)(TYPE_OF_PRIORITY_QUEUE);
    void(*print)(struct priorityQueue*, TYPE_OF_PRIORITY_QUEUE);
    void(*printAll)(struct priorityQueue*);

    //判空
    int(*isEmpty)(struct priorityQueue*);

    //清空与销毁
    void(*clear)(struct priorityQueue*);
    void(*destroy)(struct priorityQueue**);



    //获取迭代器
    Iterator*(*getIterator)(struct priorityQueue*);

}PriorityQueue;

extern PriorityQueue * iniPriorityQueue();

#endif //DATA_STRUCTURE_AND_ALGORITHM_MYPRIORITYQUEUE_H
