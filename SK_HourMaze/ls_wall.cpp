//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <allegro.h>
#include "ls_button.h"
#include "ls_wall.h"
#include "Globals.h"

CORE_NAMESPACE_BEGIN
//  ===================================================================================================================
//  Public
//  ===================================================================================================================
ls_wall::ls_wall(void):
mActive(false)
{
}

ls_wall::~ls_wall(void)
{
}

bool ls_wall::getWallState(void)
{
  return mActive;
}

void ls_wall::setWallState(bool iActive)
{
  if (mActive != iActive)
  {
    mActive = !mActive;

    // Swap bitmaps
    on_bitmap = off_bitmap;
    off_bitmap = down_bitmap;
    down_bitmap = on_bitmap;
  }
}

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END