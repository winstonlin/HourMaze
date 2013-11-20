//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include "Sprite.h"

CORE_NAMESPACE_BEGIN
//  ===================================================================================================================
//  Public
//  ===================================================================================================================
Sprite::Sprite(void):
mBitmap(0),
mPosX(0),
mPosY(0),
mWidth(0),
mHeight(0)
{
}

Sprite::~Sprite(void)
{
  destroy_bitmap(mBitmap);
}

int Sprite::positionX(void)
{
  return mPosX;
}
void Sprite::positionX(int x)
{
  mPosX = x;
}
int Sprite::positionY(void)
{
  return mPosY;
}
void Sprite::positionY(int y)
{
  mPosY = y;
}
BITMAP* Sprite::getBitmap(void)
{
  return mBitmap;
}

void Sprite::setBitmap(BITMAP* bmp, int posX, int posY, int width, int height)
{
  mBitmap = bmp;
  mPosX = posX;
  mPosY = posY;
  mWidth = width;
  mHeight = height;
}

void Sprite::draw(BITMAP* backbuffer)
{
  draw_sprite(backbuffer, mBitmap, mPosX, mPosY);
}

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END