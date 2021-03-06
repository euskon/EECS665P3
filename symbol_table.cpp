#include "symbol_table.hpp"
namespace lake{
bool VarSemSym::SetType(std::string type){
	if(type == "VOID"){
		//check error 3
		/*
		NEED A WAY TO TELL IF IT IS A POINTER OR NOT AND FLAG THE DIFFEREN ERRORS
		*/
		//throw error
		if(m_ptrDepth == 0){
			std::cerr << m_line << "," << m_col << "Non-function declared void";
			return false;
		}
		std::cerr << m_line << "," << m_col << "Invalid pointer type";
		return false;
	}
	else{
		m_type = type;
		return true;
	}
}
std::string FuncSemSym::GetFullType(){
	std::string fullType = m_argsList + "->" + m_returnType;
	return fullType;
}
// void VarSemSym::SetId(std::string Id){
// 	m_name = Id;
// }
// void VarSemSym::SetDeclared(bool isDeclared){
// 	m_isDeclared = isDeclared;
// }
// void VarSemSym::SetPtrDepth(int ptrDepth){
// 	m_ptrDepth = ptrDepth;
// }
// void FuncSemSym::SetDeclared(bool isDeclared){
// 	m_isDeclared = isDeclared;
// }
// void FuncSemSym::SetArgsList(std::string argsList){
// 	m_argsList = argsList;
// }
// bool FuncSemSym::SetType(std::string returnType){
// 	m_returnType = returnType;
// 	return true;
// }
// void FuncSemSym::SetId(std::string Id){
// 	m_name = Id;
// }

ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
}

bool ScopeTable::CheckDeclared(std::string check){
	SemSymbol * mySem;

	if(symbols->count(check))
	{
		return true;
	}
	return false;
}

SemSymbol* ScopeTable::GetSem(std::string getThis)
{
	return symbols->at(getThis);
}

bool ScopeTable::Insert(SemSymbol* sym){
	std::string myName = sym->GetId();
	std::pair< HashMap<std::string, SemSymbol *>::iterator, bool> ret;
	ret = symbols->insert(std::pair<std::string, SemSymbol *> (myName, sym));

	if(ret.second == 1)
	{

		return true;
	}
	else{
		
		//error 1
		std::cerr << sym->GetLine() << "," << sym->GetCol() << "Multiply declared identifier" << "\n";
		return false;
	}
}

SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be
	// entered into the front of the chain and upon exit the
	// latest scope table will be removed from the front of
	// the chain.
	scopeTableChain = new std::list<ScopeTable *>();
}

void SymbolTable::addFront(ScopeTable* newScope)
{
	scopeTableChain->push_front(newScope);
}

void SymbolTable::pop()
{
	scopeTableChain->pop_front();
}

bool SymbolTable::CheckDeclared(std::string check)
{
	bool result = true;
	for (std::list<ScopeTable *>::iterator it=scopeTableChain->begin(); it!=scopeTableChain->end(); ++it)
	{
		ScopeTable* temp = *it;
		result = temp->CheckDeclared(check);
		if(result == true)
		{
			return true;
		}
	}
	return result;
}

bool SymbolTable::addSym(SemSymbol* newSym){
	ScopeTable* top = scopeTableChain->front();
	return top->Insert(newSym);
}

SemSymbol* SymbolTable::GetSem(std::string getThis)
{
	
	ScopeTable* front = scopeTableChain->front();
	if(this->CheckDeclared(getThis)){
		for (std::list<ScopeTable *>::iterator it=scopeTableChain->begin(); it!=scopeTableChain->end(); ++it)
		{
			ScopeTable* temp = *it;
			if(temp->CheckDeclared(getThis))
			{
				return temp->GetSem(getThis);
			}
		}
	}
	return(nullptr);
}


}
