//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <algorithm>
#include "Algorithm.h"
#include "AlgorithmMove.h"
#include "AlgorithmNode.h"
#include "Globals.h"
#include "ls_square.h"
#include "ls_wall.h"

CORE_NAMESPACE_BEGIN
using namespace std;
//  ===================================================================================================================
//  Public
//  ===================================================================================================================
Algorithm::Algorithm(void):
mSquareButtons(0),
mWallButtonsH(0),
mWallButtonsV(0),
mNodeVector(0),
mNodeTestList(0),
mValueCounter(0)
{
}

Algorithm::~Algorithm(void)
{
}

bool Algorithm::solve(vector<ls_square*>& iSquareButtons, 
                      vector<ls_wall*>& iWallButtonsH, 
                      vector<ls_wall*>& iWallButtonsV,
                      bool iComplete)
{
  mSquareButtons = &iSquareButtons;
  mWallButtonsH = &iWallButtonsH;
  mWallButtonsV = &iWallButtonsV;

  // Build the tree
  if (!buildNodeTree())
  {
    return false;
  }

  // Iterative Loop based only on elimination
  solveTestList();
  
  // Search for a COMPLETE solution
  // - Check if the maze has been completely solved on the first run
  // - If not, now guess the solution using possibilities
  bool tmpSolved = false;
  bool tmpValid = false;
  checkSolvedValid(tmpSolved, tmpValid);
  if (iComplete && tmpValid && !tmpSolved)
  {
    // Attempt to find complete solution
    while (tmpValid && !tmpSolved)
    {
      solveComplete();

      // Check if solved again
      // We want a complete solution only
      checkSolvedValid(tmpSolved, tmpValid);
      
      // Try iteratively solving
      if (tmpValid && !tmpSolved)
      {
        reinsertSquareValues();
        solveTestList();
        checkSolvedValid(tmpSolved, tmpValid);
      }
    }
    // Set final status
    tmpValid = tmpSolved;
  }

  reinsertSquareValues();

  // Cleanup
  clearVector(mNodeVector);

  return tmpValid;
}

//  ===================================================================================================================
//  Private
//  ===================================================================================================================

void Algorithm::reinsertSquareValues(void)
{
 // Insert new values into Square Vector
  for (unsigned int i = 0; i < mSquareButtons->size(); i++)
  {    
    (*mSquareButtons)[i]->number(mNodeVector[i]->mValue);

    // Insert possible values (DEBUG)
    (*mSquareButtons)[i]->possibleValues().clear();
    for (unsigned int j = 0; j < mNodeVector[i]->mPossibleValues.size(); j++)
    {
      if (mNodeVector[i]->mPossibleValues[j] > 0)
      {
        (*mSquareButtons)[i]->possibleValues().push_back(j);
      }
    }
  }
}

bool Algorithm::buildNodeTree(void)
{
  clearVector(mNodeVector);
  list<AlgorithmNode*>::iterator tmpIt;
  for (tmpIt = mNodeTestList.begin(); tmpIt != mNodeTestList.end(); tmpIt++)
  {
    delete *tmpIt;
  }
  mNodeTestList.clear();
  mValueCounter.clear();
  mValueCounter.assign(G_MAX_NUMBER + 1, 0);

  // Create nodes
  for (unsigned int i = 0; i < mSquareButtons->size(); i++)
  {
    int tmpValue = (*mSquareButtons)[i]->number();

    // Create new node
    AlgorithmNode* tmpNode = new AlgorithmNode((*mSquareButtons)[i]->number());
    mNodeVector.push_back(tmpNode);

    if (tmpValue > 0)
    {
      // Increment global counter to keep track of number of each entry      
      tmpNode->mValueCounted = true;
      mValueCounter[tmpValue]++;

      // Push into test list
      mNodeTestList.push_back(tmpNode);
    }  

  }
  // Need at least two entries
  if (mNodeTestList.size() < G_MIN_ENTRY_COUNT)
  {
    clearVector(mNodeVector);
    return false;
  }

  // Build tree
  for (unsigned int i = 0; i < mNodeVector.size(); i++)
  {
    // Check if RIGHT entry exists
    if (buildNodeRightEntryExists(i, *mWallButtonsH))
    {
      mNodeVector[i]->mRight = mNodeVector[i+1];
      mNodeVector[i+1]->mLeft = mNodeVector[i];
    }
    // Check if DOWN entry exists 
    if (buildNodeDownEntryExists(i, *mWallButtonsV))
    {
      mNodeVector[i]->mDown = mNodeVector[i+G_SQUARE_COUNT_COLUMNS];
      mNodeVector[i+G_SQUARE_COUNT_COLUMNS]->mUp = mNodeVector[i];
    }
    // Check to make sure node isn't isolated
    if (mNodeVector[i]->mDown == NULL && 
        mNodeVector[i]->mLeft == NULL && 
        mNodeVector[i]->mUp == NULL && 
        mNodeVector[i]->mRight == NULL)
    {
      clearVector(mNodeVector);
      return false;
    }
  }

  return true;
}

