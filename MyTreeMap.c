//
// Created by ASUS on 2022/9/30.
//

#include "MyTreeMap.h"

/**
 * 函数声明区
 */
//----------------函数声明区-----------------
static Node* balance(Node* node);

static int isRed(Node* node);

static int keyInRangeSize(TreeMap* treeMap, keyTreeMap lo, keyTreeMap hi);
//----------------函数声明区-----------------
/**
 * 内部接口
 */
//------------------内部接口------------------------

/**
 * 返回以node为根节点的树高
 */
static int getHeightForNode(Node* node) {
    if (node == NULL) return 0;
    if (node->left == NULL && node->right == NULL) return 0;

    int leftHeight = getHeightForNode(node->left);
    int rightHeight = getHeightForNode(node->right);

    int max = leftHeight > rightHeight ? leftHeight : rightHeight;

    return max + 1;

}

/**
 * 获取黑高，空链接为黑链接
 */
static int getBlackHeightForNode(Node* node) {
    if (node == NULL) return 1;
    int leftHeight = getBlackHeightForNode(node->left);
    int rightHeight = getBlackHeightForNode(node->right);
    if (leftHeight == rightHeight) {
        if (node->color == RED) return leftHeight; //指向自己的链接为红链接，则放回子树的黑高
        else return leftHeight + 1;//指向自己的链接为黑链接，则放回子树黑高 + 1
    } else {
        printErr("黑高不平衡!");
        return 0;
    }
}

/**
 * 判断是否有序
 */
static int isOrderlyForNode(TreeMap* treeMap, Node* node) {
    if (node == NULL) return 1;
    if (isOrderlyForNode(treeMap, node->left) && isOrderlyForNode(treeMap, node->right)) {
        if (node->left != NULL && treeMap->cmp(node->key, node->left->key) < 0) return 0;
        if (node->right != NULL && treeMap->cmp(node->key, node->right->key) > 0) return 0;
    }

    return 1;
}

/**
 * 判断是否符合颜色规则
 */
static int isColorRuleForNode(Node* node) {
    if (node == NULL) return 1;

    if (!isColorRuleForNode(node->left) || !isColorRuleForNode(node->right)) return 0;

    if (isRed(node->right)) return 0;
    if (isRed(node->left) && isRed(node->left->left)) return 0;


    return 1;

}

/**
 * 将以node为根节点的树按序放入数组
 */
static EntryOfTree* enArr(TreeMap* treeMap, Node* node, EntryOfTree* arr, keyTreeMap lo, keyTreeMap hi) {
    if (node == NULL) return arr;
    if (node->left != NULL && treeMap->cmp(lo, node->key) < 0) arr = enArr(treeMap, node->left, arr, lo, hi);
    if (treeMap->cmp(lo, node->key) <= 0 && treeMap->cmp(hi, node->key) >= 0) {
        *arr = (EntryOfTree)node;
        arr++;
    }
    if (node->right != NULL && treeMap->cmp(hi, node->key) > 0) arr = enArr(treeMap, node->right, arr, lo, hi);

    return arr;
}

/**
 * 默认比较器
 * @param key1
 * @param key2
 * @return
 */
static int cmp(keyTreeMap key1, keyTreeMap key2) {
    if (key1 > key2) return 1;
    if (key1 < key2) return -1;
    return 0;
}
//------------------内部接口------------------------


/**
 * 基础BST相关函数
 */
//----------------基础BST相关函数-----------------

/**
 * 返回以node为根节点的树的大小
 */
static int sizeOfNode(Node *node) {
    if (node == NULL) return 0;
    return node->N;
}

/**
 * 返回以node为根节点的树中的最小节点
 */
static Node* minForNode(Node* node) {
    if (node->left == NULL) return node;
    return minForNode(node->left);
}

