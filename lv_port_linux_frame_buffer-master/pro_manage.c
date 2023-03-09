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

lv_obj_t* p3 = NULL;
lv_obj_t* p1 = NULL;//这是购买后第二次的界面
lv_obj_t* p2 = NULL;//这是购买后第一次的界面





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
//按下第三层sold out
void No_cb(lv_event_t *e){
     lv_obj_del(p3);
}

//按下第二层结算&&退出按键
void Paid_cb(lv_event_t *e){

    printf("购买商品:%s\n",((ListNode *)(e->user_data))->data.pro_name);
    printf("价格:%.1f\n",((ListNode *)(e->user_data))->data.pro_price);
    printf("余量:%d\n",((ListNode *)(e->user_data))->data.pro_num);
    printf("商品ID:%d\n",((ListNode *)(e->user_data))->data.pro_id);

    ((ListNode *)(e->user_data))->data.pro_num--;

        lv_obj_del(p1);//删除外面的pro1对象 
   
    
    
}

//按下结算按钮--调用回调函数
void Pay_cb(lv_event_t *e){
    int i = ((ListNode *)(e->user_data))->data.pro_num;
    if(i <= 0){

            lv_obj_del(p1);//删除外面的pro2对象
            p3 = lv_obj_create(lv_scr_act());//创建一个新半透明的界面
            lv_obj_set_size(p3,800,480);
            lv_obj_set_style_bg_color(p3,lv_color_hex(0x00000000),LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(p3,50,LV_STATE_DEFAULT);

            lv_obj_t *no = lv_obj_create(p3);  //半透明界面里建一个新的界面
            lv_obj_set_size(no,600,400);
            lv_obj_align(no,LV_ALIGN_CENTER,0,0);
            lv_obj_clear_flag(no,LV_OBJ_FLAG_SCROLLABLE);

            lv_obj_t *no_img = lv_img_create(no);
            //lv_img_set_zoom(pay_pic,380);
            lv_img_set_src(no_img,"S:/pic/no.png");
            lv_obj_align(no_img,LV_ALIGN_CENTER,0,0);
            lv_obj_clear_flag(no_img,LV_OBJ_FLAG_SCROLLABLE);

                lv_obj_t *btn_nono = lv_btn_create(no_img);//第二个已付款按钮
                lv_obj_set_size(btn_nono,130,60);
                lv_obj_set_style_bg_color(btn_nono, lv_color_hex(0x00FFB6C1), LV_PART_MAIN);//颜色
                lv_obj_set_style_radius(btn_nono, LV_PCT(20), LV_PART_MAIN);//圆弧
                lv_obj_align(btn_nono,LV_ALIGN_BOTTOM_MID,0,-20);//下面
                
                // 创建一个 lv_label 对象，并将其添加到 pay 按钮中
                lv_obj_t *nono_txt = lv_label_create(btn_nono);// 创建标签对象
                lv_label_set_text(nono_txt, "Have a good day!");// 设置标签对象的文本内容
                lv_obj_set_align(nono_txt,LV_ALIGN_CENTER); // 将标签对象居中对齐
                lv_obj_add_event_cb(btn_nono,No_cb,LV_EVENT_CLICKED,(void *)((ListNode *)(e->user_data)));

        }
    else{
    lv_obj_del(p2);//删除外面的pro2对象

    p1 = lv_obj_create(lv_scr_act());//创建一个新半透明的界面
    lv_obj_set_size(p1,800,480);
    lv_obj_set_style_bg_color(p1,lv_color_hex(0x00000000),LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(p1,50,LV_STATE_DEFAULT);

    lv_obj_t *paid = lv_obj_create(p1);  //半透明界面里建一个新的界面
    lv_obj_set_size(paid,600,400);
    lv_obj_align(paid,LV_ALIGN_CENTER,0,0);
    lv_obj_clear_flag(paid,LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *pay_img = lv_img_create(paid);
    //lv_img_set_zoom(pay_pic,380);
    lv_img_set_src(pay_img,"S:/pic/paid.png");
    lv_obj_align(pay_img,LV_ALIGN_CENTER,0,0);
    lv_obj_clear_flag(pay_img,LV_OBJ_FLAG_SCROLLABLE);

        lv_obj_t *btn_gogo = lv_btn_create(pay_img);//第二个已付款按钮
        lv_obj_set_size(btn_gogo,130,60);
        lv_obj_set_style_bg_color(btn_gogo, lv_color_hex(0x00FFB6C1), LV_PART_MAIN);//颜色
        lv_obj_set_style_radius(btn_gogo, LV_PCT(20), LV_PART_MAIN);//圆弧
        lv_obj_align(btn_gogo,LV_ALIGN_BOTTOM_MID,0,-20);//下面
        
        // 创建一个 lv_label 对象，并将其添加到 pay 按钮中
        lv_obj_t *gogo_txt = lv_label_create(btn_gogo);// 创建标签对象
        lv_label_set_text(gogo_txt, "Have a good day!");// 设置标签对象的文本内容
        lv_obj_set_align(gogo_txt,LV_ALIGN_CENTER); // 将标签对象居中对齐
        lv_obj_add_event_cb(btn_gogo,Paid_cb,LV_EVENT_CLICKED,(void *)((ListNode *)(e->user_data)));

        printf("购买商品:%s\n",((ListNode *)(e->user_data))->data.pro_name);
        printf("价格:%.1f\n",((ListNode *)(e->user_data))->data.pro_price);
        printf("余量:%d\n",((ListNode *)(e->user_data))->data.pro_num);
        printf("商品ID:%d\n",((ListNode *)(e->user_data))->data.pro_id);
    }
        
}

//按下第一层退出按钮--调用回调函数
void BTN_EXIT_cb(lv_event_t *e)
{
    //(lv_obj_t *)(e->user_data)
    lv_obj_del(p2);//删除外面的pro1对象  

}

//按下了对应的商品图标--调用回调函数
void Pro_Buy(lv_event_t *e)
{
    p2 = lv_obj_create(lv_scr_act());//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
    lv_obj_set_size(p2,800,480);
    lv_obj_set_style_bg_color(p2,lv_color_hex(0x00000000),LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(p2,50,LV_STATE_DEFAULT);

    //商品购物界面--在里面包好退出和结算
    lv_obj_t *pro2 = lv_obj_create(p2);//NULL--无父对象 默认不显示 需要手动用函数加载  lv_scr_act()--当前活跃屏幕  默认显示
    lv_obj_set_size(pro2,600,400);
    lv_obj_set_align(pro2,LV_ALIGN_BOTTOM_MID);

    // //pay图片
    lv_obj_t *pay_pic = lv_img_create(pro2);
    lv_img_set_zoom(pay_pic,380);
    lv_img_set_src(pay_pic,"S:/pic/pay.png");
    lv_obj_align(pay_pic,LV_ALIGN_LEFT_MID,70,-20);//左中

    //显示信息的画布1
    lv_obj_t *Board = lv_obj_create(pro2);
    lv_obj_set_size(Board,140,100);
    lv_obj_set_style_border_opa(Board,0,LV_STATE_DEFAULT);
    lv_obj_align(Board,LV_ALIGN_RIGHT_MID,-30,45);
    lv_obj_clear_flag(Board,LV_OBJ_FLAG_SCROLLABLE);


    //Have a great day!
    lv_obj_t *greatday_txt = lv_label_create(pro2);// 创建标签对象
    lv_label_set_text(greatday_txt, "Have a great day!!!I wish you to be rich!!!!!");// 设置标签对象的文本内容
    lv_obj_set_align(greatday_txt,LV_ALIGN_TOP_MID); // 将标签对象居中对齐
    //商品ID
    lv_obj_t *id_txt = lv_label_create(Board);// 创建标签对象
    lv_label_set_text_fmt(id_txt,"%d",((ListNode *)(e->user_data))->data.pro_id);// 设置标签对象的文本内容
    lv_obj_align(id_txt,LV_ALIGN_LEFT_MID,0,-30); // 将标签对象右中
    //购买商品 Purchase of goods
    lv_obj_t *Purchase_txt = lv_label_create(Board);// 创建标签对象
    lv_label_set_text(Purchase_txt, ((ListNode *)(e->user_data))->data.pro_name);// 设置标签对象的文本内容
    lv_obj_align(Purchase_txt,LV_ALIGN_LEFT_MID,0,-10); // 将标签对象右中
    //商品价格
    lv_obj_t *price_txt = lv_label_create(Board);// 创建标签对象
    lv_label_set_text_fmt(price_txt,"%.2f",((ListNode *)(e->user_data))->data.pro_price);// 设置标签对象的文本内容
    lv_obj_align(price_txt,LV_ALIGN_LEFT_MID,0,10); // 将标签对象右中
    //商品余量
    lv_obj_t *num_txt = lv_label_create(Board);// 创建标签对象
    lv_label_set_text_fmt(num_txt, "%d",((ListNode *)(e->user_data))->data.pro_num);// 设置标签对象的文本内容
    lv_obj_align(num_txt,LV_ALIGN_LEFT_MID,0,30); // 将标签对象右中



    //显示信息的画布2!!!
    lv_obj_t *Board2 = lv_obj_create(pro2);
    lv_obj_set_size(Board2,130,100);
    lv_obj_set_style_border_opa(Board2,0,LV_STATE_DEFAULT);
    lv_obj_align(Board2,LV_ALIGN_RIGHT_MID,-160,45);
    lv_obj_clear_flag(Board2,LV_OBJ_FLAG_SCROLLABLE);
    //一些信息
    lv_obj_t *inf1_txt = lv_label_create(Board2);// 创建标签对象
    lv_label_set_text(inf1_txt, "Product ID:");// 设置标签对象的文本内容
    lv_obj_align(inf1_txt,LV_ALIGN_LEFT_MID,0,-30); 

    lv_obj_t *inf2_txt = lv_label_create(Board2);// 创建标签对象
    lv_label_set_text(inf2_txt, "Product Name:");// 设置标签对象的文本内容
    lv_obj_align(inf2_txt,LV_ALIGN_LEFT_MID,0,-10); 

    lv_obj_t *inf3_txt = lv_label_create(Board2);// 创建标签对象
    lv_label_set_text(inf3_txt, "Product Price:");// 设置标签对象的文本内容
    lv_obj_align(inf3_txt,LV_ALIGN_LEFT_MID,0,10); 

    lv_obj_t *inf4_txt = lv_label_create(Board2);// 创建标签对象
    lv_label_set_text(inf4_txt, "Remain:");// 设置标签对象的文本内容
    lv_obj_align(inf4_txt,LV_ALIGN_LEFT_MID,0,30);


    //显示信息的画布3!!!
    lv_obj_t *Board3 = lv_obj_create(pro2);
    lv_obj_set_size(Board3,110,110);
    lv_obj_set_style_border_opa(Board3,0,LV_STATE_DEFAULT);
    lv_obj_align(Board3,LV_ALIGN_RIGHT_MID,-130,-65);
    lv_obj_clear_flag(Board3,LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t * imgx = lv_img_create(Board3);
    lv_img_set_src(imgx,((ListNode *)(e->user_data))->data.pro_pic);
    lv_obj_set_align(imgx,LV_ALIGN_CENTER);  






    //商品购物完毕界面--显示已结算图片

        //在pro2下再搞2个按钮  一个退出  一个是结算
        //退出界面
        lv_obj_t *btn_exit = lv_imgbtn_create(pro2);
        lv_imgbtn_set_src(btn_exit,_LV_IMGBTN_STATE_NUM,"S:/pic/close1.png",NULL,NULL);
        lv_obj_set_size(btn_exit,50,50);//设置按钮对象大小
        lv_obj_set_align(btn_exit,LV_ALIGN_TOP_RIGHT);//右上角
        lv_obj_add_event_cb(btn_exit,BTN_EXIT_cb,LV_EVENT_CLICKED,(void *)((ListNode *)(e->user_data)));//传入pro1目的是为了在里面删除pro1整个对象
        //lv_style_set_border_opa(btn_exit,LV_OPA_0);//透明度？？


        //假如已结算--结算按钮
        lv_obj_t *btn_pay = lv_btn_create(pro2);
        lv_obj_set_size(btn_pay,130,60);
        lv_obj_set_style_bg_color(btn_pay, lv_color_hex(0x00FFB6C1), LV_PART_MAIN);//颜色
        lv_obj_set_style_radius(btn_pay, LV_PCT(20), LV_PART_MAIN);//圆弧
        lv_obj_set_align(btn_pay,LV_ALIGN_BOTTOM_MID);//下面
        // 创建一个 lv_label 对象，并将其添加到 pay 按钮中
        lv_obj_t *pay_txt = lv_label_create(btn_pay);// 创建标签对象
        lv_label_set_text(pay_txt, "Already paid");// 设置标签对象的文本内容
        lv_obj_set_align(pay_txt,LV_ALIGN_CENTER); // 将标签对象居中对齐
        lv_obj_add_event_cb(btn_pay,Pay_cb,LV_EVENT_CLICKED,(void *)((ListNode *)(e->user_data)));


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
                lv_obj_add_event_cb(pro1,Pro_Buy,LV_EVENT_CLICKED,(void *)p);//设置短按调用回调函数Pro_Buy()
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











