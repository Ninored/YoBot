#ifndef INCLUDE_SIMULATOR_BUILDORDER
#define INCLUDE_SIMULATOR_BUILDORDER

#include <deque>
#include <memory>
#include "Simulator/BIActions/BIActions.h"
#include "Simulator/GameState.h"

namespace suboo {

class BuildOrder {
  GameState initial;
  std::deque<std::unique_ptr<BIA>> items;
  GameState final;

 public:
  BuildOrder(GameState init);
  BuildOrder(GameState init, GameState fin);
  BuildOrder::BuildOrder(const BuildOrder& b);
  BuildOrder clone() const;
  void addItem(BIA* i);  // push end
  void pushFront(BIA* i);
  BIA* extract(int i);
  std::deque<std::unique_ptr<BIA>>& getItems();
  GameState& getFinalState();
  void removeItem(int i);
  void swapItem(int i, int j);
  void moveFirst(int i);
  void moveLast(int i);
  friend std::ostream& operator<<(std::ostream& os, const BuildOrder& bo);
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATOR_BUILDORDER
