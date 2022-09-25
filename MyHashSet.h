//
// Created by ASUS on 2022/9/14.
//

#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYHASHSET_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYHASHSET_H
#include "MyType.h"

//----------------------------HashSet---------------------------
extern struct hashMapForSet;

typedef struct set {
    //实现CommonStl接口
    Version* version;//记录容器版本
    TYPE_OF_SET*(*toArr)(struct set*);
    int(*getSize)(struct set*);
    int(*getEleSize)();


    //属性
    struct hashMapForSet* map;
    //方法
    //判空和获取容器大小
    int(*isEmpty)(struct set*);
    //增加元素
    int(*add)(struct set*, TYPE_OF_SET);
    int(*addALl)(struct set*, TYPE_OF_SET*, int);
    //删除元素
    int(*del)(struct set*, TYPE_OF_SET);
    //清空与销毁元素
    void(*clear)(struct set*);
    void(*destroy)(struct set**);
    //判断某元素是否存在
    int(*contains)(struct set*, TYPE_OF_SET);
    //设置自定义类型的比较器
    void(*setCmp)(struct set*, int(*)(TYPE_OF_SET, TYPE_OF_SET));
    //设置自定义类型的hash函数
    void(*setHashFun)(struct set*, long long (*)(TYPE_OF_SET));
    //打印函数组
    void(*printFun)(TYPE_OF_SET);
    void(*print)(struct set*, TYPE_OF_SET);
    void(*printAll)(struct set*);

    //获取迭代器
    Iterator*(*getIterator)(struct set*);
}HashSet;

extern HashSet* iniHashSet();
//----------------------------HashSet---------------------------
#endif //DATA_STRUCTURE_AND_ALGORITHM_MYHASHSET_H
