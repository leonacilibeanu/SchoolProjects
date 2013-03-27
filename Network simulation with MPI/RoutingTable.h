class RoutingTable
{
private:
    int* gateways;
    int size;
    
public:
    RoutingTable(int size)
    {
	this->size = size;
	
	gateways = new int[size];
    }
    
    int getRouteTo(int node)
    {
	if(node >= size)
	    throw "RoutingTable::getRouteTo() index out of bounds";
	    
	return gateways[node];
    }
    
    void update(int rank, GlobalTopology* globalTopology)
    {
	for(int i = 0; i < size; i++)
	{
	    if(i == rank)
	    {
		gateways[i] = i;
		continue;
	    }
	    
	    gateways[i] = globalTopology->findGateway(rank, i);
	}
    }
    
    void print()
    {	
	for(int i = 0; i < size; i++)
	    cerr<<gateways[i]<<" ";
	
	cerr<<flush;
    }
    
    ~RoutingTable()
    {
	delete[] gateways;
    }
};

