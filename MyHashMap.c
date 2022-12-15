#include "MyHashMap.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>



/**
 * 内部方法
 */

//-----------------------------------------内部方法------------------------------------------

 //哈希函数
static long long hashFun(keyType key) {
    long long mod = CAPACITY;
	long long index = llabs((long long)key%mod);
	return index;
}

 //默认键比较器
static int cmpOfKey(keyType key1, keyType key2) {

     if (key1 < key2) return -1;
     if (key1 > key2) return 1;

     return 0;
 }

 //默认值比较器
static int cmpOfValue(eleType ele1, eleType ele2) {
   long long e1 = (long long )ele1;
   long long e2 = (long long )ele2;

     if (e1 == e2) return 0;
     if (e1 < e2) return -1;

     return 1;
 }

//创建一个指向哈希节点的指针并将其返回 
static hashNode* creatHashNode(keyType key , eleType e) {
	hashNode* temp = (hashNode*)malloc(sizeof(hashNode));
	temp->key = key;
	temp->e = e;
	temp->next = NULL;
	
	return temp;
 } 
 

//判断该数据是否存在 存在则返回 该节点 不存在则返回遍历到的最后一个哈希节点 
static  hashNode* isExist(hash*myTable , keyType key) {
 	
  long long index = myTable->hashFun(key);
  hashNode* p = myTable->list[index];
  hashNode* temp = p;
  while( p != NULL) {
  	if(myTable->cmpOfKey(p->key, key) == 0) return p;//若发现该键值已存在则返回指向该哈希节点的指针
  	temp = p;
  	p = p->next;
  }
  
  return temp;//temp记录第一个节点NULL或者最后一个节点	   
}

//得到某一节点的前驱
static hashNode* getPre(hash*myTable , keyType key) {
  long long index = myTable->hashFun(key);
  hashNode* p = myTable->list[index];
  hashNode* temp = p;
  while( p != NULL) {
  	if(myTable->cmpOfKey(p->key, key) == 0) return temp;//若发现该键值已存在则返回指向该哈希节点前一个节点的指针
  	temp = p;
	p = p->next;
  }
  
  return temp;
 }

 //得到节点数组, 通过类型强制转换，隐藏掉节点的后继
static EntryOfHash* getNodes(hash* myTable) {
    if (myTable->getSize(myTable) == 0) return NULL;

    hashNodePtr* arr = (hashNodePtr *) malloc(sizeof (hashNodePtr)*myTable->getSize(myTable));
    long long len = myTable->getListLen(myTable);

    int i, j = 0;
    for (i = 0; i < len; i++) {
        hashNode* nodePtr = myTable->list[i];
        while (nodePtr != NULL) {
            arr[j++] = nodePtr;
            nodePtr = nodePtr->next;
        }
    }

    return arr;
}
 //---------------------------------------------------------------------



//------------------------------------暴露的方法-------------------------------------

 /**
  * 添加
  */
//------------------------------------------------------
//插入一组 键值 与  元素  插入成功返回 1 否则返回 0 
static int add(hash*myTable , keyType key , eleType e) {
	hashNode* pre = isExist(myTable , key); 
	if(pre == NULL) {//对每条分支链表的第一个节点做特殊处理
		myTable->list[myTable->hashFun(key)] =  creatHashNode(key , e);
		myTable->size++;
	 }else if(myTable->cmpOfKey(pre->key, key) != 0) {//键不存在，则添加节点
		pre->next = creatHashNode(key , e);
		myTable->size++;
	} else {//键已存在，则覆盖之前的值
        pre->e = e;
    }

    changeVersion(myTable);//增加元素，版本发生改变
    return 1;
 }

 //插入全部元素, 并返回成功添加的元素个数
 static int addAll(hash* myTable, keyType keyArr[], eleType eleArr[], int size) {
    int count = 0;
    int i;
    for (i = 0; i < size; i++) {
        if (myTable->add(myTable, keyArr[i], eleArr[i])) count++;
    }

    return count;

}

//------------------------------------------------------


/**
 * 删除
 */