bool Algorithm::buildNodeRightEntryExists(int squareIndex, vector<ls_wall*>& iWallButtonsH)
{
  // Check if last column to the right  
  if ((squareIndex + 1) % G_SQUARE_COUNT_COLUMNS == 0)
  {
    return false;
  }

  // Find row
  int tmpRow = squareIndex / G_SQUARE_COUNT_COLUMNS;

  // Check if wall exists
  return !iWallButtonsH[squareIndex - tmpRow]->getWallState();

}

bool Algorithm::buildNodeDownEntryExists(int squareIndex, vector<ls_wall*>& iWallButtonsV)
{
  // Check if last row
  int tmpRow = squareIndex / G_SQUARE_COUNT_COLUMNS;
  if (tmpRow >= (G_SQUARE_COUNT_ROWS - 1))
  {
    return false;
  }

  // Check if wall exists
  return !iWallButtonsV[squareIndex]->getWallState();
}

void Algorithm::resetNodeTree(void)
{  
  mNodeTestList.clear();
  mValueCounter.clear();
  mValueCounter.assign(G_MAX_NUMBER + 1, 0);

  // Create nodes
  for (unsigned int i = 0; i < mSquareButtons->size(); i++)
  {
    int tmpValue = (*mSquareButtons)[i]->number();
    mNodeVector[i]->mValue = tmpValue;

    mNodeVector[i]->mScanned = false;
    mNodeVector[i]->mPruned = false;
    mNodeVector[i]->mPossibleValues.assign(G_MAX_NUMBER + 1, 0);
    mNodeVector[i]->mPrunedValues.assign(G_MAX_NUMBER + 1, false);
    
    if (tmpValue > 0)
    {
      // Increment global counter to keep track of number of each entry      
      mNodeVector[i]->mValueCounted = true;
      mValueCounter[tmpValue]++;

      // Push into test list
      mNodeTestList.push_back(mNodeVector[i]);
    }  
  }
}

void Algorithm::clearVector(vector<AlgorithmNode*>& iNodeVector)
{
  for (unsigned int i = 0; i < iNodeVector.size(); i++)
  {
    delete iNodeVector[i];
  }
  iNodeVector.clear();
}

