#ifndef INCLUDE_SIMULATION_SIMULATION
#define INCLUDE_SIMULATION_SIMULATION

#include "Simulator/BIActions/BIActions.h"
#include "Simulator/BuildOrder.h"
#include "Simulator/GameState.h"
#include "Simulator/Interfaces/BIOVisitor.h"

namespace suboo {

class Simulator : public BIOVisitor {
	GameState gs;

 public:
  Simulator(GameState& inital);
  virtual void visite(BIABuild& e) override;
  virtual void visite(BIAMineVespene& e) override;
  virtual void visite(BIAChronoboost& e) override;
  virtual void visite(BIAWaitGoal& e) override;
  GameState visite(BuildOrder& bo);

  friend std::ostream& operator<<(std::ostream& os, const Simulator& simu);
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATION_SIMULATION
