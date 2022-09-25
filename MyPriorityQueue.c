//
// Created by ASUS on 2022/9/18.
//
#include "MyPriorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
/**
* 内部方法
*/
//默认比较函数
static int cmp(TYPE_OF_PRIORITY_QUEUE ele1, TYPE_OF_PRIORITY_QUEUE ele2) {
    long long e1 = (long long )ele1;
    long long e2 = (long long )ele2;

    if (e1 == e2) return 0;
    if (e1 < e2) return -1;
    return 1;
}

//判断是否小于
static int less(PriorityQueue* queue, int k1, int k2) {
    return queue->cmp(queue->objs[k1], queue->objs[k2]) < 0;
}

//交换对应元素
static void exchange(PriorityQueue* queue, int k1, int k2) {
    TYPE_OF_PRIORITY_QUEUE temp = queue->objs[k1];
    queue->objs[k1] = queue->objs[k2];
    queue->objs[k2] = temp;
}


//在二叉堆中上浮
static void swim(PriorityQueue* queue, int index) {
    while (index > 1) {
        int father = index / 2;//父节点的索引
        if (!less(queue, index, father)) return;
        exchange(queue, index, father);
        index = father;
    }

}

//在二叉堆中下沉
static void sink(PriorityQueue* queue, int index) {
    while (index*2 < queue->n) {//判断该节点是否有左子节点，因为是满二叉堆，所以必定先有左子节点
        int minChildren = index*2;
        //如果存在右子节点，则取左右子节点中最小的一个进行交换
        if (minChildren < queue->n && less(queue, minChildren + 1, minChildren)) minChildren++;
        if (!less(queue, minChildren, index)) return;
        exchange(queue, minChildren, index);
        index = minChildren;
    }
}


//返回对应元素在队列数组中的索引 找到返回 索引 ，否则返回 -1
static int indexOf(PriorityQueue* queue, TYPE_OF_PRIORITY_QUEUE ele) {
    TYPE_OF_PRIORITY_QUEUE* arr = queue->objs;
    int i;
    for (i = 1; i <= queue->n; i++) {
        if (queue->cmp(arr[i], ele) == 0) {
            return i;
        }
    }
    return -1;
}

//数组容量动态变化
static void changeSize(PriorityQueue* queue) {

    if (queue->n > queue->capacity / 2) {
        queue->capacity *= 2;
        queue->objs = (TYPE_OF_PRIORITY_QUEUE*)realloc(queue->objs, sizeof(TYPE_OF_PRIORITY_QUEUE)*queue->capacity);
    } else if (queue->capacity > CAPACITY_OF_PRIORITY_QUEUE && queue->n < queue->capacity / 4) {
        queue->capacity /= 2;
        queue->objs = (TYPE_OF_PRIORITY_QUEUE*)realloc(queue->objs, sizeof(TYPE_OF_PRIORITY_QUEUE)*queue->capacity);
    }


}

/**
 * 外部方法
 */
//获取队列元素个数
static int getSize(PriorityQueue* queue) {
    return queue->n;
}

//判空
static int isEmpty(PriorityQueue* queue) {
    return queue->n == 0;
}

//判断是否包含元素
static int contains(PriorityQueue* queue, TYPE_OF_PRIORITY_QUEUE ele) {
    int index = indexOf(queue, ele);
    return index != -1;
}

//入队列
static void offer(PriorityQueue* queue, TYPE_OF_PRIORITY_QUEUE ele) {
    queue->objs[++queue->n] = ele;//从下标1开始存储，便于利用二叉树的性质
    swim(queue, queue->n);//上浮，以保持堆有序
    changeSize(queue);
    changeVersion(queue);//改变容器版本
}

//添加全部元素
static void addAll(PriorityQueue* queue , TYPE_OF_PRIORITY_QUEUE* arr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        offer(queue, arr[i]);
    }
}
//删除一个元素
static void del(PriorityQueue* queue, TYPE_OF_PRIORITY_QUEUE ele) {
    if (!contains(queue, ele)) {
        printErr("queue don't contain this value!");
        return;
    }

    int index = indexOf(queue, ele);
    exchange(queue, index, queue->n--);
    sink(queue, index);
    queue->objs[queue->n + 1] = NULL;

    changeSize(queue);
    changeVersion(queue);//改变容器版本
}