void Algorithm::solveComplete(void)
{
  // Find nodes that have only 2 possibilities
  vector<AlgorithmNode*> tmpNodeVector;
  for (unsigned int i = 0; i < mNodeVector.size(); i++)
  {
    if (mNodeVector[i]->retrievePossibilities().size() == 2)
    {
      tmpNodeVector.push_back(mNodeVector[i]);
    }
  }
  if (tmpNodeVector.size() == 0)
  {
    return;
  }

  // Determine the best nodes to work with
  // - The one with the longest path to another node with a value
  vector<AlgorithmNode*> tmpLongestPathNodes(0);
  vector<AlgorithmNode*> tmpLongestPathNodesCurr(0);
  int tmpLongestPathLength = -1;
  int tmpLongestPathIndex = -1;
  for (unsigned int i = 0; i < tmpNodeVector.size(); i++)
  {
    // Reset values for current iteration
    tmpLongestPathNodesCurr.clear();
    for (unsigned int j = 0; j < mNodeVector.size(); j++)
    {
      mNodeVector[j]->mScanned = false;
    }

    int tmpCurrentPathLength = solveCompleteFindLongestPath(tmpLongestPathNodesCurr, tmpNodeVector[i], 0); 
    if (tmpCurrentPathLength > tmpLongestPathLength)
    {      
      tmpLongestPathNodes.swap(tmpLongestPathNodesCurr);
      tmpLongestPathNodes.push_back(tmpNodeVector[i]);
      tmpLongestPathLength = tmpCurrentPathLength;
      tmpLongestPathIndex = i;
    }
  }
  tmpLongestPathNodesCurr.clear();

  // Initiate test queue
  vector<AlgorithmMove*> tmpMoveVector;
  vector<int> tmpPossValues = tmpLongestPathNodes.back()->retrievePossibilities();
  for (unsigned int i = 0; i < tmpPossValues.size(); i++)
  {
    tmpMoveVector.push_back(new AlgorithmMove(tmpPossValues[i]));
  }

  // LOOP over test queue
  while (tmpMoveVector.size() > 0)
  {
    // Reset the nodes to original values
    resetNodeTree();

    // Preset values from moveset
    AlgorithmMove* tmpMoveset = tmpMoveVector.front();
    vector<AlgorithmNode*> tmpCurrLongestPath;
    // Copy all of tmpLongestPathNodes into tmpCurrLongestPath
    for(unsigned int i = 0; i < tmpLongestPathNodes.size(); i++)
    {
      tmpCurrLongestPath.push_back(tmpLongestPathNodes[i]);
    }
    // Insert all moves into tree
    do
    {
      tmpCurrLongestPath.back()->mValue = tmpMoveset->mValue;
      tmpCurrLongestPath.back()->mValueCounted = true;      
      mValueCounter[tmpMoveset->mValue]++;
      mNodeTestList.push_back(tmpCurrLongestPath.back());

      tmpCurrLongestPath.pop_back();
      tmpMoveset = tmpMoveset->mNextMove;

    } while (tmpMoveset != NULL);

    // Attempt to solve
    bool tmpSolved = false;
    bool tmpValid = false;
    solveTestList();   
    checkSolvedValid(tmpSolved, tmpValid);
    if (tmpSolved)
    {
      // Maze has been solved
      vector<AlgorithmMove*>::iterator tmpIter;
      for(tmpIter=tmpMoveVector.begin(); tmpIter != tmpMoveVector.end(); tmpIter++)
      {
        delete *tmpIter;
      }
      tmpMoveVector.clear();
      return;
    }
    else if (tmpValid)
    {
      // Maze hasn't been solved, but is still valid           
      // Insert new possibilities into queue
      vector<int> tmpNewMoveVector = tmpCurrLongestPath.back()->retrievePossibilities();

      for (unsigned int i = 0; i < tmpNewMoveVector.size(); i++)
      {
        // Verify that the number is valid
        if (mValueCounter[tmpNewMoveVector[i]] + 1 > G_MAX_NUMBER_COUNT)
        {
          continue;
        }

        // Clone current moveset
        AlgorithmMove* tmpCurrentMoveset = tmpMoveVector.front();
        AlgorithmMove* tmpNewMoveset = new AlgorithmMove(tmpCurrentMoveset->mValue); 
        tmpMoveset = tmpNewMoveset;
        tmpCurrentMoveset = tmpCurrentMoveset->mNextMove;
        while (tmpCurrentMoveset != NULL)
        {
          tmpNewMoveset->mNextMove = new AlgorithmMove(tmpCurrentMoveset->mValue);
          tmpCurrentMoveset = tmpCurrentMoveset->mNextMove;
          tmpNewMoveset = tmpNewMoveset->mNextMove;
        }

        // Insert newest move
        tmpNewMoveset->mNextMove = new AlgorithmMove(tmpNewMoveVector[i]);
        tmpMoveVector.push_back(tmpMoveset);
      }     

      // Clear temporary vector
      tmpNewMoveVector.clear();

      // Remove current moveset
      delete *tmpMoveVector.begin();
      tmpMoveVector.erase(tmpMoveVector.begin());
      
    }    
    else
    {
      // Maze hasn't been solved, and move is invalid
      delete *tmpMoveVector.begin();
      tmpMoveVector.erase(tmpMoveVector.begin());
    }
  }
}

