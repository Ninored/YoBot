#ifndef INCLUDE_SIMULATOR_BUILDORDER
#define INCLUDE_SIMULATOR_BUILDORDER

#include <deque>
#include "GameState.h"
#include "BuildItem.h"

namespace suboo {

class BuildOrder {
  GameState initial;
  std::deque<BuildItem> items;
  GameState final;

 public:
  BuildOrder();
  void addItem(BuildItem i); // push end
  void pushFront(BuildItem i);
  BuildItem extract(int i);
  const std::deque<BuildItem>& getItems() const;
  const GameState& getFinalState() const;
  void removeItem(int i);
  void swapItem(int i, int j);
  void moveFirst(int i);
  void moveLast(int i);
  void execute();
  friend std::ostream& operator<<(std::ostream& os, const BuildOrder& bo);
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATOR_BUILDORDER