//-----------------------------------------------------------
 //删除对应键值的元素
 static int delByKey(hash*myTable , keyType key) {
 	hashNode* pre = getPre(myTable , key);
 	if (pre == NULL) {
 		return 1;
	} else if(myTable->cmpOfKey(pre->key, key) == 0) {//对每条分支链表的第一个节点做特殊处理
	 	myTable->list[myTable->hashFun(key)] = pre->next;
	 	free(pre);
	 	myTable->size--;
        changeVersion(myTable);//删除元素，容器版本发生改变
	 	return 1;
	} else if(pre->next != NULL && myTable->cmpOfKey(pre->next->key, key) == 0) {
 		hashNode* temp = pre->next;
 		pre->next = pre->next->next;
 		free(temp);
 		myTable->size--;
        changeVersion(myTable);//删除元素，版本发生改变
 		return 1;
	} else return 0;
	
  }

  //按值来删除元素并返回删除的元素个数
  static int delByValue(hash* myTable, eleType ele) {
    int count = 0;
    int i;
    for (i = 0; i < myTable->list_len; i++) {
        hashNode* node = myTable->list[i];
        hashNode* ptr;
        while (node != NULL) {
            ptr = node->next;

            if (myTable->cmpOfValue(node->e, ele)) {
                if(delByKey(myTable, node->key)) count++;
            }

            node = ptr;
        }
    }

      return count;
}

//-----------------------------------------------------------


/**
 * 判断是否包含键或值
 */
//-----------------------------------------------------------------
//判断是否包含key值
static  int containsKey(hash* myTable, keyType key) {
 	hashNode* ptr = isExist(myTable , key);
 	if(ptr != NULL && myTable->cmpOfKey(ptr->key, key) == 0) {
 		return 1;
	}
 	return 0;
 }
 
//判断是否包含value值
 static int containsValue(hash* myTable, eleType ele) {
 	int i;
 	for (i = 0; i < CAPACITY; i++) {
 		hashNode* ptr = myTable->list[i];
 		while (ptr != NULL) {
 			if (myTable->cmpOfValue(ptr->e, ele)) return 1;
 			ptr = ptr->next;
		 }
	}
	 
	return 0;
 }

//-----------------------------------------------------------------


/**
 * get 和 set
 */
//----------------------------------------------------------------
//通过键值查找元素 存在则返回 1 并将元素提取出 不存在则返回 0 并将提取的指针置为NULL
static eleType getValue(hash*myTable , keyType key) {
 	hashNode*ptr = isExist(myTable , key);
 	if(ptr != NULL && myTable->cmpOfKey(ptr->key, key) == 0) {
 		return ptr->e;
		 
	 }
 	else {
 		printf("key not contained\n");
	    return NULL;
	}
} 
  
//修改键值对应的元素 修改成功返回 1 否则返回 0  
static int setValue(hash*myTable , keyType key , eleType e) {
  	 hashNode*pre = isExist(myTable , key);
  	 if(pre != NULL && myTable->cmpOfKey(pre->key, key) == 0) {
           pre->e = e;
           changeVersion(myTable); //修改元素， 容器版本发生改变
           return 1;
	 } else return 0;
}

//----------------------------------------------------------------



/**
 * 判空 和 获取当前容量大小, 与 哈希表长度
 * @return
 */
//----------------------------------------------------
static int isEmpty(hash* myTable) {
	return myTable->size == 0;
}

static int getSize(hash* myTable) {
    return myTable->size;
}

static long long getListLen(hash* myTable) {
    return myTable->list_len;
}
//----------------------------------------------------



/**
 * 打印函数
 */
//--------------------------------------------------------
static void printFun(keyType key, eleType ele) {
    printf("%s==%lld", (const char*)key, (long long)ele);
}
//打印单个元素
static void print(hash* myTable, keyType key) {
    hashNode * ptr = isExist(myTable, key);
    if(ptr != NULL && myTable->cmpOfKey(ptr->key, key) == 0) {
        printf("{");
        myTable->printFun(ptr->key, ptr->e);
        printf("}\n");
    } else {
        printf("NULL\n");
    }
}

//打印全部元素
static void printAll(hash* myTable) {
    int size = myTable->getSize(myTable);
    printf("{HashMap:\tcounts == %d\n", size);
    hashNodePtr* ptr =  getNodes(myTable);
    int i;
    for (i = 0; i < size; i++) {
            printf("\t");
            myTable->printFun(ptr[i]->key, ptr[i]->e);
            printf("\n");
    }
    printf("}\n");
    free(ptr);
}

//--------------------------------------------------------


