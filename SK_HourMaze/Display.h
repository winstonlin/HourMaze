#pragma once
#ifndef DISPLAY_H
#define DISPLAY_H

//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <allegro.h>
#include <string>
#include <vector>
#include "Globals.h"
#include "Sprite.h"
#include "ls_button.h"
#include "ls_square.h"
#include "ls_wall.h"

CORE_NAMESPACE_BEGIN

//  ===================================================================================================================
//  Forward Declarations
//  ===================================================================================================================
class Game;

//  ===================================================================================================================
//  Class
//  This class handles the display components of the game
//  ===================================================================================================================
class Display
{
public:
  Display(void);
  ~Display(void);

//  -------------------------------------------------------------------------------------------------------------------
//  Getters and Setters
//  -------------------------------------------------------------------------------------------------------------------
  std::vector<ls_square*>& squareButtons(void);
  std::vector<ls_wall*>& wallButtonsH(void);
  std::vector<ls_wall*>& wallButtonsV(void);

//  -------------------------------------------------------------------------------------------------------------------
//  Implementation
//  -------------------------------------------------------------------------------------------------------------------
  ///
  /// Initialize the display components  
  ///
  void init(Game* iGame);

  ///
  /// Set the status text  
  ///
  void setStatusText(const std::string& iText);

  ///
  /// Update loop
  ///
  void update(void);

private:
  Display(const Display& src);                // Disabled copy constructor
  Display& operator=(const Display& rhs);     // Disabled assignment operator

  ///
  /// Utility function to help create button
  ///
  void createButton(ls_button* iButton, BITMAP* iOnBitmap, BITMAP* iOffBitmap, int iPosX, int iPosY);

  ///
  /// Allegro helper code to display text on multiple lines
  /// Source: http://www.allegro.cc/forums/thread/594780/1
  ///
  void textout_multiline(BITMAP* bmp, FONT *font, const char *ml_str, int x, int y, int v_spacing, int fg, int bg,
                           void (*text_func) (BITMAP* , const FONT* , const char* , int , int , int , int));

  Game* mGame;                                    // Pointer to Game class
  BITMAP* mBuffer;                                // Display buffer
  std::vector<Sprite*> mSprites;                  // Container holding sprites
  std::vector<ls_square*> mSquareButtons;         // Container holding the squares
  std::vector<ls_wall*> mWallHoriButtons;         // Container holding horizontal walls (separating columns)
  std::vector<ls_wall*> mWallVertButtons;         // Container holding vertical walls (separating rows)
  std::vector<ls_button*> mButtons;               // Container holding non-wall and non-square buttons (eg. solve, clear)

  int mColorText;               // Color of the status text
  std::string mStatusText;      // Status text
};

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END
#endif