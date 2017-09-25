#include "myLib.h"

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color) {

	videoBuffer[OFFSET(row, col, SCREENWIDTH)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
    for (int r = 0; r < height; r++) {
    	for (int c = 0; c < width; c++) {
    		setPixel(row + r, col + c, color);
    	}
    }
}

void fillScreen(unsigned short color) {
	for (int i = 0; i < 38400; i++) {
		videoBuffer[i] = color;
	}
}

void waitForVBlank() {

	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

int collision(int rowA, int colA, int heightA, int widthA, int rowB, int colB, int heightB, int widthB) {

	if (((colB <= colA + widthA) && (colB + widthB >= colA)) && ((rowB <= rowA + heightA) && (rowB + heightB >= rowA))) {
		return 1;
	}
	
	return 0;
}