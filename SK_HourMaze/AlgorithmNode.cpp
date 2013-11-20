//  ===================================================================================================================
//  Includes
//  ===================================================================================================================
#include <vector>
#include "AlgorithmNode.h"
#include "Globals.h"

CORE_NAMESPACE_BEGIN
using namespace std;
//  ===================================================================================================================
//  Public
//  ===================================================================================================================
AlgorithmNode::AlgorithmNode(int iValue):
mScanned(false),
mValueCounted(false),
mPruned(false),
mValue(iValue),
mLeft(0),
mUp(0),
mRight(0),
mDown(0),
mPossibleValues(G_MAX_NUMBER + 1, 0),
mPrunedValues(G_MAX_NUMBER + 1, false)
{
}

AlgorithmNode::~AlgorithmNode(void)
{
}

vector<int> AlgorithmNode::retrievePossibilities(void)
{
  vector<int> tmpVector(0);
  for (unsigned int i = 0; i < mPossibleValues.size(); i++)
  {
    if (mValue == 0 && mPossibleValues[i] > 0)
    {
      tmpVector.push_back(i);
    }
  }
  return tmpVector;
}


//  ===================================================================================================================
//  End
//  ===================================================================================================================
CORE_NAMESPACE_END