/*
 * comp-attr-graph.cc
 *
 *  Created on: Feb 27, 2016
 *      Author: cchen
 */

#include "comp-attr-graph.h"

NS_LOG_COMPONENT_DEFINE ("AttriGraph");

int AttriGraph::defaultSize = 1000;

/*
 * Create an attribute graph for a given program and event name
 *
 * The graph is constructed only for the subprogram where
 * its execution is influenced by the event
 *
 * NOTE: We require that variables at the right-hand side of an assignment
 * and in a selection be defined before its appearance
 */
AttriGraph::AttriGraph(Ptr<OlContext> ctxt, string evName)
{
	NS_LOG_FUNCTION(this);
	varCount = 0;
	event = NULL;

	nodeArray = new AttriList[defaultSize];
	for (int i = 0;i < defaultSize;i++)
	{
		nodeArray[i].reached = false;
		nodeArray[i].head = NULL;
	}

	OlContext::RuleList* rules = ctxt->GetRules();

	//Pre-processing: ensure there is only one event in the program
	int evCount = 0;
	OlContext::Rule* initRule = NULL; //The rule of the event
	OlContext::RuleList::iterator itrRules;
	for (itrRules = rules->begin();itrRules != rules->end();++itrRules)
	{
		OlContext::Rule* rnode = *itrRules;
		list<ParseTerm*>& terms = rnode->terms;
		std::list<ParseTerm*>::iterator itrParse;
		for (itrParse = terms.begin();itrParse != terms.end();++itrParse)
		{
			ParseFunctor* btuple = dynamic_cast<ParseFunctor*>(*itrParse);
			if (btuple != NULL)
			{
				ParseFunctorName* tname = btuple->fName;
				if (tname->ToString() == evName)
				{
					initRule = rnode;
					event = btuple;

					evCount++;
					break; //A rule does not contain self-join
				}
			}
		}

		if (evCount > 1)
		{
			NS_LOG_ERROR("More than one event is found!!!");
			return;
		}
	}

	//Create nodes for the original event tuple
	ParseExprList* args = event->m_args;
	deque<ParseExpr*>::iterator itrExpr = args->begin();
	for(int i = 0;itrExpr != args->end();++i,++itrExpr)
	{
		//Create mapping between variables and node IDs
		string tname = event->fName->ToString();
		VarID vi(tname, i);
		vnMap.insert(VarNodeMap::value_type(vi, varCount));
		varCount++;
	}

	//Construct the attribute graph for the subproblem related to the event
	BuildRuleGraph(initRule, event, rules);
}

/*
 * Recursively construct the dependency graph
 */
