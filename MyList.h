//
// Created by ASUS on 2022/9/14.
//

#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYLIST_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYLIST_H
#include "MyType.h"


//---------------------------LinkedList------------------------------
typedef struct nodeForList {
    TYPE_OF_LIST ele;
    struct nodeForList* pre;
    struct nodeForList* next;
}NodeOfList;

typedef struct linkedList {
    //实现CommonStl接口
    Version* version;
    TYPE_OF_LIST*(*toArr)(struct linkedList*);
    int(*getSize)(struct linkedList*);
    int(*getEleSize)();

    //属性
    int size;
    NodeOfList* head;
    NodeOfList* last;
    //方法
    //增
    int(*add)(struct linkedList*, int, TYPE_OF_LIST);
    int(*addBack)(struct linkedList*, TYPE_OF_LIST);
    int(*addHead)(struct linkedList*, TYPE_OF_LIST);
    int(*addAll)(struct linkedList*, TYPE_OF_LIST*, int);

    //删
    TYPE_OF_LIST(*del)(struct linkedList*, int);
    TYPE_OF_LIST(*delHead)(struct linkedList*);
    TYPE_OF_LIST(*delBack)(struct linkedList*);
    TYPE_OF_LIST(*delVal)(struct linkedList*, TYPE_OF_LIST);
    TYPE_OF_LIST(*delValRe)(struct linkedList*, TYPE_OF_LIST);
    void (*delAll)(struct linkedList*, TYPE_OF_LIST);
    void (*delAllByArr)(struct linkedList*, TYPE_OF_LIST*, int);

    //改
    TYPE_OF_LIST(*set)(struct linkedList*, int, TYPE_OF_LIST);

    //查
    TYPE_OF_LIST(*get)(struct linkedList*, int);

    //判断是否存在
    int(*contains)(struct linkedList*, TYPE_OF_LIST);

    //遍历执行
    void(*exec)(struct linkedList*, void(*)(TYPE_OF_LIST*));

    //打印
    void(*printFun)(TYPE_OF_LIST);
    void(*print)(struct linkedList*, int index);
    void(*printAll)(struct linkedList*);

    //比较器
    int(*cmp)(TYPE_OF_LIST, TYPE_OF_LIST);



    //根据元素获取索引
    int(*indexOf)(struct linkedList*, TYPE_OF_LIST);
    int(*lastIndexOf)(struct linkedList*, TYPE_OF_LIST);

    //判空
    int(*isEmpty)(struct linkedList*);


    //排序
    void(*sort)(struct linkedList*);

    //清空与销毁
    void(*clear)(struct linkedList*);
    void(*destroy)(struct linkedList**);
    //获取迭代器
    Iterator*(*getIterator)(struct linkedList*);

}LinkedList;

extern LinkedList* iniLinkedList();
//---------------------------LinkedList------------------------------
#endif //DATA_STRUCTURE_AND_ALGORITHM_MYLIST_H
