class BFSInfo
{
public:
    int priority;
    int gateway;
    int node;
    
    BFSInfo(int priority, int gateway, int node)
    {
	this->priority = priority;
	this->gateway = gateway;
	this->node = node;
    }
    
/*    
    void operator=(BFSInfo &info)
    {
	this->priority = info.priority;
	this->gateway = info.gateway;
	this->node = info.node;
    }
*/
};

//folosit in algoritmul BFS pentru priority_queue
class BFSComparator
{
public:
    //returneaza true daca t1 < t2
    bool operator()(BFSInfo& t1, BFSInfo& t2)
    {
       if (t1.priority < t2.priority) 
	   return true;
       
       return false;
    }
};

//folosit de algoritmul BFS
class BFSSuccessor
{
public:
    int gateway;
    int node;
    
    BFSSuccessor(int gateway, int node)
    {
	this->gateway = gateway;
	this->node = node;
    }
};

class GlobalTopology
{
private:
    Matrix* adjacencyMatrix;
    
public:
    GlobalTopology(int size)
    {
	adjacencyMatrix = new Matrix(size, size);
	
	for(int i = 0; i < size; i++)
	    for(int j = 0; j < size; j++)
		adjacencyMatrix->set(i, j, 0);
    }
    
    int* getData()
    {
	return adjacencyMatrix->getData();
    }
    
    void merge(int* data)
    {
	Matrix m(adjacencyMatrix->getRowCount(), adjacencyMatrix->getColumnCount(), data, false);
	
	for(int i = 0; i < m.getRowCount(); i++)
	    for(int j = 0; j < m.getColumnCount(); j++)
		if(m.get(i, j))
		    adjacencyMatrix->set(i, j, 1);
    }
    
    void merge(LocalTopology* localTopology, int rank)
    {
	for(int i = 0; i < localTopology->neighborCount(); i++)
	{
	    adjacencyMatrix->set(localTopology->neighborAt(i), rank, 1);
	    adjacencyMatrix->set(rank, localTopology->neighborAt(i), 1);
	}
    }
    
    int getSize()
    {
	return adjacencyMatrix->getRowCount();
    }
    
    int isAdjacent(int i, int j)
    {
	return adjacencyMatrix->get(i, j);
    }
    
    void print()
    {
	cerr<<endl<<"Matricea de adiacenta:"<<endl;
	
	adjacencyMatrix->print();
    }
    
    vector<BFSSuccessor> getSuccessors(BFSInfo &state, int startNode)
    {
	vector<BFSSuccessor> result;

	for(int i = 0; i < getSize(); i++)
	    if(adjacencyMatrix->get(state.node, i))
	    {
		BFSSuccessor successor(state.node == startNode ? i : state.gateway, i);
		
		result.push_back(successor);
	    }
	    
	return result;
    }
    
    //drumul cel mai scurt se determina utilizand algoritmul BFS (breadth first search) extins pentru grafuri
    int findGateway(int startNode, int goalNode)
    {
	set<int> expandedNodes;
	priority_queue<BFSInfo, vector<BFSInfo>, BFSComparator> fringe;
	
	int priority = 0;

	BFSInfo state(priority, startNode, startNode);
	
	while(state.node != goalNode)
	{
	    //verifica daca acest nod a fost deja expandat
	    if(expandedNodes.find(state.node) != expandedNodes.end())
	    {
		state = fringe.top();
		fringe.pop();

		continue;
	    }
	    
	    vector<BFSSuccessor> successors = getSuccessors(state, startNode);
	    expandedNodes.insert(state.node);
	    
	    priority++;
	    
	    for(int i = 0; i < successors.size(); i++)
	    {
		BFSSuccessor successor = successors.at(i);
		
		//daca nodul nu a fost deja expandat, va fi explorat
		if(expandedNodes.find(successor.node) == expandedNodes.end())
		{
		    BFSInfo bfsInfo(priority, successor.gateway, successor.node);

		    fringe.push(bfsInfo);
		}
	    }
	    
	    if(fringe.empty())
		break;
	    
	    state = fringe.top();
	    fringe.pop();
	}
	
	return state.gateway;
    }
    
    ~GlobalTopology()
    {
	delete adjacencyMatrix;
    }
}; 