void AttriGraph::BuildRuleGraph(OlContext::Rule* rl,
								ParseFunctor* ev,
								OlContext::RuleList* rules)
{
	NS_LOG_FUNCTION(rl->ruleID);

	//Loop check
	if (std::find(procsedRules.begin(), procsedRules.end(), rl) != procsedRules.end())
	{
		NS_LOG_INFO("Repeated rules!");
		return;
	}
	else
	{
		procsedRules.push_back(rl);
	}

	ParseFunctor* head = rl->head;
	list<ParseTerm*>& terms = rl->terms;

	int assignID = 0; //Identify the assignment in a rule
	int selectID = 0; //Identify the selection in a rule
	map<string, int> asnVarNode; // Map the head variable of an assignment to nodeID
	std::list<ParseTerm*>::iterator itrParse;
	for (itrParse = terms.begin();itrParse != terms.end();++itrParse)
	{
		//Process an assignment
		//Assignment needs to be processed ahead of body tuples,
		//because the assigned variable could appear as body attributes
		ParseAssign* assi = dynamic_cast<ParseAssign*>(*itrParse);
		if (assi != NULL)
		{
			NS_LOG_DEBUG("Process an assignment...");
			ostringstream os;
			os << "asgn-" << rl->ruleID << "-" << assignID;
			string assignName = os.str();
			assignID++;

			//Process the assigned head variable
			int varIdx = 0; // Identification of variables in an assignment
			int headID = varCount;
			VarID vi(assignName, varIdx);
			vnMap.insert(VarNodeMap::value_type(vi, headID));

			ParseVar* assiHead = assi->var;
			string headName = assiHead->ToString();
			asnVarNode.insert(std::pair<string, int>(headName, headID));
			varIdx++;
			varCount++;

			//Process the expression on the right side of the assignment
			ParseExpr* assiExpr = assi->assign;
			ProcExpr(assiExpr, AttriNode::Assign, assignName, varIdx, ev, asnVarNode);
			//For each variable on the right-hand side, connect to left-side variable node
			int tempVarIdx = 1; // 0 is reserved for the left side variable
			ConnAssignHead(assiExpr, assignName, headID, tempVarIdx);
		}

		//Process a body tuple
		ParseFunctor* tp = dynamic_cast<ParseFunctor*>(*itrParse);
		if (tp != NULL && tp != ev)
		{
			NS_LOG_DEBUG("Process a body tuple...");
			ProcNonEv(tp, AttriNode::Base, ev, asnVarNode);
		}

		//Process a selection
		ParseSelect* slt = dynamic_cast<ParseSelect*>(*itrParse);
		if (slt != NULL)
		{
			NS_LOG_DEBUG("Process a selection...");
			ostringstream osSlt;
			osSlt << "select-" << rl->ruleID << "" << selectID;
			string selectName = osSlt.str();
			selectID++;

			int varSltIdx = 0; // Identification of variables in a selection
			ParseBool* sltBody = slt->select;
			ProcExpr(sltBody->lhs, AttriNode::Select, selectName, varSltIdx, ev, asnVarNode);
			ProcExpr(sltBody->rhs, AttriNode::Select, selectName, varSltIdx, ev, asnVarNode);
		}

		//We do not handle Aggregation for now
	}

	//Process the head tuple
	NS_LOG_DEBUG("Process the head tuple...");
	ProcNonEv(head, AttriNode::Intm, ev, asnVarNode);

	//Find rules where the head tuple serves as the event
	string headName = head->fName->ToString();
	OlContext::RuleList::iterator itrRules;
	for (itrRules = rules->begin();itrRules != rules->end();++itrRules)
	{
		list<ParseTerm*>& terms = (*itrRules)->terms;
		std::list<ParseTerm*>::iterator itrParTerm;
		for (itrParTerm = terms.begin();itrParTerm != terms.end();++itrParTerm)
		{
			ParseFunctor* tp = dynamic_cast<ParseFunctor*>(*itrParTerm);
			if (tp != NULL)
			{
				string tpName = tp->fName->ToString();
				if (headName == tpName)
				{
					BuildRuleGraph(*itrRules, tp, rules);
					break;
				}
			}
		}
	}
}

bool AttriGraph::ConnectEventVar(ParseVar* var, int nodeID,
								 AttriNode::PredType ptype, ParseFunctor* ev)
{
	string varName = var->ToString();
	NS_LOG_FUNCTION(varName);

	bool connFlag = false; // Indicate if the variable is found in the event

	ParseExprList* evArgs = ev->m_args;
	deque<ParseExpr*>::iterator itrExprEv = evArgs->begin();
	for(int j = 0;itrExprEv != evArgs->end();++j,++itrExprEv)
	{
		string evVarName = (*itrExprEv)->ToString();
		if (varName == evVarName)
		{
			connFlag = true;

			//Create the edge
			string evName = ev->fName->ToString();
			VarID vi(evName, j);
			int evIdx =  vnMap.find(vi)->second;
			AddEdge(nodeID, ptype, evIdx, AttriNode::Event);
		}
	}

	return connFlag;
}

/*
 * Connect variables on the right hand side of an assignment to
 * the variable on the left side.
 */
void AttriGraph::ConnAssignHead(ParseExpr* expr, string assignName, int headID, int& varIdx)
{
	NS_LOG_FUNCTION(headID);

	//Process a variable
	ParseVar* var = dynamic_cast<ParseVar*>(expr);
	if (var != NULL)
	{
		VarID vi(assignName, varIdx);
		VarNodeMap::iterator itrVn = vnMap.find(vi);
		if (itrVn == vnMap.end())
		{
			NS_LOG_ERROR("The variable is out of scope!!!");
			return;
		}
		else
		{
			int varId = itrVn->second;
			AddEdge(varId, AttriNode::Assign, headID, AttriNode::Assign);
			varIdx++;
		}
	}

	//Process an arithmetic expression
	ParseMath* math = dynamic_cast<ParseMath*>(expr);
	if (math != NULL)
	{
		ConnAssignHead(math->lhs, assignName, headID, varIdx);
		ConnAssignHead(math->rhs, assignName, headID, varIdx);
	}

	//Process a user-defined function
	ParseFunction* func = dynamic_cast<ParseFunction*>(expr);
	if (func != NULL)
	{
		ParseExprList* args = func->m_args;
		ParseExprList::iterator itrExprl;
		for (itrExprl = args->begin();itrExprl != args->end();itrExprl++)
		{
			ConnAssignHead(*itrExprl, assignName, headID, varIdx);
		}
	}
}

