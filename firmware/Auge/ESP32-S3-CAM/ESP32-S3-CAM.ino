#include "esp_camera.h"

#define STREAM_MODE_USB  1
#define STREAM_MODE_WIFI 2

#ifndef STREAM_MODE
#define STREAM_MODE STREAM_MODE_USB
#endif

#if STREAM_MODE == STREAM_MODE_USB
#include "USB.h"
#elif STREAM_MODE == STREAM_MODE_WIFI
#include <WiFi.h>
#include <WebServer.h>
#else
#error "Unsupported STREAM_MODE. Use STREAM_MODE_USB or STREAM_MODE_WIFI."
#endif

// =========================================================================
// 📌 PIN DEFINITIONS FOR ESP32-S3 WROOM CAM
// =========================================================================
#define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM     15
#define SIOD_GPIO_NUM      4
#define SIOC_GPIO_NUM      5

#define Y9_GPIO_NUM       16
#define Y8_GPIO_NUM       17
#define Y7_GPIO_NUM       18
#define Y6_GPIO_NUM       12
#define Y5_GPIO_NUM       10
#define Y4_GPIO_NUM        8
#define Y3_GPIO_NUM        9
#define Y2_GPIO_NUM       11

#define VSYNC_GPIO_NUM     6
#define HREF_GPIO_NUM      7
#define PCLK_GPIO_NUM     13

static camera_config_t camera_config = {
  .pin_pwdn        = PWDN_GPIO_NUM,
  .pin_reset       = RESET_GPIO_NUM,
  .pin_xclk        = XCLK_GPIO_NUM,
  .pin_sccb_sda    = SIOD_GPIO_NUM,
  .pin_sccb_scl    = SIOC_GPIO_NUM,
  .pin_d7          = Y9_GPIO_NUM,
  .pin_d6          = Y8_GPIO_NUM,
  .pin_d5          = Y7_GPIO_NUM,
  .pin_d4          = Y6_GPIO_NUM,
  .pin_d3          = Y5_GPIO_NUM,
  .pin_d2          = Y4_GPIO_NUM,
  .pin_d1          = Y3_GPIO_NUM,
  .pin_d0          = Y2_GPIO_NUM,
  .pin_vsync       = VSYNC_GPIO_NUM,
  .pin_href        = HREF_GPIO_NUM,
  .pin_pclk        = PCLK_GPIO_NUM,

  .xclk_freq_hz    = 20000000,
  .ledc_timer      = LEDC_TIMER_0,
  .ledc_channel    = LEDC_CHANNEL_0,

  .pixel_format    = PIXFORMAT_JPEG,
  .frame_size      = FRAMESIZE_VGA,
  .jpeg_quality    = 12,
  .fb_count        = 2,
  .fb_location     = CAMERA_FB_IN_PSRAM
};

#if STREAM_MODE == STREAM_MODE_WIFI
const char* ssid = "Greifbandit-CAM";
const char* password = "greifbandit123";
WebServer server(80);

static const char PROGMEM INDEX_HTML[] = R"rawl(
<html>
  <head>
    <title>Greifbandit Camera</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { background: #111; color: white; font-family: Arial, sans-serif; }
      img { width: 100%; max-width: 800px; border: 2px solid #444; }
    </style>
  </head>
  <body>
    <h2>Greifbandit Live Camera</h2>
    <img src="/stream" />
  </body>
</html>
)rawl";

void handleRoot() {
  server.send(200, "text/html", INDEX_HTML);
}

void handleStream() {
  Serial.println("STREAM: Request received");
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("STREAM: esp_camera_fb_get() failed");
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }

  Serial.printf("STREAM: Frame captured, len=%u\n", fb->len);
  server.sendHeader("Content-Type", "image/jpeg");
  server.sendHeader("Content-Disposition", "inline; filename=capture.jpg");
  server.sendHeader("Cache-Control", "no-store, no-cache, must-revalidate, max-age=0");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send_P(200, "image/jpeg", (const char *)fb->buf, fb->len);

  esp_camera_fb_return(fb);
}
#endif

void initCamera() {
  Serial.println("BOOT: Initializing camera...");
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK) {
    Serial.printf("FATAL: esp_camera_init() failed: 0x%x\n", err);
    while (true) { delay(1000); }
  }
  Serial.println("BOOT: Camera initialized OK");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Wait for the monitor to connect (up to 6 seconds) to prevent missing early logs
  unsigned long start_wait = millis();
  while (!Serial && (millis() - start_wait < 6000)) { 
    delay(10); 
  }
  delay(200); 

  Serial.println("BOOT: Starting Greifbandit Eye firmware");
  initCamera();

#if STREAM_MODE == STREAM_MODE_USB
  Serial.println("BOOT: USB Stream Mode selected");
  USB.begin();
  Serial.println("BOOT: USB interface ready");
#elif STREAM_MODE == STREAM_MODE_WIFI
  Serial.println("BOOT: WiFi Stream Mode selected");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println("BOOT: Access Point started");
  Serial.print("BOOT: AP IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/stream", handleStream);
  server.begin();
  Serial.println("BOOT: HTTP server running");
#endif
}

void loop() {
#if STREAM_MODE == STREAM_MODE_USB
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("USB: Frame capture failed");
    delay(100);
    return;
  }

  // Stream raw bytes directly to native USB CDC
  Serial.write(fb->buf, fb->len);
  esp_camera_fb_return(fb);
  delay(10); // Throttle for stable stream (~30 FPS)
#elif STREAM_MODE == STREAM_MODE_WIFI
  server.handleClient();
#endif
}
