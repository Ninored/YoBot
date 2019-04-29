#include "Optimizer/Opti.h"
#include "Optimizer/Mutator.h"
#include "Simulator/BuildOrder.h"
#include "Simulator/GameState.h"

namespace suboo {

BuildOrder improveBO(const BuildOrder& bo, int depth) {
  std::vector<pboo> optimizers;
  std::vector<pboo> fastoptimizers;
  optimizers.emplace_back(new NoWaitShifter());
  fastoptimizers.emplace_back(new NoWaitShifter());
  optimizers.emplace_back(new AddMineralGathererStack());
  optimizers.emplace_back(new AddProductionForceful());
  optimizers.emplace_back(new AddMineralGatherer());
  optimizers.emplace_back(new AddProduction());
  optimizers.emplace_back(new LeftShifter());
  fastoptimizers.emplace_back(new LeftShifter());
  fastoptimizers.emplace_back(new RemoveExtra());
  optimizers.emplace_back(new AddVespeneGatherer());

  if (depth == 0) return bo;
  //
  BuildOrder best = bo;
  int gain = 0;
  do {
    gain = 0;

    for (auto& p : optimizers) {
      int optgain = 0;
      // nested loop mostly does not help, most rules already try many positions
      // it's redundant
      auto res = p->improve(best, depth);
      if (res.first > 0) {
        gain += res.first;
        optgain += res.first;
        best = res.second;
        std::cout << "Improved results using " << p->getName() << " by "
                  << res.first << " delta. Current best timing :"
                  << best.getFinalState().getTimeStamp() << "s at depth " << depth
                  << std::endl;
        // best.print(std::cout);
      } else {
        // std::cout << "No improvement of results using " << p->getName() << ".
        // Current best timing :" << best.getFinal().getTimeStamp() << "s" <<
        // std::endl;
      }
      do {
        optgain = 0;

        for (auto& p : fastoptimizers) {
          auto res = p->improve(best, depth);
          if (res.first > 0) {
            gain += res.first;
            optgain += res.first;
            best = res.second;
            std::cout << "Improved results (fast) using " << p->getName()
                      << " by " << res.first << " delta. Current best timing :"
                      << best.getFinalState().getTimeStamp() << "s at depth "
                      << depth << std::endl;
            // best.print(std::cout);
          }
        }
      } while (optgain > 0);
    }
  } while (gain > 0);

  return best;
}

}  // namespace suboo