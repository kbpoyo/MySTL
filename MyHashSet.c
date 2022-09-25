//
// Created by ASUS on 2022/9/11.
//
#include"MyHashSet.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef TYPE_OF_SET keyTypeOfSet;
typedef TYPE_OF_SET eleTypeOfSet;

//---------------------------hashNodeForSet-----------------------------------
//存储元素和键值的节点
typedef struct hashNodeForSet {
    keyTypeOfSet key;//键值
    eleTypeOfSet e;//存储元素
    struct hashNodeForSet* next; //后继
}hashNodeForSet , *hashNodePtrForSet;
//---------------------------hashNodeForSet------------------------------------

//------------------------------------哈希表结构体-----------------------------------
typedef struct hashMapForSet {
    //属性
    long long  list_len;//哈希表主链的大小
    int size;//哈希表当前的大小
    hashNodePtrForSet* list;//存储哈希节点的指针的链表
    //方法
    //判空
    int(*isEmpty)(struct hashMapForSet*);
    //增
    int(*add)(struct hashMapForSet*, keyTypeOfSet, eleTypeOfSet);
    //删
    int(*del)(struct hashMapForSet*, keyTypeOfSet);
    //改
    int(*setValue)(struct hashMapForSet* , keyTypeOfSet, eleTypeOfSet);
    //查
    eleTypeOfSet(*getValue)(struct hashMapForSet*, keyTypeOfSet);
    //是否包含键值
    int(*containsKey)(struct hashMapForSet*, keyTypeOfSet);
    //是否包含值
    int(*containsValue)(struct hashMapForSet*, eleTypeOfSet);

    //设置自定义类型比较器
    int(*cmpOfKey)(keyTypeOfSet, keyTypeOfSet);
    int(*cmpOfValue)(eleTypeOfSet , eleTypeOfSet);

    //设置自定义类型hash函数
    long long (*hashFun)(keyTypeOfSet);
    //清空与销毁容器
    void(*clear)(struct hashMapForSet*);
    void(*destroy)(struct hashMapForSet*);

} hashMapForSet, HashMapForSet;

//------------------------------------哈希表结构体-----------------------------------


/**
 * 内部函数
 */

//-------------------------------------------------
 //哈希函数
static long long hashFun(keyTypeOfSet key) {
    long long mod = CAPACITY_OF_SET;
	long long index = llabs((long long)key%mod);
	return index;
}

 //自定义比较器
static int cmpOfKey(keyTypeOfSet k1, keyTypeOfSet k2) {

     if (k1 == k2) return 0;
     if (k1 < k2)  return -1;

     return 1;
 }

static int cmpOfValue(eleTypeOfSet e1, eleTypeOfSet e2) {
    return cmpOfKey(e1, e2);
 }


//创建一个指向哈希节点的指针并将其返回
static hashNodeForSet* creatHashNode(keyTypeOfSet key , eleTypeOfSet e) {
	hashNodeForSet* temp = (hashNodeForSet*)malloc(sizeof(hashNodeForSet));
	temp->key = key;
	temp->e = e;
	temp->next = NULL;

	return temp;
 }


//判断该数据是否存在 存在则返回 该节点 不存在则返回遍历到的最后一个哈希节点
static  hashNodeForSet* isExist(hashMapForSet* myTable , keyTypeOfSet key) {

  long long index = myTable->hashFun(key);
  hashNodeForSet* p = myTable->list[index];
  hashNodeForSet* temp = p;
  while( p != NULL) {
  	if(myTable->cmpOfKey(p->key, key) == 0) return p;//若发现该键值已存在则返回指向该哈希节点的指针
  	temp = p;
  	p = p->next;
  }

  return temp;//temp记录第一个节点NULL或者最后一个节点
}

//得到某一节点的前驱
static hashNodeForSet* getPre(hashMapForSet* myTable , keyTypeOfSet key) {
  long long index = myTable->hashFun(key);
  hashNodeForSet* p = myTable->list[index];
  hashNodeForSet* temp = p;
  while( p != NULL) {
  	if(myTable->cmpOfKey(p->key, key) == 0) return temp;//若发现该键值已存在则返回指向该哈希节点前一个节点的指针
  	temp = p;
	p = p->next;
  }

  return temp;
 }

//得到节点数组
static hashNodePtrForSet* getNodes(hashMapForSet* myTable) {
    if (myTable->size == 0) return NULL;

    hashNodePtrForSet* arr = (hashNodePtrForSet *) malloc(sizeof (hashNodePtrForSet)*myTable->size);
    long long len = myTable->list_len;

    int i, j = 0;
    for (i = 0; i < len; i++) {
        hashNodePtrForSet nodePtr = myTable->list[i];
        while (nodePtr != NULL) {
            arr[j++] = nodePtr;
            nodePtr = nodePtr->next;
        }
    }

    return arr;
}

