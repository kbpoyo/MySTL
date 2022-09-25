//
// Created by ASUS on 2022/9/12.
//
#include "MyList.h"
#include <stdio.h>
#include <stdlib.h>
//声明修改迭代器版本方法
/**
 * 内部方法
 */
//-----------------------------内部方法-----------------------------------
//检测访问是否越界, 但可以访问到最后一个有效节点的下一个即 list->last
static int isOut(LinkedList* list, int index) {
    return (index < 0 || index > list->size) ? 1 : 0;
}

//默认比较器 0: ele1==ele2, 1: ele1>ele2, -1: ele1<ele2
static int cmp(TYPE_OF_LIST ele1, TYPE_OF_LIST ele2) {
    long long e1 = (long long )ele1;
    long long e2 = (long long )ele2;
    if (e1 == e2) return 0;
    else if (e1 < e2) return -1;
    else return 1;
}

//排序时使用的比较函数
typedef struct cmpObj {
    int(*cmp)(TYPE_OF_LIST, TYPE_OF_LIST);
}CmpObj;
static CmpObj* cmpObj;
static int cmpForSort(const void* ele1, const void* ele2) {
     TYPE_OF_LIST* ptrEle1 = (TYPE_OF_LIST*)ele1;
     TYPE_OF_LIST* ptrEle2 = (TYPE_OF_LIST*)ele2;
    return cmpObj->cmp(*ptrEle1, *ptrEle2);
}


//获取指向指定位置的节点的指针, 有可能获取到 list->last
static NodeOfList* getPtr(LinkedList* list, int index) {
     //进行越界检测
    if (isOut(list, index)) {
        printErr("range is out");
        return NULL;
    }
    //获得指向指定位置的当前节点的指针
    NodeOfList* ptr;

    if (index <= list->size / 2) {//插入链表前半区
        ptr = list->head->next;
        while(index != 0) {
            ptr = ptr->next;
            index--;
        }
    } else {// 插入链表后半区
        ptr = list->last;
        while (index != list->size) {
            ptr = ptr->pre;
            index++;
        }
    }

    return ptr;

}
//-----------------------------------------------------------------

/**
 * 暴露的方法
 */
//-------------------------------暴露的方法---------------------------------
/**
 * 判空
 */
 static int isEmpty(LinkedList* list) {
    return list->size == 0;
 }

/**
 * indexOf() 和 lastIndexOf()
 */
//---------------------------------------------------
static int indexOf(LinkedList* list, TYPE_OF_LIST ele) {
    NodeOfList* ptr = list->head->next;
    if (ptr == list->last) return -1;

    int index = 0;
    while (ptr != list->last) {
        if (list->cmp(ptr->ele, ele) == 0) return index;
        ptr = ptr->next;
        index++;
    }

    return -1;
}

static int lastIndexOf(LinkedList* list, TYPE_OF_LIST ele) {
    NodeOfList* ptr = list->last->pre;
    if (ptr == list->head) return -1;

    int index = list->size - 1;
    while (ptr != list->head) {
         if (list->cmp(ptr->ele, ele) == 0) return index;
         ptr = ptr->pre;
         index--;
    }

    return -1;

}

//---------------------------------------------------

/**
 * 判断元素是否存在
 */
static int contains(LinkedList* list, TYPE_OF_LIST ele) {
    int index = indexOf(list, ele);
    if (index == -1) return 0;
    else return 1;
}


/**
 *增加元素
 */
//--------------------------增加元素-----------------------------------
//指定位置增添元素
static int add(LinkedList* list, int index, TYPE_OF_LIST ele) {


    //创建新节点
    NodeOfList* newNode = (NodeOfList*) malloc(sizeof(NodeOfList));
    newNode->ele = ele;


    //获得指向指定位置的当前节点的指针
    NodeOfList* ptr = getPtr(list, index);
    if (ptr == NULL) return 0;

    //获取指定位置的前驱
    NodeOfList* pre = ptr->pre;

    //插入元素
    newNode->next = pre->next;
    newNode->next->pre = newNode;
    pre->next = newNode;
    newNode->pre = pre;

    list->size++;
    changeVersion(list);//迭代器版本改变

    return 1;
}