/**
 * 返回以node为根节点的树中的最大节点
 */
 static Node* maxForNode(Node* node) {
    if (node->right == NULL) return node;
    return maxForNode(node->right);
 }

 /**
  * 将键值对插入以node根节点的树中
  */
 static Node* putForNode(TreeMap* treeMap, Node* node, keyTreeMap key, eleTreeMap ele) {
     if (node == NULL) {//当前节点为空，直接创建该节点并返回其链接
       Node* rst = (Node* )malloc(sizeof (Node));
       rst->key = key;
       rst->ele = ele;
       rst->color = RED;
       rst->N = 1;
       rst->left = rst->right = NULL;
       return rst;
     }

     int cmp = treeMap->cmp(key, node->key);

     if (cmp < 0) node->left = putForNode(treeMap, node->left, key, ele);//去左子树中插入
     else if (cmp > 0) node->right = putForNode(treeMap, node->right, key, ele);//去右子树中插入
     else node->ele = ele; //键值重复，对值进行覆盖

     //插入完成后对红黑规则进行矫正
     node = balance(node);

     return node;
 }

 /**
  * 去以node为根节点的树中进行查找, 并返回对应键值的node链接
  */
  static Node* getForNode(TreeMap* treeMap, Node* node, keyTreeMap key) {
     if (node == NULL) return NULL;

     int cmp = treeMap->cmp(key, node->key);
     if (cmp < 0) return getForNode(treeMap, node->left, key);//去左子树中查找
     else if (cmp > 0) return getForNode(treeMap, node->right, key); //去右子树中查找
     else return node; //找到该节点
  }


/**
 * 返回以node为根节点的树中键值小于key的数量
 */
static int rankForNode(TreeMap* treeMap, Node* node, keyTreeMap key) {
    if (node == NULL) return 0;
    int cmp = treeMap->cmp(key, node->key);
    if (cmp < 0) return rankForNode(treeMap, node->left, key);//key < x.key, 去左子树中查找排名（因为右子树中的元素和node节点都大于左子树中的元素， 所以key在左子树中的排名，即为在node为根节点的树中的排名）
    else if (cmp > 0) return sizeOfNode(node->left) + 1 + rankForNode(treeMap, node->right, key);//key > x.key 所以已知有 node 和 node的左子树中的元素都小于key, 所以返回 key在右子树中的排名 + 上左子树的大小 + 1
    else return sizeOfNode(node->left);//递归到key了，返回其左子树大小即为小于key的键的数量

}

/**
 * 返回在以node为根节点的树中排名为k的键
 */
static keyTreeMap selectForNode(Node* node, int k) {
    if (node == NULL) return NULL; //node为NULL, 返回空
    int rankOfNode = sizeOfNode(node->left) ;//node节点自己的排名
    if (k < rankOfNode) return selectForNode(node->left, k);//所找节点排名小于node,去左子树中查找（因为右子树中的元素和node节点都大于左子树中的元素， 所以key在左子树中的排名，即为在node为根节点的树中的排名）
    else if (k > rankOfNode) return selectForNode(node->right, k - rankOfNode - 1);//去右子树中查找排名为k - rankOfNode - 1的键
    else return node->key;
}


/**
 * 返回以node为根节点的树中，小于等于key的最大键
 */
static keyTreeMap floorForNode(TreeMap* treeMap, Node* node, keyTreeMap key) {
    int rank = rankForNode(treeMap, node, key);
    keyTreeMap temp = selectForNode(node, rank);
    int cmp = treeMap->cmp(temp, key);
    if ( cmp == 0) return key;
    else return selectForNode(node, rank - 1);
}

/**
 * 返回以node为根节点的树中，大于等于key的键的最小值
 */

static keyTreeMap ceilingForNode(TreeMap* treeMap, Node* node, keyTreeMap key) {
    int rank = rankForNode(treeMap, node, key);
    keyTreeMap temp = selectForNode(node, rank);
    int cmp = treeMap->cmp(temp, key);
    if (cmp == 0) return key;
    else return temp;
}

/**
 * 范围查询，查询在以node为根节点的树中， lo <= key >= hi 的键的数组
 */
static EntryOfTree* entrys(TreeMap* treeMap,  keyTreeMap lo, keyTreeMap hi) {
    int size = keyInRangeSize(treeMap, lo, hi);
    EntryOfTree* arr = (EntryOfTree*) malloc(sizeof(EntryOfTree) * (size + 1));
    enArr(treeMap, treeMap->root, arr, lo, hi);

    return arr;
}


