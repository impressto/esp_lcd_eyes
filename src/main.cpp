#include <LovyanGFX.hpp>

// Create a display instance
class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_GC9A01 _panel_instance;
    lgfx::Bus_SPI      _bus_instance;

public:
    LGFX(void) {
        {   // SPI bus setup
            auto cfg = _bus_instance.config();

            cfg.spi_host   = SPI2_HOST;  // HSPI for ESP32
            cfg.spi_mode   = 0;
            cfg.freq_write = 40000000;   // 40MHz
            cfg.freq_read  = 16000000;   // 16MHz
            cfg.spi_3wire  = false;
            cfg.use_lock   = true;
            cfg.dma_channel = 1;

            cfg.pin_sclk = 18;  // SPI SCK
            cfg.pin_mosi = 23;  // SPI MOSI
            cfg.pin_miso = -1;  // Not used
            cfg.pin_dc   = 2;   // Data/Command

            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {   // Panel config
            auto cfg = _panel_instance.config();

            cfg.pin_cs    = 5;   // Chip select
            cfg.pin_rst   = 4;   // Reset
            cfg.pin_busy  = -1;

            cfg.memory_width  = 240;
            cfg.memory_height = 240;
            cfg.panel_width   = 240;
            cfg.panel_height  = 240;
            cfg.offset_x      = 0;
            cfg.offset_y      = 0;

            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance);
    }
};

LGFX tft;

void setup() {
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawCentreString("Hello GC9A01!", 120, 120, 4);

    tft.fillCircle(120, 120, 100, TFT_BLUE);
    tft.drawCircle(120, 120, 110, TFT_RED);
}

void loop() {
    static int r = 0;
    tft.fillCircle(120, 120, 50, tft.color565(r % 255, 100, 200));
    delay(100);
    r += 10;
}
