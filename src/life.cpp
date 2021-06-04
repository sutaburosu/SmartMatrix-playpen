/* https://forum.arduino.cc/t/the-fastest-way-to-move-a-block-of-ram/865841 */

#include "config.h"

#define PRINT_INFO(a) {}
// #define PRINT_INFO(a) Serial.println(a)

#define LIFE_TRAILS true
uint32_t seed = 322524;

const byte BYTES_X = kMatrixWidth / 8;
const uint16_t bytes = BYTES_X * kMatrixHeight;
byte grid[Y_RES][BYTES_X];
byte tempGrid[Y_RES][BYTES_X];


void paintRasterGrid(byte src[][BYTES_X], bool blue) {
  byte *grid = (byte *) src;
  for (uint16_t y = 0; y < Y_RES; y++) {
    for (uint16_t x = 0; x < X_RES; x += 8) {
      byte gridval = *grid++;
      for (uint8_t bitpos = 0; bitpos < 8; bitpos++) {
        if (gridval & 1) {
            backgroundLayer.drawPixel(x+bitpos, y, rgb24(255, 255, 255));
        } else {
          if (blue) {
            rgb24 tmp = backgroundLayer.readPixel(x+bitpos, y);
            backgroundLayer.drawPixel(x+bitpos, y, rgb24(0, 0, tmp.blue > 64 ? tmp.blue - 1 : tmp.blue));
          } else {
            backgroundLayer.drawPixel(x+bitpos, y, rgb24(0, 0, 0));
          }
        }
        gridval >>= 1;
      }
    }
  }
}


void randomFillArray(byte *gPtr, uint32_t seed) {
  PRINT_INFO("Seed: ");
  PRINT_INFO(seed);
  randomSeed(seed);
  for (int i = 0; i < X_RES / 8 * Y_RES; i++) {
    *gPtr++ = random(255) & random(255) & random(255);
  }
  paintRasterGrid(grid, false);
}


void randomFill() {
  randomFillArray((byte *) grid, seed);
}


void tempToGrid(void) {
  byte* gPtr;
  byte* tPtr;

  gPtr = (byte*) grid;
  tPtr = (byte*) tempGrid;
  memcpy(gPtr, tPtr, bytes);
}

void setGrid(byte grid[][BYTES_X], int x, int y, bool value) {

  int   xIndex;
  byte  xBit;

  if (x < 0 || x >= X_RES) return;
  if (y < 0 || y >= Y_RES) return;
  xIndex = x >> 3;
  // xBit = x - (xIndex << 3);
  xBit = x & 7;
  if (value) {
    bitSet(grid[y][xIndex], xBit);
  } else {
    bitClear(grid[y][xIndex], xBit);
  }
}


bool getGrid(byte grid[][BYTES_X], int x, int y) {

  int   xIndex;
  byte  xBit;
  bool  result;

  result = false;
  if (x < 0 || x >= X_RES) return result;
  if (y < 0 || y >= Y_RES) return result;
  xIndex = x >> 3;
  xBit = x & 7;
  result = (bool) bitRead(grid[y][xIndex], xBit);
  return result;
}


// Any live cell with two or three live neighbours survives.
// Any dead cell with three live neighbours becomes a live cell.
// All other live cells die in the next generation.
// Similarly, all other dead cells stay dead.
bool updatePoint(int x, int y) {
  byte  numLiveCells = 0;
  int   xMinus = x - 1;
  int   xPlus = x + 1;
  y--;
  numLiveCells = numLiveCells + (int)getGrid(grid, xMinus, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, x, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, xPlus, y);
  y++;
  numLiveCells = numLiveCells + (int)getGrid(grid, xMinus, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, xPlus, y);
  y++;
  numLiveCells = numLiveCells + (int)getGrid(grid, xMinus, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, x, y);
  numLiveCells = numLiveCells + (int)getGrid(grid, xPlus, y);
  y--;
  if (getGrid(grid, x, y)) {
    if (numLiveCells == 2 || numLiveCells == 3) {
      return true;
    } else {
      return false;
    }
  } else {
    if (numLiveCells == 3) {
      return true;
    }
  }
  return false;
}


// a terrible hash function, to quickly distinguish similar frames
uint32_t hash(byte * src, uint16_t length) {
  uint32_t total = 0;
  while (length--)
    total = (total ^ (total << 13)) + *src++ * (length & 0x3f);
  return total;
}


void stepTime(void) {
  bool  result;
  for (int y = 0; y < Y_RES; y++) {
    for (int x = 0; x < X_RES; x++) {
      result = updatePoint(x, y);
      setGrid(tempGrid, x, y, result);
    }
  }

  // detect looping states, up to max_loop + 1 frames in length
  const uint8_t max_loop = 14;
  static uint32_t framehash[max_loop];
  static uint16_t consecutives = 0;
  static uint32_t seed_frames = 0;
  seed_frames++;
  uint32_t newhash = hash((byte *) tempGrid, bytes);
  bool match = false;
  for (uint8_t i = 0; i < max_loop; i++)
    if (newhash == framehash[i])
      match = true;
  if (match) {
    consecutives++;
    if (consecutives >= 240) {
      PRINT_INFO(" frames: ");
      PRINT_INFO(seed_frames);
      // start with the next seed
      randomFillArray((byte *) tempGrid, ++seed);
      seed_frames = 0;
    }
    if (consecutives >= 500) {
      if (consecutives % 500 == 0) {
        PRINT_INFO(" Seed: ");
        PRINT_INFO(seed);
      }
      delay(250);
    }
  } else
    consecutives = 0;
  for (uint8_t i = 0; i < max_loop - 1; i++)
    framehash[i] = framehash[i+1];
  framehash[max_loop - 1] = newhash;
}


void life() {
  stepTime();
  paintRasterGrid(tempGrid, LIFE_TRAILS);
  tempToGrid();
}
