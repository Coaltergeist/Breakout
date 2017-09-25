#include "myLib.h"
#include <math.h>

// Prototypes
void initialize();
void update();
void draw();

// Button Variables
unsigned short buttons;
unsigned short oldButtons;

// Background Variables
unsigned short bgColor;

struct rectangleStruct paddle;
struct rectangleStruct ball;
struct rectangleStruct bar[18];
struct rectangleStruct barGone;

int points;

int initSpeedX;
int initSpeedY;

int speedX;
int speedY;
int starting;

int currentCol;
int currentRow;

int main() {

	initialize();

	while(1) {

		oldButtons = buttons;
		buttons = BUTTONS;

		update();
		waitForVBlank();
		draw();
	}
}

// Sets up the display and the game objects
void initialize() {

	REG_DISPCTL = MODE3 | BG2_ENABLE;

	buttons = BUTTONS;
	oldButtons = BUTTONS;

	bgColor = CYAN;
	fillScreen(bgColor);

	points = 0;

	initSpeedX = 1;
	initSpeedY = 1;
	starting = 1;

	speedX = 0;
	speedY = 0;

	currentRow = 0;
	currentCol = 0;

	// Initialize paddle
	paddle.row = 141;  //141
	paddle.col = 80;
	paddle.oldRow = paddle.row;
	paddle.oldCol = paddle.col;
	paddle.rDel = 0;
	paddle.cDel = 0;
	paddle.height = 10;
	paddle.width = 80;

	// Initialize ball
	ball.row = 76;
	ball.col = 116;
	ball.oldRow = paddle.row;
	ball.oldCol = paddle.col;
	ball.rDel = 0;
	ball.cDel = 0;
	ball.height = 8;
	ball.width = 8;

	// Initialize the bars
	for (int j = 0; j < 18; j++) {

		bar[j].height = 5;
		bar[j].width = 40;
		bar[j].col = currentCol;
		bar[j].row = currentRow;

		bar[j].isDead = 0;

		currentCol += 40;

		if ((j + 1) % 6 == 0) {
			currentCol = 0;
			currentRow += 5;
		}
	}

	//initialize the empty bar

	barGone.row = 0;
	barGone.col = 0;
	barGone.height = 0;
	barGone.width = 0;
	barGone.cDel = 0;
	barGone.rDel = 0;
	barGone.oldRow = 0;
	barGone.oldCol = 0;

}

// Performs all of the game's calculations
void update() {

	// starts/pauses the game if Start is pressed

	initSpeedX = initSpeedX *= -1;


	if (BUTTON_PRESSED(BUTTON_START)) {
		if (ball.rDel == 0) {

			paddle.rDel = 0;
			paddle.cDel = 2;

			ball.rDel = speedY;
			ball.cDel = speedX;

			if (starting) {
				ball.cDel = initSpeedX;
				ball.rDel = 1;
				starting = 0;
			}

		}
		else {

			speedY = ball.rDel;
			speedX = ball.cDel;

			paddle.rDel = 0;
			paddle.cDel = 0;
			
			ball.rDel = 0;
			ball.cDel = 0;

		}
	}

	// Move the paddle with the buttons

	if (BUTTON_HELD(BUTTON_RIGHT)) {
		if (paddle.col <= 159) {
			paddle.col += paddle.cDel;
		}
	}

	if (BUTTON_HELD(BUTTON_LEFT)) {
		if (paddle.col >= 1) {
			paddle.col -= paddle.cDel;
		}
	}


	// Bounce ball off the walls
	if (ball.row <= 0 || ball.row + ball.height - 1 >= SCREENHEIGHT - 1)
		ball.rDel *= -1;
	if (ball.col <= 0 || ball.col + ball.width - 1 >= SCREENWIDTH - 1)
		ball.cDel *= -1;

	// Bounce ball off the paddle

	if (collision(ball.row, ball.col, ball.height, ball.width, paddle.row, paddle.col + 1, 1, paddle.width - 2)) {
		ball.rDel *= -1;
	}
	else if (collision(ball.row, ball.col, ball.height, ball.width, paddle.row + 1, paddle.col, paddle.height, paddle.width)) {
		ball.cDel *= -1;
	}

	if (collision(ball.row, ball.col, ball.height, ball.width, paddle.row + 2, paddle.col + 2, paddle.height - 2, paddle.width - 2)) {
		ball.cDel *= -1;
		while (collision(ball.row, ball.col, ball.height, ball.width, paddle.row + 1, paddle.col + 1, paddle.height - 1, paddle.width - 1)) {
			ball.row += ball.rDel;
			ball.col += ball.cDel;
		}
	}
	
	// checks if ball collided with bar

	for (int j = 0; j < 18; j++) {
		if (collision(bar[j].row + bar[j].height, bar[j].col + 1, 1, bar[j].width - 2, ball.row, ball.col, ball.height, ball.width)) {
			ball.rDel *= -1;
			bar[j].isDead = 1;
			drawRect(bar[j].row, bar[j].col, bar[j].height, bar[j].width, bgColor);
			bar[j] = barGone;
			points++;
		}

		else if (collision(bar[j].row, bar[j].col, bar[j].height - 1, bar[j].width, ball.row, ball.col, ball.height, ball.width)) {
			ball.cDel *= -1;
			bar[j].isDead = 1;
			drawRect(bar[j].row, bar[j].col, bar[j].height, bar[j].width, bgColor);
			bar[j] = barGone;
			points++;
		}
	}
	
	// Update ball's position
	ball.row += ball.rDel;
	ball.col += ball.cDel;

}

// Performs all of the writing to the screen
void draw() {

	// Erase the previous locations
	drawRect(ball.oldRow, ball.oldCol, ball.height, ball.width, bgColor);
	drawRect(paddle.oldRow, paddle.oldCol, paddle.height, paddle.width, bgColor);


	if (ball.row >= SCREENHEIGHT - ball.height) {
		initialize();
	}

	if (points >= 18) {
		initialize();
	}

	// Draw the new locations
	drawRect(ball.row, ball.col, ball.height, ball.width, GREEN);
	drawRect(paddle.row, paddle.col, paddle.height, paddle.width, BLUE);

	for (int j = 0; j < 18; j++) {
		if (!(bar[j].isDead)) {
			drawRect(bar[j].row, bar[j].col, bar[j].height, bar[j].width, RED);
		}
	}

	// drawRect(bar[OFFSET(0, 0, 5)].row, bar[OFFSET(0, 0, 5)].col, bar[OFFSET(0, 0, 5)].height, bar[OFFSET(0, 0, 5)].width, RED);

	// Update old variables
	ball.oldRow = ball.row;
	ball.oldCol = ball.col;
	paddle.oldRow = paddle.row;
	paddle.oldCol = paddle.col;

}