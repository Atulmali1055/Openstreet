
#include <Arduino.h>
#include <WiFi.h>
#include <LovyanGFX.hpp>
#include <OpenStreetMap-esp32.hpp>

// ---------- Wi-Fi ----------
const char* ssid = "Room 203";
const char* password = "81494747";

// ---------- ILI9341 display configuration ----------
class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9341 panel;
  lgfx::Bus_SPI bus;

public:
  LGFX() {
    auto b = bus.config();
    b.spi_host = SPI3_HOST;
    b.spi_mode = 0;
    b.freq_write = 40000000;
    b.freq_read = 16000000;
    b.spi_3wire = false;
    b.use_lock = true;
    b.dma_channel = SPI_DMA_CH_AUTO;
    b.pin_sclk = 18;
    b.pin_mosi = 23;
    b.pin_miso = 19;
    b.pin_dc = 27;
    bus.config(b);
    panel.setBus(&bus);

    auto p = panel.config();
    p.pin_cs = 5;
    p.pin_rst = 4;
    p.pin_busy = -1;
    p.panel_width = 240;
    p.panel_height = 320;
    p.offset_x = 0;
    p.offset_y = 0;
    p.offset_rotation = 0;
    p.readable = false;
    p.invert = false;
    p.rgb_order = false;
    panel.config(p);

    setPanel(&panel);
  }
};

LGFX display;
OpenStreetMap osm;

// Example coordinates: London
double longitude = -0.09;
double latitude = 51.5;
uint8_t zoom = 12;

void setup() {
  Serial.begin(115200);
  delay(1000);

  display.init();
  display.setRotation(1);  // Landscape: 320 x 240
  display.fillScreen(TFT_BLACK);
  display.setTextColor(TFT_WHITE);
  display.setTextSize(2);
  display.setCursor(10, 10);
  display.println("Connecting WiFi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  display.fillScreen(TFT_BLACK);
  display.setCursor(10, 10);
  display.println("Fetching map...");

  LGFX_Sprite map(&display);

  // Requires suitable PSRAM for the map/cache.
  const bool success =
      osm.fetchMap(map, longitude, latitude, zoom);

  display.fillScreen(TFT_BLACK);

  if (success) {
    map.pushSprite(0, 0);
    Serial.println("Map displayed successfully");
  } else {
    display.setCursor(10, 10);
    display.println("Map fetch failed");
    Serial.println("Map fetch failed");
  }

  map.deleteSprite();
}

void loop() {
  delay(1000);
}
