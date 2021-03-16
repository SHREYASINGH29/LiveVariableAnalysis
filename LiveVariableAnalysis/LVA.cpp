#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"

using namespace llvm;

namespace {

#define DEBUG_TYPE "lva"                                                   

class LVAPass : public FunctionPass {
       public:
	static char ID;
	LVAPass() : FunctionPass(ID) {}
	bool runOnFunction(Function &F) override {
		return false;
	}
};
}  // namespace

char LVAPass::ID = 0;
static RegisterPass<LVAPass> X(
    "lva",			      // the option name -> -mba
    "Live Variable Analysis",  // option description
    true,			      // true as we don't modify the CFG
    true
);
