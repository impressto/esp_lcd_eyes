#include <Arduino.h>
#include <LovyanGFX.hpp>
#include "blue_spiral.h"
#include "hypnotic_spiral.h"
#include "rainbow_spiral.h"
#include "crazy_spiral.h"

// First display instance
class LGFX_Display1 : public lgfx::LGFX_Device
{
  lgfx::Panel_GC9A01 _panel_instance;
  lgfx::Bus_SPI _bus_instance;

public:
  LGFX_Display1(void)
  {
    { // SPI bus setup
      auto cfg = _bus_instance.config();

      cfg.spi_host = SPI2_HOST; // HSPI for ESP32
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000; // 40MHz
      cfg.freq_read = 16000000;  // 16MHz
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = 1;

      cfg.pin_sclk = 18; // SPI SCK
      cfg.pin_mosi = 23; // SPI MOSI
      cfg.pin_miso = -1; // Not used
      cfg.pin_dc = 2;    // Data/Command

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    { // Panel config
      auto cfg = _panel_instance.config();

      cfg.pin_cs = 5;  // Chip select
      cfg.pin_rst = 4; // Reset
      cfg.pin_busy = -1;

      cfg.memory_width = 240;
      cfg.memory_height = 240;
      cfg.panel_width = 240;
      cfg.panel_height = 240;
      cfg.offset_x = 0;
      cfg.offset_y = 0;

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};

// Second display instance
class LGFX_Display2 : public lgfx::LGFX_Device
{
  lgfx::Panel_GC9A01 _panel_instance;
  lgfx::Bus_SPI _bus_instance;

public:
  LGFX_Display2(void)
  {
    { // SPI bus setup
      auto cfg = _bus_instance.config();

      cfg.spi_host = SPI2_HOST; // Use same SPI host as first display
      cfg.spi_mode = 0;
      cfg.freq_write = 40000000; // 40MHz
      cfg.freq_read = 16000000;  // 16MHz
      cfg.spi_3wire = false;
      cfg.use_lock = true;
      cfg.dma_channel = 1; // Use same DMA channel as first display

      cfg.pin_sclk = 18; // Use same SPI SCK as first display
      cfg.pin_mosi = 23; // Use same SPI MOSI as first display
      cfg.pin_miso = -1; // Not used
      cfg.pin_dc = 27;   // Data/Command for second display

      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    { // Panel config
      auto cfg = _panel_instance.config();

      cfg.pin_cs = 15;  // Chip select for second display
      cfg.pin_rst = 26; // Reset for second display
      cfg.pin_busy = -1;

      cfg.memory_width = 240;
      cfg.memory_height = 240;
      cfg.panel_width = 240;
      cfg.panel_height = 240;
      cfg.offset_x = 0;
      cfg.offset_y = 0;

      _panel_instance.config(cfg);
    }

    setPanel(&_panel_instance);
  }
};

LGFX_Display1 tft1;
LGFX_Display2 tft2;

LGFX_Sprite sprite1(&tft1);
LGFX_Sprite sprite2(&tft2);

float display1_angle = 0.0;
float display2_angle = 0.0;
int centerX, centerY;

int cx, cy;

void spiral();

void drawSprite(LGFX_Sprite &sprite);
void drawImage();
void rotateSprite1();
void rotateSprite2();

int rotationCounter = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("LGFX start");

  // Initialize both displays
  tft1.init();
  tft2.init();

  tft1.invertDisplay(true);
  tft2.invertDisplay(true);

  tft1.setRotation(0);
  tft2.setRotation(0);

  tft1.fillScreen(TFT_BLACK);
  tft2.fillScreen(TFT_BLACK);

  // Display text on both screens
  tft1.setTextColor(TFT_WHITE, TFT_BLACK);
  tft2.setTextColor(TFT_WHITE, TFT_BLACK);

  tft1.drawCentreString("Display 1", 120, 120, 4);
  tft2.drawCentreString("Display 2", 120, 120, 4);

  delay(2000);

  // Serial.println("Drawing the images...");

  // fixes messed up color mapping - Set up both displays for image drawing
  tft1.setSwapBytes(true);
  tft2.setSwapBytes(true);

  // the sprite needs to be the max display witdh/height minus 1 pixel
  // othewise nothing will be drawn
  // sprite1.setPsram(true);
  sprite1.createSprite(239, 239);
  // call this BEFORE pushImage
  sprite1.setSwapBytes(true);
  sprite1.pushImage(0, 0, 240, 240, blue_spiral);
  // drawSprite(sprite1);

  // delay(1000);

  // the sprite needs to be the max display witdh/height minus 1 pixel
  // othewise nothing will be drawn
  // sprite2.setPsram(true);
  // sprite2.createSprite(239, 239);
  // call this BEFORE pushImage
  // sprite2.setSwapBytes(true);
  // sprite2.pushImage(0, 0, 240, 240, hypnotic_spiral);
}

void loop()
{

  if (rotationCounter % 12 == 0)
  {
    sprite1.pushImage(0, 0, 240, 240, crazy_spiral);
  }
  else if (rotationCounter % 9 == 0)
  {
    sprite1.pushImage(0, 0, 240, 240, hypnotic_spiral);
  }
  else if (rotationCounter % 6 == 0)
  {
    sprite1.pushImage(0, 0, 240, 240, rainbow_spiral);
  }
  else if (rotationCounter % 3 == 0)
  {
    sprite1.pushImage(0, 0, 240, 240, blue_spiral);
  }

  if (rotationCounter > 1000)
  {
    rotationCounter = 0;
  }

  // delay(10);
  rotateSprite1();

  delay(10);
}

void drawImage()
{

  tft1.startWrite();
  tft1.pushImage(0, 0, 240, 240, hypnotic_spiral);
  tft1.endWrite();

  tft2.startWrite();
  tft2.pushImage(0, 0, 240, 240, hypnotic_spiral);
  tft2.endWrite();

  Serial.println("Images drawn");
}

void drawSprite(LGFX_Sprite &sprite)
{

  // the sprite needs to be the max display witdh/height minus 1 pixel
  // othewise nothing will be drawn
  sprite.createSprite(239, 239);
  // call this BEFORE pushImage
  sprite.setSwapBytes(true);
  sprite.pushImage(0, 0, 240, 240, hypnotic_spiral);
}

void rotateSprite1()
{

  // Instead of sprite1.pushSprite(), rotate when pushing to TFT:
  sprite1.pushRotateZoom(
      &tft1,
      120, 120,       // rotate around screen center (for 240x240 display)
      display1_angle, // angle in degrees
      1.0, 1.0        // zoom factors (keep at 1.0 unless you want scaling)
  );

  display1_angle += 5.0; // slowly increase angle

  if (display1_angle >= 360)
  {
    display1_angle = 0;
    rotationCounter++;
  }

  Serial.println("Sprite1 drawn");
}

void rotateSprite2()
{

  // Instead of sprite1.pushSprite(), rotate when pushing to TFT:
  sprite2.pushRotateZoom(
      &tft2,
      120, 120,       // rotate around screen center (for 240x240 display)
      display2_angle, // angle in degrees
      1.0, 1.0        // zoom factors (keep at 1.0 unless you want scaling)
  );

  display2_angle += 5.0; // slowly increase angle
  if (display2_angle >= 360)
    display2_angle = 0;

  Serial.println("Sprite2 drawn");
}