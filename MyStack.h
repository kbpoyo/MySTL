//
// Created by ASUS on 2022/9/14.
//

#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYSTACK_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYSTACK_H
#include "MyType.h"

//----------------------------Stack---------------------------
typedef struct nodeOfStack {
	TYPE_OF_STACK  ele;
	struct nodeOfStack* next;
}NodeOfStack;


typedef struct stack {
    //实现CommonStl接口
    Version* version;//记录容器版本
    TYPE_OF_STACK*(*toArr)(struct stack*);
    int(*getSize)(struct stack*);
    int(*getEleSize)();

    //属性
	int size;
	struct nodeOfStack* first;
    //方法
    //判空与获取容器大小
	int(*isEmpty)(struct stack*);
    //压栈和出栈
	TYPE_OF_STACK (*push)(struct stack*, TYPE_OF_STACK );
	TYPE_OF_STACK (*pop)(struct stack*);
    //访问栈顶元素，但不出栈
	TYPE_OF_STACK (*peek)(struct stack*);
    //遍历执行指定函数
	void(*exec)(struct stack*, void(*fun)(TYPE_OF_STACK *));
    //打印函数组
    void(*printFun)(TYPE_OF_STACK);
    void(*print)(struct stack*, TYPE_OF_STACK);
    void(*printAll)(struct stack*);
    //获取迭代器
    Iterator*(*getIterator)(struct stack*);

    //清空与销毁
    void(*clear)(struct stack*);
    void(*destroy)(struct stack**);

}Stack;


extern Stack* iniStack();

//----------------------------Stack--------------------------

#endif //DATA_STRUCTURE_AND_ALGORITHM_MYSTACK_H
