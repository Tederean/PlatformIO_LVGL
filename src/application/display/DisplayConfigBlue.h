#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.h>

class LGFX : public lgfx::LGFX_Device
{

  lgfx::Panel_ILI9341 _panel_instance;

	lgfx::Bus_Parallel8 _bus_instance;

public:

  LGFX(void)
  {
    { 
      auto cfg = _bus_instance.config();

      cfg.i2s_port = I2S_NUM_0;
      cfg.freq_write = 20000000;
      cfg.pin_wr =  4;
      cfg.pin_rd =  2;
      cfg.pin_rs = 15;
      cfg.pin_d0 = 12;
      cfg.pin_d1 = 13;
      cfg.pin_d2 = 26;
      cfg.pin_d3 = 25;
      cfg.pin_d4 = 17;
      cfg.pin_d5 = 16;
      cfg.pin_d6 = 27;
      cfg.pin_d7 = 14;

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    {
      auto cfg = _panel_instance.config();

      cfg.pin_cs           =     5;
      cfg.pin_rst          =    33;
      cfg.pin_busy         =    -1;

      cfg.memory_width     =   240;
      cfg.memory_height    =   320;
      cfg.panel_width      =   240;
      cfg.panel_height     =   320;
      cfg.offset_x         =     0;
      cfg.offset_y         =     0;
      cfg.offset_rotation  =     0;
      cfg.dummy_read_pixel =     8;
      cfg.dummy_read_bits  =     1;
      cfg.readable         =  true;
      cfg.invert           = false;
      cfg.rgb_order        = false;
      cfg.dlen_16bit       = false;
      cfg.bus_shared       =  true;

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};
