#pragma once
#ifndef ALGORITHM_NODE_H
#define ALGORITHM_NODE_H

//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <vector>
#include "Globals.h"

CORE_NAMESPACE_BEGIN
using namespace std;
//  ===================================================================================================================
//  Class
//  This class represents a square on the grid and holds useful information pertaining to that square.
//  It is used by the Algorithm class to solve the maze.
//  ===================================================================================================================
class AlgorithmNode
{
public:
  AlgorithmNode(int iValue);
  ~AlgorithmNode(void);

  ///
  /// Retrieve the possible values in a new vector<int>
  ///
  vector<int> retrievePossibilities();

  bool mScanned;          // Whether the node has been scanned during the current iteration
  bool mValueCounted;     // Whether the node has been accounted for in the global count vector
  bool mPruned;           // Whether this node has been pruned during this iteration

  int mValue;                   // Correct value for this node
  AlgorithmNode* mLeft;         // Square to the left of this node
  AlgorithmNode* mUp;           // Square above this node
  AlgorithmNode* mRight;        // Square to the right of this node
  AlgorithmNode* mDown;         // Square below this node
  vector<int> mPossibleValues;  // Counter for the possible values that this node may be.  Always a vector of size 13
  vector<bool> mPrunedValues;   // Always a vector of size 13, and keeps track of which values are no longer possible

private:
  AlgorithmNode(const AlgorithmNode& src);
  AlgorithmNode& operator=(const AlgorithmNode& rhs);
};

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END
#endif