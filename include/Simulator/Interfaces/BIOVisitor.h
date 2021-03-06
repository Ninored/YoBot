#ifndef INCLUDE_SIMULATOR_INTERFACES_BIOVISITOR
#define INCLUDE_SIMULATOR_INTERFACES_BIOVISITOR

namespace suboo {
class BIABuild;
class BIAMineVespene;
class BIAWaitGoal;
class BIAChronoboost;

class BIOVisitor {
 public:
  virtual void visite(BIABuild& e) = 0;
  virtual void visite(BIAMineVespene& e) = 0;
  virtual void visite(BIAWaitGoal& e) = 0;
  virtual void visite(BIAChronoboost& e) = 0;
};
}  // namespace suboo
#endif  // !INCLUDE_SIMULATOR_INTERFACES_IBOVISITOR
