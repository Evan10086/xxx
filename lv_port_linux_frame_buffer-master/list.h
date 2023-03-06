#include <stdlib.h>
#include <stdbool.h>

#ifndef DATATYPE
#define DATATYPE int  //��֤DATATYPE��û����  ��������벻����� ����Ĭ������
#endif


typedef DATATYPE datatype;

//ͨ��������ڵ����Ͷ���
typedef struct node
{
    //������
    datatype data;

    //ָ����
    struct node *next;
    struct node *prev;

}ListNode;

//�ձ���
static ListNode *List_init(void)
{
    ListNode *head = malloc(sizeof(ListNode));
    head->next = head;
    head->prev = head;
    return head;
}

//��NewNode���뵽p1��p2֮��
static void __NodeInsert(ListNode *p1,ListNode *p2,ListNode *NewNode)
{
    p1->next = NewNode;
    NewNode->next = p2;
    p2->prev = NewNode;
    NewNode->prev = p1;   
}

//��p1��p2֮��Ľڵ��Ƴ�����ʽ��ϵ����
static void __NodeDelete(ListNode *p1,ListNode *p2)
{
    p1->next = p2;
    p2->prev = p1;
}

//ͷ��
static int List_HeadInsert(ListNode *head,datatype dat)
{
    //���жϴ����ͷָ���Ƿ���Ч
    if(head == NULL)
    {
        return -1;
    }

    //���½ڵ����ռ�
    ListNode *NewNode =malloc(sizeof(ListNode));
    if(NewNode == NULL)
    {
        perror("malloc");
        return -2;
    }

    //�����½ڵ��������
    NewNode->data = dat;

    //�����½ڵ��ָ����
    __NodeInsert(head,head->next,NewNode);
    return 0;//�ɹ�
}

//β��
static int List_TailInsert(ListNode *head,datatype dat)
{
    //���жϴ����ͷָ���Ƿ���Ч
    if(head == NULL)
    {
        return -1;
    }

    //���½ڵ����ռ�
    ListNode *NewNode =malloc(sizeof(ListNode));
    if(NewNode == NULL)
    {
        perror("malloc");
        return -2;
    }

    //�����½ڵ��������
    NewNode->data = dat;

    //�����½ڵ��ָ����
    __NodeInsert(head->prev,head,NewNode);
    return 0;//�ɹ�

}

//����
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

//���������ҽڵ�
static ListNode *List_FindNode(ListNode *head,datatype data,bool (*cmp)(datatype,datatype))
{
    ListNode *p=head->next;//�ӵ�һ����Ч�ڵ㿪ʼ

    for(;p!=head;p=p->next)
    {
        if(cmp(data,p->data))//���ô���Ļص�����ʵ���������ݺ͵�ǰ�ڵ����ݵıȶ�
        {
            return p;//�ҵ���
        } 
    }
    return NULL;//��ʾû�ҵ�
}