//----------------基础BST相关函数-----------------

/**
 * 红黑规则相关函数
 */

//-----------红黑规则相关函数----------------------------


/**
 * 将以node为根节点的树左旋 旋转条件 node右边出现红链接
 */
static Node* rotateLeft(Node *node) {
    Node *temp  = node->right;
    node->right = temp->left;
    temp->left = node;
    temp->color = node->color;
    node->color = RED;//将右边的红链接旋转到左边

    temp->N = node->N;
    node->N = sizeOfNode(node->left) + sizeOfNode(node->right) + 1;

    return temp;
}


/**
 * 将以node为根节点的树右旋 旋转条件 node左边出现两条连续的红链接
 */
static Node* rotateRight(Node *node) {
    Node *temp  = node->left;
    node->left = temp->right;
    temp->right = node;
    temp->color = node->color;
    node->color = RED;//将左边的红链接旋转到左边（左边右连续的两条红链接时才右旋）

    temp->N = node->N;
    node->N = sizeOfNode(node->left) + sizeOfNode(node->right) + 1;

    return temp;
}

/**
 * 翻转颜色，翻转条件 当前节点与子节点异色，且子节点同色 对应4-节点的向上分解 与 向下转移
 */
static void flipColor(Node* node) {
    node->color = !node->color;
    node->left->color = !node->left->color;
    node->right->color = !node->right->color;
}


/**
 * 判断指向当前节点的链接是否为红链接
 */
static int isRed(Node* node) {
    if (node == NULL) return BLACK; //默认空链接为黑链接
    return node->color == RED;
}

/**
 * 通过旋转修正当前节点的红黑规则
 */
static Node* balance(Node* node) {

    //下面3个if在递归插入的过程中保证了 红链接必定为左连接，且没有两条连续的红链接
    if (!isRed(node->left) && isRed(node->right)) node = rotateLeft(node); //左黑右红需要进行左旋，使红链接只能为左链接
    if (isRed(node->left) && isRed(node->left->left)) node = rotateRight(node); //左边有两条红链接，需要进行右旋，保证不出现两条连续的红链接
    if (isRed(node->left) && isRed(node->right)) flipColor(node); //当子节点都为红链接时4-节点向上分解，以当前节点为根节点的树的黑高 + 1

    node->N = sizeOfNode(node->left) + sizeOfNode(node->right) + 1;

    return node;
}

/**
 * 若node的 左节点为2-节点 则给node的左节点传递红链接，将其变为3-节点或4-节点
 * 传递条件：  1.该节点本身必须为3-节点或4-节点（指向该节点的链接必须为红链接）
 *           2.该节点的左子节点为2-节点，即node->left->color = node->left->left->color = black
 * 函数最终效果：将当前指向当前node的左子节点的链接染红，注意是当前node，
 *            当只经过一次flipColor时（即借的为父节点的红链接）则当前返回的node依然为以前的node，此时染红的就是指向左节点的链接(即返回后, node->left.color = RED)
 *            当经过两次flipColor时(即借的为兄弟节点的红链接) 则当前返回的node变为之前node的右节点，但是染红的依然是之前node的左链接（即返回后，node->left->left.color = RED)
 *            以上两点非常重要，因为这才确保了在递归的过程中，在需要递归到子节点时，红链接是一步一步传递，只能染红自己的子节点，即使旋转后返回的是以前的右子节点，但按递归逻辑，一定会再次当递归到原来的node，并进入到node的左子节点
 *            确保了当进入到左子节点后（即当前节点为之前的左子节点）当前节点一定为一个3节点或4节点，且指向本节点的链接一定为红链接（子节点都为黑链接）确保了下一次的递归传递满足传递条件
 **/