//头插
static int addHead(LinkedList* list, TYPE_OF_LIST ele) {
    return list->add(list, 0, ele);
}
//尾插
static int addBack(LinkedList* list, TYPE_OF_LIST ele) {
    return list->add(list, list->size, ele);
}
//添加全部元素
static int addAll(LinkedList* list, TYPE_OF_LIST arr[], int len) {
    int i;
    int count = 0;
    for (i = 0; i < len; i++) {
        if (list->addBack(list, arr[i])) count++;
    }

    return count;
}

//--------------------------增加元素-----------------------------------

/**
 * 删除元素
 */
//-----------------------------------------------------------------------
//删除指定位置的元素
static TYPE_OF_LIST del(LinkedList* list, int index) {
    NodeOfList* ptr = getPtr(list, index);
    if (ptr == NULL) return 0;
    if (ptr == list->last) {
        printErr("range is out");
        return 0;
    }

    NodeOfList* pre = ptr->pre;
    pre->next = ptr->next;
    ptr->next->pre = pre;

    list->size--;
    changeVersion(list);//迭代器版本改变

    TYPE_OF_LIST ele = ptr->ele;
    free(ptr);

    return ele;
}

//头删
static TYPE_OF_LIST delHead(LinkedList* list) {
    return del(list, 0);
}

//尾删
static TYPE_OF_LIST delBack(LinkedList* list) {
    return del(list, list->size - 1);
}

//从前往后按值删除
static TYPE_OF_LIST delVal(LinkedList* list, TYPE_OF_LIST ele) {
    int index = list->indexOf(list, ele);
    if (index == -1) return NULL;
    return del(list, index);
}

//从后往前按值删除
static TYPE_OF_LIST delValRe(LinkedList* list, TYPE_OF_LIST ele) {
    int index = list->lastIndexOf(list, ele);
    if (index == -1) return NULL;
    return del(list, index);
}

//删除所有与指定元素相等的元素
static void delAll(LinkedList* list, TYPE_OF_LIST ele) {
    while (list->contains(list, ele)) {
        list->delVal(list, ele);
    }
}

//删除指定数组中的所有元素
static void delAllByArr(LinkedList* list, TYPE_OF_LIST arr[], int size) {
    int i;
    for (i = 0; i < size; i++) {
        delAll(list, arr[i]);
    }
}

//-----------------------------------------------------------------------

/**
 * 修改元素
 */
static TYPE_OF_LIST set(LinkedList* list, int index, TYPE_OF_LIST ele) {
    NodeOfList* ptr = getPtr(list, index);
    if (ptr == NULL) return NULL;
    if (ptr == list->last) {
        printErr("range is out");
        return NULL;
    }

    TYPE_OF_LIST result = ptr->ele;
    ptr->ele = ele;
    changeVersion(list);//修改元素，容器版本改变

    return result;
}

/**
 *查询元素
 */
static TYPE_OF_LIST get(LinkedList* list, int index) {
    NodeOfList* ptr =  getPtr(list, index);
    if (ptr == NULL) {//访问越界
        return NULL;
    }
    if (ptr == list->last) {//查询越界
        printErr("range is out");
        return NULL;
    }

    return ptr->ele;
}

/**
 * 获取链表长度
 */
static int getSize(LinkedList* list) {
    return list->size;
}
//获取元素所占字节大小
static int getEleSize() {
    return sizeof(TYPE_OF_LIST);
}



/**
 * 打印函数
 */
//-------------------------------------------------------
//默认打印格式函数
static void printFun(TYPE_OF_LIST ele) {
    printf("%lld", (long long)ele);
}

