#include <stdio.h>
#include "lvgl/lvgl.h"

//售货机商品的结构体类型
typedef struct 
{
    char  pro_name[32];//商品名称
    float pro_price;//商品价格
    int   pro_num;//商品数量
    char  pro_pic[128];//存放商品的图片的路径
}pro;

#define DATATYPE pro
#include "list.h"


//遍历的回调函数
void  traval_name(datatype arg)
{
    printf("%s\t",arg.pro_name);
}

//商品信息初始化
//从本地获取当前商品信息，并记录在某条链表中
static ListNode * Pro_InfoInit(void)
{
    ListNode *head = List_init();

    FILE *fp = fopen("./pro.txt","r+");
    if(fp == NULL)
    {
        perror("fopen");
        return;
    }

    //从文件中读取这些数据 并插入链表
    datatype tmp;
    while(1)
    {
        if(EOF == fscanf(fp,"%s %f %d %s",tmp.pro_name,&tmp.pro_price,&tmp.pro_num,tmp.pro_pic))
        {
            break;
        }
        List_TailInsert(head,tmp);//尾插到链表
    }

    List_Traval(head,traval_name,0);   //按名字遍历
    printf("\n");
    fclose(fp);
    return head;
}


void BTN_EXIT_cb(lv_event_t *e)
{
    //(lv_obj_t *)(e->user_data)
    lv_obj_del((lv_obj_t *)(e->user_data));//删除外面的pro1对象
    
}



//按下了对应的商品图标--调用回调函数
void Pro_Buy(lv_event_t *e)
{
    lv_obj_t *pro1 = lv_obj_create(lv_scr_act());//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
    lv_obj_set_size(pro1,800,480);
    lv_obj_set_style_bg_color(pro1,lv_color_hex(0x00000000),LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(pro1,50,LV_STATE_DEFAULT);

    //商品购物车界面--在里面包好退出和结算
    lv_obj_t *pro2 = lv_obj_create(pro1);//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
    lv_obj_set_size(pro2,600,400);
    lv_obj_set_align(pro2,LV_ALIGN_BOTTOM_MID);

        //在pro2下再搞2个按钮  一个退出购买  一个是结算
        lv_obj_t * btn_exit = lv_btn_create(pro2);
        lv_obj_set_size(btn_exit,30,30);//设置按钮对象大小
        lv_obj_set_align(btn_exit,LV_ALIGN_TOP_RIGHT);//右上角
        lv_obj_add_event_cb(btn_exit,BTN_EXIT_cb,LV_EVENT_PRESSED,(void *)pro1);//传入pro1目的是为了在里面删除pro1整个对象


    printf("购买商品:%s\n",((ListNode *)(e->user_data))->data.pro_name);
    printf("价格:%.1f\n",((ListNode *)(e->user_data))->data.pro_price);
    printf("余量:%d\n",((ListNode *)(e->user_data))->data.pro_num);
}




//根据上面函数得到的商品信息链表绘制基本的商品浏览界面
void Pro_BrowerCreate(void)
{
    ListNode *head = Pro_InfoInit();

    ListNode *p=head->next;
    for(int i=0;p!=head;p=p->next,i++)
    {
        // p->data.pro_name 
        // p->data.pro_price

        lv_obj_t *pro1 = lv_obj_create(lv_scr_act());//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
        lv_obj_set_size(pro1,100,180);
        lv_obj_set_pos(pro1,i*100,0);
        lv_obj_clear_flag(pro1,LV_OBJ_FLAG_SCROLLABLE);//清除可滚动--里面内容过多的情况下自动生成滚动条-这里是取消
        //lv_obj_align(pro1,LV_ALIGN_TOP_LEFT,0,0);//边角有弧度 背景白色
        //lv_obj_set_style_bg_color(pro1,lv_color_hex(0x00FF0000),LV_STATE_DEFAULT);//设置pro1对象默认状态下背景颜色为红色
        lv_obj_add_event_cb(pro1,Pro_Buy,LV_EVENT_LONG_PRESSED,(void *)p);//设置长按调用回调函数Pro_Buy()


        //创建一个显示商品名的标签
        lv_obj_t * proname = lv_label_create(pro1);
        lv_label_set_text(proname,p->data.pro_name);
        lv_obj_set_align(proname,LV_ALIGN_TOP_MID);

        //创建一个图像对象，将图像显示当前商品对象中间
        lv_obj_t * img1 = lv_img_create(pro1);
        lv_img_set_src(img1,p->data.pro_pic);
        lv_obj_set_align(proname,LV_ALIGN_CENTER);

    }


}











