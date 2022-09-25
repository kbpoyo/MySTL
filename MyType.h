//
// Created by ASUS on 2022/9/14.
//

#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYTYPE_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYTYPE_H
#include <stdlib.h>
#include <stdio.h>
#define NULL 0
/**
 *自定义类型声明区
 */
//--------Custom type declaration area------------------------
extern struct a;
typedef struct a A;
//--------Custom type declaration area------------------------


/**
 * 泛型声明区
 */
//----------------------泛型声明区----------------------------

/**
 * MyList
 */
//-------------------------------
//char* 类型和 自定义类型需要重写 printFun， cmp 函数
#define TYPE_OF_LIST int
//-------------------------------

/**
 * MyStack
 */

//-------------------------------
//char* 类型和 自定义类型需要重写 printFun
#define TYPE_OF_STACK int
//-------------------------------

/**
 * MyQueue
 */

//-------------------------------
//char* 类型和 自定义类型需要重写 printFun
#define TYPE_OF_QUEUE int
//-------------------------------

/**
 * MyHashMap
 */
//-------------------------------
//eleType 为 char* 类型和自定义类型， 或 keyType 为自定义类型时 需要重写函数 cmpOfKey, cmpOfValue, hashFun, printFun
#define CAPACITY 100
#define keyType int //定义键值类型 ,默认为 char*
#define eleType int  //定义元素类型, 默认为 int
//-------------------------------

/**
 * MyHashSet
 */
//-------------------------------
//char*类型 和 自定义类型需要重写函数 printFun , 并调用setCmp, setHashFun
#define CAPACITY_OF_SET 100
#define TYPE_OF_SET int
//-------------------------------


/**
 * MyPriorityQueue
 */

//-------------------------------
//char* 和自定义类型需重写 cmp和 printFun函数
#define CAPACITY_OF_PRIORITY_QUEUE 10
#define TYPE_OF_PRIORITY_QUEUE int

//-------------------------------
//----------------------泛型声明区----------------------------

/**
 * 通用方法
 */

//-------------------通用方法-------------------------------
//输出异常信息
static void printErr(const char* err) {
    printf("\nerror: %s\n", err);
}

//-------------------通用方法-------------------------------


/**
 * 迭代器
 */
//-----------------------迭代器------------------------------------
//迭代器不支持float类型， 其访问double类型时，用long long类型来转换

//记录迭代器版本状态
typedef struct version {
    void * arrForIterator;
    int optCounts;//记录自迭代器初始化后 又进行的增删操作
    int size;
    int size_t;
    int counts;//引用计数, 销毁同一个状态的最后一个迭代器时，将版本进行销毁
}Version;

//获取迭代器后，若容器进行增删操作，则该迭代器版本失效
typedef struct iterator {
    int(*hasNext)(struct iterator*);
    void*(*next)(struct iterator*);
    void(*destroy)(struct iterator**);
    int index;
    Version* version;
}Iterator;

//通用stl，相当于所有容器必须实现的接口
typedef struct commonStl {
    Version *version;
    void*(*toArr)(struct commonStl*);
    int(*getSize)(struct commonStl*);
    int(*getEleSize)();
}CommonStl;

/**
 * 迭代器方法
 */
//更改容器和迭代器版本
static void changeVersion(void * STL) {
    CommonStl* stl = (CommonStl*)STL;
    if (stl->version != NULL) {
        stl->version->optCounts++;
        stl->version = NULL;
    }
}

//初始化容器版本
static Version* iniVersion(void * STL) {
    CommonStl* stl = (CommonStl*)STL;
    Version* version = (Version*) malloc(sizeof(Version));
    version->optCounts = 0;
    version->arrForIterator = stl->toArr(stl);
    version->size = stl->getSize(stl);
    version->counts = 0;
    version->size_t = stl->getEleSize();
    return version;
}

//销毁迭代器
static void destroyIterator(Iterator** iteratorPtr) {
    Iterator* iterator = *iteratorPtr;
    if (iterator != NULL) {
        iterator->version->counts--;
        if (iterator->version->counts == 0) {
            free(iterator->version->arrForIterator);
            free(iterator->version);
        }
        free(iterator);
        *iteratorPtr = NULL;
    }
}


//判断是否有可以访问的元素
static int hasNext(Iterator* iterator) {
    Version* version = iterator->version;
    int flag = iterator->index < version->size;
    if (version->optCounts == 0) {//容器状态未更改，迭代器有效
        if (!flag) destroyIterator(&iterator);//迭代器已访问完毕，自行摧毁
        return flag;
    }
    else {
        destroyIterator(&iterator);
        printErr("iterator is out");
        return 0;
    };//容器状态改变，迭代器失效
}

//获取当前元素
static void* next(Iterator* iterator) {
    Version* version = iterator->version;

    if (iterator->hasNext(iterator)) {
        if (version->size_t == 4)return ((int*)version->arrForIterator)[iterator->index++];
        else return ((void **)version->arrForIterator)[iterator->index++];
    }

    return NULL;
}

//初始化迭代器
static Iterator* iniIterator(void * STL) {
    CommonStl* stl = (CommonStl*)STL;

    Iterator* iterator = (Iterator*)malloc(sizeof (Iterator));
    iterator->hasNext = hasNext;
    iterator->next = next;
    iterator->index = 0;
    iterator->destroy = destroyIterator;

    //创建容器当前版本， 并用当前迭代器进行记录
    if (stl->version == NULL) stl->version = iniVersion(stl);
    iterator->version = stl->version;
    //引用计数，记录迭代器的数量
    stl->version->counts++;

    return iterator;
}

//-----------------------迭代器------------------------------------

#endif //DATA_STRUCTURE_AND_ALGORITHM_MYTYPE_H
