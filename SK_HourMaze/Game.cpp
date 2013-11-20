//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <allegro.h>
#include <vector>
#include "Algorithm.h"
#include "Globals.h"
#include "Game.h"
#include "Display.h"

CORE_NAMESPACE_BEGIN
//  ===================================================================================================================
//  Public
//  ===================================================================================================================
Game::Game(void):
mGameIsPlaying(true),
mDisplay(0),
mState(eSTART),
mActiveSquare(0),
mActiveSquareOldNum(0)
{
}

Game::~Game(void)
{
  delete mDisplay;
  mDisplay = 0;
}

bool Game::gameIsPlaying(void)
{
  return mGameIsPlaying;
}

E_STATES Game::gameState(void)
{
  return mState;
}

void Game::init(void)
{
  // Initialize Allegro
  allegro_init();
  install_keyboard();
  install_mouse();

  // Initialize Display
  mDisplay = new Display();
  mDisplay->init(this);

  mGameIsPlaying = true;
}

void Game::presetMazeA(void)
{
  if (mState != eSTART)
  {
    return;
  }

  btnClearPressed();

  // Squares
  mDisplay->squareButtons()[0]->number(12);
  mDisplay->squareButtons()[5]->number(7);
  mDisplay->squareButtons()[18]->number(9);
  mDisplay->squareButtons()[23]->number(2);

  // Walls Horizontal
  mDisplay->wallButtonsH()[0]->setWallState(true);
  mDisplay->wallButtonsH()[3]->setWallState(true);
  mDisplay->wallButtonsH()[5]->setWallState(true);
  mDisplay->wallButtonsH()[6]->setWallState(true);
  mDisplay->wallButtonsH()[7]->setWallState(true);
  mDisplay->wallButtonsH()[11]->setWallState(true);

  // Walls Vertical
  mDisplay->wallButtonsV()[2]->setWallState(true);
  mDisplay->wallButtonsV()[4]->setWallState(true);
  mDisplay->wallButtonsV()[10]->setWallState(true);
  mDisplay->wallButtonsV()[11]->setWallState(true);
  mDisplay->wallButtonsV()[12]->setWallState(true);
  mDisplay->wallButtonsV()[14]->setWallState(true);
  mDisplay->wallButtonsV()[15]->setWallState(true);
  mDisplay->wallButtonsV()[16]->setWallState(true);

}

void Game::presetMazeB(void)
{
  if (mState != eSTART)
  {
    return;
  }

  btnClearPressed();

  // Squares
  mDisplay->squareButtons()[0]->number(10);
  mDisplay->squareButtons()[10]->number(3);
  mDisplay->squareButtons()[13]->number(7);
  mDisplay->squareButtons()[23]->number(8);

  // Walls Horizontal
  mDisplay->wallButtonsH()[2]->setWallState(true);
  mDisplay->wallButtonsH()[6]->setWallState(true);
  mDisplay->wallButtonsH()[9]->setWallState(true);
  mDisplay->wallButtonsH()[11]->setWallState(true);
  mDisplay->wallButtonsH()[12]->setWallState(true);
  mDisplay->wallButtonsH()[14]->setWallState(true);
  mDisplay->wallButtonsH()[17]->setWallState(true);

  // Walls Vertical
  mDisplay->wallButtonsV()[1]->setWallState(true);
  mDisplay->wallButtonsV()[4]->setWallState(true);
  mDisplay->wallButtonsV()[6]->setWallState(true);
  mDisplay->wallButtonsV()[9]->setWallState(true);
  mDisplay->wallButtonsV()[13]->setWallState(true);
  mDisplay->wallButtonsV()[16]->setWallState(true);
}

void Game::presetMazeC(void)
{
  if (mState != eSTART)
  {
    return;
  }

  btnClearPressed();

  // Squares
  mDisplay->squareButtons()[0]->number(1);
  mDisplay->squareButtons()[9]->number(5);
  mDisplay->squareButtons()[14]->number(7);
  mDisplay->squareButtons()[23]->number(9);

  // Walls Horizontal
  mDisplay->wallButtonsH()[2]->setWallState(true);
  mDisplay->wallButtonsH()[6]->setWallState(true);
  mDisplay->wallButtonsH()[8]->setWallState(true);
  mDisplay->wallButtonsH()[9]->setWallState(true);
  mDisplay->wallButtonsH()[10]->setWallState(true);
  mDisplay->wallButtonsH()[14]->setWallState(true);  

  // Walls Vertical
  mDisplay->wallButtonsV()[0]->setWallState(true);
  mDisplay->wallButtonsV()[3]->setWallState(true);
  mDisplay->wallButtonsV()[7]->setWallState(true);
  mDisplay->wallButtonsV()[8]->setWallState(true);
  mDisplay->wallButtonsV()[9]->setWallState(true);
  mDisplay->wallButtonsV()[14]->setWallState(true);
  mDisplay->wallButtonsV()[15]->setWallState(true);
  mDisplay->wallButtonsV()[16]->setWallState(true);

}

//  -------------------------------------------------------------------------------------------------------------------
//  Square button is pressed
//  -------------------------------------------------------------------------------------------------------------------
void Game::btnSquarePressed(ls_square* iSquare)
{
  if (mState != eSTART || iSquare == NULL)
  {
    return;
  }

  mState = eINPUT;
  mDisplay->setStatusText(G_TEXT_STATUS_INSERT_NUMBER);

  mActiveSquare = iSquare;
  mActiveSquareOldNum = mActiveSquare->number();
}

