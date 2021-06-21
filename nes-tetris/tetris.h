#ifndef tetris_h
#define tetris_h

#define BOARD_WIDTH       10
#define BOARD_HEIGHT      20
#define BOARD_OFFSET_X    2
#define BOARD_OFFSET_Y    17
#define MIN(X, Y)         (((X) < (Y)) ? (X) : (Y))
#define BLOCK_SIZE        MIN((tft.width() - 1) / BOARD_WIDTH, (tft.height() - 1) / BOARD_HEIGHT)


//level
unsigned int level = 0;
unsigned int lvclr = 0;
void levelup(){
  if(level<10 and lvclr>=(level+1)*10){
    lvclr-=(level+1)*10;
    level++;
  }else if(level>=10 and level<16 and lvclr>=100){
    lvclr-=100;
    level++;
  }else if(level>=16 and level<26 and lvclr>=100+(level-15)*10){
    lvclr-=(100+(level-15)*10);
    level++;
  }else if(level>=26 and lvclr>=200){
    lvclr-=200;
    level++;
  }
}

//fps
unsigned int wait = 0;
unsigned int fps(){
  if(level<9){
    return 48-level*5;
  }else if(level==9){
    return 6;
  }else if(level>=10 and level<=18){
    return 5-(level-10)/3;
  }else if(level>=19 and level<=28){
    return 2;
  }else{
    return 1;
  }
}
bool drop=true;

unsigned int line = 0;
unsigned int score = 0;
uint16_t unsigned grid[BOARD_WIDTH][BOARD_HEIGHT] = {0x0000};
int currentShape;
int currentRotation;
int nextShape;
int currentX = 2;
int currentY = -3;
int lastX;
int lastY;
int lastShape;
int lastRotation;


#define SHAPE_COUNT  7
const byte shapeRotations[] = { 4, 4, 2, 1, 2, 4, 2 };
const byte initRotations[] = { 2, 3, 0, 0, 0, 1, 1 };
const byte shapes[SHAPE_COUNT][5][5] = {
  { // SHAPE_T
    {
      B00000,
      B00100,
      B01110,
      B00000,
      B00000
    },
    {
      B00000,
      B00100,
      B00110,
      B00100,
      B00000
    },
    {
      B00000,
      B00000,
      B01110,
      B00100,
      B00000
    },
    {
      B00000,
      B00100,
      B01100,
      B00100,
      B00000
    }
  },
  { // SHAPE_J
    {
      B00000,
      B00100,
      B00100,
      B01100,
      B00000
    },
    {
      
      B00000,
      B01000,
      B01110,
      B00000,
      B00000
    },
    {
      B00000,
      B00110,
      B00100,
      B00100,
      B00000
    },
    {
      B00000,
      B00000,
      B01110,
      B00010,
      B00000
    }
  },
   { // SHAPE_Z
    {
      B00000,
      B00000,
      B01100,
      B00110,
      B00000
    },
    {
      B00000,
      B00010,
      B00110,
      B00100,
      B00000
    }
  },
  { // SHAPE_O
    {
      B00000,
      B00000,
      B01100,
      B01100,
      B00000
    }
  },
  { // SHAPE_S
    {
      B00000,
      B00000,
      B00110,
      B01100,
      B00000
    },
    {
      B00000,
      B00100,
      B00110,
      B00010,
      B00000
    }
  },
  { // SHAPE_L
    {
      B00000,
      B00100,
      B00100,
      B00110,
      B00000
    },
    {
      B00000,
      B00000,
      B01110,
      B01000,
      B00000
    },
    {
      B00000,
      B01100,
      B00100,
      B00100,
      B00000
    },
    {
      B00000,
      B00010,
      B01110,
      B00000,
      B00000
    }
  },
  { // SHAPE_I
    {
      B00100,
      B00100,
      B00100,
      B00100,
      B00000
    },
    {
      B00000,
      B00000,
      B11110,
      B00000,
      B00000
    }
  }
};

#endif
