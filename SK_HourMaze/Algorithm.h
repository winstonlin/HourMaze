#pragma once
#ifndef ALGORITHM_H
#define ALGORITHM_H

//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <vector>
#include <list>
#include "Globals.h"

CORE_NAMESPACE_BEGIN
using namespace std;

//  ===================================================================================================================
//  Forward Declarations
//  ===================================================================================================================

class AlgorithmNode;
class AlgorithmMove;
class ls_square;
class ls_wall;

//  ===================================================================================================================
//  Class
//  This class holds and executes all the algorithms needed to solve the Hour Maze.
//  ===================================================================================================================
class Algorithm
{
public:
  Algorithm(void);
  ~Algorithm(void);

  ///
  /// Attempt to solve the hour maze
  ///
  bool solve(vector<ls_square*>& iSquareButtons, 
             vector<ls_wall*>& iWallButtonsH, 
             vector<ls_wall*>& iWallButtonsV,
             bool iComplete);
private:
  Algorithm(const Algorithm& src);                // Disabled copy constructor
  Algorithm& operator=(const Algorithm& rhs);     // Disabled assignment operator

  ///
  /// Reinsert the values back into the square buttons container
  ///
  void reinsertSquareValues(void);

  ///
  /// Build a tree using the AlgorithmNode class
  /// This tree can be accessed through mNodeVector.
  /// This function must be called before solving.
  ///
  bool buildNodeTree(void);

  ///
  /// buildNode(void) helper function to determine whether the right entry exists
  ///
  bool buildNodeRightEntryExists(int squareIndex, vector<ls_wall*>& iWallButtonsH);

  ///
  /// buildNode(void) helper function to determine whether the down entry exists
  ///
  bool buildNodeDownEntryExists(int squareIndex, vector<ls_wall*>& iWallButtonsV);

  ///
  /// Reset the value of the tree to its original value prior to solving
  ///
  void resetNodeTree(void);

  ///
  /// Clear a vector, freeing memory
  ///
  void clearVector(vector<AlgorithmNode*>& iNodeVector);

  ///
  /// Attempt to find a complete solution to the Hour Maze
  ///
  void solveComplete(void);  

  ///
  /// solveComplete(void) helper function for finding the longest path between two nodes with a known value.
  /// By testing along this path, this helps limit the number of guesses required to find a complete solution.
  ///
  int solveCompleteFindLongestPath(vector<AlgorithmNode*>& iLongestPath, AlgorithmNode* iNextNode, int iCurrentDistance);

  ///
  /// Performs a pruning of possibilities based on known values
  ///
  void solveTestList(void);

  ///
  /// solveTestList(void) helper function for pruning numbers
  ///
  void solveTestListPruneNumber(int iNumber);

  ///
  /// Verify whether the current node tree is solved and valid
  ///
  void checkSolvedValid(bool& iSolved, bool& iValid);

  ///
  /// Helper function to determine whether the neighboring node is valid
  ///
  bool checkValidNeighbor(AlgorithmNode* iCurrent, AlgorithmNode* iNeighbor);

  ///
  /// solveTestList{void} helper function to perform a recursive scan
  ///
  void scanNode(AlgorithmNode* iNode,  bool iPrune);

  ///
  /// solveTestList{void} helper function to update node possibilities and prune as necessary
  ///
  void updatePossbilities(vector<int>& iValues, bool iPrune, AlgorithmNode* iNeighboringNode);


  vector<ls_square*>* mSquareButtons;           // Pointer to the Square Buttons container
  vector<ls_wall*>* mWallButtonsH;              // Pointer to the Horizontal Walls container
  vector<ls_wall*>* mWallButtonsV;              // Pointer to the Vertical Walls container

  vector<AlgorithmNode*> mNodeVector;           // Holds the representation of the maze in the form of AlgorithmNodes
  list<AlgorithmNode*> mNodeTestList;           // Holds the nodes that still have to be tested on by solveTestList(void)
  vector<int> mValueCounter;                    // Vector of size 13, counts the number of times a value is being used (2 is max)
  
};

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END
#endif