int Algorithm::solveCompleteFindLongestPath(vector<AlgorithmNode*>& iLongestPath, AlgorithmNode* iNextNode, int iCurrentDistance)
{
  if (iNextNode == NULL || iNextNode->mValue != 0 || iNextNode->mScanned)
  {
    return iCurrentDistance;
  }
  else
  {
    iNextNode->mScanned = true;

    vector<int> tmpPathLengths;
    vector<AlgorithmNode*> tmpPathNodes;
    tmpPathNodes.push_back(iNextNode->mDown);
    tmpPathNodes.push_back(iNextNode->mLeft);
    tmpPathNodes.push_back(iNextNode->mUp);
    tmpPathNodes.push_back(iNextNode->mRight);
    for (unsigned int i = 0; i < tmpPathNodes.size(); i++)
    {
      tmpPathLengths.push_back(solveCompleteFindLongestPath(iLongestPath, tmpPathNodes[i], iCurrentDistance + 1));
    }
    
    int tmpLongestIndex = -1;
    int tmpLongestLength = -1;
    for (unsigned int i = 0; i < tmpPathLengths.size(); i++)
    {
      if (tmpPathLengths[i] > tmpLongestLength)
      {
        tmpLongestLength = tmpPathLengths[i];
        tmpLongestIndex = i;
      }
    }
    if (tmpPathNodes[tmpLongestIndex] != NULL)
    {
      iLongestPath.push_back(tmpPathNodes[tmpLongestIndex]);
    }    
    return tmpLongestLength;
  }
}

void Algorithm::solveTestList(void)
{
  while (mNodeTestList.size() > 0)
  {
    // Reset pruned for all    
    for (unsigned int i = 0; i < mNodeVector.size(); i++)
    {
      mNodeVector[i]->mPruned = false;
    }

    while (mNodeTestList.size() > 0)
    {
      // Reset scanned for all    
      for (unsigned int i = 0; i < mNodeVector.size(); i++)
      {
        mNodeVector[i]->mScanned = false;       
      }

      // Restart scan
      AlgorithmNode* tmpNode = mNodeTestList.back();
      scanNode(tmpNode, true);
      mNodeTestList.pop_back();
    }

    // Search for squares with only one possibility and set the value
    for (unsigned int i = 0; i < mNodeVector.size(); i++)
    {
      bool tmpNewlyAssigned = false;
      int tmpCountPossibilities = 0;
      int tmpIndex = 0;
      for (unsigned int j = 0; j < mNodeVector[i]->mPossibleValues.size(); j++)
      {
        if (mNodeVector[i]->mValue == 0 && mNodeVector[i]->mPossibleValues[j] > 0)
        {
          tmpCountPossibilities++;
          tmpIndex = j;
        }
      }
      if (tmpCountPossibilities == 1)
      {
        tmpNewlyAssigned = true;
        mNodeVector[i]->mValue = tmpIndex;
        mNodeVector[i]->mValueCounted = true;
        mValueCounter[tmpIndex]++;
      }

      // If value is now used twice, it should now be removed from possibilities of others
      if (mValueCounter[tmpIndex] == G_MAX_NUMBER_COUNT)
      {
        solveTestListPruneNumber(tmpIndex);
      }

      // Insert pruned nodes into search
      // Also insert if square has newly been assigned a value
      if (mNodeVector[i]->mPruned || tmpNewlyAssigned)
      {
        mNodeTestList.push_back(mNodeVector[i]);
      }
    }    
  }
}

void Algorithm::solveTestListPruneNumber(int iNumber)
{         
  for (unsigned int i = 0; i < mNodeVector.size(); i++)
  {
    if (mNodeVector[i]->mValue == 0)
    {
      for (unsigned int j = 0; j < mNodeVector[i]->mPossibleValues.size(); j++)
      {
        if (mNodeVector[i]->mPossibleValues[j] > 0 && j == iNumber)
        {
          mNodeVector[i]->mPrunedValues[j] = true;
          mNodeVector[i]->mPossibleValues[j] = 0;
        }
      }
    }        
  }         
}

void Algorithm::checkSolvedValid(bool& iSolved, bool& iValid)
{
  bool tmpValid = true;
  int tmpCount = 0;  
  for (unsigned int i = 0; i < mNodeVector.size(); i++)
  {
    if (mNodeVector[i]->mValue != 0)
    {
      tmpCount++;

      // Make sure maze values are valid
      if (!checkValidNeighbor(mNodeVector[i], mNodeVector[i]->mLeft) || 
          !checkValidNeighbor(mNodeVector[i], mNodeVector[i]->mRight) || 
          !checkValidNeighbor(mNodeVector[i], mNodeVector[i]->mUp) || 
          !checkValidNeighbor(mNodeVector[i], mNodeVector[i]->mDown))
      {
        tmpValid = false;
      }

    }
    else if (tmpValid)
    {
      // Check that there are possible values for validity
      if (mNodeVector[i]->retrievePossibilities().size() == 0)
      {
        tmpValid = false;
      }
    }
  }
  iSolved = (tmpCount == mNodeVector.size());
  iValid = tmpValid;
}

