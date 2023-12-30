#include <Arduino_GFX_Library.h>
#include <math.h>

Arduino_DataBus *bus = new Arduino_ESP32SPI(27 /* DC */, 5 /* CS */, 4 /* SCK */, 22 /* MOSI */, 16 /* MISO */);
Arduino_GFX *gfx = new Arduino_ILI9341(bus, 33, 3 /* rotation */, false /* IPS */);

#define TFT_BL 12

float length = 50;

float vertices[8][3] = {
  { -length, -length, -length },
  { length, -length, -length },
  { length, length, -length },
  { -length, length, -length },
  { -length, -length, length },
  { length, -length, length },
  { length, length, length },
  { -length, length, length }
};

int edges[12][2] = {
  { 0, 1 },
  { 1, 2 },
  { 2, 3 },
  { 3, 0 },
  { 4, 5 },
  { 5, 6 },
  { 6, 7 },
  { 7, 4 },
  { 0, 4 },
  { 1, 5 },
  { 2, 6 },
  { 3, 7 }
};

int centerX;
int centerY;

float angleX = 0.0;
float angleY = 0.0;
float angleZ = 0.0;

void setup() {
  gfx->begin();
  gfx->setRotation(3); 

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); 

  centerX = gfx->width() / 2;
  centerY = gfx->height() / 2;
}

void loop() {
  gfx->fillScreen(BLACK); 
  
  gfx->setTextSize(2);
  gfx->setTextColor(RED); 
  // 文字
  gfx->setCursor(centerX - 25, centerY - 20); 
  gfx->println("NEW");

  gfx->setCursor(centerX - 30, centerY + 10); 
  gfx->println("YEAR");
  // 圖形
  for (int i = 0; i < 12; i++) {
    int v0 = edges[i][0];
    int v1 = edges[i][1];

    float x0 = vertices[v0][0];
    float y0 = vertices[v0][1];
    float z0 = vertices[v0][2];
    float x1 = vertices[v1][0];
    float y1 = vertices[v1][1];
    float z1 = vertices[v1][2];

    rotateX(y0, z0, angleX);
    rotateX(y1, z1, angleX);
    rotateY(x0, z0, angleY);
    rotateY(x1, z1, angleY);
    rotateZ(x0, y0, angleZ);
    rotateZ(x1, y1, angleZ);

    drawLine(x0, y0, x1, y1);
  }

  delay(1);
  angleX++;
  angleY++;
  angleZ++;
  angleX = fmod(angleX, 360.0);
  angleY = fmod(angleY, 360.0);
  angleZ = fmod(angleZ, 360.0);
}

void drawLine(float x0, float y0, float x1, float y1) {
  gfx->drawLine(centerX + x0, centerY + y0, centerX + x1, centerY + y1, gfx->color565(0, 0, 255)); 
}

void rotateX(float& y, float& z, float angle) {
  float rad = angle * PI / 180.0;
  float new_y = y * cos(rad) - z * sin(rad);
  float new_z = z * cos(rad) + y * sin(rad);
  y = new_y;
  z = new_z;
}

void rotateY(float& x, float& z, float angle) {
  float rad = angle * PI / 180.0;
  float new_x = x * cos(rad) + z * sin(rad);
  float new_z = z * cos(rad) - x * sin(rad);
  x = new_x;
  z = new_z;
}

void rotateZ(float& x, float& y, float angle) {
  float rad = angle * PI / 180.0;
  float new_x = x * cos(rad) - y * sin(rad);
  float new_y = y * cos(rad) + x * sin(rad);
  x = new_x;
  y = new_y;
}
