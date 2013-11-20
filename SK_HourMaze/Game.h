#pragma once
#ifndef GAME_H
#define GAME_H

//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <string>
#include "Display.h"
#include "Globals.h"

CORE_NAMESPACE_BEGIN
//  ===================================================================================================================
//  Class
//  Primary game class tying the components together.
//  ===================================================================================================================

class Game
{
public:
  Game(void);
  ~Game(void);

//  -------------------------------------------------------------------------------------------------------------------
//  Getters and Setters
//  -------------------------------------------------------------------------------------------------------------------
  bool gameIsPlaying(void);
  E_STATES gameState(void);

//  -------------------------------------------------------------------------------------------------------------------
//  Implementation
//  -------------------------------------------------------------------------------------------------------------------
  ///
  /// Initialize the game
  ///
  void init(void);

  ///
  /// Display update loop
  ///
  void updateDisplay(void);

  ///
  /// Input update loop
  ///
  void updateCatchInput(void);

  ///
  /// F1 to preload this maze
  ///
  void presetMazeA(void);

  ///
  /// F2 to preload this maze
  ///
  void presetMazeB(void);

  ///
  /// F3 to preload this maze
  ///
  void presetMazeC(void);

  ///
  /// User clicks on a square button
  ///
  void btnSquarePressed(ls_square* iSquare);

  ///
  /// User clicks on CLEAR button
  ///
  void btnClearPressed(void);

  ///
  /// User clicks on a SOLVE button
  ///
  void btnSolvePressed(bool iComplete);

  ///
  /// User presses ESC
  ///
  void endGame(void);
  

private:
  Game(const Game& src);                // Disabled copy constructor
  Game& operator=(const Game& rhs);     // Disabled assignment operator

  ///
  /// User confirms or cancels a number during INPUT state
  ///
  void squareNumberConfirmed(bool iConfirm);

  ///
  /// Set the input of a square during INPUT state
  ///
  void squareNumberInput(char iASCII, char iScanCode);

  bool mGameIsPlaying;            // Whether the game is playing
  Display* mDisplay;              // Pointer to Display component
  E_STATES mState;                // State of the game

  ls_square* mActiveSquare;       // Active square during INPUT state and changing its value
  int mActiveSquareOldNum;        // Keep track of the old number of the square to reset on cancel
  
};


//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END
#endif