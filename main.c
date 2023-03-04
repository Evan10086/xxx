#include "lvgl/lvgl.h"
#include "lvgl/demos/lv_demos.h"
#include "lv_drivers/display/fbdev.h"
#include "lv_drivers/indev/evdev.h"
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

#include "pro_manage.h"


#define DISP_BUF_SIZE (480 * 1024)


int main(void)
{
    /*lvgl初始化*/
    lv_init();


    /*输出设备初始化及注册*/
    fbdev_init();
    /*A small buffer for LittlevGL to draw the screen's content*/
    static lv_color_t buf[DISP_BUF_SIZE];
    /*Initialize a descriptor for the buffer*/
    static lv_disp_draw_buf_t disp_buf;
    lv_disp_draw_buf_init(&disp_buf, buf, NULL, DISP_BUF_SIZE);
    /*Initialize and register a display driver*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.draw_buf   = &disp_buf;
    disp_drv.flush_cb   = fbdev_flush;
    disp_drv.hor_res    = 800;
    disp_drv.ver_res    = 480;
    lv_disp_drv_register(&disp_drv);



    //输入设备初始化及注册
    evdev_init();
    static lv_indev_drv_t indev_drv_1;
    lv_indev_drv_init(&indev_drv_1); /*Basic initialization*/
    indev_drv_1.type = LV_INDEV_TYPE_POINTER;
    /*This function will be called periodically (by the library) to get the mouse position and state*/
    indev_drv_1.read_cb = evdev_read;
    lv_indev_t *mouse_indev = lv_indev_drv_register(&indev_drv_1);

    
    //ListNode *head = Pro_InfoInit();//测试是否成功从文件获取了商品信息并插入链表

    Pro_BrowerCreate();//根据商品信息绘制浏览界面


    //官方demo---可以换为自己的demo
    //lv_demo_widgets();
    // lv_obj_t * btn1 = lv_btn_create(lv_scr_act());
    // lv_obj_set_size(btn1,100,100);//设置按钮对象大小
    // //lv_obj_set_pos(btn1,350,190);
    // lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 0);

    //lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, NULL);
    
    // lv_obj_t * wp;
    // wp = lv_img_create(lv_scr_act());
    // lv_img_set_src(wp, "S:/kdm_work/1.jpg");


    /*事务处理及告知lvgl节拍数*/
    while(1) {
        lv_timer_handler();//事务处理
        lv_tick_inc(5);//节拍累计
        usleep(5000);//5ms   1s会循环200次   200Hz
    }

    return 0;
}


/*用户节拍获取*/
uint32_t custom_tick_get(void)
{
    static uint64_t start_ms = 0;
    if(start_ms == 0) {
        struct timeval tv_start;
        gettimeofday(&tv_start, NULL);
        start_ms = (tv_start.tv_sec * 1000000 + tv_start.tv_usec) / 1000;
    }

    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    uint64_t now_ms;
    now_ms = (tv_now.tv_sec * 1000000 + tv_now.tv_usec) / 1000;

    uint32_t time_ms = now_ms - start_ms;
    return time_ms;
}
