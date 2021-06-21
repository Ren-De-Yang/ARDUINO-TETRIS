#include <Adafruit_ST7735.h>

#include "config.h"
#include "tetris.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

#define blue tft.color565(0, 88, 248)
#define cyan tft.color565(60, 188, 252)
#define forest tft.color565(0, 168, 0)
#define chartreuse tft.color565(184, 248, 24)
#define magenta tft.color565(216, 0, 204)
#define pink tft.color565(248, 120, 248)
#define green tft.color565(88, 216, 84)
#define pred tft.color565(228, 0, 88)
#define red tft.color565(248, 56, 0)
#define seafoam tft.color565(88, 248, 152)
#define cornflower tft.color565(104, 136, 252)
#define orange tft.color565(168, 0, 32)
#define gray tft.color565(124, 124, 124)
#define purple tft.color565(104, 68, 252)
#define burgundy tft.color565(168, 0, 32)
int palette[10][2]={
  {blue,cyan},
  {forest, chartreuse},
  {magenta, pink},
  {blue, green},
  {pred, seafoam},
  {seafoam, cornflower},
  {red, gray},
  {purple, burgundy},
  {blue, red},
  {red, orange}
};


void centerWrite(String text, byte yPos, uint16_t color);
void waitForClick();
void (* resetFunc)(void)= 0;
void setup() {
  //init
  Serial.begin(9600);
  pinMode(y1, INPUT_PULLUP);
  pinMode(y2, INPUT_PULLUP);
  pinMode(x1, INPUT_PULLUP);
  pinMode(x2, INPUT_PULLUP);

  randomSeed(analogRead(A0));
  
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST7735_BLACK);
  //start
  unsigned short txtYpos = (tft.height() / 2) - 30;
  centerWrite("TETRIS", txtYpos, COLOR_RED);
  centerWrite("PUSH START", txtYpos + 70, COLOR_WHITE);
  centerWrite("(c) 1989 Nintendo", txtYpos + 100, COLOR_WHITE);
  waitForClick();

  //draw game
  tft.fillScreen(COLOR_BLACK);
  tft.drawRect(BOARD_OFFSET_X,  BOARD_OFFSET_Y, 1 + (BOARD_WIDTH * BLOCK_SIZE), 1+(BOARD_HEIGHT * BLOCK_SIZE), 0XFFFF);
  initline();
  initnext();
  initlv();
  initscore();
  
  //init game
  drawline();
  drawlv();
  changeShape();
}

int softdrop=0;
void loop() {
  
  //changeShape();
  wait++;
  if((wait==fps()) or  ((wait>=2) and (digitalRead(y2) == LOW) and drop)){
    if(gravity()){
      if(((wait>=2) and (digitalRead(y2) == LOW) and drop)){
        softdrop++;
      }
      currentY++;
      clearShape(lastShape, lastRotation, lastX, lastY);
      
      for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 4; j++) {
          if (bitRead(shapes[currentShape][currentRotation][j], 4-i) == 1) {
            fillBlock(currentX+i, currentY+j, getcolor());
          }
        }
      }
      
      lastX = currentX;
      lastY = currentY;
      lastRotation = currentRotation;
      lastShape = currentShape;
    }else if(!gravity() and currentY<=0){
      tft.fillScreen(COLOR_BLACK);
      resetFunc();
    }
    wait=0;
  }
  //lock soft drop
  if(digitalRead(y2) != py2){
    drop=true;
  }
  py2=digitalRead(y2);
  
  //LEFT
  if(left() and (digitalRead(y1) == LOW) and (py1==HIGH) and (currentY>-3)){
    
    clearShape(lastShape, lastRotation, lastX, lastY);
    currentX--;
    for (int x = 0; x <= 4; x++) {
      for (int y = 0; y <= 4; y++) {
        if (bitRead(shapes[currentShape][currentRotation][y], 4-x) == 1) {
          fillBlock(currentX+x, currentY+y, getcolor());
        }
      }
    }
    
      
    lastX = currentX;
    lastY = currentY;
    lastRotation = currentRotation;
    lastShape = currentShape;
  }
  py1=digitalRead(y1);
  
  //right
  if(right() and (digitalRead(x1) == LOW) and (px1==HIGH) and (currentY>-3)){
    
    clearShape(lastShape, lastRotation, lastX, lastY);
    currentX++;
    for (int x = 0; x <= 4; x++) {
      for (int y = 0; y <= 4; y++) {
        if (bitRead(shapes[currentShape][currentRotation][y], 4-x) == 1) {
          fillBlock(currentX+x, currentY+y, getcolor());
        }
      }
    }
    
      
    lastX = currentX;
    lastY = currentY;
    lastRotation = currentRotation;
    lastShape = currentShape;
  }
  px1=digitalRead(x1);

  //changerotation
  if((digitalRead(x2)==LOW) and changerotation() and (px2==HIGH) and gravity()){
    currentRotation++;
    if(currentRotation==shapeRotations[currentShape]){
      currentRotation=0;
    }
    
    clearShape(lastShape, lastRotation, lastX, lastY);
      
    for (int i = 0; i <= 4; i++) {
      for (int j = 0; j <= 4; j++) {
        if (bitRead(shapes[currentShape][currentRotation][j], 4-i) == 1) {
          fillBlock(currentX+i, currentY+j, getcolor());
        }
      }
    }
      
    lastX = currentX;
    lastY = currentY;
    lastRotation = currentRotation;
    lastShape = currentShape;
  }
  px2=digitalRead(x2);

  
  
  delay(1000/60.0988);
}