/**
 * 遍历执行指定函数
 */
//遍历每一个元素并执行指定函数
static void exec(hash* myTable, void(*fun)(eleType* ele)) {
    int i;
    for (i = 0; i < myTable->list_len; i++) {
        hashNode* ptr = myTable->list[i];
        while (ptr != NULL) {
            fun(&ptr->e);
            ptr = ptr->next;
        }
    }
}


/**
 * 转为 键 值 数组
 */
//---------------------------------------------
//键数组
keyType* toKeyArr(hash* myTable) {
    int size = myTable->getSize(myTable);
    if (size == 0) return NULL;

    keyType* arr = (keyType*) malloc(sizeof (keyType)*myTable->getSize(myTable));
    hashNodePtr* ptr = getNodes(myTable);
    int i;
    for (i = 0; i < size; i++) {
            arr[i] = ptr[i]->key;
    }

    free(ptr);
    return arr;
}

//值数组
eleType* toValArr(hash* myTable) {
    int size = myTable->getSize(myTable);
    if (size == 0) return NULL;

   eleType* arr = (eleType*) malloc(sizeof (eleType)*myTable->getSize(myTable));
   hashNodePtr* ptr = getNodes(myTable);
    int i;
    for (i = 0; i < size; i++) {
            arr[i] = ptr[i]->e;
    }
    free(ptr);
    return arr;
}
//---------------------------------------------

/**
 * 清空 与 销毁
 */
//---------------------------------------------
void clear(hash* myTable) {
    hashNodePtr* ptr = myTable->list;
    int i;
    long long len = myTable->list_len;
    for (i = 0; i < len; i++) {
        hashNode* nodePtr = ptr[i];
        while (nodePtr != NULL) {
            hashNode* temp = nodePtr;
            nodePtr = nodePtr->next;
            free(temp);
        }

        ptr[i] = NULL;
    }

    myTable->size = 0;
    changeVersion(myTable);//清空容器，容器版本发生改变
}

void destroy(hash** myTable) {
    (*myTable)->clear(*myTable);
    free((*myTable)->list);
    free(*myTable);
    *myTable = NULL;
}
//---------------------------------------------
//获取迭代器所迭代的元素大小
 static int getEleSize() {
    return sizeof (EntryOfHash);
 }

//获取迭代器
static Iterator* getIterator(HashMap* map) {
    return iniIterator(map);
}

//-----------------------迭代器方法-----------------------
/**
 * 初始化一个指定大小的哈希表
 */
HashMap* iniHashMap() {
	 HashMap* myMap = (HashMap*) malloc (sizeof(HashMap));
     myMap->version = NULL;
	 myMap->size = 0;
     myMap->list_len = CAPACITY;//确定哈希表的主链表长度
	 myMap->list = (hashNodePtr*)malloc(sizeof(hashNodePtr)*CAPACITY); //为主链表分配空间
	 memset(myMap->list , 0 , sizeof(hashNodePtr)*CAPACITY);//将主链表所含哈希节点指针全置为NUll 
	 
	 myMap->add = add;
     myMap->addAll = addAll;
	 myMap->delByKey = delByKey;
	 myMap->delByValue = delByValue;
     myMap->getValue = getValue;
	 myMap->setValue = setValue;
	 myMap->containsKey = containsKey;
	 myMap->containsValue = containsValue;
	 myMap->isEmpty = isEmpty;

	 //初始比较器
	 myMap->cmpOfKey = cmpOfKey;
	 myMap->cmpOfValue = cmpOfValue;

     //初始化hash函数
     myMap->hashFun = hashFun;

     //初始打印函数
     myMap->printFun = printFun; //确定 打印格式 的函数
     myMap->print = print; //打印单个元素
     myMap->printAll = printAll; //打印全部元素

     //获取当前存储大小, 哈希表长度 和 清空, 销毁容器
     myMap->getSize = getSize;
     myMap->getListLen = getListLen;
     myMap->clear = clear;
     myMap->destroy = destroy;

     //遍历执行
     myMap->exec = exec;

     //转为键值数组
     myMap->toKeyArr = toKeyArr;
     myMap->toValArr = toValArr;

     myMap->toArr = getNodes;
     myMap->getEleSize = getEleSize;

     //获取迭代器
     myMap->getIterator = getIterator;
	 return myMap;
	 
}


