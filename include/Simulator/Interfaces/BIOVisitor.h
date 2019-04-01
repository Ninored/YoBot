#ifndef INCLUDE_SIMULATOR_INTERFACES_BIOVISITOR
#define INCLUDE_SIMULATOR_INTERFACES_BIOVISITOR

namespace suboo {
class BIABuild;

class BIOVisitor {
 public:
  virtual void visite(BIABuild& e) = 0;
};
}  // namespace suboo
#endif  // !INCLUDE_SIMULATOR_INTERFACES_IBOVISITOR
