#ifndef INCLUDE_SIMULATOR_BIACTIONS_BIACTIONS
#define INCLUDE_SIMULATOR_BIACTIONS_BIACTIONS

#include "Simulator/Interfaces/BIOVisitor.h"
#include "TechTree/TechTree.h"
#include <variant>

namespace suboo {

enum BIAType { Build, MineVespene, WaitGoal, Chronoboost, unknown};


/*
class TypeGet {
  virtual BIAType visite(BIABuild& e) { return BIAType.Build; }
  virtual BIAType visite(BIAMineVespene& e) { return BIAType.MineVespene; }
  virtual BIAType visite(BIAWaitGoal& e) { return BIAType.WaitGoal; }
  virtual BIAType visite(BIAChronoboost& e) { return BIAType.Chronoboost; }
};
*/
// Helpers
/*
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
*/

/*
using var_t = std::variant<BIA, BIABuild, BIAChronoboost, BIAMineVespene, BIAWaitGoal>;
auto typeCheck = [](auto&& arg) -> BIAType {
  if (typeid(arg).name() == "BIABuild")
    return BIAType.Build;
  else if (typeid(arg).name() == "BIAMineVespene")
    return BIAType.MineVespene;
  else if (typeid)
  using T = std::decay_t<decltype(arg)>;
  if constexpr (std::is_same<T, BIABuild>)
  else if constexpr (std::is_same<T, BIAMineVespene>)
    return BIAType.MineVespene;
  else if constexpr (std::is_same<T, BIAWaitGoal>)
    return BIAType.WaitGoal;
  else if constexpr (std::is_same < T, BIAChronoboost)
    return BIAType.Chronoboost;
  else
    static_assert(std::false_type<T>::value, "non-exhostif visitor");
};*/

// Surcharget l'op d'égalité
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

  virtual const std::string getName() const = 0;
  virtual void accept(BIOVisitor& v) = 0;
  // Return BIA
  virtual std::unique_ptr<BIA> clone() = 0;
  virtual bool operator==(const BIA& b) = 0;
  /*
  {
    return typeid(b).name() == typeid(this).name();
//    std::visit(typeCheck, this) == std::visit(typeCheck, b);
	};*/

  template <class T>
  T* cast() {
    return (T*)this;
  }

  // Metrics
  float timeMinerals;
  float timeVespene;
  float timeFood;
  float timeFreeUnit;
  float totalWait() {
    return timeMinerals + timeVespene + timeFood + timeFreeUnit;
  }
};

class BIABuild : public BIA {
  UnitId target;

 public:
  BIABuild(UnitId t) : target(t) {}
  virtual const std::string getName() const { return "BUILD"; }
  virtual void accept(BIOVisitor& v) override { return v.visite(*this); }
  virtual std::unique_ptr<BIA> clone() override {
    return std::make_unique<BIABuild>(*this);
  }
  UnitId getTarget() { return target; }
  bool operator==(const BIA& b) {
    return typeid(b) == typeid(*this) && target == ((const BIABuild&)b).target;
  }
};

class BIAMineVespene : public BIA {
 public:
  BIAMineVespene() {}
  virtual const std::string getName() const { return "MINEVESPENE"; }
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
  virtual std::unique_ptr<BIA> clone() override {
    return std::make_unique<BIAMineVespene>(*this);
  }
  bool operator==(const BIA& b) {
    return typeid(b).name() == typeid(this).name();
  }
};

class BIAWaitGoal : public BIA {
 public:
  BIAWaitGoal() {}
  virtual const std::string getName() const { return "WAITGOAL"; }
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
  virtual std::unique_ptr<BIA> clone() override {
    return std::make_unique<BIAWaitGoal>(*this);
  }
  bool operator==(const BIA& b) { 
		
    return typeid(b).name() == typeid(this).name();
	}
};

class BIAChronoboost : public BIA {
  UnitId target;

 public:
  BIAChronoboost(UnitId target) : target(target) {}
  virtual const std::string getName() const { return "CHRONOBOOST"; }
  virtual void accept(BIOVisitor& v) override { v.visite(*this); }
  virtual std::unique_ptr<BIA> clone() override {
    return std::make_unique<BIAChronoboost>(*this);
  }
  UnitId getTarget() { return target; }
  bool operator==(const BIA& b) {
    return typeid(b).name() == typeid(this).name();
  }
};

}  // namespace suboo
#endif  // !INCLUDE_SIMULATOR_BUILDACTIONS_BUILDACTIONS
