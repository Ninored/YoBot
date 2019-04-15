#ifndef INCLUDE_SIMULATOR_BUILDITEM
#define INCLUDE_SIMULATOR_BUILDITEM

#include "GameState.h"
#include "TechTree/TechTree.h"

namespace suboo {
class BuildItem {
 public:
  enum Action {
    BUILD,
    TRANSFER_VESPENE,
    TRANSFER_MINERALS,
    WAIT_GOAL,
    CHRONOBOOST
  };
  BuildItem(Action a, UnitId t);
  void execute(GameState& s);

  Action getAction() const;
  UnitId getTarget() const;
  int getTime() const;
  void setTime(int val);
  bool operator==(const BuildItem& other) const;

  friend std::ostream& operator<<(std::ostream& os, const BuildItem& i);

 private:
  Action action;
  UnitId target;
  int time;
};
}  // namespace suboo
#endif  // !INCLUDE_SIMULATOR_BUILDITEM
