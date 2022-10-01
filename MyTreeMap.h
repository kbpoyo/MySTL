//
// Created by ASUS on 2022/9/30.
//

#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYTREEMAP_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYTREEMAP_H
#include "MyType.h"

static const int RED = 1;
static const int BLACK = 0;

/**
 * 定义抽象数据类型
 */
typedef struct node {
    keyTreeMap key;
    eleTreeMap ele;

    struct node* left;
    struct node* right;

    int N;//以当前节点为根节点的树的大小
    int color; //指向当前节点的链接颜色
} Node;

typedef struct entryOfTree {
    keyTreeMap key;
    eleTreeMap ele;
} *EntryOfTree;
typedef struct treeMap {
    //实现CommonStl接口
    Version* version;//记录容器版本
    EntryOfTree*(*toArr)(struct treeMap*);
    int(*getSize)(struct treeMap*);
    int(*getEleSize)();

    //属性
    Node* root;

    //方法
    //自定义比较器
    int(*cmp)(keyTreeMap, keyTreeMap);

    int(*isEmpty)(struct treeMap*);

    //插入节点
    void(*put)(struct treeMap*, keyTreeMap, eleTreeMap);

    //查找
    eleTreeMap(*get)(struct treeMap*, keyTreeMap);

    //返回最大最小键值
    keyTreeMap(*min)(struct treeMap*);
    keyTreeMap(*max)(struct treeMap*);


    //判断是否含有该键值
    int(*contains)(struct treeMap*, keyTreeMap);

    //删除
    void(*deleteMin)(struct treeMap*);
    void(*deleteMax)(struct treeMap*);
    void(*delete)(struct treeMap*, keyTreeMap);

     //向下取整，返回小于等于 key 的最大键
    keyTreeMap(*floor)(struct treeMap*, keyTreeMap);
    //向上取整，返回大于等于 key 的最大键
    keyTreeMap(*ceiling)(struct treeMap*, keyTreeMap);

    //返回小于key的键的数量，即当前key在树中的排名（下标从0开始，类似数组）
    int(*rank)(struct treeMap*, keyTreeMap);
    //返回排名为k的键
    keyTreeMap(*select)(struct treeMap*, int k);

   //返回键值在[lo,hi]之间的节点数量
    int(*keyInRangeSize)(struct treeMap*, keyTreeMap lo, keyTreeMap hi);

    //返回当前树高
    int(*getHeight)(struct treeMap*);

    //清空与销毁
    void(*clear)(struct treeMap*);
    void(*destroy)(struct treeMap**);


    //获取迭代器
    Iterator*(*getIterator)(struct treeMap*);
    //范围查询
    Iterator*(*getEntrysIterator)(struct treeMap*, keyTreeMap lo, keyTreeMap hi);


    //打印函数组
    void(*printFun)(keyTreeMap, eleTreeMap);
    void(*print)(struct treeMap*, keyTreeMap, eleTreeMap);
    void(*printAll)(struct treeMap*);

    //获取黑高
    int(*getBlackHeight)(struct treeMap*);

    //校验是否为红黑树
    int(*isColorRule)(struct treeMap*);//检验是否符合颜色规则
    int(*isOrderly)(struct treeMap*);//检验是否有序

}TreeMap;

extern TreeMap* iniTreeMap();
#endif //DATA_STRUCTURE_AND_ALGORITHM_MYTREEMAP_H
