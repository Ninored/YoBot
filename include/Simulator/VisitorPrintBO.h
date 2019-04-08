#ifndef INCLUDE_SIMULATOR_VISITORPRINTBO
#define INCLUDE_SIMULATOR_VISITORPRINTBO

#include <deque>
#include <iostream>
#include <sstream>
#include "Simulator/BIActions/BIActions.h"
#include "Simulator/BuildOrder.h"
#include "Simulator/Interfaces/BIOVisitor.h"
#include "TechTree/TechTree.h"

namespace suboo {
class PrintBo : public BIOVisitor {
  std::ostringstream ss;
  const TechTree& tt;
  BuildOrder& bo;

 public:
  PrintBo(BuildOrder& cc) : tt(TechTree::getTechTree()), bo(cc) {
    for (const auto i : bo.getItems()) {
      i->accept(*this);
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const PrintBo& pbo) {
    os << pbo.ss.str();
    return os;
  }

  virtual void visite(BIABuild& e) override {
    const Unit& u = tt.getUnit(e.getTarget());
    ss << "[BUILD]" << u.name << " " << e.getTime() << " s" << std::endl;
  }
  virtual void visite(BIAWaitGoal& e) override {
    ss << "[WaitGoal] " << e.getTime() << " s" << std::endl;
  }
};

}  // namespace suboo
#endif  // !INCLUDE_SIMULATOR_VISITORPRINTBO
