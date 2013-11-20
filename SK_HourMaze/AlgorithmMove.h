#pragma once
#ifndef ALGORITHM_MOVE_H
#define ALGORITHM_MOVE_H

//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <vector>
#include "Globals.h"

CORE_NAMESPACE_BEGIN
//  ===================================================================================================================
//  Class
//  This class is used as a linked list for queueing up possible moves when trying to obtain a complete solution.  
//  A custom class is used to allow more flexibility with implementation.
//  ===================================================================================================================
class AlgorithmMove
{
public:
  AlgorithmMove(int iValue);
  ~AlgorithmMove(void);

  int mValue;                   // Value associated with the move
  AlgorithmMove* mNextMove;     // Next move to be tried

private:
  AlgorithmMove(const AlgorithmMove& src);                // Disabled copy constructor
  AlgorithmMove& operator=(const AlgorithmMove& rhs);     // Disabled assignment operator

};

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END
#endif