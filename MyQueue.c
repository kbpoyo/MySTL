#include"MyQueue.h"
#include<stdio.h>
#include<stdlib.h>
static int isEmpty(Queue* myQueue) {
	return myQueue->size == 0;
}

static TYPE_OF_QUEUE offer(Queue* myQueue, TYPE_OF_QUEUE ele) {
	NodeOfQueue* node = (NodeOfQueue*) malloc(sizeof(NodeOfQueue));
	node->ele = ele;
	node->next = NULL;
	if (myQueue->isEmpty(myQueue)) {
		myQueue->first = node;
		myQueue->last = node;
	} else {
		myQueue->last->next = node;
		myQueue->last = node;
	}
	
	myQueue->size++;
    changeVersion(myQueue);
	
	return ele;
}

static TYPE_OF_QUEUE peek(Queue* myQueue) {
	if (myQueue->isEmpty(myQueue)) {
		printErr("my queue is empty");
		return NULL;
	}
	
	return myQueue->first->ele;
}

static TYPE_OF_QUEUE poll(Queue* myQueue) {
	
	TYPE_OF_QUEUE ele = myQueue->peek(myQueue);
	NodeOfQueue* oldNode = myQueue->first;
	myQueue->first = oldNode->next;
	free(oldNode);
	myQueue->size--;
    changeVersion(myQueue);
	
	return ele;
}

static void exec(Queue* myQueue, void(*fun)(TYPE_OF_QUEUE*)) {
	NodeOfQueue* start = myQueue->first;
	int size = myQueue->size;
	int i;
	for (i = 0; i < size && start != NULL; i++) {
		TYPE_OF_QUEUE* ele = &start->ele;
		fun(ele);
		start = start->next; 
	}
	
}

//获取存储数量
static int getSize(Queue* queue) {
    return queue->size;
}

//转为数组
static TYPE_OF_QUEUE* toArr(Queue* queue) {
    int size = queue->getSize(queue);
    if (size == 0) return NULL;

    TYPE_OF_QUEUE* arr = (TYPE_OF_QUEUE*) malloc(sizeof (TYPE_OF_QUEUE)*(size + 1));

    NodeOfQueue* ptr = queue->first;

    int i;
    for (i = 0; i < size && ptr != NULL; i++) {
        arr[i] = ptr->ele;
        ptr = ptr->next;
    }

    return arr;
}


/**
 * 打印函数
 */
static void printFun(TYPE_OF_QUEUE ele) {
    printf("%lld", (long long )ele);
}

static void print(Queue* queue, TYPE_OF_QUEUE ele) {
    printf("{");
    queue->printFun(ele);
    printf("}\n");
}

static void printAll(Queue* queue) {
    int size = queue->getSize(queue);
    printf("{Queue:\tcounts==%d\n", size);
    TYPE_OF_QUEUE* arr = queue->toArr(queue);
    if (arr != NULL) {
        int i;
        for (i = 0; i < size; i++) {
            printf("\t");
            queue->printFun(arr[i]);
            printf("\n");
        }
    }
    printf("}\n");
    free(arr);
}

/**
 * 清空与销毁
 */
//----------------------------------
static void clear(Queue* queue) {
    int size = queue->getSize(queue);
    if (size == 0) return;
    NodeOfQueue* ptr = queue->first;
    int i;
    for (i = 0; i < size && ptr != NULL; i++) {
        NodeOfQueue* temp = ptr;
        ptr = ptr->next;
        free(temp);
    }

    queue->first = queue->last = NULL;
    queue->size = 0;
    changeVersion(queue);
}

static void destroy(Queue** queue) {
    (*queue)->clear(*queue);
    free(*queue);
    *queue = NULL;
}

//----------------------------------
static int getEleSize() {
    return sizeof (TYPE_OF_QUEUE);
}

//获取迭代器
static Iterator* getIterator(Queue* queue) {
    return iniIterator(queue);
}
//-----------------------------------------------------------------------

Queue* iniQueue() {
	
	Queue* myQueue = (Queue*) malloc (sizeof(Queue));
    myQueue->version = NULL;
	myQueue->first = myQueue->last = NULL;
	myQueue->size = 0;
	myQueue->isEmpty = isEmpty;
	myQueue->peek = peek;
	myQueue->offer = offer;
	myQueue->poll = poll;
	myQueue->exec = exec;

    myQueue->getSize = getSize;
    myQueue->toArr = toArr;

    myQueue->printFun = printFun;
    myQueue->print = print;
    myQueue->printAll = printAll;

    myQueue->getIterator = getIterator;

    myQueue->clear = clear;
    myQueue->destroy = destroy;

    myQueue->getEleSize = getEleSize;
	return myQueue;
}




