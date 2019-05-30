#ifndef INCLUDE_SIMULATOR_BUILDORDER
#define INCLUDE_SIMULATOR_BUILDORDER

#include <deque>
#include <memory>
#include "Simulator/BIActions/BIActions.h"
#include "Simulator/GameState.h"
#include "TechTree/TechTree.h"

namespace suboo {

class BuildOrder {
  GameState initial;
  std::deque<std::unique_ptr<BIA>> items;
  GameState final;

 public:
  BuildOrder();
  BuildOrder(GameState init);
  BuildOrder(GameState init, GameState fin);
  BuildOrder::BuildOrder(const BuildOrder& b);
  BuildOrder clone() const;
  void addItem(BIA* item);  // push end
  void addItem(const UnitId& id);
  void addItem(const std::unique_ptr<BIA>& item);
  void addItemFront(BIA* item);
  void addItemFront(const UnitId& id);
  void addItemFront(const std::unique_ptr<BIA>& item);
	void insertItem(const UnitId& id, int index);


  BIA* extract(int i);
  std::deque<std::unique_ptr<BIA>>& getItems();
  const std::deque<std::unique_ptr<BIA>>& getItems() const;
  GameState& getInitialState();
  const GameState& getInitialState() const;
	GameState& getFinalState();
  const GameState& getFinalState() const;
  void removeItem(int i);
  void swapItems(int i, int j);
  void moveFirst(int i);
  void moveLast(int i);
  BuildOrder operator=(const BuildOrder& b);
	
	friend std::ostream& operator<<(std::ostream& os, const BuildOrder& bo);
	
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATOR_BUILDORDER