/**
 * 内部map函数
 */
//插入一组 键值 与  元素  插入成功返回 1 否则返回 0
static int addOfMap(hashMapForSet* myTable , keyTypeOfSet key , eleTypeOfSet e) {
	hashNodeForSet* pre = isExist(myTable , key);
	if(pre == NULL) {//对每条分支链表的第一个节点做特殊处理
		myTable->list[myTable->hashFun(key)] =  creatHashNode(key , e);
		myTable->size++;
	 } else if(myTable->cmpOfKey(pre->key, key) != 0) {
		pre->next = creatHashNode(key , e);
		myTable->size++;
	} else {
        pre->e = e;
    }

    return 1;
 }

 //删除对应键值的元素
 static int delByKey(hashMapForSet* myTable , keyTypeOfSet key) {
 	hashNodeForSet* pre = getPre(myTable , key);
 	if (pre == NULL) {
 		return 1;
	} else if(myTable->cmpOfKey(pre->key, key) == 0) {//对每条分支链表的第一个节点做特殊处理
	 	myTable->list[myTable->hashFun(key)] = pre->next;
	 	free(pre);
	 	myTable->size--;
	 	return 1;
	} else if(pre->next != NULL && myTable->cmpOfKey(pre->next->key, key) == 0) {
 		hashNodeForSet* temp = pre->next;
 		pre->next = pre->next->next;
 		free(temp);
 		myTable->size--;
 		return 1;
	} else return 0;

  }

 //判断是否包含key值
static  int containsKey(hashMapForSet* myTable, keyTypeOfSet key) {
 	hashNodeForSet* ptr = isExist(myTable , key);
 	if(ptr != NULL && myTable->cmpOfKey(ptr->key, key) == 0) {
 		return 1;
	}
 	return 0;
 }

//判断是否包含value值
 static int containsValue(hashMapForSet* myTable, eleTypeOfSet ele) {
 	int i;
 	for (i = 0; i < CAPACITY_OF_SET; i++) {
 		hashNodeForSet* ptr = myTable->list[i];
 		while (ptr != NULL) {
 			if (myTable->cmpOfValue(ptr->e, ele) == 0) return 1;
 			ptr = ptr->next;
		 }
	}

	return 0;
 }

 //通过键值查找元素 存在则返回 1 并将元素提取出 不存在则返回 0 并将提取的指针置为NULL
static eleTypeOfSet getValue(hashMapForSet* myTable , keyTypeOfSet key) {
 	hashNodeForSet*ptr = isExist(myTable , key);
 	if(ptr != NULL && myTable->cmpOfKey(ptr->key, key) == 0) {
 		return ptr->e;

	 }
 	else {
 		printf("key not contained\n");
	    return NULL;
	}
}

//修改键值对应的元素 修改成功返回 1 否则返回 0
static int setValue(hashMapForSet* myTable , keyTypeOfSet key , eleTypeOfSet e) {
  	 hashNodeForSet*pre = isExist(myTable , key);
  	 if(pre != NULL && myTable->cmpOfKey(pre->key, key) == 0) {
  	 	pre->e = e;
  	 	return 1;
	 } else return 0;
}

static int isEmptyOfMap(hashMapForSet* myTable) {
	return myTable->size == 0;
}


//清空与销毁
void clearOfMap(hashMapForSet* myTable) {
    hashNodePtrForSet* ptr = myTable->list;
    int i;
    long long len = myTable->list_len;
    for (i = 0; i < len; i++) {
        hashNodeForSet* nodePtr = ptr[i];
        while (nodePtr != NULL) {
            hashNodeForSet* temp = nodePtr;
            nodePtr = nodePtr->next;
            free(temp);
        }

        ptr[i] = NULL;
    }

    myTable->size = 0;
}

void destroyOfMap(hashMapForSet* myTable) {
    myTable->clear(myTable);
    free(myTable->list);
    free(myTable);
}
//-----------------------------------------------------------------

//初始化一个指定大小的哈希表
static hashMapForSet* iniHashMapForSet() {
	 hashMapForSet* myMap = (hashMapForSet*) malloc (sizeof(hashMapForSet));
	 myMap->size = 0;
     myMap->list_len = CAPACITY_OF_SET;//确定哈希表的主链表长度
	 myMap->list = (hashNodePtrForSet*)malloc(sizeof(hashNodePtrForSet)*CAPACITY_OF_SET); //为主链表分配空间
	 memset(myMap->list , 0 , sizeof(hashNodePtrForSet)*CAPACITY_OF_SET);//将主链表所含哈希节点指针全置为NUll

	 myMap->add = addOfMap;
	 myMap->del = delByKey;
	 myMap->getValue = getValue;
	 myMap->setValue = setValue;
	 myMap->containsKey = containsKey;
	 myMap->containsValue = containsValue;
	 myMap->isEmpty = isEmptyOfMap;

	 //初始比较器
	 myMap->cmpOfKey = cmpOfKey;
	 myMap->cmpOfValue = cmpOfValue;

     //初始化hash函数
     myMap->hashFun = hashFun;

     //清空与销毁
     myMap->clear = clearOfMap;
     myMap->destroy = destroyOfMap;
	 return myMap;

}




