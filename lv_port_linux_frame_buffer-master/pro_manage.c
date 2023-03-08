#include <stdio.h>
#include "lvgl/lvgl.h"

//售货机商品的结构体类型
typedef struct 
{
    char  pro_name[32];//商品名称
    float pro_price;//商品价格
    int   pro_num;//商品数量
    char  pro_pic[128];//存放商品的图片的路径
    int   pro_id;//商品id
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
static ListNode *Pro_InfoInit(void)
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
        if(EOF == fscanf(fp,"%s %f %d %s %d",tmp.pro_name,&tmp.pro_price,&tmp.pro_num,tmp.pro_pic,&tmp.pro_id))
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

// 处理滚动事件，完成页面循环切换(无限切换)--回调函数
static void scroll_begin_event(lv_event_t * e)
{
    lv_obj_t * cont = lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * ttt = lv_obj_get_parent(cont);

    if(lv_event_get_code(e) == LV_EVENT_SCROLL_END) {
        lv_tabview_t * tabview = (lv_tabview_t *)ttt;

        lv_coord_t s = lv_obj_get_scroll_x(cont);

        lv_point_t p;
        lv_obj_get_scroll_end(cont, &p);

        lv_coord_t w = lv_obj_get_content_width(cont);
        lv_coord_t t;

        if(lv_obj_get_style_base_dir(ttt, LV_PART_MAIN) == LV_BASE_DIR_RTL)  t = -(p.x - w / 2) / w;
        else t = (p.x + w / 2) / w;

        if(s < 0) t = tabview->tab_cnt - 1;
        else if((t == (tabview->tab_cnt - 1)) && (s > p.x)) t = 0;

        bool new_tab = false;
        if(t != lv_tabview_get_tab_act(ttt)) new_tab = true;
        lv_tabview_set_act(ttt, t, LV_ANIM_ON);
    }
}

/*********************************************************************************************************/

//按下结算按钮--调用回调函数

//按下退出按钮--调用回调函数
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

    //商品购物界面--在里面包好退出和结算
    lv_obj_t *pro2 = lv_obj_create(pro1);//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
    lv_obj_set_size(pro2,600,400);
    lv_obj_set_align(pro2,LV_ALIGN_BOTTOM_MID);


    // //pay图片
    lv_obj_t *pay_pic = lv_img_create(pro2);
    lv_img_set_zoom(pay_pic,380);
    lv_img_set_src(pay_pic,"S:/pic/pay.png");
    lv_obj_align(pay_pic,LV_ALIGN_LEFT_MID,70,-20);//左中

    //Have a great day!
    lv_obj_t *greatday_txt = lv_label_create(pro2);// 创建标签对象
    lv_label_set_text(greatday_txt, "Have a great day!!!I wish you to be rich!!!!!");// 设置标签对象的文本内容
    lv_obj_set_align(greatday_txt,LV_ALIGN_TOP_MID); // 将标签对象居中对齐

    
    

    //

    //商品购物完毕界面--显示已结算图片

        //在pro2下再搞2个按钮  一个退出  一个是结算
        //退出界面
        lv_obj_t *btn_exit = lv_imgbtn_create(pro2);
        lv_imgbtn_set_src(btn_exit,_LV_IMGBTN_STATE_NUM,"S:/pic/close1.png",NULL,NULL);
        lv_obj_set_size(btn_exit,50,50);//设置按钮对象大小
        lv_obj_set_align(btn_exit,LV_ALIGN_TOP_RIGHT);//右上角
        lv_obj_add_event_cb(btn_exit,BTN_EXIT_cb,LV_EVENT_PRESSED,(void *)pro1);//传入pro1目的是为了在里面删除pro1整个对象
        //lv_style_set_border_opa(btn_exit,LV_OPA_0);//透明度？？


        //假如已结算
        lv_obj_t *pay = lv_btn_create(pro2);
        lv_obj_set_size(pay,130,60);
        lv_obj_set_style_bg_color(pay, lv_color_hex(0x00FFB6C1), LV_PART_MAIN);//颜色
        lv_obj_set_style_radius(pay, LV_PCT(20), LV_PART_MAIN);//圆弧
        lv_obj_set_align(pay,LV_ALIGN_BOTTOM_MID);//下面
        // 创建一个 lv_label 对象，并将其添加到 pay 按钮中
        lv_obj_t *pay_txt = lv_label_create(pay);// 创建标签对象
        lv_label_set_text(pay_txt, "Already paid");// 设置标签对象的文本内容
        lv_obj_set_align(pay_txt,LV_ALIGN_CENTER); // 将标签对象居中对齐
        //lv_obj_add_event_cb(btn_exit,BTN_EXIT_cb,LV_EVENT_PRESSED,(void *)AP);//传入AP目的是为了进入第三个界面

        
       

        





    printf("购买商品:%s\n",((ListNode *)(e->user_data))->data.pro_name);
    printf("价格:%.1f\n",((ListNode *)(e->user_data))->data.pro_price);
    printf("余量:%d\n",((ListNode *)(e->user_data))->data.pro_num);
    printf("商品ID:%d\n",((ListNode *)(e->user_data))->data.pro_id);
}

