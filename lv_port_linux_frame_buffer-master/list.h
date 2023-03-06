#include <stdlib.h>
#include <stdbool.h>

#ifndef DATATYPE
#define DATATYPE int  //保证DATATYPE宏没定义  在这里编译不会出错 给个默认类型
#endif


typedef DATATYPE datatype;

//通用型链表节点类型定义
typedef struct node
{
    //数据域
    datatype data;

    //指针域
    struct node *next;
    struct node *prev;

}ListNode;

//空表创建
static ListNode *List_init(void)
{
    ListNode *head = malloc(sizeof(ListNode));
    head->next = head;
    head->prev = head;
    return head;
}

//把NewNode插入到p1和p2之间
static void __NodeInsert(ListNode *p1,ListNode *p2,ListNode *NewNode)
{
    p1->next = NewNode;
    NewNode->next = p2;
    p2->prev = NewNode;
    NewNode->prev = p1;   
}

//把p1和p2之间的节点移除的链式关系处理
static void __NodeDelete(ListNode *p1,ListNode *p2)
{
    p1->next = p2;
    p2->prev = p1;
}

//头插
static int List_HeadInsert(ListNode *head,datatype dat)
{
    //先判断传入的头指针是否有效
    if(head == NULL)
    {
        return -1;
    }

    //给新节点分配空间
    ListNode *NewNode =malloc(sizeof(ListNode));
    if(NewNode == NULL)
    {
        perror("malloc");
        return -2;
    }

    //处理新节点的数据域
    NewNode->data = dat;

    //处理新节点的指针域
    __NodeInsert(head,head->next,NewNode);
    return 0;//成功
}

//尾插
static int List_TailInsert(ListNode *head,datatype dat)
{
    //先判断传入的头指针是否有效
    if(head == NULL)
    {
        return -1;
    }

    //给新节点分配空间
    ListNode *NewNode =malloc(sizeof(ListNode));
    if(NewNode == NULL)
    {
        perror("malloc");
        return -2;
    }

    //处理新节点的数据域
    NewNode->data = dat;

    //处理新节点的指针域
    __NodeInsert(head->prev,head,NewNode);
    return 0;//成功

}

//遍历
static void List_Traval(ListNode *head,void (*handler)(datatype),char dir)
{
    ListNode *p;
    if(dir ==  0)
    {
        p = head->next;
        for(;p != head;p=p->next)
        {
            handler(p->data);
        }
    }
    else
    {
        p = head->prev;
        for(;p != head;p=p->prev)
        {
            handler(p->data);
        } 
    }
}

//按条件查找节点
static ListNode *List_FindNode(ListNode *head,datatype data,bool (*cmp)(datatype,datatype))
{
    ListNode *p=head->next;//从第一个有效节点开始

    for(;p!=head;p=p->next)
    {
        if(cmp(data,p->data))//调用传入的回调函数实现条件数据和当前节点数据的比对
        {
            return p;//找到了
        } 
    }
    return NULL;//表示没找到
}