//  -------------------------------------------------------------------------------------------------------------------
//  Clear button
//  -------------------------------------------------------------------------------------------------------------------
void Game::btnClearPressed(void)
{
  // Check for input mode and exit
  if (mState == eINPUT)
  {
    squareNumberConfirmed(false);
  }

  mState = eSTART;
  mDisplay->setStatusText(G_TEXT_STATUS_START);

  for (unsigned int i = 0; i < mDisplay->squareButtons().size(); i++)
  {
    mDisplay->squareButtons()[i]->number(0);
    mDisplay->squareButtons()[i]->possibleValues().clear();
  }
  for (unsigned int i = 0; i < mDisplay->wallButtonsH().size(); i++)
  {
    mDisplay->wallButtonsH()[i]->setWallState(false);
  }
  for (unsigned int i = 0; i < mDisplay->wallButtonsV().size(); i++)
  {
    mDisplay->wallButtonsV()[i]->setWallState(false);
  }
}

//  -------------------------------------------------------------------------------------------------------------------
//  Solve button
//  -------------------------------------------------------------------------------------------------------------------
void Game::btnSolvePressed(bool iComplete)
{
  mState = eSOLVING;
  mDisplay->setStatusText(G_TEXT_STATUS_SOLVING);

  Algorithm* tmpAlgo = new Algorithm();
  bool tmpSuccess = tmpAlgo->solve(mDisplay->squareButtons(), mDisplay->wallButtonsH(), mDisplay->wallButtonsV(), iComplete);

  mState = eSTART;
  if (!tmpSuccess && !iComplete)
  {
    mDisplay->setStatusText(G_TEXT_STATUS_SOLVING_FAIL);
  }
  else if (!tmpSuccess && iComplete)
  {
    mDisplay->setStatusText(G_TEXT_STATUS_SOLVING_FAIL_C);
  }
  else
  {
    mDisplay->setStatusText(G_TEXT_STATUS_SOLVING_DONE);
  }

}

//  -------------------------------------------------------------------------------------------------------------------
//  End the game loop
//  -------------------------------------------------------------------------------------------------------------------
void Game::endGame(void)
{
  mGameIsPlaying = false;
}


//  ===================================================================================================================
//  Private
//  ===================================================================================================================


//  -------------------------------------------------------------------------------------------------------------------
//  Game loop
//  -------------------------------------------------------------------------------------------------------------------
void Game::updateDisplay(void)
{
  mDisplay->update();  
}
void Game::updateCatchInput(void)
{
  if (keypressed())
  {
    int  newkey   = readkey();
    char ASCII    = newkey & 0xff;
    char scancode = newkey >> 8;
    
    if (mState == eINPUT)
    {
      // Change input number of a square
      squareNumberInput(ASCII, scancode);
    }
    else
    {
      switch(scancode)
      {
      case KEY_F1:
        presetMazeA();
        break;
      case KEY_F2:
        presetMazeB();
        break;
      case KEY_F3:
        presetMazeC();
        break;
      case KEY_ESC:
        endGame();
        break;
      }
    }
  }

}

//  -------------------------------------------------------------------------------------------------------------------
//  Scan code for entering number inputs
//  -------------------------------------------------------------------------------------------------------------------
void Game::squareNumberInput(char iASCII, char iScanCode)
{  
  // Only numbers accepted
  if(iASCII >= 48 && iASCII <= 57)
  {
    int tmpNum = mActiveSquare->number();
    int tmpNewNum = atoi(&iASCII);
    if (tmpNum == 1 && tmpNewNum <= 2)
    {
      mActiveSquare->number(10 + tmpNewNum);
    }
    else
    {
      mActiveSquare->number(tmpNewNum);
    }
  }
  else
  {
    switch(iScanCode)
    {
    case KEY_ESC:
      squareNumberConfirmed(false);
      break;
    case KEY_ENTER:
      squareNumberConfirmed(true);
      break;
    case KEY_ENTER_PAD:
      squareNumberConfirmed(true);
      break;
    case KEY_SPACE:
      squareNumberConfirmed(true);
      break;
    }
  }
}

//  -------------------------------------------------------------------------------------------------------------------
//  Confirmation of Square Input
//  -------------------------------------------------------------------------------------------------------------------
void Game::squareNumberConfirmed(bool iConfirm)
{
  if (mState != eINPUT)
  {
    return;
  }

  bool valid = true;

  if (!iConfirm)
  {
    mActiveSquare->number(mActiveSquareOldNum);     
  }
  else
  {
    // Verify valid number
    // - Only allowed a max of two of each number
    // - Number must be between 0 and 12
    std::vector<int> tmpNumbers(G_MAX_NUMBER + 1, 0);
    std::vector<ls_square*> tmpSquareButtons = mDisplay->squareButtons();    
    for (unsigned int i = 0; i < tmpSquareButtons.size(); i++)
    {
      int index = tmpSquareButtons[i]->number();
      tmpNumbers[index] += 1;

      if (index != 0 && tmpNumbers[index] > G_MAX_NUMBER_COUNT)
      {
        valid = false;
        break;
      }
    }
  }

  if (valid)
  {
    mState = eSTART;
    mDisplay->setStatusText(G_TEXT_STATUS_START);
    mActiveSquare = NULL;
  }
  else
  {
    mDisplay->setStatusText(G_TEXT_STATUS_INVALID_NUMBER);
  }

}

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END