//根据上面函数得到的商品信息链表绘制基本的商品浏览界面
void Pro_BrowerCreate(void)
{

    //标签
    static lv_obj_t* tv;
    static lv_obj_t* t1;
    static lv_obj_t* t2;
    static lv_obj_t* t3;

    tv = lv_tabview_create(lv_scr_act(), LV_DIR_BOTTOM, LV_DPI_DEF / 3);

    t1 = lv_tabview_add_tab(tv, "drink");
    t2 = lv_tabview_add_tab(tv, "Snacks");
    t3 = lv_tabview_add_tab(tv, "Daily_necessities");



    lv_obj_t* tab_btns = lv_tabview_get_tab_btns(tv);
    lv_obj_set_style_border_color(tab_btns, lv_palette_main(LV_PALETTE_LIGHT_GREEN), LV_PART_ITEMS | LV_STATE_CHECKED);

    lv_obj_add_event_cb(lv_tabview_get_content(tv), scroll_begin_event, LV_EVENT_SCROLL_END, NULL);

    /*************************************************************************************************************************************/    

        //lv_obj_t* cont = lv_obj_create(t1);               // 创建一个对象容器 cont
        lv_obj_set_size(t1, 800, 420);                            // 设置对象容器大小
        lv_obj_align(t1, LV_ALIGN_TOP_MID, 0, 5);                 // 设置对象容器基于屏幕中间对齐
        lv_obj_set_style_pad_all(t1, 15, LV_PART_MAIN);           // 设置对象容器内部 item 与容器边的上下左右间距
        lv_obj_set_style_pad_row(t1, 15, LV_PART_MAIN);           // 设置对象容器内部 item 之间的行间距
        lv_obj_set_style_pad_column(t1, 15, LV_PART_MAIN);        // 设置对象容器内部 item 之间的列间距
        lv_obj_clear_flag(t1, LV_OBJ_FLAG_SCROLL_ELASTIC);            // 取消滚动条
        lv_obj_set_flex_flow(t1, LV_FLEX_FLOW_ROW_WRAP);          // 设置对象容器使用基于行的流失弹性布局flex，设置超出部分换行模式
        /**
         * 设置容器弹性模式
         * 1. 容器指针
         * 2. LV_FLEX_ALIGN_SPACE_EVENLY 均匀分部子元素之间的间距
         * 3. LV_FLEX_ALIGN_END          容器中所有的子元素底部对齐
         * 4. LV_FLEX_ALIGN_CENTER       容器中内容的子元素向容器顶部对齐
        */
        lv_obj_set_flex_align(t1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_SPACE_EVENLY,0);

        lv_obj_set_size(t2, 800, 420);                            // 设置对象容器大小
        lv_obj_align(t2, LV_ALIGN_TOP_MID, 0, 5);                 // 设置对象容器基于屏幕中间对齐
        lv_obj_set_style_pad_all(t2, 15, LV_PART_MAIN);           // 设置对象容器内部 item 与容器边的上下左右间距
        lv_obj_set_style_pad_row(t2, 15, LV_PART_MAIN);           // 设置对象容器内部 item 之间的行间距
        lv_obj_set_style_pad_column(t2, 15, LV_PART_MAIN);        // 设置对象容器内部 item 之间的列间距
        lv_obj_clear_flag(t2, LV_OBJ_FLAG_SCROLL_ELASTIC);            // 取消滚动条
        lv_obj_set_flex_flow(t2, LV_FLEX_FLOW_ROW_WRAP);          // 设置对象容器使用基于行的流失弹性布局flex，设置超出部分换行模式
        lv_obj_set_flex_align(t2, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_SPACE_EVENLY,0);

        lv_obj_set_size(t3, 800, 420);                            // 设置对象容器大小
        lv_obj_align(t3, LV_ALIGN_TOP_MID, 0, 5);                 // 设置对象容器基于屏幕中间对齐
        lv_obj_set_style_pad_all(t3, 15, LV_PART_MAIN);           // 设置对象容器内部 item 与容器边的上下左右间距
        lv_obj_set_style_pad_row(t3, 15, LV_PART_MAIN);           // 设置对象容器内部 item 之间的行间距
        lv_obj_set_style_pad_column(t3, 15, LV_PART_MAIN);        // 设置对象容器内部 item 之间的列间距
        lv_obj_clear_flag(t3, LV_OBJ_FLAG_SCROLL_ELASTIC);            // 取消滚动条
        lv_obj_set_flex_flow(t3, LV_FLEX_FLOW_ROW_WRAP);          // 设置对象容器使用基于行的流失弹性布局flex，设置超出部分换行模式
        //lv_obj_set_flex_align(t3, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START);
        lv_obj_set_flex_align(t3, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_SPACE_EVENLY,0);



            ListNode *head = Pro_InfoInit();
            ListNode *p=head->next;
            for(int i=0;p!=head;p=p->next,i++)
            {
                // p->data.pro_name 
                // p->data.pro_price
                if(p->data.pro_id<50){
                lv_obj_t *pro1 = lv_obj_create(t1);//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
                lv_obj_set_size(pro1,110,190);
                lv_obj_set_pos(pro1,i*100,0);
                lv_obj_clear_flag(pro1,LV_OBJ_FLAG_SCROLLABLE);//清除可滚动--里面内容过多的情况下自动生成滚动条-这里是取消
                //lv_obj_align(pro1,LV_ALIGN_TOP_LEFT,0,0);//边角有弧度 背景白色
                //lv_obj_set_style_bg_color(pro1,lv_color_hex(0x00FF0000),LV_STATE_DEFAULT);//设置pro1对象默认状态下背景颜色为红色
                lv_obj_add_event_cb(pro1,Pro_Buy,LV_EVENT_SHORT_CLICKED,(void *)p);//设置短按调用回调函数Pro_Buy()
                //创建一个显示商品名的标签
                lv_obj_t *proname = lv_label_create(pro1);
                lv_label_set_text(proname,p->data.pro_name);
                lv_obj_set_align(proname,LV_ALIGN_TOP_MID);
                //创建一个图像对象，将图像显示当前商品对象中间
                lv_obj_t * img1 = lv_img_create(pro1);
                lv_img_set_src(img1,p->data.pro_pic);
                lv_obj_set_align(img1,LV_ALIGN_BOTTOM_MID);  //改到按键中间
                }
                else if((50<=p->data.pro_id) && (p->data.pro_id<99)){
                    lv_obj_t *pro1 = lv_obj_create(t2);//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
                    lv_obj_set_size(pro1,110,190);
                    lv_obj_set_pos(pro1,i*100,0);
                    lv_obj_clear_flag(pro1,LV_OBJ_FLAG_SCROLLABLE);//清除可滚动--里面内容过多的情况下自动生成滚动条-这里是取消
                    //lv_obj_align(pro1,LV_ALIGN_TOP_LEFT,0,0);//边角有弧度 背景白色
                    //lv_obj_set_style_bg_color(pro1,lv_color_hex(0x00FF0000),LV_STATE_DEFAULT);//设置pro1对象默认状态下背景颜色为红色
                    lv_obj_add_event_cb(pro1,Pro_Buy,LV_EVENT_SHORT_CLICKED,(void *)p);//设置短按调用回调函数Pro_Buy()
                    //创建一个显示商品名的标签
                    lv_obj_t *proname = lv_label_create(pro1);
                    lv_label_set_text(proname,p->data.pro_name);
                    lv_obj_set_align(proname,LV_ALIGN_TOP_MID);
                    //创建一个图像对象，将图像显示当前商品对象中间
                    lv_obj_t * img1 = lv_img_create(pro1);
                    lv_img_set_src(img1,p->data.pro_pic);
                    lv_obj_set_align(img1,LV_ALIGN_BOTTOM_MID);  //改到按键中间
                }
                else if(p->data.pro_id>=100){
                    lv_obj_t *pro1 = lv_obj_create(t3);//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
                    lv_obj_set_size(pro1,110,190);
                    lv_obj_set_pos(pro1,i*100,0);
                    lv_obj_clear_flag(pro1,LV_OBJ_FLAG_SCROLLABLE);//清除可滚动--里面内容过多的情况下自动生成滚动条-这里是取消
                    //lv_obj_align(pro1,LV_ALIGN_TOP_LEFT,0,0);//边角有弧度 背景白色
                    //lv_obj_set_style_bg_color(pro1,lv_color_hex(0x00FF0000),LV_STATE_DEFAULT);//设置pro1对象默认状态下背景颜色为红色
                    lv_obj_add_event_cb(pro1,Pro_Buy,LV_EVENT_SHORT_CLICKED,(void *)p);//设置短按调用回调函数Pro_Buy()
                    //创建一个显示商品名的标签
                    lv_obj_t *proname = lv_label_create(pro1);
                    lv_label_set_text(proname,p->data.pro_name);
                    lv_obj_set_align(proname,LV_ALIGN_TOP_MID);
                    //创建一个图像对象，将图像显示当前商品对象中间
                    lv_obj_t * img1 = lv_img_create(pro1);
                    lv_img_set_src(img1,p->data.pro_pic);
                    lv_obj_set_align(img1,LV_ALIGN_BOTTOM_MID);  //改到按键中间
                }

            }


}











