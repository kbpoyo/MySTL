#include"MyStack.h"
#include<stdio.h>
#include<stdlib.h>

static int isEmpty(Stack* myStack) {
	return myStack->size == 0; 
}

static TYPE_OF_STACK  push(Stack* myStack, TYPE_OF_STACK  addEle) {
	NodeOfStack* newNode = (NodeOfStack*) malloc (sizeof(NodeOfStack));
	newNode->ele = addEle;
	newNode->next = myStack->first;
	myStack->first = newNode;
	myStack->size++;
    changeVersion(myStack);
	
	return addEle;
	
}

static TYPE_OF_STACK  peek(Stack* myStack) {
	if (myStack->isEmpty(myStack)) {
		printf("myStack is empty\n");
		return NULL;
	}
	
	return myStack->first->ele;
	
}

static TYPE_OF_STACK  pop(Stack* myStack) {

	TYPE_OF_STACK  ret = myStack->peek(myStack);
	NodeOfStack* oldNode = myStack->first;
	myStack->first = oldNode->next;
	myStack->size--;
    changeVersion(myStack);
	free(oldNode);

	return ret;
	
}

static void exec(Stack* myStack, void(*fun)(TYPE_OF_STACK *)) {
	NodeOfStack* start = myStack->first;
	int size = myStack->size;
	int i;
	for (i = 0; i < size && start != NULL; i++) {
		TYPE_OF_STACK * ele = &start->ele;
		fun(ele);
		start = start->next; 
	}
	
}


//获取大小
static int getSize(Stack* stack) {
    return stack->size;
}

//转为数组
static TYPE_OF_STACK* toArr(Stack* stack) {
    int size = stack->getSize(stack);
    if (size == 0) return NULL;
    NodeOfStack* ptr = stack->first;

    TYPE_OF_STACK* arr = (TYPE_OF_STACK*) malloc(sizeof(TYPE_OF_STACK)*(size + 1));
    int i;
    for (i = 0; i < size && ptr != NULL; i++) {
        arr[i] = ptr->ele;
        ptr = ptr->next;
    }

    return arr;
}

//清空与销毁
static void clear(Stack* stack) {
    int size = stack->getSize(stack);
    if (size == 0) return;
    NodeOfStack* ptr = stack->first;
    int i;
    for (i = 0; i < size && ptr != NULL; i++) {
        NodeOfStack* temp = ptr;
        ptr = ptr->next;
        free(temp);
    }

    changeVersion(stack);
    stack->first = NULL;
    stack->size = 0;
}

static void destroy(Stack** stack) {
    (*stack)->clear(*stack);
    free(*stack);
    *stack = NULL;
}


/**
 * 打印函数
 */
static void printFun(TYPE_OF_STACK ele) {
    printf("%lld", (long long )ele);
}

static void print(Stack* stack, TYPE_OF_STACK ele) {
    printf("{");
    stack->printFun(ele);
    printf("}\n");
}

static void printAll(Stack* stack) {
    int size = stack->getSize(stack);
    printf("{Stack:\tcounts==%d\n", size);
    TYPE_OF_STACK* arr = stack->toArr(stack);
    if (arr != NULL) {
        int i;
        for (i = 0; i < size; i++) {
            printf("\t");
            stack->printFun(arr[i]);
            printf("\n");
        }
    }
    printf("}\n");

    free(arr);

}

//获取迭代器所迭代的元素大小
static int getEleSize() {
    return sizeof (TYPE_OF_STACK);
}

//获取迭代器
static Iterator* getIterator(Stack* Stack) {
    return iniIterator(Stack);
}
//-----------------------------------------------------------------------

Stack* iniStack() {
	
	Stack* myStack = (Stack*) malloc (sizeof(Stack));
    myStack->version = NULL;
	myStack->size = 0;
	myStack->first = NULL;
	myStack->isEmpty = isEmpty;
	myStack->peek = peek;
	myStack->pop = pop;
	myStack->push = push;
	myStack->exec = exec;

    myStack->getSize = getSize;
    myStack->toArr = toArr;

    myStack->printFun = printFun;
    myStack->print = print;
    myStack->printAll = printAll;

    myStack->getIterator = getIterator;

    myStack->clear = clear;
    myStack->destroy = destroy;

    myStack->getEleSize = getEleSize;
	return myStack;
	
}