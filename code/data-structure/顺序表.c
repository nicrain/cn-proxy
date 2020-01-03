#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define INIT_SIZE 1
#define INCREMENT_SIZE 5
typedef struct {
    int id; //学号
    int age; //年龄
    char name[10]; //姓名
} Student;
typedef Student ElemType;
typedef struct {
    ElemType *base;
    int size; /* 顺序表的最大容量 */
    int length; /* 记录顺序表的元素个数 */
} SqList;

/**
 * 初始化顺序表
 * @param p 指向顺序表的指针
 * @return 如果初始化成功返回true否则返回false
 */
bool init(SqList *p) {
    p->base = malloc(sizeof(SqList) * INIT_SIZE);
    if (p->base == NULL) {
        return false;
    }
    p->size = INIT_SIZE;
    p->length = 0;
    return true;
}

/**
 * 指定位置插入数据元素
 * @param p 指向顺序表的指针
 * @param index 插入的下标
 * @param elem 插入的元素值
 * @return 如果插入成功返回true，否则返回false
 */
bool insert(SqList *p, int index, ElemType elem) {
    if (index < 0 || index > p->length) {
        perror("插入下标不合法");
        return false;
    }

    //如果顺序表满了，则重新分配更大的容量
    if (p->length == p->size) {
        int newSize = p->size + INCREMENT_SIZE;
        ElemType *newBase = realloc(p->base, newSize);
        if (newBase == NULL) {
            perror("顺序表已满，重新分配内存失败");
            return false;
        }
        p->base = newBase;
        p->size = newSize;
    }

    //从最后一个元素开始依次把元素复制到后面的位置
    for (int i = p->length - 1; i >= index; --i) {
        p->base[i + 1] = p->base[i];
    }
    p->base[index] = elem;
    p->length++;
    return true;
}

/**
 * 删除指定下标的数据元素
 * @param p 指向顺序表的指针
 * @param index 删除的元素的下标
 * @param elem 返回删除的元素
 * @return 如果删除成功返回true否则返回false
 */
bool del(SqList *p, int index, ElemType *elem) {
    if (p->length == 0) {
        perror("顺序是空的，无法执行删除操作");
        return false;
    }
    if (index < 0 || index > p->length - 1) {
        perror("删除位置不合法");
        return false;
    }

    //把删除的元素保存起来
    *elem = p->base[index];
    //从删除位置开始依次把后面的元素赋值到前面
    for (int i = index; i < p->length - 1; ++i) {
        p->base[i] = p->base[i + 1];
    }
    p->length--;
    return true;
}

/**
 * 更新顺序表中特定的元素值
 * @param p 指向顺序表的指针
 * @param index 更新下标
 * @param elem 更改后的新元素值
 * @return 如果更改成功则返回true，否则返回false
 */
bool update(SqList *p, int index, ElemType elem) {
    if (p->length == 0) {
        perror("顺序表是空的，无法指向更新");
        return false;
    }

    if (index < 0 || index > p->length - 1) {
        perror("更新下标不合法");
        return false;
    }

    p->base[index] = elem;
    return true;
}

/**
 * 搜索顺序表中特定下标的元素
 * @param list 指定的顺序表
 * @param index 搜索的下标
 * @param elem 保存搜索到的元素
 * @return 如果搜索成功则返回true，否则返回false
 */
bool search(SqList list, int index, ElemType *elem) {
    if (list.length == 0) {
        perror("顺序表没有任何元素，无法查找");
        return 0;
    }

    if (index < 0 || index > list.length - 1) {
        perror("查找下标不合法");
        return false;
    }

    *elem = list.base[index - 1];
    return true;
}

/**
 * 打印顺序表
 * @param list 指定顺序表
 */
void output(SqList list) {
    printf("学号\t年龄\t姓名\n");
    for (int i = 0; i < list.length; ++i) {
        printf("%d\t%d\t%s\n", list.base[i].id, list.base[i].age, list.base[i].name);
    }
    printf("\n");
}

int main() {
    SqList list;
    while (1) {
        printf("\t\t顺序表的基本操作\n");
        printf("\t\t1.初始化顺序表\n");
        printf("\t\t2.顺序表的插入\n");
        printf("\t\t3.顺序表的删除\n");
        printf("\t\t4.顺序表的查找(下标)\n");
        printf("\t\t5.顺序表的修改\n");
        printf("\t\t6.打印\n");
        printf("\t\t0.退出\n");

        int choice;
        printf("请输入功能编号:");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                if (init(&list)) {
                    printf("初始化成功\n");
                }
                assert(list.length == 0);
                break;
            case 2:;
                ElemType elem;
                printf("请输入插入的学生学号:");
                scanf("%d", &elem.id);
                printf("请输入插入的学生年龄:");
                scanf("%d", &elem.age);
                printf("请输入插入的学生姓名:");
                scanf("%s", elem.name);

                printf("请输入插入位置:");
                int index;
                scanf("%d", &index);

                if (insert(&list, index, elem)) {
                    printf("插入成功\n");
                }
                break;
            case 3:
                printf("请输入删除位置:");
                scanf("%d", &index);
                if (del(&list, index, &elem)) {
                    printf("删除的学生 学号:%d\t年龄:%d\t姓名:%s\n", elem.id, elem.age, elem.name);
                }
                break;
            case 4:
                printf("请输入要查找的位置:");
                scanf("%d", &index);
                if (search(list, index, &elem)) {
                    printf("查找的学生 学号:%d\t年龄:%d\t姓名:%s\n", elem.id, elem.age, elem.name);
                }
                break;
            case 5:
                printf("请输入更新位置:");
                scanf("%d", &index);

                printf("请输入更新后的学生学号:");
                scanf("%d", &elem.id);
                printf("请输入更新后的学生年龄:");
                scanf("%d", &elem.age);
                printf("请输入更新后的学生姓名:");
                scanf("%s", elem.name);
                if (update(&list, index, elem)) {
                    printf("更新成功\n");
                }
                break;
            case 6:
                output(list);
                break;
            case 0:
                exit(0);
            default:
                printf("输入编号有误，请重新输入\n");
                break;
        }
    }
    return 0;
}
