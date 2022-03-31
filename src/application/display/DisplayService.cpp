#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.h>

#ifdef DISPLAY_CONFIG_BLUE
	#include <application/display/DisplayConfigBlue.h>
#endif

#ifdef DISPLAY_CONFIG_RED
	#include <application/display/DisplayConfigRed.h>
#endif

#ifdef DISPLAY_CONFIG_LARGE
	#include <application/display/DisplayConfigLarge.h>
#endif

#include <lvgl.h>
#include <application/display/DisplayService.h>
#include <framework/services/SystemService.h>
#include <framework/common/Event.h>


using namespace std;

namespace Services
{
  namespace Display
  {

    const timespan_t LvglRenderInterval_us = 5 * 1000;

#if DISPLAY_RESOLUTION_240_320

    static const uint32_t ScreenWidth  = 320;

    static const uint32_t ScreenHeight = 240;

#endif

#ifdef DISPLAY_RESOLUTION_800_480

    static const uint32_t ScreenWidth  = 800;

    static const uint32_t ScreenHeight = 480;

#endif


    LGFX tft;

    Event<void> LvglRenderEvent;


    static lv_disp_draw_buf_t DisplayBuffer;

    static lv_color_t ByteBuffer[ ScreenWidth * 10 ];

    static lv_disp_drv_t DisplayDriver;

#ifdef DISPLAY_DIGITIZER
    static lv_indev_drv_t DigitizerDriver;
#endif


    void Initialize();

    void OnLvglRenderEvent(void *args);

    void OnDisplayFlush(lv_disp_drv_t *displayDriver, const lv_area_t *screenArea, lv_color_t *colorsPointer);

#ifdef DISPLAY_DIGITIZER
    void OnReadDigitizer(lv_indev_drv_t *indevDriver, lv_indev_data_t *data);
#endif



    void Initialize()
    {
      tft.begin();        
      tft.setRotation(1);
      tft.setBrightness(255);

#ifdef DISPLAY_DIGITIZER
      uint16_t calData[] = { 239, 3926, 233, 265, 3856, 3896, 3714, 308 };

      tft.setTouchCalibrate(calData);
#endif


      lv_init();
      lv_disp_draw_buf_init(&DisplayBuffer, ByteBuffer, NULL, ScreenWidth * 10);


      lv_disp_drv_init(&DisplayDriver);

      DisplayDriver.hor_res = ScreenWidth;
      DisplayDriver.ver_res = ScreenHeight;
      DisplayDriver.flush_cb = OnDisplayFlush;
      DisplayDriver.draw_buf = &DisplayBuffer;

      lv_disp_drv_register(&DisplayDriver);


#ifdef DISPLAY_DIGITIZER
      lv_indev_drv_init(&DigitizerDriver);

      DigitizerDriver.type = LV_INDEV_TYPE_POINTER;
      DigitizerDriver.read_cb = OnReadDigitizer;

      lv_indev_drv_register(&DigitizerDriver);
#endif


      LvglRenderEvent.Subscribe(OnLvglRenderEvent);
      Services::System::InvokeLater(&LvglRenderEvent, LvglRenderInterval_us, SchedulingBehaviour::FixedPeriodSkipTicks);
    }

    void OnLvglRenderEvent(void *args)
    {
      lv_timer_handler();
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

#ifdef DISPLAY_DIGITIZER
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
#endif

  } // namespace Display
} // namespace Services