bool Algorithm::checkValidNeighbor(AlgorithmNode* iCurrent, AlgorithmNode* iNeighbor)
{
  return !(iNeighbor != NULL &&
           iNeighbor->mValue != 0 &&
           iCurrent->mValue != iNeighbor->mValue + 1 && 
           iCurrent->mValue != iNeighbor->mValue - 1 && 
           !(iCurrent->mValue == G_MAX_NUMBER && iNeighbor->mValue == 1) &&
           !(iCurrent->mValue == 1 && iNeighbor->mValue == G_MAX_NUMBER));
}

void Algorithm::scanNode(AlgorithmNode* iNode, bool iPrune)
{
  if (iNode == NULL)
  {
    return;
  }
  if (!iNode->mScanned)
  {
    iNode->mScanned = true;

    // Loop through all possibilities
    vector<int> tmpValues;

    if (iNode->mValue != 0)
    {
      tmpValues.push_back(iNode->mValue);
    }
    else
    {
      for (unsigned int i = 0; i < iNode->mPossibleValues.size(); i++)
      {
        if (iNode->mPossibleValues[i] > 0)
        {
          tmpValues.push_back(i);
        }
      }
    }
    updatePossbilities(tmpValues, iPrune, iNode->mLeft);
    updatePossbilities(tmpValues, iPrune, iNode->mUp);
    updatePossbilities(tmpValues, iPrune, iNode->mRight);
    updatePossbilities(tmpValues, iPrune, iNode->mDown);

    // Scan adjacent nodes
    scanNode(iNode->mLeft, false);
    scanNode(iNode->mUp, false);
    scanNode(iNode->mRight, false);
    scanNode(iNode->mDown, false);
  }

}

void Algorithm::updatePossbilities(vector<int>& iValues, bool iPrune, AlgorithmNode* iNeighboringNode)
{
  // Null check
  // Check if the node already has a set value
  // Check if already scanned/updated for this iteration
  // Check if there are values
  if (iNeighboringNode == NULL || iNeighboringNode->mValue != 0 || iNeighboringNode->mScanned)
  {
    return;
  }

  vector<int>::iterator tmpIt;
  vector<int> tmpNewPossibleValues;

  vector<int>& tmpPossibleValues = iNeighboringNode->mPossibleValues;
  vector<bool>& tmpPrunedValues = iNeighboringNode->mPrunedValues;

  // Determine possible values
  for (unsigned int i = 0; i < iValues.size(); i++)
  {
    // Set possible values
    int tmpValueA = iValues[i] + 1;
    int tmpValueB = iValues[i] - 1;

    // Wrap around values if necessary
    if (tmpValueA > G_MAX_NUMBER)
    {
      tmpValueA = 1;
    }  
    if (tmpValueB == 0)
    {
      tmpValueB = G_MAX_NUMBER;
    }

    // Check validity
    // - Check whether value has already been used up twice
    // - Also check whether value has already been pruned
    if (mValueCounter[tmpValueA] < G_MAX_NUMBER_COUNT && !tmpPrunedValues[tmpValueA])
    {
      tmpNewPossibleValues.push_back(tmpValueA);  
    }  
    if (mValueCounter[tmpValueB] < G_MAX_NUMBER_COUNT && !tmpPrunedValues[tmpValueB])
    {
      tmpNewPossibleValues.push_back(tmpValueB);      
    }
  }

  for (unsigned int i = 0; i < tmpNewPossibleValues.size(); i++)
  {
    // Set possible values
    tmpPossibleValues[tmpNewPossibleValues[i]]++;

    // Prune values
    // Look at each possible value and compare with new values
    if (iPrune)
    {
      for (unsigned int j = 0; j < tmpPossibleValues.size(); j++)
      { 
        if (!tmpPrunedValues[j])
        {
          vector<int>::iterator it;
          it = find (tmpNewPossibleValues.begin(), tmpNewPossibleValues.end(), j);
          if (it == tmpNewPossibleValues.end())
          {
            tmpPrunedValues[j] = true;
            tmpPossibleValues[j] = 0;

            // Signal that this node has been pruned during this iteration
            // It should now prune its neighbors
            iNeighboringNode->mPruned = true;
          }
        }
      }
    }
  }

}

//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END