static Node* moveRedLeft(Node* node) {
    flipColor(node);//先从父节点借红链接，将自己与自己的子节点链接为3-节点或4-节点(对应子节点个数)
    if (isRed(node->right->left)) {//若右子节点的左子节点为红链接，则右子节点变为红链接后出现了两条连续的红链接，所以通过旋转借该红链接一用
        node->right = rotateRight(node->right);//先将右子节点右选，使右子节点左边的红链跑到右边，与右子节点本身形成两条连续的红链接
        node = rotateLeft(node);//再将node左旋，使右边连续的红链接变为一个以node为中节点的4节点
        flipColor(node);//由于刚才从父节点借了一条红链接，所以此时node->color为黑链接，通过颜色翻转，将该4节点分解，归还刚才借的红链接
    }
    return node;
}

/**
 * 向右节点传递红链接，道理同moveRedLeft
 */
static Node* moveRedRight(Node* node) {
    flipColor(node);
    if (isRed(node->left->left)) {//能往右子节点进行移动红链接的，必须满足右子节点为黑链接，且右子节点的左子节点也为黑链接
                                        //即右边黑高至少为2，所以左边黑高也至少为2，所以node->left必定不为空
        node = rotateRight(node);
        flipColor(node);
    }
    return node;
}

/**
 * 删除以node为根节点的树的最小节点
 * 在删除的时候会向下传递红链接，将2-节点转换为3-节点或4-节点，所以要确保node满足（node.color = RED, node.left.color = node.right.color = BLACK）
 * 一直将红链接传递到叶子节点，便可以直接删除叶子节点，从而不影响黑高平衡
 */
static Node* deleteMinForNode(Node* node) {
    if (node->left == NULL)  {
        free(node);
        return NULL;//已经到达最左边的最小节点, 按规则传递变换，则该节点必为叶子节点，直接删除即可
    }
    if (!isRed(node->left) && !isRed(node->left->left)) {   //左子树不为空，则去左子树中删除最小节点
                                                                        //先判断左子节点是否为2节点，若是，则进行转换，确保之后的递归中，当前节点可以向下分解
        node = moveRedLeft(node);
    }

    node->left = deleteMinForNode(node->left);

    node = balance(node);//在向下传递的过程中会出现红右链接，需要在回溯的过程中进行红黑规则的修复

    return node;

}

