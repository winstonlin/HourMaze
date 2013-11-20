//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <sstream>
#include <string>
#include "Display.h"
#include "Game.h"
#include "Globals.h"
#include "ls_button.h"
#include "ls_wall.h"

CORE_NAMESPACE_BEGIN
//  ===================================================================================================================
//  Public
//  ===================================================================================================================

Display::Display(void):
mGame(0),
mBuffer(0),
mSprites(0),
mSquareButtons(0),
mWallHoriButtons(0),
mWallVertButtons(0),
mButtons(0),
mColorText(0),
mStatusText("")
{
}

Display::~Display(void)
{
  mGame = 0;
  mSprites.clear();
  mButtons.clear();
}


std::vector<ls_square*>& Display::squareButtons(void)
{
  return mSquareButtons;
}
std::vector<ls_wall*>& Display::wallButtonsH(void)
{
  return mWallHoriButtons;
}
std::vector<ls_wall*>& Display::wallButtonsV(void)
{
  return mWallVertButtons;
}


void Display::init(Game* iGame)
{
  mGame = iGame;

  // Text colors
  // Black
  mColorText = makecol(0,0,0);
  mStatusText = G_TEXT_STATUS_START;

  // Set color depth
  int tmpCd = desktop_color_depth();
  if (tmpCd < 15) tmpCd = 32;
  set_color_depth(tmpCd);

  // Display
  set_window_title(G_TITLE);
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, G_SCREEN_WIDTH, G_SCREEN_HEIGHT,0,0);
  // De-activate normal mouse, it will be displayed manually
  show_mouse(NULL);

  // Create buffer for double buffering display update
  mBuffer = create_bitmap(G_SCREEN_WIDTH, G_SCREEN_HEIGHT);  

  // Load bitmaps
  BITMAP* tmpBmp = NULL;
  Sprite* tmpSprite = NULL;
  // Background
  tmpBmp = load_bitmap(G_IMG_BACKGROUND, NULL);
  tmpSprite = new Sprite();
  tmpSprite->setBitmap(tmpBmp, 0, 0, tmpBmp->w, tmpBmp->h);
  mSprites.push_back(tmpSprite);

  // -------------------------------------------------
  // BUTTONS
  // -------------------------------------------------
	BITMAP* tmpOnBmp = NULL;
  BITMAP* tmpOffBmp = NULL;
  ls_button* tmpButton = NULL;
  ls_square* tmpButtonSquare = NULL;
  ls_wall* tmpButtonWall = NULL;

  // SQUARE BUTTONS
	tmpOnBmp = load_bitmap(G_IMG_SQ_ON, NULL);
  tmpOffBmp = load_bitmap(G_IMG_SQ_OFF, NULL);
  int tmpBorderAdjustX = (G_SCREEN_WIDTH - (G_SQUARE_SIZE * G_SQUARE_COUNT_COLUMNS) - (G_WALL_SIZE * (G_SQUARE_COUNT_COLUMNS - 1)))/2;
  int tmpBorderAdjustY = G_BASE_Y_ADJUST + (G_SCREEN_HEIGHT - (G_SQUARE_SIZE * G_SQUARE_COUNT_ROWS) - (G_WALL_SIZE * (G_SQUARE_COUNT_ROWS - 1)))/2;
  for (int i = 0; i < G_SQUARE_COUNT_ROWS; i++) {
    for (int j = 0; j < G_SQUARE_COUNT_COLUMNS; j++) {
      tmpButtonSquare = new ls_square();
      createButton(tmpButtonSquare, tmpOnBmp, tmpOffBmp, tmpBorderAdjustX + j*(G_SQUARE_SIZE+G_WALL_SIZE), tmpBorderAdjustY + i*(G_SQUARE_SIZE+G_WALL_SIZE));
      mSquareButtons.push_back(tmpButtonSquare);
    }
  }
  destroy_bitmap(tmpOnBmp);
  destroy_bitmap(tmpOffBmp);

  // WALL BUTTONS
  // Vertical (Walls separating an UP and DOWN square)
  tmpOnBmp = load_bitmap(G_IMG_WALL_V_ON, NULL);
  tmpOffBmp = load_bitmap(G_IMG_WALL_V_OFF, NULL);  
  for (int i = 0; i < G_SQUARE_COUNT_ROWS; i++) {
    for (int j = 0; j < G_SQUARE_COUNT_COLUMNS-1; j++) {
      tmpButtonWall = new ls_wall();
      createButton(tmpButtonWall, tmpOnBmp, tmpOffBmp, tmpBorderAdjustX + j*(G_SQUARE_SIZE+G_WALL_SIZE) + G_SQUARE_SIZE, tmpBorderAdjustY + i*(G_SQUARE_SIZE+G_WALL_SIZE));
      mWallHoriButtons.push_back(tmpButtonWall);
      
    }
  }
  destroy_bitmap(tmpOnBmp);
  destroy_bitmap(tmpOffBmp);
  // Horizontal (Walls separating a LEFT and RIGHT square)
  tmpOnBmp = load_bitmap(G_IMG_WALL_H_ON, NULL);
  tmpOffBmp = load_bitmap(G_IMG_WALL_H_OFF, NULL);  
  for (int i = 0; i < G_SQUARE_COUNT_ROWS-1; i++) {
    for (int j = 0; j < G_SQUARE_COUNT_COLUMNS; j++) {
      tmpButtonWall = new ls_wall();
      createButton(tmpButtonWall, tmpOnBmp, tmpOffBmp, tmpBorderAdjustX + j*(G_SQUARE_SIZE+G_WALL_SIZE), tmpBorderAdjustY + i*(G_SQUARE_SIZE+G_WALL_SIZE) + G_SQUARE_SIZE);
      mWallVertButtons.push_back(tmpButtonWall);
    }
  }
  destroy_bitmap(tmpOnBmp);
  destroy_bitmap(tmpOffBmp);

  // NORMAL BUTTONS
  // Solve
	tmpOnBmp = load_bitmap(G_IMG_SOLVE_ON, NULL);
  tmpOffBmp = load_bitmap(G_IMG_SOLVE_OFF, NULL);
  tmpButton = new ls_button();
  createButton(tmpButton, tmpOnBmp, tmpOffBmp, G_SOLVE_BTN_X, G_SOLVE_BTN_Y);
  tmpButton->set_tag(eSOLVE_BTN);
  mButtons.push_back(tmpButton);
  destroy_bitmap(tmpOnBmp);
  destroy_bitmap(tmpOffBmp);
  // Solve Complete
	tmpOnBmp = load_bitmap(G_IMG_SOLVE_C_ON, NULL);
  tmpOffBmp = load_bitmap(G_IMG_SOLVE_C_OFF, NULL);
  tmpButton = new ls_button();
  createButton(tmpButton, tmpOnBmp, tmpOffBmp, G_SOLVE_C_BTN_X, G_SOLVE_C_BTN_Y);
  tmpButton->set_tag(eSOLVE_C_BTN);
  mButtons.push_back(tmpButton);
  destroy_bitmap(tmpOnBmp);
  destroy_bitmap(tmpOffBmp);
  // Clear
	tmpOnBmp = load_bitmap(G_IMG_CLEAR_ON, NULL);
  tmpOffBmp = load_bitmap(G_IMG_CLEAR_OFF, NULL);
  tmpButton = new ls_button();
  createButton(tmpButton, tmpOnBmp, tmpOffBmp, G_CLEAR_BTN_X, G_CLEAR_BTN_Y);
  tmpButton->set_tag(eCLEAR_BTN);
  mButtons.push_back(tmpButton);
  destroy_bitmap(tmpOnBmp);
  destroy_bitmap(tmpOffBmp);

}

