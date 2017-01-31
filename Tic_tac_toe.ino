#include <MeggyJrSimple.h>

// all necessary global variables
int x = 3;
int y = 3;
int color = 2;
long interval = 300;
unsigned long previousMillis = 0;
int ledState = HIGH;
int board[8][8];
int moves = 0;

// functions
int ledOFF(void);
int ledON(void);
bool won(void);
bool blinking(void);

// setup the game or restart the game
void setup() 
{
  MeggyJrSimpleSetup();

  // reset the # of moves
  moves = 0;
  // move the cursor to the middle of the board
  x = 3;
  y = 3;
  // reset the color of the cursor
  color = 2;
  // variable that controls the blinking of the cursor
  previousMillis = 0;
  ClearSlate();
  // reset the board
  for (int i = 0; i < 8; i++)
  {
    for (int j = 0; j < 8; j++)
    {
      board[i][j] = 40;
    }
  }
  // draw tic-tac-toe border layout
  for (int i = 0; i < 8; i++) 
  {
    DrawPx(i, 5, 1);
    DrawPx(2, i, 1);
    DrawPx(5, i, 1);
    DrawPx(i, 2, 1);
  }
}

// actual game loop
void loop() 
{
  // get the time since the game has started
  unsigned long currentMillis = millis();
  // makes the cursor blink if the spot that it is on has not already been chosen
  if (currentMillis - previousMillis >= interval && blinking()) 
  {
    previousMillis = currentMillis;
    // if the led is off, turn it on (for blinking)
    if (ledState == LOW) 
    {
      ledON();
    }
    // otherwise, if it's on, turn it off (for blinking)
    else
    {
      ledOFF();
    }
  }
  // blinks a single pixel if the cursor is over a tile that has already been chosen
  else if (currentMillis - previousMillis >= interval) 
  {
    previousMillis = currentMillis;
    // if the led is off, turn it on (for blinking)
    if (ledState == LOW) 
    {
      ledState = HIGH;
      DrawPx(x, y, board[x][y]);
    }
    // otherwise, if it's on, turn it off (for blinking)
    else
    {
      ledState = LOW;
      DrawPx(x, y, 0);
    }
  }

  
  CheckButtonsPress();
  // if the A button is pressed, mark the spot on the board with the color
  // of the cursor. Then, change the color of the cursor to the opponent's color
  if (Button_A) 
  {
    // marks the board, so that the cursor doesn't "delete" any moves that
    // were already made
    if (color == 2)
    {
      board[x][y] = 2;
    }
    else
    {
      board[x][y] = 5;
    }
    // adds to a counter that checks when the game is done
    moves++;
    
    // 
    ledON();
    Tone_Start(ToneB4, 200);

    // changes the cursor to the other color
    if (color == 2) 
    {
      color = 5;
    }
    else 
    {
      color = 2;
    }
  }
  // moves the cursor upwards
  if (Button_Up && y != 6) 
  {
    if (blinking())
    {
      ledOFF();
    }
    else 
    {
      ledON();
    }
    y += 3;
  }
  // moves the cursor down
  if (Button_Down && y != 0) 
  {
    if (blinking())
    {
      ledOFF();
    }
    else 
    {
      ledON();
    }
    y -= 3;
  }
  // moves the cursor right
  if (Button_Right && x != 6) 
  {
    if (blinking())
    {
      ledOFF();
    }
    else 
    {
      ledON();
    }
    x += 3;
  }
  // moves the cursor left
  if (Button_Left && x != 0) 
  {
    if (blinking())
    {
      ledOFF();
    }
    else 
    {
      ledON();
    }
    x-= 3;
  }
  DisplaySlate();
  // resets the game
  if (Button_B)
  {
    setup();
  }
  // if the game is won, restart the game.
  if (won())
  {
    for (int i = 0; i < 3; i++)
    {
      Tone_Start(ToneE5, 200);
      delay(225);
    }
    Tone_Start(ToneC5, 100);
    delay(150);
    Tone_Start(ToneE5, 150);
    delay(200);
    Tone_Start(ToneG5, 200);
    delay(500);
    setup();
  }

  if (moves == 9)
  {
    Tone_Start(ToneC4, 1000);
    delay(1000);
    setup();
  }
}

// function to turn a group of led's off
int ledOFF(void)
{
  ledState = LOW;
  DrawPx(x, y, 0);
  DrawPx(x + 1, y, 0);
  DrawPx(x, y + 1, 0);
  DrawPx(x + 1, y + 1, 0);
}

// function to turn a group of led's on
int ledON(void)
{
  if (blinking())
  {
    ledState = HIGH;
    DrawPx(x, y, color);
    DrawPx(x + 1, y, color);
    DrawPx(x, y + 1, color);
    DrawPx(x + 1, y + 1, color);
  }
  else
  {
    ledState = HIGH;
    DrawPx(x, y, board[x][y]);
    DrawPx(x + 1, y, board[x][y]);
    DrawPx(x, y + 1, board[x][y]);
    DrawPx(x + 1, y + 1, board[x][y]);
  }
}

// checks to see if someone has won the game
bool won(void)
{
  int total = 0;
  // checks each row for tic-tac-toe
  for (int i = 0; i < 3; i++)
  {
    total = 0;
    for (int j = 0; j < 3; j++)
    {
      total += board[i * 3][j * 3];
    }
    if (total == 6 || total == 15)
    {
      return true;
    }
  }
  // checks each cloumn for tic-tac-toe
  for (int i = 0; i < 3; i++)
  {
    total = 0;
    for (int j = 0; j < 3; j++)
    {
      total += board[j * 3][i * 3];
    }
    if (total == 6 || total == 15)
    {
      return true;
    }
  }
  // checks up-right diagonal for tic-tac-toe
  total = 0;
  for (int i = 0; i < 3; i++)
  {
    total += board[i * 3][6 - (i * 3)];
  }
  if (total == 6 || total == 15)
  {
    return true;
  }
  // checks down-right diagonal for tic-tac-toe
  total = 0;
  for (int i = 0; i < 3; i++)
  {
    total += board[6 - (i * 3)][6 - (i * 3)];
  }
  if (total == 6 || total == 15)
  {
    return true;
  }
  return false;
}

bool blinking(void)
{
  if (board[x][y] == 2 || board[x][y] == 5)
  {
    return false;
  }
  else
  {
    return true;
  }
}

