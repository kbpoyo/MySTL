//
// Created by ASUS on 2022/9/14.
//

#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYQUEUE_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYQUEUE_H
#include "MyType.h"
//----------------------------Queue---------------------------
typedef struct nodeOfQueue {
	TYPE_OF_QUEUE ele;
	struct nodeOfQueue* next;
}NodeOfQueue;

typedef struct queue {

    //实现CommonStl接口
    Version* version;//记录容器版本
    TYPE_OF_QUEUE*(*toArr)(struct queue*);
    int(*getSize)(struct queue*);
    int(*getEleSize)();


    //属性
	NodeOfQueue* first;
	NodeOfQueue* last;
	int size;
	//方法
    //判空和获取容器大小
    int(*isEmpty)(struct queue*);

    //进出队列，与访问对头元素，但不出队列
    TYPE_OF_QUEUE(*offer)(struct queue*, TYPE_OF_QUEUE ele);
    TYPE_OF_QUEUE(*poll)(struct queue*);
	TYPE_OF_QUEUE(*peek)(struct queue*);
	//遍历执行指定函数
    void(*exec)(struct queue*, void(*fun)(TYPE_OF_QUEUE*));

    //获取迭代器
    Iterator*(*getIterator)(struct queue*);
    //打印函数组
    void(*printFun)(TYPE_OF_QUEUE);
    void(*print)(struct queue*, TYPE_OF_QUEUE);
    void(*printAll)(struct queue*);

    //清空与销毁容器
    void(*clear)(struct queue*);
    void(*destroy)(struct queue**);


}Queue;


extern Queue* iniQueue();

//----------------------------Queue--------------------------
#endif //DATA_STRUCTURE_AND_ALGORITHM_MYQUEUE_H
