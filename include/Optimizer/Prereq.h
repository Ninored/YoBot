#ifndef INCLUDE_OPTIMIZER_PREREQ
#define INCLUDE_OPTIMIZER_PREREQ

#include <memory>
#include <unordered_set>
#include "TechTree/TechTree.h"
#include "Simulator/BuildOrder.h"

namespace suboo {
//BuildOrder enforcePrereq(const BuildOrder& bo);
//bool enforcePrereqBySwap(BuildOrder& bo);
void addPreReq(std::vector<std::unique_ptr<BIA>> & pre, GameState & state, std::unordered_set<int> & seen, UnitId target, const TechTree & tech);

class EnforcePrereq : BIOVisitor {
  BuildOrder out_bo;
  TechTree& tech;
  std::unordered_set<int> seen;
  GameState state;
  int vesp;

 public:
  EnforcePrereq(BuildOrder& b);
  EnforcePrereq();

	virtual void visite(BIABuild& e);
  virtual void visite(BIAMineVespene& e);
  virtual void visite(BIAWaitGoal& e);
  virtual void visite(BIAChronoboost& e);
  BuildOrder visite(BuildOrder& bo);
};

class EnforcePrereqBySwap : BIOVisitor {
  BuildOrder& bo;
  BuildOrder bo_pre;
  TechTree& tech;
  std::unordered_set<int> seen;
  GameState state;
  std::vector<std::unique_ptr<BIA>> pre;
  int vesp;
  int i;

 public:
  EnforcePrereqBySwap(BuildOrder& bo);

  virtual void visite(BIABuild& e);
  virtual void visite(BIAMineVespene& e);
  virtual void visite(BIAWaitGoal& e);
  virtual void visite(BIAChronoboost& e);
  void visite(BuildOrder& bo);
};
}

#endif  // !INCLUDE_OPTIMIZER_PREREQ
