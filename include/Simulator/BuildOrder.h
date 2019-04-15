#ifndef INCLUDE_SIMULATOR_BUILDORDER
#define INCLUDE_SIMULATOR_BUILDORDER

#include <deque>
#include "Simulator/BIActions/BIActions.h"
#include "Simulator/GameState.h"

namespace suboo {

class BuildOrder {
  GameState initial;
  std::deque<BIA*> items;
  GameState final;

 public:
  BuildOrder(GameState init);
  void addItem(BIA* i);  // push end
  void pushFront(BIA* i);
  BIA* extract(int i);
  std::deque<BIA*>& getItems();
  GameState& getFinalState();
  void removeItem(int i);
  void swapItem(int i, int j);
  void moveFirst(int i);
  void moveLast(int i);
  friend std::ostream& operator<<(std::ostream& os, const BuildOrder& bo);
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATOR_BUILDORDER