/*
 * Process an NDLog expression
 *
 * Arguments:
 * expr: the expression to be processed
 * ptype: the type of the expression
 * bodyName: the name assigned to the expression
 * varIdx: index of the variable
 * ev: the event relation
 * asnVarNode: mapping between left-hand side variables of assignments
 * 			   and their nodes in the graph
 */

void AttriGraph::ProcExpr(ParseExpr* expr,
						  AttriNode::PredType ptype,
						  string bodyName,
						  int& varIdx,
						  ParseFunctor* ev,
						  map<string, int>& asnVarNode)
{
	NS_LOG_FUNCTION(bodyName);

	//Process a variable
	ParseVar* var = dynamic_cast<ParseVar*>(expr);
	if (var != NULL)
	{
		//Create a node for the variable
		VarID vi(bodyName, varIdx);
		int nodeID = varCount;
		vnMap.insert(VarNodeMap::value_type(vi, nodeID));
		varCount++;
		varIdx++;

		//Create an edge to the event variable, if possible
		ConnectEventVar(var, nodeID, ptype, ev);

		//Create edges to the same head variables of other assignments
		string varName = var->ToString();
		map<string, int>::iterator itrVar = asnVarNode.find(varName);
		if (itrVar != asnVarNode.end())
		{
			int headID = itrVar->second;
			AddEdge(nodeID, ptype, headID, AttriNode::Assign);
		}
	}

	//Process an arithmetic expression
	ParseMath* math = dynamic_cast<ParseMath*>(expr);
	if (math != NULL)
	{
		ProcExpr(math->lhs, ptype, bodyName, varIdx, ev, asnVarNode);
		ProcExpr(math->rhs, ptype, bodyName, varIdx, ev, asnVarNode);
	}

	//Process a user-defined function
	ParseFunction* func = dynamic_cast<ParseFunction*>(expr);
	if (func != NULL)
	{
		ParseExprList* args = func->m_args;
		ParseExprList::iterator itrExprl;
		for (itrExprl = args->begin();itrExprl != args->end();itrExprl++)
		{
			ProcExpr(*itrExprl, ptype, bodyName, varIdx, ev, asnVarNode);
		}
	}
}

bool AttriGraph::ProcNonEv(ParseFunctor* tp, AttriNode::PredType ptype,
						   ParseFunctor* ev, map<string, int>& asnVarNode)
{
	string tname = tp->fName->ToString();
	NS_LOG_FUNCTION(tname);

	ParseExprList* args = tp->m_args;
	deque<ParseExpr*>::iterator itrExpr = args->begin();
	for(int i = 0;itrExpr != args->end();++i,++itrExpr)
	{
		//Create mapping between variables and node IDs
		VarID vi(tname, i);
		pair<VarNodeMap::iterator, bool> ret;
		ret = vnMap.insert(VarNodeMap::value_type(vi, varCount));
		int tpIdx = 0;
		if (ret.second == false)
		{
			//A relation of the same name has existed
			tpIdx = ret.first->second;
		}
		else
		{
			tpIdx = varCount;
			varCount++;
		}

		//Create edges for join keys
		ParseVar* varArg = dynamic_cast<ParseVar*>(*itrExpr);
		if (varArg != NULL)
		{
			string varName = varArg->ToString();

			//Create edges to the same variables of the event
			ParseExprList* evArgs = ev->m_args;
			deque<ParseExpr*>::iterator itrExprEv = evArgs->begin();
			for(int j = 0;itrExprEv != evArgs->end();++j,++itrExprEv)
			{
				string evVarName = (*itrExprEv)->ToString();
				if (varName == evVarName)
				{
					//Create the edge
					string evName = ev->fName->ToString();
					VarID vi(evName, j);
					int evIdx =  vnMap.find(vi)->second;
					AddEdge(tpIdx, ptype, evIdx, AttriNode::Event);
				}
			}

			//Create edges to the same head variables of assignment
			map<string, int>::iterator itrVar = asnVarNode.find(varName);
			if (itrVar != asnVarNode.end())
			{
				int headID = itrVar->second;
				AddEdge(tpIdx, ptype, headID, AttriNode::Assign);
			}
		}
	}

	return true;
}

/*
 * Create an AttriNode for a given nodeID
 */
AttriNode* AttriGraph::CreateNode(int nodeID, AttriNode::PredType ptype)
{
	NS_LOG_FUNCTION(nodeID);

	AttriNode* newNode = new AttriNode;
	newNode->nodeID = nodeID;
	newNode->nodeType = ptype;
	newNode->next = NULL;

	return newNode;
}

/*
 * Add an undirected edge
 */
