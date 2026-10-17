#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341 _panel;
  lgfx::Bus_SPI       _bus;

public:
  LGFX() {
    // SPI bus configuration
    {
      auto cfg = _bus.config();
      cfg.spi_host    = SPI2_HOST;
      cfg.spi_mode    = 0;
      cfg.freq_write  = 40000000;
      cfg.freq_read   = 16000000;
      cfg.spi_3wire   = false;
      cfg.use_lock    = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk    = 12;   // TFT_CLK
      cfg.pin_mosi    = 11;   // TFT_MOSI
      cfg.pin_miso    = 13;   // TFT_MISO
      cfg.pin_dc      = 8;    // TFT_DC
      _bus.config(cfg);
      _panel.setBus(&_bus);
    }
    // Panel configuration
    {
      auto cfg = _panel.config();
      cfg.pin_cs   = 10;      // TFT_CS
      cfg.pin_rst  = 9;       // TFT_RST
      cfg.pin_busy = -1;
      cfg.memory_width  = 240;
      cfg.memory_height = 320;
      cfg.panel_width   = 240;
      cfg.panel_height  = 320;
      cfg.offset_x      = 0;
      cfg.offset_y      = 0;
      cfg.offset_rotation = 0;
      _panel.config(cfg);
    }
    setPanel(&_panel);
  }
};

LGFX display; // Global display object