/**
 * set相关
 */

//--------------------------------set相关函数---------------------------------------------------
static int add(struct set* set, TYPE_OF_SET ele) {
    int flag = set->map->add(set->map, ele, ele);
    if (flag) changeVersion(set);
    return flag;
}
static int del(struct set* set, TYPE_OF_SET ele) {
    int flag = set->map->del(set->map, ele);
    if (flag) changeVersion(set);
    return flag;
}
static int contains(struct set* set, TYPE_OF_SET ele) {
    return set->map->containsKey(set->map, ele);
}
//添加所有元素，并返回成功添加的个数
static int addAll(struct set* set, TYPE_OF_SET* arr, int size) {
    int count = 0;
    int i;
    for (i = 0; i < size; i++) {
        if (set->add(set, arr[i])) count++;
    }
    return count;
}

//设置自定义比较器
static void setCmp(struct set* set, int(*myCmp)(TYPE_OF_SET, TYPE_OF_SET)) {
    set->map->cmpOfKey = myCmp;
    set->map->cmpOfValue = myCmp;
}

//设置自定义hash函数
static void setHashFun(struct set* set, long long (*hashFun)(TYPE_OF_SET ele)) {
    set->map->hashFun = hashFun;
}

static int isEmpty(struct set* set) {
    return isEmptyOfMap(set->map);
}

static int getSize(struct set* set) {
    return set->map->size;
}

static TYPE_OF_SET*toArr(struct set* set) {
    int size = set->getSize(set);
    if (size == 0) return NULL;
    hashNodePtrForSet* ptr =  getNodes(set->map);
    TYPE_OF_SET* arr = (TYPE_OF_SET*) malloc(sizeof (TYPE_OF_SET)*size);
    int i;
    for (i = 0;i <size; i++) {
        arr[i] = ptr[i]->key;
    }
    free(ptr);
    return arr;
}

//默认打印格式函数
static void printFun(TYPE_OF_SET key) {
    printf("%lld", (long long)key);
}
//打印单个元素
static void print(struct set* set, TYPE_OF_SET key) {
    hashNodeForSet * ptr = isExist(set->map, key);
    if(ptr != NULL && set->map->cmpOfKey(ptr->key, key) == 0) {
        printf("{");
        set->printFun(ptr->key);
        printf("}\n");
    } else {
        printf("NULL\n");
    }
}
//打印全部元素
static void printAll(struct set* set) {
    printf("{HashSet:\tcounts == %d\n", set->getSize(set));

    TYPE_OF_SET* arr = toArr(set);
    int i;
    int size = set->getSize(set);
    for (i = 0; i < size; i++) {
            printf("\t");
            set->printFun(arr[i]);
            printf("\n");
    }
    printf("}\n");
    free(arr);
}

/**
 * 清空与销毁
 */
//清空
static void clear(struct set* set) {
    set->map->clear(set->map);
    changeVersion(set);
}

static void destroy(struct set** set) {
    (*set)->map->destroy((*set)->map);
    changeVersion(*set);
    free(*set);
    *set = NULL;
}

//记录迭代器所迭代的元素大小
static int getEleSize() {
    return sizeof (TYPE_OF_SET);
}

//获取迭代器
static Iterator* getIterator(HashSet* set) {
    return iniIterator(set);
}

//-----------------------迭代器方法-----------------------

//初始化HashSet
struct set* iniHashSet() {
    struct set* set = (struct set*) malloc (sizeof(struct set));
    set->version = NULL;
    set->map = iniHashMapForSet();
    set->add = add;
    set->del = del;
    set->contains = contains;
    set->addALl = addAll;
    set->isEmpty = isEmpty;
    set->getSize = getSize;

    //自定义比较器
    set->setCmp = setCmp;
    //自定义hash函数
    set->setHashFun = setHashFun;
    //自定义打印函数
    set->printFun = printFun; //打印格式函数
    set->print = print; //打印当个元素
    set->printAll = printAll; //打印所有元素

    set->toArr = toArr;
    set->getIterator = getIterator;

    set->clear = clear;
    set->destroy = destroy;

    set->getEleSize = getEleSize;
    return set;
}


