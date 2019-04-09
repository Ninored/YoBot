#ifndef INCLUDE_SIMULATOR_BIACTIONS_BIACTIONS
#define INCLUDE_SIMULATOR_BIACTIONS_BIACTIONS

#include "Simulator/Interfaces/BIOVisitor.h"
#include "TechTree/TechTree.h"

namespace suboo {

class BIA {
  int time;

 public:
  BIA() : time(0) {}
  void setTime(int newTime) { time = newTime; }
  int getTime() const { return time; }

  virtual void accept(BIOVisitor& v) = 0;
};

class BIABuild : public BIA {
  UnitId target;

 public:
  BIABuild(UnitId t) : target(t) {}
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
  UnitId getTarget() { return target; }
};

class BIAMineVespene : public BIA {
 public:
  BIAMineVespene() {}
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
};

class BIAWaitGoal : public BIA {
 public:
  BIAWaitGoal() {}
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
};

}  // namespace suboo
#endif  // !INCLUDE_SIMULATOR_BUILDACTIONS_BUILDACTIONS