/**
 * 删除以node为根节点的树的最大节点
 * 在删除的时候会向下传递红链接，将2-节点转换为3-节点或4-节点，所以要确保node满足（node.color = RED, node.left.color = node.right.color = BLACK）
 * 一直将红链接传递到叶子节点，便可以直接删除叶子节点，从而不影响黑高平衡
 */
 static Node* deleteMaxForNode(Node* node) {
    if (isRed(node->left)) {
        //当前节点的左节点为红链接，先进行右旋，将红链接转到右边，即将指向当前节点的链接染红
        //（之后返回的node指向的是现在node的左节点，染红的仍是指向现在node的链接，按递归逻辑，之后必定马上右移再次递归到node，所以此时node满足当前链接为红色，子节点链接为黑色）
        //也确保了递归到叶子节点时，叶子节点（最大节点）同时为3-节点的右节点
        node = rotateRight(node);
    }

    if (node->right == NULL) {
        free(node);
        return NULL; //已经到达最右边节点，按变换规则，当前节点必为叶子节点，且也为3节点，所以直接删除即可，不会影响黑高
    }

    if (!isRed(node->right) && !isRed(node->right->left)) {//由于之前可能经历过右旋，此时右节点链接可能为红链接，所以判断右子节点是否为2节点时多判断一个node->right是否为红
        node = moveRedRight(node);
    }

    node->right = deleteMaxForNode(node->right);

    node = balance(node);

    return node;

 }

 /**
  * 删除指定节点
  * @param treeMap
  * @param node
  * @param key
  * @return
  */
 static Node* deleteForNode(TreeMap* treeMap, Node* node, keyTreeMap key) {
     if (node == NULL) return NULL;

     if (treeMap->cmp(key, node->key) < 0) { //key小于当前节点键值，需要去左子树中删除
         if (!isRed(node->left) && !isRed(node->left->left)) {  //因为只有当key存在时才会调用删除，所以此时，节点必不为叶子节点，因为当已经递归到叶子节点时，键值必定相等
                                                                            //当前节点的左链接必定不为空，因为当前节点不可能为叶子节点，并且若当前节点左节点为空，则当前键值必定大于或等于节点键值，不可能再去左子树中删除
             node = moveRedLeft(node);
         }

         node->left = deleteForNode(treeMap, node->left, key); //去左子树中删除

     } else {   //key >= node->key 所以需要删除当前节点或者去右子树中删除
                //两种情况：1. key = node.key 删除当前节点，将当前节点与右子树中的最小节点进行替换，并删除右子树中的最小值， 因为需要去右子树中删除最小值，所以可能要向右子节点中传递红链接，确保其为3节点的右子节点
                //        2. key > node.key 去右子树中删除，也可能要向右子节点中传递红链接，确保其为3-节点的右子节点
                //两种情况都需要在下一步移动到右子节点，所以要确保其为3-节点的右链接

                if (isRed(node->left)) node = rotateRight(node); //左子节点为红链接，直接右旋，将指向当前的node链接变换为红链接，在洗一次递归中会回到当前node节点（此时为3-节点的右节点）

                if (treeMap->cmp(key, node->key) == 0 && (node->right == NULL)) {   //当前if与上一个if只能指向一个，因为若在上一个if中进行右旋，则 node->right == NULL 必为false
                                                                                    //与之对应的也说明了，若执行这一个if，则node的左链接必为黑链接, 再当又满足 node->right == NULL 时，即此时右链接也为黑链接
                    free(node);                                                                //因为必定满足黑高平衡，所以左链接的黑链接必定为一个空链接，所以当前节点必为叶子节点，直接删除即可
                    return NULL;

                }

                if (!isRed(node->right) && !isRed(node->right->left)) { //执行到这个位置，代表着，当前节点可能为需要直接删除的节点， key = x.key，但却不是叶子节点（且是3-节点的右子节点）无法直接删除
                                                                                    //也可能 key > x.key 即需要去右子树中删除
                                                                                    //这两种可能性都需要去递归到右子树中去删除，所以要确保右子节点为3-节点或4-节点的右节点
                    node = moveRedRight(node);
                }

                if (treeMap->cmp(key, node->key) == 0) {// 情况1： 当前节点为需要删除节点，将其与右子树中的最小节点进行替换，并删除右子树的最小节点，实现该节点的删除
                    Node* temp = minForNode(node->right);
                    node->key = temp->key;
                    node->ele = temp->ele;
                    node->right = deleteMinForNode(node->right);

                } else node->right = deleteForNode(treeMap, node->right, key); //情况2：去右子树中删除


     }

     //完成删除后恢复当前以节点为根节点的树的红黑规则
     node = balance(node);

     return node;


 }

//-----------红黑规则相关函数----------------------------


/**
 *  暴露出的接口
 */

//------------------暴露出的接口------------------

/**
 * 判空
 * @param treeMap
 * @return
 */
static int isEmpty(TreeMap* treeMap) {
    if (treeMap->root == NULL) return 1;
    return 0;
}

/**
 * 插入节点
 * @param treeMap
 * @param key
 * @param ele
 */
static void put(TreeMap* treeMap, keyTreeMap key, eleTreeMap ele) {
    treeMap->root = putForNode(treeMap, treeMap->root, key, ele);
    treeMap->root->color = BLACK;
}

/**
 * 获取key对应的值
 * @param treeMap
 * @param key
 * @return
 */
static eleTreeMap get(TreeMap* treeMap, keyTreeMap key) {
    if (isEmpty(treeMap)) return NULL;
    Node* temp = getForNode(treeMap, treeMap->root, key);
    if (temp == NULL) return NULL;
    return temp->ele;
}

/**
 * 返回最小值
 */
static keyTreeMap min(TreeMap* treeMap) {
    if (isEmpty(treeMap)) return NULL;
    return minForNode(treeMap->root)->key;
}

/**
 * 返回最大值
 */
static keyTreeMap max(TreeMap* treeMap) {
    if (isEmpty(treeMap)) return NULL;
    return maxForNode(treeMap->root)->key;
}