//删除全部元素
static void delAll(PriorityQueue* queue, TYPE_OF_PRIORITY_QUEUE ele) {
    while (contains(queue, ele)) del(queue, ele);
}

//改变一个元素
static void change(PriorityQueue* queue, TYPE_OF_PRIORITY_QUEUE oldEle, TYPE_OF_PRIORITY_QUEUE newEle) {
    int index = indexOf(queue, oldEle);
    if (index == -1) {
        printErr("there is no the oldEle!");
        return;
    }

    queue->objs[index] = newEle;
    //先上浮再下沉，保持堆有序
    swim(queue, index);
    sink(queue, index);
    changeVersion(queue);//改变容器版本

}

//访问但不删除队列头元素
static TYPE_OF_PRIORITY_QUEUE peek(PriorityQueue* queue) {
    if (queue->isEmpty(queue)) {
        printErr("queue is empty!");
        return NULL;
    }
    return queue->objs[1];
}


//弹出队头元素,即当前最小元素
static TYPE_OF_PRIORITY_QUEUE poll(PriorityQueue* queue) {
    TYPE_OF_PRIORITY_QUEUE rst = peek(queue);
    if (rst == NULL) {
        printErr("fail poll!");
        return rst;
    }

    del(queue, rst);

    return rst;
}

//转为数组
static TYPE_OF_PRIORITY_QUEUE* toArr(PriorityQueue* queue) {
    TYPE_OF_PRIORITY_QUEUE* arr =  (TYPE_OF_PRIORITY_QUEUE*) malloc(sizeof(TYPE_OF_PRIORITY_QUEUE)*(queue->n + 1));
    int i;
    for (i = 0; i < queue->n; i++) {
        arr[i] = queue->objs[i+1];
    }

    return arr;
}


//打印函数
static void printFun(TYPE_OF_PRIORITY_QUEUE ele) {
    printf("%lld", (long long )ele);
}

static void print(PriorityQueue* queue, TYPE_OF_PRIORITY_QUEUE ele) {
    printf("\n{\n");
    queue->printFun(ele);
    printf("\n}");
}

static void printAll(PriorityQueue* queue) {
    int Size = getSize(queue);
    printf("{PriorityQueue:\tcounts==%d\n", Size);
    int i;
    for (i = 1; i <= Size; i++) {
        printf("\t");
        queue->printFun(queue->objs[i]);
        printf("\n");
    }

    printf("}\n");

}


//清空与销毁
static void clear(PriorityQueue* queue) {
    queue->n = 0;
    queue->capacity = CAPACITY_OF_PRIORITY_QUEUE;
    free(queue->objs);
    queue->objs = (TYPE_OF_PRIORITY_QUEUE*)malloc(sizeof(TYPE_OF_PRIORITY_QUEUE)*queue->capacity);
    changeVersion(queue);
}

static void destroy(PriorityQueue** pQueue) {
    changeVersion(*pQueue);
    free((*pQueue)->objs);
    free(*pQueue);
    *pQueue = NULL;
}


//获取迭代器所迭代元素大小
static int getEleSize() {
    return sizeof (TYPE_OF_PRIORITY_QUEUE);
}

//获取迭代器
static Iterator* getIterator(PriorityQueue* queue) {
    return iniIterator(queue);
}
//-----------------------------------------------------------------------



//初始一个优先队列
PriorityQueue* iniPriorityQueue() {
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    queue->version = NULL;
    queue->capacity = CAPACITY_OF_PRIORITY_QUEUE;
    queue->n = 0;
    queue->objs = (TYPE_OF_PRIORITY_QUEUE*) malloc(sizeof(TYPE_OF_PRIORITY_QUEUE)*queue->capacity);

    queue->cmp = cmp;
    queue->getSize = getSize;
    queue->offer = offer;
    queue->del = del;
    queue->contains = contains;
    queue->peek = peek;
    queue->poll = poll;
    queue->change = change;
    queue->isEmpty = isEmpty;

    queue->printFun = printFun;
    queue->print = print;
    queue->printAll = printAll;

    queue->clear = clear;
    queue->destroy = destroy;

    queue->toArr = toArr;
    queue->addAll = addAll;
    queue->delAll = delAll;
    queue->getEleSize = getEleSize;

    queue->getIterator = getIterator;

    return queue;

}