#pragma once

#define LGFX_USE_V1

#include <LovyanGFX.h>

class LGFX : public lgfx::LGFX_Device
{

  lgfx::Panel_ILI9341 _panel_instance;

	lgfx::Bus_Parallel8 _bus_instance;

  lgfx::Light_PWM _light_instance;

	lgfx::Touch_XPT2046 _touch_instance;

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
      auto cfg = _panel_instance.config();    // 表示パネル設定用の構造体を取得します。

      cfg.pin_cs           =     5;  // CSが接続されているピン番号   (-1 = disable)
      cfg.pin_rst          =    33;  // RSTが接続されているピン番号  (-1 = disable)
      cfg.pin_busy         =    -1;  // BUSYが接続されているピン番号 (-1 = disable)

      cfg.memory_width     =   240;  // ドライバICがサポートしている最大の幅
      cfg.memory_height    =   320;  // ドライバICがサポートしている最大の高さ
      cfg.panel_width      =   240;  // 実際に表示可能な幅
      cfg.panel_height     =   320;  // 実際に表示可能な高さ
      cfg.offset_x         =     0;  // パネルのX方向オフセット量
      cfg.offset_y         =     0;  // パネルのY方向オフセット量
      cfg.offset_rotation  =     0;  // 回転方向の値のオフセット 0~7 (4~7は上下反転)
      cfg.dummy_read_pixel =     8;  // ピクセル読出し前のダミーリードのビット数
      cfg.dummy_read_bits  =     1;  // ピクセル以外のデータ読出し前のダミーリードのビット数
      cfg.readable         =  true;  // データ読出しが可能な場合 trueに設定
      cfg.invert           = false;  // パネルの明暗が反転してしまう場合 trueに設定
      cfg.rgb_order        = false;  // パネルの赤と青が入れ替わってしまう場合 trueに設定
      cfg.dlen_16bit       = false;  // データ長を16bit単位で送信するパネルの場合 trueに設定
      cfg.bus_shared       =  true;  // SDカードとバスを共有している場合 trueに設定(drawJpgFile等でバス制御を行います)

      _panel_instance.config(cfg);
    }

    //{
    //  auto cfg = _light_instance.config();    // バックライト設定用の構造体を取得します。
    //
    //  cfg.pin_bl = 32;              // バックライトが接続されているピン番号
    //  cfg.invert = false;           // バックライトの輝度を反転させる場合 true
    //  cfg.freq   = 44100;           // バックライトのPWM周波数
    //  cfg.pwm_channel = 7;          // 使用するPWMのチャンネル番号

    //  _light_instance.config(cfg);
    //  _panel_instance.setLight(&_light_instance);  // バックライトをパネルにセットします。
    //}

    {
      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;    // タッチスクリーンから得られる最小のX値(生の値)
      cfg.x_max      = 239;  // タッチスクリーンから得られる最大のX値(生の値)
      cfg.y_min      = 0;    // タッチスクリーンから得られる最小のY値(生の値)
      cfg.y_max      = 319;  // タッチスクリーンから得られる最大のY値(生の値)
      cfg.pin_int    = 38;   // INTが接続されているピン番号
      cfg.bus_shared = true; // 画面と共通のバスを使用している場合 trueを設定
      cfg.offset_rotation = 0;// 表示とタッチの向きのが一致しない場合の調整 0~7の値で設定

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};