/**
 * 判断是否包含该键值
 * @param treeMap
 * @param key
 * @return
 */
static int contains(TreeMap* treeMap, keyTreeMap key) {
    Node* temp = getForNode(treeMap, treeMap->root, key);
    if (temp != NULL) return 1;
    return 0;
}

/**
 * 删除最小值
 * @param treeMap
 */
static void deleteMin(TreeMap* treeMap) {
    if (isEmpty(treeMap)) return;
    Node* root = treeMap->root;
    if (!isRed(root->left) && !isRed(root->right)) root->color = RED ;//根节点为2-节点，所以要将其转换为3-节点，将其链接变为红链接
    root = deleteMinForNode(root);
    treeMap->root = root;
    if (!isEmpty(treeMap)) treeMap->root->color = BLACK;
}

/**
 * 删除最大值
 * @param treeMap
 */
static void deleteMax(TreeMap* treeMap) {
    if (isEmpty(treeMap)) return;
    Node* root = treeMap->root;
    if (!isRed(root->left) && !isRed(root->right)) root->color = RED ;//根节点为2-节点，所以要将其转换为3-节点，将其链接变为红链接
    root = deleteMaxForNode(root);
    treeMap->root = root;
    if (!isEmpty(treeMap)) treeMap->root->color = BLACK;

}

/**
 * 删除指定节点
 * @param treeMap
 * @param key
 */
static void delete(TreeMap* treeMap, keyTreeMap key) {
    if (isEmpty(treeMap) || !contains(treeMap, key)) return;
    Node* root = treeMap->root;
    if (!isRed(root->left) && !isRed(root->right)) root->color = RED ;//根节点为2-节点，所以要将其转换为3-节点，将其链接变为红链接
    root = deleteForNode(treeMap, root, key);
    treeMap->root = root;
    if (!isEmpty(treeMap)) treeMap->root->color = BLACK;
}

/**
 * 返回树高
 */

static int getHeight(TreeMap* treeMap) {
    return getHeightForNode(treeMap->root);
}

/**
 * 返回键在 [lo,hi]之间的数量
 */
 static int keyInRangeSize(TreeMap* treeMap, keyTreeMap lo, keyTreeMap hi) {
     return rankForNode(treeMap, treeMap->root, hi) - rankForNode(treeMap, treeMap->root, lo) + 1;
 }

/**
 * 返回树中小于key的键的数量，即key在树中的排名（下标从0开始）
 */
static int rank(TreeMap* treeMap, keyTreeMap key) {
    return rankForNode(treeMap, treeMap->root, key);
}

/**
 * 返回树中排名为k的键，即小于所查键的键的数量为k
 */
static keyTreeMap select(TreeMap* treeMap, int k) {
    return selectForNode(treeMap->root, k);
}



/**
 * 转为数组
 */
static EntryOfTree* toArr(TreeMap* treeMap) {
    EntryOfTree* arr = (EntryOfTree*) malloc(sizeof (EntryOfTree) * (treeMap->root->N + 1));
    enArr(treeMap, treeMap->root, arr, min(treeMap), max(treeMap));
    return arr;
}

/**
 * 校验是否为红黑树的一组函数
 */
static int getBlackHeight(TreeMap* treeMap) {
    return getBlackHeightForNode(treeMap->root);
}

static int isOrderly(TreeMap* treeMap) {
    return isOrderlyForNode(treeMap, treeMap->root);
}

static int isColorRule(TreeMap* treeMap) {
    return isColorRuleForNode(treeMap->root);
}



/**
 * 返回当前树的大小
 * @param treeMap
 * @return
 */
static int getSize(TreeMap* treeMap) {
    return sizeOfNode(treeMap->root);
}




/**
 * 向下取整，返回小于等于 key 的最大键
 */
static keyTreeMap floor(TreeMap* treeMap, keyTreeMap key) {
    return floorForNode(treeMap, treeMap->root, key);
}

/**
 * 向上取整，返回大于等于key的最小键
 */
