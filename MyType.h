//
// Created by ASUS on 2022/9/14.
//

#ifndef DATA_STRUCTURE_AND_ALGORITHM_MYTYPE_H
#define DATA_STRUCTURE_AND_ALGORITHM_MYTYPE_H
#include <stdlib.h>
#include <stdio.h>
#include "MyIterator.h"
#define NULL 0
/**
 *自定义类型声明区
 */
//--------Custom type declaration area------------------------
extern struct car;
typedef struct car car;
extern struct Huffman;
typedef struct Huffman Huffman;
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
#define TYPE_OF_STACK car*
//-------------------------------

/**
 * MyQueue
 */

//-------------------------------
//char* 类型和 自定义类型需要重写 printFun
#define TYPE_OF_QUEUE car*
//-------------------------------

/**
 * MyHashMap
 */
//-------------------------------
//char* 类型和自定义类型， 或 keyType 为自定义类型时 需要重写函数 cmpOfKey, cmpOfValue, hashFun, printFun
#define CAPACITY 100
#define keyType int //定义键值类型 ,默认为 int
#define eleType int  //定义元素类型, 默认为 int
//-------------------------------

/**
 * MyTreeMap
 */
//-------------------------------
//char*类型 和 自定义类型需要重写函数 printFun , cmp
#define keyTreeMap int //定义键值类型， 默认为 int
#define eleTreeMap int //定义元素类型 默认为int

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
#define TYPE_OF_PRIORITY_QUEUE Huffman*

//-------------------------------
//----------------------泛型声明区----------------------------

#endif //DATA_STRUCTURE_AND_ALGORITHM_MYTYPE_H
