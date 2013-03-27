class Message
{
public:
    int source;
    int destination;
    string data;
    
    string line;
    
    Message(string line)
    {
	this->line = line;
	
	stringstream s(line, stringstream::in);
	s>>source;
	
	streampos p = s.tellg();
	
	string strDestination;
	s>>strDestination;

	if(strDestination.at(0) != 'B')
	{
	    s.seekg(p);
	    s>>destination;
	}
	else
	{
	    destination = MESSAGE_DESTINATION_BROADCAST;
	}
	
	std::getline(s, data);
	
	while(data.at(0) == ' ')
	    data = data.substr(1);
    }
    
    void print()
    {
	cerr<<"{source: "<<source<<", destination: ";
	
	if(destination == MESSAGE_DESTINATION_BROADCAST)
	    cerr<<"B";
	else
	    cerr<<destination;
	
	cerr<<", data: '"<<data<<"'}"<<flush;
    }
};