static keyTreeMap ceiling(TreeMap* treeMap, keyTreeMap key) {
    return ceilingForNode(treeMap, treeMap->root, key);
}



/**
 * 打印函数组
 */
static void printFun(keyTreeMap key, eleTreeMap ele) { //默认打印格式函数
    printf("%d==%d", key, ele);
}

static void print(TreeMap* treeMap, keyTreeMap key, eleTreeMap ele) {
    printf("{");
    treeMap->printFun(key, ele);
    printf("}");
}

static void printAll(TreeMap* treeMap) {
    int size = treeMap->getSize(treeMap);
    printf("{TreeMap:\tcounts==%d\n", size);
    EntryOfTree* arr = toArr(treeMap);
    int i;
    for (i = 0; i < size; i++) {
        printf("\t");
        treeMap->printFun(arr[i]->key, arr[i]->ele);
        printf("\n");
    }

    printf("}\n");
}

/**
 * 清空和销毁
 */
static void clear(TreeMap* treeMap) {
    while (treeMap->getSize(treeMap) > 0) deleteMin(treeMap);
}

static void destroy(TreeMap** treeMap) {
    clear(*treeMap);
    free((*treeMap)->version);
    free(*treeMap);
    *treeMap = NULL;
}


//------------------暴露出的接口------------------


/**
 * 迭代器相关方法
 */
//---------------------------------------------------------------
/**
 * 获取迭代器所迭代元素的大小
 * @return
 */
static int getEleSize() {
    return sizeof (EntryOfTree);
}

/**
 * 返回迭代器
 * @param treeMap
 * @return
 */
static Iterator* getIterator(TreeMap* treeMap) {
    return iniIterator(treeMap);
}


/**
 * 范围查找并返回迭代器
 */
static keyTreeMap temporaryLo, temporaryHi;
static TreeMap* temporaryTreeMap;
static int getTemporarySize() {
    return keyInRangeSize(temporaryTreeMap, temporaryLo, temporaryHi);
}

static EntryOfTree* temporaryToArr() {
    return entrys(temporaryTreeMap, temporaryLo, temporaryHi);
}

static Iterator* getEntrysIterator(TreeMap* treeMap, keyTreeMap lo, keyTreeMap hi) {

    temporaryTreeMap = treeMap;
    temporaryLo = lo;
    temporaryHi = hi;

    treeMap->toArr = temporaryToArr;
    treeMap->getSize = getTemporarySize;

    Iterator* iterator = iniIterator(treeMap);

    treeMap->toArr = toArr;
    treeMap->getSize = getSize;

    return iterator;
}
//--------------------------------------------------------------


/**
* 初始一个TreeMap
*/
TreeMap* iniTreeMap() {
    TreeMap* treeMap = (TreeMap*) malloc(sizeof(TreeMap));
    //初始化迭代器接口
    treeMap->version = NULL;
    treeMap->toArr = toArr;
    treeMap->getSize = getSize;
    treeMap->getEleSize = getEleSize;

    //初始化迭代器接口方法
    treeMap->root = NULL;

    treeMap->cmp = cmp;

    treeMap->isEmpty = isEmpty;

    treeMap->put = put;

    treeMap->get = get;

    treeMap->min = min;
    treeMap->max = max;

    treeMap->contains = contains;

    treeMap->delete = delete;
    treeMap->deleteMin = deleteMin;
    treeMap->deleteMax = deleteMax;

    treeMap->floor = floor;
    treeMap->ceiling = ceiling;

    treeMap->rank = rank;
    treeMap->select = select;


    treeMap->getHeight = getHeight;

    treeMap->getBlackHeight = getBlackHeight;
    treeMap->isOrderly = isOrderly;
    treeMap->isColorRule = isColorRule;


    treeMap->keyInRangeSize = keyInRangeSize;
    treeMap->getEntrysIterator = getEntrysIterator;
    treeMap->getIterator = getIterator;

    treeMap->printFun = printFun;
    treeMap->print = print;
    treeMap->printAll = printAll;

    treeMap->clear = clear;
    treeMap->destroy = destroy;

    return treeMap;

}
