#include <Arduino.h>

#include <lvgl.h>
#include <application/gui/GuiService.h>
#include <framework/services/SystemService.h>
#include <framework/common/Event.h>

using namespace std;

namespace Services
{
  namespace Gui
  {

    const timespan_t ArcUpdateInterval_us = 1 * 1000 * 1000;


    Event<void> ArcUpdateEvent;


    lv_obj_t *Arc;


    void Initialize();

    void OnArcUpdateEvent(void *args);

    void DrawLvglObjects();


    void Initialize()
    {
      DrawLvglObjects();

      ArcUpdateEvent.Subscribe(OnArcUpdateEvent);
      Services::System::InvokeLater(&ArcUpdateEvent, ArcUpdateInterval_us, SchedulingBehaviour::FixedPeriodSkipTicks);
    }

    void OnArcUpdateEvent(void *args)
    {
      auto nextValue = random(0, 105);

      lv_arc_set_value(Arc, nextValue);
    }

    void DrawLvglObjects()
    {
      auto *display = lv_disp_get_default();
      auto *screen = lv_disp_get_scr_act(display);
      auto gray = lv_color_make(204, 204, 204);

      // This sh*t does not work...
      lv_disp_set_bg_color(display, gray);


      Arc = lv_arc_create(screen);

      lv_obj_remove_style(Arc, NULL, LV_PART_KNOB);

      lv_obj_set_size(Arc, 200, 200);
      lv_arc_set_end_angle(Arc, 200);
      lv_obj_align(Arc, LV_ALIGN_CENTER, 0, 0);

      lv_arc_set_range(Arc, 0, 105);
      lv_arc_set_value(Arc, 75);
    }
    
  } // namespace Display
} // namespace Services