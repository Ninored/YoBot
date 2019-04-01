#ifndef INCLUDE_SIMULATION_SIMULATION
#define INCLUDE_SIMULATION_SIMULATION

#include "Simulator/BIActions/BIActions.h"
#include "Simulator/BuildOrder.h"
#include "Simulator/GameState.h"
#include "Simulator/Interfaces/BIOVisitor.h"

namespace suboo {

class Simulator : public BIOVisitor {
  BuildOrder& bo;
  GameState& gs;

  // Metrics
  int timeMinerals;
  int timeVespene;

 public:
  Simulator(BuildOrder& bo);
  virtual void visite(BIABuild& e) override;
  void execute();

  friend std::ostream& operator<<(std::ostream& os, const Simulator& simu);
};
}  // namespace suboo

#endif  // !INCLUDE_SIMULATION_SIMULATION
