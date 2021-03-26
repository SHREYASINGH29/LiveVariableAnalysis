#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/CFG.h"
#include <iostream>
#include <vector>
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "map"
#include "set"
#include "llvm/IR/Instructions.h"

using namespace llvm;

namespace {

#define DEBUG_TYPE "lva"                                                   

class LVAPass : public FunctionPass {
       public:
	static char ID;
	LVAPass() : FunctionPass(ID) {}
	std::map<Instruction*, std::set<Value*>> In, Out;
	std::set<Value*> gen(Instruction *I){
		std::set<Value*> var;
		if(AllocaInst *AI = dyn_cast<AllocaInst>(I)){
			
		}else if(LoadInst *LI = dyn_cast<LoadInst>(I)){
			var.insert(LI->getPointerOperand());
		}else if(ReturnInst *RI = dyn_cast<ReturnInst>(I)){
			Value* RetVal = RI->getReturnValue();
			if(!RetVal){
				
			}else{
				var.insert(RetVal);
			}
		}else if(StoreInst *SI = dyn_cast<StoreInst>(I)){
			var.insert(SI->getPointerOperand());
			var.insert(SI->getValueOperand());
		}else if(BitCastInst *BCI = dyn_cast<BitCastInst>(I)){
			var.insert(BCI->getOperand(0));
		}else if(I->isBinaryOp()){
			var.insert(I->getOperand(0));
			var.insert(I->getOperand(1));
		}else if(CallInst *CI = dyn_cast<CallInst>(I)){
			if(!(CI->doesNotReturn())){
			
			}
			unsigned Num = CI->getNumArgOperands();
			for(int i = 0; i < Num; i++){
				var.insert(CI->getArgOperand(i));
			}
		}else if(GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(I)){
			var.insert(GEP->getPointerOperand());
			for(Value* Idx: GEP->indices()){
				 var.insert(Idx);
			}
		}
		return var;
	}
	std::set<Value*> kill(Instruction *I){
		std::set<Value*> var;
		if(AllocaInst *AI = dyn_cast<AllocaInst>(I)){
			var.insert(cast<Value>(AI));
		}else if(LoadInst *LI = dyn_cast<LoadInst>(I)){
			var.insert(cast<Value>(LI));
		}else if(BitCastInst *BCI = dyn_cast<BitCastInst>(I)){
			var.insert(cast<Value>(BCI));
		}else if(I->isBinaryOp()){
			var.insert(cast<Value>(I));
		}else if(GetElementPtrInst *GEP = dyn_cast<GetElementPtrInst>(I)){
			var.insert(cast<Value>(GEP));
		}else if(CallInst *CI = dyn_cast<CallInst>(I)){
			if(Value* V = dyn_cast<Value>(I)){
				var.insert(V);
			}
		}
		return var;
	}
	std::vector<Instruction*> getSucc(Instruction *I){
		std::vector<Instruction*> SuccI;
		BasicBlock* ParentBB = I -> getParent();
		if(ParentBB -> getTerminator() == I){
			for(BasicBlock *Succ : successors(ParentBB)){
				SuccI.push_back(&(Succ->front()));
			}
		}else{
			SuccI.push_back(I->getNextNonDebugInstruction());
		}
		return SuccI;	
	} 
	std::set<Value*> setUnion(std::set<Value*> Val1,std::set<Value*> Val2){
		std::set<Value*> Sumup;
		Sumup = Val1;
		for(auto V : Val2){
			Sumup.insert(V);
		}
		return Sumup;
	}
	std::set<Value*> getDifference(std::set<Value*> Val1,std::set<Value*> Val2){
		std::set<Value*> difference;
		difference = Val1;
		for(auto d: Val2){
			difference.erase(d);
		}
		return difference;
	}
	std::set<Value*> getIn(Instruction *I){
		std::set<Value*> d_content;
		std::set<Value*> in_content;
		d_content = getDifference(Out[I], kill(I));
		in_content = setUnion(gen(I), d_content);
		return in_content; 
	}

	std::set<Value*> getOut(Instruction *I){
		std::set<Value*> OutVal;
		for(Instruction *Succ:getSucc(I)){
			OutVal = setUnion(OutVal, In[Succ]);	
		}
		return OutVal;
	}
	void printSet(std::set<Value*> Val){
		for(auto temp : Val){
			std::cout<<temp->getName().str()<<" ";
		}
		std::cout<<"\n";	
	}
	bool runOnFunction(Function &F) override {
		auto end = inst_end(F);
		do{
			end--;
			Out[&*end] = getOut(&*end);
			In[&*end] = getIn(&*end);
			std::cout << "Out: "; printSet(Out[&*end]);
			errs()<< "Instruction: " << *end<<"\n";
			std::cout << "In: "; printSet(In[&*end]);
			std::cout << "--------------\n";
		}while(end != inst_begin(F));
		
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
