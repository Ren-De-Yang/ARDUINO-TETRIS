#ifndef config_h
#define config_h

#define TFT_CS 10
#define TFT_DC 8
#define TFT_RST 9

#define x1 4
#define x2 5
#define y1 6
#define y2 7
int px1=LOW;
int px2=LOW;
int py1=LOW;
int py2=LOW;

#define COLOR_GRAY        tft.color565(33, 33, 33)
#define COLOR_WHITE       ST7735_WHITE
#define COLOR_BLACK       ST7735_BLACK
#define COLOR_CYAN        ST7735_CYAN
#define COLOR_YELLOW      ST7735_YELLOW
#define COLOR_RED         ST7735_BLUE
#define COLOR_ORANGE      tft.color565(255, 165, 0)
#define COLOR_LIME        tft.color565(204, 255, 0)
#define COLOR_PURPLE      tft.color565(128,0,128)
#define COLOR_BLUE        ST7735_RED

#endif