void AttriGraph::AddEdge(int src, AttriNode::PredType srcType,
			 	 	 	 int dst, AttriNode::PredType dstType)
{
	NS_LOG_FUNCTION(src << dst);

	AttriNode* newSrc = CreateNode(src, srcType);
	newSrc->next = nodeArray[dst].head;
	nodeArray[dst].head = newSrc;

	AttriNode* newDst = CreateNode(dst, dstType);
	newDst->next = nodeArray[src].head;
	nodeArray[src].head = newDst;
}

/*
 * Find the event attributes that determine the equivalence class
 */
void AttriGraph::FindEquiAttrs()
{
	NS_LOG_FUNCTION(event->fName->ToString());

	assert (event != NULL);

	ParseExprList* args = event->m_args;
	for(int i = 0;i < args->size();i++)
	{
		//Find the node corresponding to the event attribute
		string evName = event->fName->ToString();
		VarID vid(evName, i);
		int evNodeID = vnMap.find(vid)->second;

		ResetLabels();
		bool res = CouldReachBaseSel(evNodeID);
		if (res == true)
		{
			attrs.push_back(i);
		}
	}
}

/*
 * Reset the labels of all nodes to false
 */
void AttriGraph::ResetLabels()
{
	NS_LOG_FUNCTION(this);
	for (int i = 0;i < varCount;i++)
	{
		nodeArray[i].reached = false;
	}
}

/*
 * Determine if an event attribute could reach a base attribute
 */
bool AttriGraph::CouldReachBaseSel(int nodeID)
{
	NS_LOG_FUNCTION(nodeID);
	nodeArray[nodeID].reached = true;

	//Recursively check each neighbor node
	AttriNode* curNode = nodeArray[nodeID].head;
	while (curNode != NULL)
	{
		//An event attribute that joins with base attributes or
		//appears in a selection should be qualified
		if (curNode->nodeType == AttriNode::Base ||
			curNode->nodeType == AttriNode::Select)
		{
			return true;
		}
		else
		{
			int curID = curNode->nodeID;
			if (nodeArray[curID].reached == false)
			{
				bool res = CouldReachBaseSel(curNode->nodeID);
				if (res == true)
				{
					return res;
				}
			}
		}
		curNode = curNode->next;
	}

	return false;
}

vector<int> AttriGraph::GetEquiAttrs()
{
	return attrs;
}

void AttriGraph::PrintGraph()
{
	NS_LOG_FUNCTION(varCount);
	std::cout << endl;

	//Print the mapping between variables and nodes
	VarNodeMap::iterator itrVnode;
	for (itrVnode = vnMap.begin();itrVnode != vnMap.end();itrVnode++)
	{
		VarID vid = itrVnode->first;
		int nodeID = itrVnode->second;

		std::cout << "(" << vid.first << "," << vid.second << ")";
		std::cout << ": " << nodeID << endl;
	}

	std::cout << endl;
	//Print the graph
	for (int i = 0;i < varCount;i++)
	{
		std::cout << "node " << i << ":";
		AttriNode* curNode = nodeArray[i].head;
		while (curNode != NULL)
		{
			std::cout << "node (" << curNode->nodeID << ",";
			switch (curNode->nodeType)
			{
			case AttriNode::Base: std::cout << "Base";break;
			case AttriNode::Event: std::cout << "Event";break;
			case AttriNode::Assign: std::cout << "Assign";break;
			case AttriNode::Intm: std::cout << "Intm";break;
			case AttriNode::Select: std::cout << "Select";break;
			}

			std::cout << "), ";
			curNode = curNode->next;
		}
		std::cout << endl;
	}

	std::cout << endl << endl;
}

void AttriGraph::PrintEquiAttrs()
{
	NS_LOG_FUNCTION(event->fName->ToString());
	std::cout << endl;
	std::cout << "Print the variables that determine equivalence classes";
	std::cout << endl;
	vector<int>::iterator itr;
	for (itr = attrs.begin();itr != attrs.end();itr++)
	{
		cout << *itr << ",";
	}

	std::cout << endl;
}

/*
 * Recursively destroy the adjacency list
 */
void AttriGraph::DestroyRestList(AttriNode* anode)
{
	while (anode != NULL)
	{
		AttriNode* next = anode->next;
		delete anode;
		anode = next;
	}

	return;
}

AttriGraph::~AttriGraph()
{
	NS_LOG_FUNCTION(event->fName->ToString());
	for (int i = 0; i < varCount; i++)
	{
		AttriNode* head = nodeArray[i].head;
		DestroyRestList(head);
	}

	delete[] nodeArray;
}