void Display::setStatusText(const std::string& iText)
{
  mStatusText = iText;
}

void Display::update(void)
{
  // Draw sprites
  for (unsigned int i = 0; i < mSprites.size(); i++)
  {
    mSprites[i]->draw(mBuffer);    
  }

  // Draw Buttons and update status
  for (unsigned int i = 0; i < mSquareButtons.size(); i++)
  {
    if (mGame->gameState() != eINPUT && mGame->gameState() != eSOLVING && mSquareButtons[i]->is_clicked())
    {
      mGame->btnSquarePressed(mSquareButtons[i]);
    }
    else
    {
      mSquareButtons[i]->poll();
      mSquareButtons[i]->draw_button(mBuffer);

      // Draw text over the square
      int tmpSquareNum = mSquareButtons[i]->number();      
      if (tmpSquareNum > 0 && tmpSquareNum <= G_MAX_NUMBER)
      {
        // Convert int to string
        std::stringstream tmpStr;
        tmpStr << tmpSquareNum;
        // Print it out
        textout_ex(mBuffer, font, tmpStr.str().c_str(), G_SQUARE_NUM_X_ADJUST + mSquareButtons[i]->get_x_coord() + (G_SQUARE_SIZE/2), mSquareButtons[i]->get_y_coord()+(G_SQUARE_SIZE/2), mColorText, -1);
      }
      else
      {
        // Draw possible values
        for (unsigned int j = 0; j < mSquareButtons[i]->possibleValues().size(); j++)
        {
          std::stringstream tmpStr;
          tmpStr << mSquareButtons[i]->possibleValues()[j];
          // Print it out
          textout_ex(mBuffer, font, tmpStr.str().c_str(), -G_SQUARE_NUM_X_ADJUST + (font->height * 3 * (j%3)) + mSquareButtons[i]->get_x_coord(), font->height + (font->height * (j / 3)) + mSquareButtons[i]->get_y_coord(), mColorText, -1);
        }
      }

    }
  }
  for (unsigned int i = 0; i < mWallHoriButtons.size(); i++)
  {
    if (mGame->gameState() != eSOLVING && mWallHoriButtons[i]->is_clicked())
    {
      bool tmpState = mWallHoriButtons[i]->getWallState();
      mWallHoriButtons[i]->setWallState(!tmpState);      
    }
    else
    {
      mWallHoriButtons[i]->poll();
      mWallHoriButtons[i]->draw_button(mBuffer);
    }
  }
  for (unsigned int i = 0; i < mWallVertButtons.size(); i++)
  {
    if (mGame->gameState() != eSOLVING && mWallVertButtons[i]->is_clicked())
    {
      bool tmpState = mWallVertButtons[i]->getWallState();
      mWallVertButtons[i]->setWallState(!tmpState);      
    }
    else
    {
      mWallVertButtons[i]->poll();
      mWallVertButtons[i]->draw_button(mBuffer);
    }
  }
  for (unsigned int i = 0; i < mButtons.size(); i++)
  {
    if (mButtons[i]->is_clicked() && mGame->gameState() != eSOLVING)
    {
      if (mButtons[i]->get_tag() == eCLEAR_BTN)
      {
        mGame->btnClearPressed();
      }
      else if (mButtons[i]->get_tag() == eSOLVE_BTN)
      {
        mGame->btnSolvePressed(false);
      }
      else if (mButtons[i]->get_tag() == eSOLVE_C_BTN)
      {
        mGame->btnSolvePressed(true);
      }
    }
    else
    {
      mButtons[i]->poll();
      mButtons[i]->draw_button(mBuffer);
    }
  }

  // Draw Status Text
  textout_multiline(mBuffer, font, mStatusText.c_str(), G_TEXT_STATUS_X, G_TEXT_STATUS_Y, 
    text_height(font), mColorText, -1, textout_ex); 

  // Draw Mouse
  draw_sprite(mBuffer, mouse_sprite, mouse_x, mouse_y);

  // Blit buffer to the screen
  blit(mBuffer, screen, 0, 0, 0, 0, mBuffer->w, mBuffer->h);  
  clear(mBuffer);
}

//  ===================================================================================================================
//  Private
//  ===================================================================================================================
void Display::createButton(ls_button* iButton, BITMAP* iOnBitmap, BITMAP* iOffBitmap, int iPosX, int iPosY)
{
  iButton->initialize(iOnBitmap, iOnBitmap, iOffBitmap, NULL, NULL, NULL, ls_button::PIXEL_PERFECT, ls_button::CLICK_MODE_RELEASE);
  iButton->set_coords(iPosX, iPosY);
  iButton->internalize();
  iButton->set_active(TRUE);
}

void Display::textout_multiline(BITMAP* bmp, FONT *font, const char *ml_str, int x, int y, int v_spacing, int fg, int bg,
                           void (*text_func) (BITMAP* , const FONT* , const char* , int , int , int , int))
{
  char* copy = _strdup(ml_str);

  char* line = copy;
  char* endline = strchr(line , '\n');

  while(endline) {
   *endline = '\0';
   text_func(bmp , font , line , x , y , fg , bg);
   line = endline + 1;
   endline = strchr(line , '\n');
   y += v_spacing;
  }
  endline = strchr(line , '\0');
  if (line != endline) {
   text_func(bmp , font , line , x , y , fg , bg);
  }

  free(copy);
}

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END