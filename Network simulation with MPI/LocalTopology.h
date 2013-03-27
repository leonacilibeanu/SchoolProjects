class LocalTopology
{
private:
    vector<int> neighbors;
    
public:
    LocalTopology(int rank, const char* topology_filename)
    {
	fstream f;
	f.open(topology_filename, fstream::in);
	
	int current_rank = -1;
	
	while(!f.eof())
	{
	    string str;
	    
	    f>>str;

	    if(str.size() == 0)
		continue;
	    
	    stringstream s(str, stringstream::in);
	    
	    if(str.at(str.size() - 1) == ':')
	    {
		s>>current_rank;
	    }
	    else if(current_rank == rank)
	    {
		int adjacent_node;

		s>>adjacent_node;

		neighbors.push_back(adjacent_node);
	    }
	}
	
	f.close();
    }
    
    void print()
    {
	for(int i = 0; i < neighbors.size(); i++)
	    cerr<<" "<<neighbors.at(i);
    }
    
    int neighborCount()
    {
	return neighbors.size();
    }
    
    int neighborAt(int i)
    {
	return neighbors.at(i);
    }
};

