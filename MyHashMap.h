//
// Created by ASUS on 2022/9/14.
//
#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYHASHMAP_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYHASHMAP_H

#include "MyType.h"

/**
 * 自定义HashMap
 */
//----------------------------HashMap---------------------------
/**
 * 节点结构体
 */
//------------------hashNode---------------------
//存储元素和键值的节点
typedef struct hashNode {
    keyType key;//键值
    eleType e;//存储元素
    struct hashNode* next; //后继
}hashNode , *hashNodePtr, *Entry;
//-----------------hashNode----------------------

/**
 *  哈希表结构体
 */
typedef struct hash {
    //实现CommonStl接口
    Version* version;//记录容器版本
    Entry*(*toArr)(struct hash*);
    int(*getSize)(struct hash*);
    int(*getEleSize)();

    //属性
    long long  list_len;//哈希表主链的大小
    int size;//哈希表当前的大小
    hashNodePtr* list;//存储哈希节点的指针的链表

    //方法
    //增
    int(*add)(struct hash*, keyType, eleType);
    int(*addAll)(struct hash*, keyType*, eleType*, int);

    //删
    int(*delByKey)(struct hash*, keyType);
    int(*delByValue)(struct hash*, eleType);

    //改
    int(*setValue)(struct hash* , keyType, eleType);

    //查
    eleType(*getValue)(struct hash*, keyType);

    //是否存在键或值
    int(*containsKey)(struct hash*, keyType);
    int(*containsValue)(struct hash*, eleType);

    //是否为空
    int(*isEmpty)(struct hash*);

    //键和值的比较器
    int(*cmpOfKey)(keyType, keyType);
    int(*cmpOfValue)(eleType, eleType);

    //hash函数
    long long (*hashFun)(keyType);

    //打印函数
    void(*printFun)(keyType, eleType);
    void(*print)(struct hash*, keyType);
    void(*printAll)(struct hash*);

    //转为键数组和值数组
    keyType*(*toKeyArr)(struct hash*);
    eleType*(*toValArr)(struct hash*);

    //清空与销毁
    void(*clear)(struct hash*);
    void(*destroy)(struct hash**);
    //获取当前存储大小 与 哈希表长度
    long long (*getListLen)(struct hash*);

    //对值进行遍历执行指定函数
    void(*exec)(struct hash*, void(*)(eleType*));

    //获取迭代器
    Iterator*(*getIterator)(struct hash*);
} hash, HashMap;

extern HashMap* iniHashMap();
//----------------------------HashMap---------------------------

#endif //DATA_STRUCTURE_AND_ALGORITHM_MYHASHMAP_H
