/* jimLee - 2021
  https://forum.arduino.cc/t/the-fastest-way-to-move-a-block-of-ram/865841
*/

#pragma once
#include "../config.h"

class Life : public Effect {
private:
  bool trails          = true;
  uint32_t seed        = 322524;
  const byte BYTES_X   = kMatrixWidth / 8;
  const uint16_t bytes = BYTES_X * kMatrixHeight;
  byte* grid;
  byte* tempGrid;

public:
  Life() {
    name     = (char*)F("Life");
    grid     = (byte*)malloc(bytes);
    tempGrid = (byte*)malloc(bytes);
    randomFill();
  }

  virtual ~Life() {
    free(grid);
    free(tempGrid);
  }

  virtual void drawFrame() {
    stepTime();
    paintRasterGrid(tempGrid, trails);
    tempToGrid();
  }

private:
  void paintRasterGrid(byte* grid, bool show_trails) {
    for (uint16_t y = 0; y < kMatrixHeight; y++) {
      for (uint16_t x = 0; x < kMatrixWidth; x += 8) {
        byte gridval = *grid++;
        for (uint8_t bitpos = 0; bitpos < 8; bitpos++) {
          if (gridval & 1) {
            backgroundLayer->drawPixel(x + bitpos, y, rgb24(255, 255, 255));
          } else {
            if (show_trails) {
              rgb24 tmp = backgroundLayer->readPixel(x + bitpos, y);
              backgroundLayer->drawPixel(x + bitpos, y, rgb24(0, 0, tmp.blue > 64 ? tmp.blue - 1 : tmp.blue));
            } else {
              backgroundLayer->drawPixel(x + bitpos, y, rgb24(0, 0, 0));
            }
          }
          gridval >>= 1;
        }
      }
    }
  }

  void randomFillArray(byte* gPtr, uint32_t seed) {
    // PRINT_INFO("Seed: ");
    // PRINT_INFO(seed);
    randomSeed(seed);
    for (int i = 0; i < kMatrixWidth / 8 * kMatrixHeight; i++) {
      *gPtr++ = random(255) & random(255) & random(255);
    }
    paintRasterGrid(grid, false);
  }

  void randomFill() {
    randomFillArray(grid, seed);
  }

  void tempToGrid(void) {
    memcpy(grid, tempGrid, bytes);
  }

  void setGrid(byte* grid, int x, int y, bool value) {

    int xIndex;
    byte xBit;

    if (x < 0 || x >= kMatrixWidth) return;
    if (y < 0 || y >= kMatrixHeight) return;
    xIndex = x >> 3;
    // xBit = x - (xIndex << 3);
    xBit = x & 7;
    if (value) {
      bitSet(grid[y * BYTES_X + xIndex], xBit);
    } else {
      bitClear(grid[y * BYTES_X + xIndex], xBit);
    }
  }

  bool getGrid(byte* grid, int x, int y) {

    int xIndex;
    byte xBit;
    bool result;

    result = false;
    if (x < 0 || x >= kMatrixWidth) return result;
    if (y < 0 || y >= kMatrixHeight) return result;
    xIndex = x >> 3;
    xBit   = x & 7;
    result = (bool)bitRead(grid[y * BYTES_X + xIndex], xBit);
    return result;
  }

  // Any live cell with two or three live neighbours survives.
  // Any dead cell with three live neighbours becomes a live cell.
  // All other live cells die in the next generation.
  // Similarly, all other dead cells stay dead.
  bool updatePoint(int x, int y) {
    byte numLiveCells = 0;
    int xMinus        = x - 1;
    int xPlus         = x + 1;
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
  uint32_t hash(byte* src, uint16_t length) {
    uint32_t total = 0;
    while (length--)
      total = (total ^ (total << 13)) + *src++ * (length & 0x3f);
    return total;
  }

  void stepTime(void) {
    bool result;
    for (int y = 0; y < kMatrixHeight; y++) {
      for (int x = 0; x < kMatrixWidth; x++) {
        result = updatePoint(x, y);
        setGrid(tempGrid, x, y, result);
      }
    }

    // detect looping states, up to max_loop + 1 frames in length
    const uint8_t max_loop = 14;
    static uint32_t framehash[max_loop];
    static uint16_t consecutives = 0;
    static uint32_t seed_frames  = 0;
    seed_frames++;
    uint32_t newhash = hash(tempGrid, bytes);
    bool match       = false;
    for (uint8_t i = 0; i < max_loop; i++)
      if (newhash == framehash[i])
        match = true;
    if (match) {
      consecutives++;
      if (consecutives >= 240) {
        // PRINT_INFO(" frames: ");
        // PRINT_INFO(seed_frames);
        // start with the next seed
        randomFillArray(tempGrid, ++seed);
        seed_frames = 0;
      }
      if (consecutives >= 500) {
        if (consecutives % 500 == 0) {
          // PRINT_INFO(" Seed: ");
          // PRINT_INFO(seed);
        }
        delay(250);
      }
    } else
      consecutives = 0;
    for (uint8_t i = 0; i < max_loop - 1; i++)
      framehash[i] = framehash[i + 1];
    framehash[max_loop - 1] = newhash;
  }
};