//打印单个元素
static void print(LinkedList* list, int index) {
    NodeOfList* ptr = getPtr(list, index);
    if (ptr == NULL || ptr == list->last) {
        printErr("range is out");
        return;
    }

    list->printFun(ptr->ele);
    printf("\n");
}

//打印全部元素
static void printAll(LinkedList* list) {
    printf("{LinkedList:\tcounts==%d\n", list->getSize(list));
    int i;
    for (i = 0; i < list->getSize(list); i++) {
        printf("\t");
        list->print(list, i);
    }
    printf("}\n");
}

//-------------------------------------------------------

/**
 * 遍历所有元素并执行指定函数
 */
static void exec(LinkedList* list, void(*fun)(TYPE_OF_LIST* ele)) {
     if (isEmpty(list)) return;
     NodeOfList* ptr = list->head->next;
    while (ptr != list->last) {
        fun(&ptr->ele);
        ptr = ptr->next;
    }
 }


 /**
  * 清空, 销毁容器
  */
 static void clear(LinkedList* list) {
     if (list->size == 0) return;
     NodeOfList* ptr = list->head->next;
     while (ptr != list->last) {
         NodeOfList* temp = ptr;
         ptr = ptr->next;
         free(temp);
     }

     list->head->next = list->last;
     list->last->pre = list->head;
     list->size = 0;
     changeVersion(list);
 }

 static void destroy(LinkedList** list) {
     (*list)->clear(*list);
     free(*list);
     *list = NULL;
 }

 /**
  * 转为数组
  */
 TYPE_OF_LIST* toArr(LinkedList* list) {
     TYPE_OF_LIST* arr = (TYPE_OF_LIST*) malloc(sizeof (TYPE_OF_LIST)*(list->size + 1));
     if (list->size == 0) return NULL;
     NodeOfList* ptr = list->head->next;
     int i;
     for (i = 0; i < list->size; i++) {
         arr[i] = ptr->ele;
         ptr = ptr->next;
     }

     return arr;
 }

 /**
  * 排序
  */
 static void sort(LinkedList* list) {
     TYPE_OF_LIST* arrForSort = toArr(list);
     if (arrForSort == NULL) return;

     cmpObj = (CmpObj*) malloc(sizeof (CmpObj));
     cmpObj->cmp = list->cmp;
     qsort(arrForSort, list->size ,sizeof(TYPE_OF_LIST), cmpForSort);

     int size = list->size;

     clear(list);

     addAll(list, arrForSort, size);

     free(arrForSort);

 }

//获取迭代器
static Iterator* getIterator(LinkedList* list) {
    return iniIterator(list);
}
//-----------------------------------------------------------------------


//----------------------------初始一个LinkedList---------------------------
LinkedList* iniLinkedList() {
    LinkedList* list = (LinkedList*) malloc(sizeof(LinkedList));

    list->version = NULL;
    list->size = 0;
    list->head = (NodeOfList*) malloc(sizeof(NodeOfList));
    list->last = (NodeOfList*) malloc(sizeof(NodeOfList));
    list->head->next = list->last;
    list->last->pre = list->head;

    list->add = add;
    list->addHead = addHead;
    list->addBack = addBack;
    list->addAll = addAll;

    list->del = del;
    list->delHead = delHead;
    list->delBack = delBack;
    list->delVal = delVal;
    list->delValRe = delValRe;
    list->delAll = delAll;
    list->delAllByArr = delAllByArr;

    list->set = set;

    list->get = get;

    list->contains = contains;

    list->indexOf = indexOf;
    list->lastIndexOf = lastIndexOf;

    list->cmp = cmp;

    list->getSize = getSize;
    list->getEleSize = getEleSize;

    list->printFun = printFun;
    list->print = print;
    list->printAll = printAll;

    list->isEmpty = isEmpty;
    list->exec = exec;

    list->toArr = toArr;
    list->sort = sort;
    list->clear = clear;
    list->destroy = destroy;

    list->getIterator = getIterator;


    return list;
}