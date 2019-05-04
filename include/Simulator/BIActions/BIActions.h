#ifndef INCLUDE_SIMULATOR_BIACTIONS_BIACTIONS
#define INCLUDE_SIMULATOR_BIACTIONS_BIACTIONS

#include "Simulator/Interfaces/BIOVisitor.h"
#include "TechTree/TechTree.h"

namespace suboo {

class BIA {
  int time;

 public:
  BIA()
      : time(0),
        timeMinerals(0.0f),
        timeVespene(0.0f),
        timeFood(0.0f),
        timeFreeUnit(0.0f) {}
  virtual ~BIA() {}
  void setTime(int newTime) { time = newTime; }
  int getTime() const { return time; }

  virtual std::string getName() = 0;
  virtual void accept(BIOVisitor& v) = 0;
  virtual std::unique_ptr<BIA> clone() = 0;

  // Metrics
  float timeMinerals;
  float timeVespene;
  float timeFood;
  float timeFreeUnit;
};

class BIABuild : public BIA {
  UnitId target;

 public:
  BIABuild(UnitId t) : target(t) {}
  virtual std::string getName() { return "BIABuild"; }
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
  virtual std::unique_ptr<BIA> clone() override {
    return std::make_unique<BIABuild>(*this);
  }
  UnitId getTarget() { return target; }
};

class BIAMineVespene : public BIA {
 public:
  BIAMineVespene() {}
  virtual std::string getName() { return "BIAMineVespene"; }
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
  virtual std::unique_ptr<BIA> clone() override {
    return std::make_unique<BIAMineVespene>(*this);
  }
};

class BIAWaitGoal : public BIA {
 public:
  BIAWaitGoal() {}
  virtual std::string getName() { return "BIAWaitGoal"; }
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
  virtual std::unique_ptr<BIA> clone() override {
    return std::make_unique<BIAWaitGoal>(*this);
  }
};

class BIAChronoboost : public BIA {
  UnitId target;

 public:
  BIAChronoboost(UnitId target) : target(target) {}
  virtual std::string getName() { return "BIAChronoboost"; }
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
  virtual std::unique_ptr<BIA> clone() override {
    return std::make_unique<BIAChronoboost>(*this);
  }
  UnitId getTarget() { return target; }
};

}  // namespace suboo
#endif  // !INCLUDE_SIMULATOR_BUILDACTIONS_BUILDACTIONS
