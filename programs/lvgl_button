#define LGFX_USE_V1
#include <LovyanGFX.h>

#ifdef DISPLAY_CONFIG_BLUE
	#include <DisplayConfigBlue.h>
#endif

#ifdef DISPLAY_CONFIG_RED
	#include <DisplayConfigRed.h>
#endif

#include <lvgl.h>

LGFX tft;


static const uint32_t ScreenWidth  = 320;
static const uint32_t ScreenHeight = 240;

static lv_disp_draw_buf_t _DisplayBuffer;
static lv_color_t _ByteBuffer[ ScreenWidth * 10 ];

static lv_disp_drv_t _DisplayDriver;
static lv_indev_drv_t _IndevDriver;

static uint8_t counter = 0;


void OnReadDigitizer(lv_indev_drv_t *indevDriver, lv_indev_data_t *data)
{
   uint16_t touchX, touchY;

   auto touched = tft.getTouch(&touchX, &touchY);

   if (touched)
   {
      data->state = LV_INDEV_STATE_PR;

      data->point.x = touchX;
      data->point.y = touchY;
   }
   else
   {
      data->state = LV_INDEV_STATE_REL;
   }
}

void OnDisplayFlush(lv_disp_drv_t *displayDriver, const lv_area_t *screenArea, lv_color_t *colorsPointer)
{
   uint32_t width = (screenArea->x2 - screenArea->x1 + 1);
   uint32_t height = (screenArea->y2 - screenArea->y1 + 1);

   tft.startWrite();
   tft.setAddrWindow(screenArea->x1, screenArea->y1, width, height);

   tft.writePixels((lgfx::rgb565_t*)&colorsPointer->full, width * height);
   tft.endWrite();

   lv_disp_flush_ready(displayDriver);
}

void OnButtonTapEvent(lv_event_t *event)
{
   auto code = lv_event_get_code(event);
    
   if (code == LV_EVENT_CLICKED)
   {
      auto *button = lv_event_get_target(event);
      auto *label = lv_obj_get_child(button, 0);

      counter++;

      lv_label_set_text_fmt(label, "Button: %d", counter);
   }
}

void InitializeLvgl()
{
   tft.begin();        
   tft.setRotation(1);
   tft.setBrightness(255);

   uint16_t calData[] = { 239, 3926, 233, 265, 3856, 3896, 3714, 308 };

   tft.setTouchCalibrate(calData);

   lv_init();
   lv_disp_draw_buf_init(&_DisplayBuffer, _ByteBuffer, NULL, ScreenWidth * 10);


   lv_disp_drv_init(&_DisplayDriver);

   _DisplayDriver.hor_res = ScreenWidth;
   _DisplayDriver.ver_res = ScreenHeight;
   _DisplayDriver.flush_cb = OnDisplayFlush;
   _DisplayDriver.draw_buf = &_DisplayBuffer;

   lv_disp_drv_register(&_DisplayDriver);


   lv_indev_drv_init(&_IndevDriver);

   _IndevDriver.type = LV_INDEV_TYPE_POINTER;
   _IndevDriver.read_cb = OnReadDigitizer;

   lv_indev_drv_register(&_IndevDriver);
}

void CreateGui()
{
   auto *button = lv_btn_create(lv_scr_act());

   lv_obj_set_size(button, 120, 50);
   lv_obj_align(button, LV_ALIGN_CENTER, 0,0);
   lv_obj_add_event_cb(button, OnButtonTapEvent, LV_EVENT_ALL, NULL);

   auto *label = lv_label_create(button);

   lv_label_set_text(label, "Button");
   lv_obj_center(label);
}

void loop()
{
   auto delayTime_ms = lv_timer_handler();

   delay(delayTime_ms);
}

void setup()
{
   InitializeLvgl();
   CreateGui();
}