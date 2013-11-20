#pragma once
#ifndef GLOBALS_H
#define GLOBALS_H

//  ===========================================================================
//  Defines
//  ===========================================================================
#define NULL 0
#define CORE_NAMESPACE_BEGIN  namespace core {
#define CORE_NAMESPACE_END    };

//  ===========================================================================
//  Constant Definitions
//  ===========================================================================

CORE_NAMESPACE_BEGIN

const int G_SCREEN_WIDTH = 640;
const int G_SCREEN_HEIGHT = 480;

const char* const G_TITLE = "SK Hour Maze - Winston Lin";

// STATUS TEXT
const int G_TEXT_STATUS_X = 15;     // Adjust X position of status text
const int G_TEXT_STATUS_Y = 365;    // Adjust Y position of status text
const char* const G_TEXT_STATUS_START = "Click on a square to change its value.\nClick on a wall to add or remove.\n\nSOLVE shows multiple solutions if they exist.\nSOLVE COMPLETE shows one possible solution.\n\nPress F1, F2, F3 to load preset mazes.\nPress Esc to exit";
const char* const G_TEXT_STATUS_INSERT_NUMBER = "Type a number between 0 and 12. (0 is blank)\nPress Space or Enter to confirm.\nPress Esc to cancel.";
const char* const G_TEXT_STATUS_INVALID_NUMBER = "Type a number between 0 and 12. (0 is blank)\nPress Space or Enter to confirm.\nPress Esc to cancel.\n\nThe same number cannot exist more than twice.";
const char* const G_TEXT_STATUS_SOLVING = "Solving...";
const char* const G_TEXT_STATUS_SOLVING_DONE = "Solved!";
const char* const G_TEXT_STATUS_SOLVING_FAIL = "Could not solve Hour Maze.\nPlease make sure the puzzle is valid.\n\nAt least 2 preset numbers must exist.\nAll squares must have a neighbor.";
const char* const G_TEXT_STATUS_SOLVING_FAIL_C = "Could not find a complete solution.\nPlease make sure the puzzle is valid.\n\nAt least 2 preset numbers must exist.\nAll squares must have a neighbor.";

const int G_MIN_ENTRY_COUNT = 2;   // Minimum number of entries in a valid Hour Maze
const int G_MAX_NUMBER_COUNT = 2;  // Max number of individual entries
const int G_MAX_NUMBER = 12;       // Max entry that can be in a square

const int G_BASE_Y_ADJUST = -50;         // Adjust the Y position of all squares
const int G_WALL_SIZE = 10;              // Width of a Wall
const int G_SQUARE_SIZE = 70;            // Width/Height of a Square
const int G_SQUARE_COUNT_ROWS = 4;       // Number of rows
const int G_SQUARE_COUNT_COLUMNS = 6;    // Number of columns
const int G_SQUARE_NUM_X_ADJUST = -5;    // Adjust position of number in the square

// BUTTON POSITIONS
const int G_SOLVE_BTN_X = 460;
const int G_SOLVE_BTN_Y = 370;
const int G_SOLVE_C_BTN_X = 540;
const int G_SOLVE_C_BTN_Y = 370;
const int G_CLEAR_BTN_X = 380;
const int G_CLEAR_BTN_Y = 370;

// IMAGE PATHS
const char* const G_IMG_BACKGROUND = "Resources/background.tga";
const char* const G_IMG_SQ_ON = "Resources/square_on.tga";
const char* const G_IMG_SQ_OFF = "Resources/square_off.tga";
const char* const G_IMG_WALL_V_ON = "Resources/wall_v_on.tga";
const char* const G_IMG_WALL_V_OFF = "Resources/wall_v_off.tga";
const char* const G_IMG_WALL_H_ON = "Resources/wall_h_on.tga";
const char* const G_IMG_WALL_H_OFF = "Resources/wall_h_off.tga";
const char* const G_IMG_SOLVE_ON = "Resources/solve_on.tga";
const char* const G_IMG_SOLVE_OFF = "Resources/solve_off.tga";
const char* const G_IMG_SOLVE_C_ON = "Resources/solveC_on.tga";
const char* const G_IMG_SOLVE_C_OFF = "Resources/solveC_off.tga";
const char* const G_IMG_CLEAR_ON = "Resources/clear_on.tga";
const char* const G_IMG_CLEAR_OFF = "Resources/clear_off.tga";

enum E_STATES
{
  eSTART,
  eINPUT,
  eSOLVING
};

enum E_BUTTONS
{
  eSOLVE_BTN,
  eSOLVE_C_BTN,
  eCLEAR_BTN
};

//  ===========================================================================
//  End
//  ===========================================================================
CORE_NAMESPACE_END
#endif