void centerWrite(String text, byte yPos, uint16_t color) {
  tft.setCursor((tft.width() / 2) - (text.length() * 3), yPos);
  tft.setTextColor(color);
  tft.print(text);
}
void waitForClick() {
  while (true) {
    delay(50);
    if ((digitalRead(x1) == LOW) or (digitalRead(x2) == LOW) or (digitalRead(y1) == LOW) or (digitalRead(y2) == LOW)) {
      return;
    }
  }
}

void initline(){
  tft.fillRect(80, 130, 40, 25, COLOR_BLACK);
  tft.drawRect(80, 130, 40, 25, COLOR_WHITE);
  tft.setCursor(85, 135);
  tft.print("LINE");
}
void drawline(){
  tft.fillRect(85, 145, 18, 8, COLOR_BLACK);
  tft.setCursor(85, 145);
  if(line<10){
    tft.print("00");
    tft.print(line);
  }else if(line<100){
    tft.print("0");
    tft.print(line);
  }else{
    tft.print(line);
  }
}
void initnext(){
  tft.fillRect(80, 85, 40, 35, COLOR_BLACK);
  tft.drawRect(80, 85, 40, 35, COLOR_WHITE);
  tft.setCursor(85, 90);
  tft.print("NEXT");
}
void drawNext(){
  tft.fillRect(85, 103, 2+(4 * BLOCK_SIZE), 2 * BLOCK_SIZE, COLOR_BLACK);
  for (byte i = 0; i < 5; i++) {
    for (short j = 4, x = 0; j != -1; j--, x++) {
      if (bitRead(shapes[nextShape][initRotations[nextShape]][i], j) == 1) {
        tft.fillRect(85 + (x * BLOCK_SIZE), 90 + (i * BLOCK_SIZE), BLOCK_SIZE - 1, BLOCK_SIZE - 1, 0xFFFF);
      }
    }
  }
}

void initlv(){
  tft.fillRect(80, 60, 40, 15, COLOR_BLACK);
  tft.drawRect(80, 60, 40, 15, COLOR_WHITE);
  tft.setCursor(85, 65);
  tft.print("LV ");
}
void drawlv(){
  tft.fillRect(105, 62, 12, 12, COLOR_BLACK);
  tft.setCursor(105, 65);
  if(level<10){
    tft.print("0");
    tft.print(level);
  }else if(level<100){
    tft.print(level);
  }
}
void initscore(){
  tft.fillRect(1, 1, 75, 10, COLOR_BLACK);
  tft.setCursor(1,1);
  tft.print("score ");
}
void drawscore(){
  tft.fillRect(40, 1, 35, 10, COLOR_BLACK);
  tft.setCursor(40,1);
  tft.print(score);
}

void fillBlock(byte x, byte y, uint16_t color) {
  grid[x][y] = color;
  tft.fillRect(1 + BOARD_OFFSET_X + (x * BLOCK_SIZE), 1 + BOARD_OFFSET_Y + (y * BLOCK_SIZE), BLOCK_SIZE - 1, BLOCK_SIZE - 1, color);
}

int history;
int randomShape(){
  int t = random(0, SHAPE_COUNT);
  
  if(t==history){
    t = random(SHAPE_COUNT);
  }
  history=t;
  return t;
}
void changeShape(){
  currentShape = nextShape;
  currentRotation = initRotations[currentShape];
  nextShape = randomShape();
  currentX = 3;
  currentY = -3;
  
  lastX = -5;
  lastY = -5;
  lastRotation = 0;
  lastShape = 0;

  
  clearline();
  drawNext();
  drop=false;
  score+=softdrop;
  softdrop=0;
  drawscore();
  drawlv();
  
}
void clearline(){
  bool next = false;
  bool temp = false;
  int lineforone=0;
  for(int i=BOARD_HEIGHT-1; i>=0; i--){
    next = false;
    for(int j=0; j<BOARD_WIDTH; j++){
      if(grid[j][i]==0x0000){
        next = true;
        break;
      }
    }
    if(!next){
      temp=true;
      for(int j=0; j<BOARD_WIDTH; j++){
        grid[j][i]=0x0000;
      }
      for(int y=i; y>0; y--){
        for(int j=0; j<BOARD_WIDTH; j++){
          grid[j][y]=grid[j][y-1];
        }
      }
      line++;
      lvclr++;
      i=BOARD_HEIGHT;
      lineforone++;
    }
  }
  
  if(temp){
    for(int i=BOARD_HEIGHT-1; i>0; i--){
      for(int j=0; j<BOARD_WIDTH; j++){
        fillBlock(j, i, grid[j][i]);
      }
    }
    if(lineforone==1){
      score=score+(40*(level+1));
    }else if(lineforone==2){
      score=score+(100*(level+1));
    }else if(lineforone==3){
      score=score+(300*(level+1));
    }else if(lineforone==4){
      score=score+(1200*(level+1));
    }
    drawline();
    levelup();
    drawlv();
    drawscore();
  }
}
void clearShape(int bitmap, int rotation, int x, int y){
  for (int i = 4; i >= 0; i--) {
    for (int j = 4; j >= 0; j--) {
      if (bitRead(shapes[bitmap][rotation][j], 4-i) == 1) {
        fillBlock(x+i, y+j, 0x0000);
      }
    }
  }
}

bool gravity(){
  for (int i = 4; i >= 0; i--) {
    for (int j = 4; j >= 0; j--) {
      if(bitRead(shapes[currentShape][currentRotation][j], 4-i) == 1){
        if(grid[currentX+i][currentY+j+1]!=0x0000 and bitRead(shapes[currentShape][currentRotation][j+1], 4-i) == 0x0000){
          changeShape();
          return false;
        }else if(currentY+j+1==BOARD_HEIGHT){
          changeShape();
          return false;
        }
      }
    }
  }
  return true;
}
bool left(){
  for (int i = 4; i >= 0; i--) {
    for (int j = 4; j >= 0; j--) {
      if(bitRead(shapes[currentShape][currentRotation][j], 4-i) == 1){
        if(grid[currentX+i-1][currentY+j]!=0x0000 and bitRead(shapes[currentShape][currentRotation][j], 4-i+1) == 0x0000){
          return false;
        }else if(currentX+i-1==-1){
          return false;
        }
      }
    }
  }
  return true;
}
bool right(){
  for (int i = 4; i >= 0; i--) {
    for (int j = 4; j >= 0; j--) {
      if(bitRead(shapes[currentShape][currentRotation][j], 4-i) == 1){
        if(grid[currentX+i+1][currentY+j]!=0x0000 and bitRead(shapes[currentShape][currentRotation][j], 4-i-1) == 0x0000){
          return false;
        }else if(currentX+i+1==BOARD_WIDTH){
          return false;
        }
      }
    }
  }
  return true;
}
bool changerotation(){
  int t=currentRotation;
  t++;
  if(t==shapeRotations[currentShape]){
    t=0;
  }
  for (int i = 4; i >= 0; i--) {
    for (int j = 4; j >= 0; j--) {
      if(bitRead(shapes[currentShape][t][j], 4-i) == 1){
        if(grid[currentX+i][currentY+j]!=0x0000 and bitRead(shapes[currentShape][t][j], 4-i) == 0x0000){
          return false;
        }else if((currentX+i<0) or (currentX+i>=BOARD_WIDTH) or (currentY+j>=BOARD_HEIGHT) or (currentY+j<0)){
          return false;
        }
      }
    }
  }
  return true;
}

int getcolor(){
  int t;
  switch(currentShape){
    case 0: case 1: case 3: case 4: case 6:
      t=0;
      break;
    case 2: case 5:
      t=1;
      break;
  }
  return palette[level%10][t];
}
