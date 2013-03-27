class Process
{
private:
    int rank;
    int size;
    LocalTopology* localTopology;
    GlobalTopology* globalTopology;
    RoutingTable* routingTable;
    
    bool probePacketReceived;
    int firstProbeSender;
    
    int topologyEchosToReceive;
    
    bool topologyEchoSent;
    bool topologyInfoReceived;
    
    vector<Message> messages;
    
    int* messagesSent;
    
    int* isDismantled;
    
    int* mainLeaderVotes;
    int* secondaryLeaderVotes;
    int votesReceived;
    
    int mainLeader;
    int secondaryLeader;
    
    bool isTie;
    int* votingStarted;
    
    void loadMessages(const char* message_filename)
    {
	fstream f;
	f.open(message_filename, fstream::in);
	
	int current_rank = -1;
	
	int lineCount;
	f>>lineCount;

	for(int i = 0; i < lineCount; i++)
	{
	    string str;
	    
	    std::getline(f, str);
	    
	    if(str.size() == 0)
	    {
		i--;
		continue;
	    }
	    
	    stringstream s(str, stringstream::in);
	    s>>current_rank;
	    
	    if(current_rank == rank)
	    {
		Message message(str);
		
		messages.push_back(message);
	    }
	}
	
	f.close();
    }
    
public:
    Process(int rank, int size, const char* topology_filename, const char* message_filename)
    {
	this->rank = rank;
	this->size = size;
	
	localTopology = new LocalTopology(rank, topology_filename);
	globalTopology = new GlobalTopology(size);
	
	globalTopology->merge(localTopology, rank);
	
	probePacketReceived = false;
	topologyEchoSent = false;
	topologyInfoReceived = false;
	
	firstProbeSender = 0;
	
	routingTable = new RoutingTable(size);
	
	loadMessages(message_filename);
	
	messagesSent = new int[size];
	
	for(int i = 0; i < size; i++)
	    messagesSent[i] = 0;
	
	isDismantled = new int[size];
	
	for(int i = 0; i < size; i++)
	    isDismantled[i] = 0;
	
	mainLeaderVotes = new int[size];
	
	for(int i = 0; i < size; i++)
	    mainLeaderVotes[i] = 0;
	
	secondaryLeaderVotes = new int[size];
	
	for(int i = 0; i < size; i++)
	    secondaryLeaderVotes[i] = 0;
	
	votingStarted = new int[size];
	
	for(int i = 0; i < size; i++)
	    votingStarted[i] = 0;

	
	votesReceived = 0;
	
	isTie = false;
    }

    bool votingIsStarted()
    {
	return votingStarted[0];
	/*
	for(int i = 0; i < size; i++)
	    if(!votingStarted[i])
		return false;
	    
	return true;*/
    }
    
    void resetVotes()
    {
	for(int i = 0; i < size; i++)
	    mainLeaderVotes[i] = 0;
	
	for(int i = 0; i < size; i++)
	    secondaryLeaderVotes[i] = 0;
	
	votesReceived = 0;
	isTie = false;
	
	for(int i = 0; i < size; i++)
	    votingStarted[i] = 0;
    }
    
    void resetDismantling()
    {
	for(int i = 0; i < size; i++)
	    isDismantled[i] = 0;
    }
    
    void resetMessagesSent()
    {
	for(int i = 0; i < size; i++)
	    messagesSent[i] = 0;
    }
    
    void updateRoutingTable()
    {
	routingTable->update(rank, globalTopology);
    }
    
    void sendProbeToNeighbors()
    {
	
	topologyEchosToReceive = localTopology->neighborCount() - (rank ? 1 : 0);
	
	for(int i = 0; i < localTopology->neighborCount(); i++)
	{   
	    //nu mai trimite packet de sondare si celui de la care a primit
	    if(localTopology->neighborAt(i) == firstProbeSender)
		continue;
	    
	    int buf[] = {};
	    
	    MPI_Send(&buf[0], sizeof(buf), MPI_INT, localTopology->neighborAt(i), CUSTOM_TAG_TOPOLOGY_PROBE, MPI_COMM_WORLD);
	}
    }
    
    void sendTopologyEcho()
    {
	//calculeaza o tabela de routare provizorie
	updateRoutingTable();

	if(rank)
	    MPI_Send(globalTopology->getData(), size * size, MPI_INT, firstProbeSender, CUSTOM_TAG_TOPOLOGY_ECHO, MPI_COMM_WORLD);
	else
	    topologyInfoReceived = true;
	    
	topologyEchoSent = true;
	
    }
    
    void sendTopologyToNeighbors()
    {
	topologyEchosToReceive = localTopology->neighborCount() - (rank ? 1 : 0);
	
	for(int i = 0; i < localTopology->neighborCount(); i++)
	{
	    MPI_Send(globalTopology->getData(), size * size, MPI_INT, localTopology->neighborAt(i), CUSTOM_TAG_TOPOLOGY_INFO, MPI_COMM_WORLD);
	}
    }
    
    bool discoveryComplete()
    {
	return topologyEchoSent;
    }
    
    bool messageSendingComplete()
    {
	for(int i = 0; i < size; i++)
	    if(!messagesSent[i] && !isDismantled[routingTable->getRouteTo(i)])
		return false;
	    
	return true;
    }
    
    bool isLeaf()
    {
	return (localTopology->neighborCount() < 2);
    }
    
    bool hasReceivedFullTopologyInfo()
    {
	return topologyInfoReceived;
    }
    
    void receiveDiscoveryPacket()
    {
	int* buf = new int[size * size];

	MPI_Status status;
	MPI_Recv(buf, size * size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	
	switch(status.MPI_TAG)
	{
	    case CUSTOM_TAG_TOPOLOGY_PROBE:
		if(!probePacketReceived)
		{
			probePacketReceived = true;
			firstProbeSender = status.MPI_SOURCE;
			
			sendProbeToNeighbors();
			
			//daca este nod frunza, nu are de ce sa mai astepte ecouri
			if(isLeaf())
			    sendTopologyEcho();
			
			//cerr<<endl<<rank<<" received FIRST PROBE packet from "<<firstProbeSender<<flush;
		}
		else
		{
			//send NULL echo
			int tmp[] = {};
			
			MPI_Send(&tmp[0], sizeof(tmp), MPI_INT, status.MPI_SOURCE, CUSTOM_TAG_TOPOLOGY_NULL_ECHO, MPI_COMM_WORLD);
			
			//cerr<<endl<<rank<<" received ANOTHER PROBE packet from "<<status.MPI_SOURCE<<flush;
		}
		
		break;
		
	    case CUSTOM_TAG_TOPOLOGY_NULL_ECHO:
		topologyEchosToReceive--;
		
		if(topologyEchosToReceive == 0)
		    sendTopologyEcho();
		
		//cerr<<endl<<rank<<" received NULL ECHO packet from "<<status.MPI_SOURCE<<" (waiting "<<topologyEchosToReceive<<" more echos)"<<flush;
		
		break;
		
	    case CUSTOM_TAG_TOPOLOGY_ECHO:
		globalTopology->merge(buf);
		
		topologyEchosToReceive--;
		
		if(topologyEchosToReceive == 0)
		    sendTopologyEcho();
		
		//cerr<<endl<<rank<<" received ECHO packet from "<<status.MPI_SOURCE<<" (waiting "<<topologyEchosToReceive<<" more echos)"<<flush;
		
		break;
		
	    case CUSTOM_TAG_TOPOLOGY_INFO:
		
		if(!topologyInfoReceived)
		{
		    topologyInfoReceived = true;
		    
		    delete globalTopology;
		    
		    globalTopology = new GlobalTopology(size);
		    globalTopology->merge(buf);
		    
		    //calculeaza tabela de routare
		    updateRoutingTable();
		    
		    sendTopologyToNeighbors();
		}
		
		break;
		
	    default:
		cerr<<endl<<"Unexpected packet with tag = "<<status.MPI_TAG;
		break;
	}

	delete [] buf;
    }
    
    void sendMessage(Message &message)
    {
	if(message.destination == MESSAGE_DESTINATION_BROADCAST)
	{
	    for(int i = 0; i < localTopology->neighborCount(); i++)
	    {
		if(i == rank)
		    continue;
	    
		MPI_Send((void*)message.line.data(), message.line.size() + 1, MPI_CHAR, localTopology->neighborAt(i), CUSTOM_TAG_MESSAGE, MPI_COMM_WORLD);
	    }
	}
	else
	{
	    int route = routingTable->getRouteTo(message.destination);
	    
	    MPI_Send((void*)message.line.data(), message.line.size() + 1, MPI_CHAR, route, CUSTOM_TAG_MESSAGE, MPI_COMM_WORLD);
	}

    }
    
    bool canDismantle()
    {
	int dismantledNeighborCount = 0;
	
	for(int i = 0; i < localTopology->neighborCount(); i++)
	    dismantledNeighborCount += isDismantled[localTopology->neighborAt(i)];
	
	if(localTopology->neighborCount() - dismantledNeighborCount <= 1)
	    return true;
	
	return false;
    }
    
    bool allNeighborsDismantled()
    {
	int dismantledNeighborCount = 0;
	
	for(int i = 0; i < localTopology->neighborCount(); i++)
	    if(!isDismantled[localTopology->neighborAt(i)])
		return false;
		
	return true;
    }
    
    void dismantle()
    {
	//cerr<<endl<<"Dismantle "<<rank;
	
	for(int i = 0; i < localTopology->neighborCount(); i++)
	    if(!isDismantled[localTopology->neighborAt(i)])
	    {
		    char tmp[] = {};
		    
		    MPI_Send(&tmp[0], sizeof(tmp), MPI_CHAR, localTopology->neighborAt(i), CUSTOM_TAG_DISMANTLE, MPI_COMM_WORLD);
	    }
	    
	isDismantled[rank] = 1;
    }
    
    void routingOnly()
    {
	char* buf = new char[0xFFF];

	MPI_Request req;
	MPI_Irecv(buf, 0xFFF, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &req);

	int flag = 1;

	time_t start = time(NULL);
	
	while(time(NULL) - start < 1 || flag)
	{
	    MPI_Status status;
	    
	    MPI_Test(&req, &flag, &status);
	    
	    if(!flag)
		continue;
	    
	    switch(status.MPI_TAG)
	    {
		case CUSTOM_TAG_MESSAGE:
		{
		    string line(buf);
		    Message message(line);
		    

		    if(message.destination != rank && message.destination != MESSAGE_DESTINATION_BROADCAST)
			sendMessage(message);
		    
		    break;
		}
	    }
	    
	    MPI_Irecv(buf, 0xFFF, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &req);
	}
	
	MPI_Cancel(&req);
    }
    
    bool dismantleComplete()
    {
	return isDismantled[rank];
    }
    
    bool isTieVote()
    {
	return isTie;
    }
    
    void sendAllMessagesSentNotification()
    {
	for(int i = 0; i < size; i++)
	{
	    if(i == rank)
		continue;
	    
	    stringstream s;
	    
	    s<<rank<<" "<<i<<" "<<ALL_MESSAGES_SENT_TEXT;
	    
	    Message m(s.str());
	    
	    sendMessage(m);
	}
	
	messagesSent[rank] = 1;
    }
    
    void receiveMessagePacket()
    {
	char* buf = new char[0xFFF];

	MPI_Status status;
	MPI_Recv(buf, 0xFFF, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	
	switch(status.MPI_TAG)
	{
	    case CUSTOM_TAG_MESSAGE:
	    {
		string line(buf);
		Message message(line);
		

		if(message.data != ALL_MESSAGES_SENT_TEXT && message.data != MESSAGE_TEXT_VOTING_STARTED/* && message.destination == rank*/)
		{
		    cerr<<endl;
		    
		    if(message.destination == rank)
			cerr<<"[REC] ";
		    else
			cerr<<"[FWD] ";
		    
		    cerr<<status.MPI_SOURCE<<" -> "<<rank<<": ";
		    message.print();
		}
		
		if(message.destination != rank && message.destination != MESSAGE_DESTINATION_BROADCAST)
		    sendMessage(message);

		if(message.destination == rank && message.data.substr(0, strlen(MESSAGE_TEXT_VOTE)) == MESSAGE_TEXT_VOTE)
		{
		    int mainLeaderVote;
		    int secondaryLeaderVote;
		    
		    stringstream s(message.data.substr(strlen(MESSAGE_TEXT_VOTE))); 
		    
		    s>>mainLeaderVote;
		    s>>secondaryLeaderVote;
		    
		    mainLeaderVotes[mainLeaderVote]++;
		    secondaryLeaderVotes[secondaryLeaderVote]++;
		    
		    votesReceived++;
		    
		    //cerr<<endl<<"[ "<<rank<<" ] Received "<<votesReceived<<" / "<<size<<" votes";
		    
		    if(votesReceived == size)
			sendVoteResult();
		}
		
		if(message.destination == rank && message.data.substr(0, strlen(MESSAGE_TEXT_VOTE_RESULT)) == MESSAGE_TEXT_VOTE_RESULT)
		{
		    //voteResultReceived = true;
		    
		    stringstream s(message.data.substr(strlen(MESSAGE_TEXT_VOTE_RESULT))); 
		    
		    s>>mainLeader;
		    s>>secondaryLeader;
		    
		    isTie = false;
		    
		    sendAllMessagesSentNotification();
		}
		
		if(message.destination == rank && message.data == MESSAGE_TEXT_VOTING_STARTED)
		{
		    votingStarted[message.source] = 1;
		}
		
		if(message.destination == rank && message.data == MESSAGE_TEXT_TIE_VOTE)
		{
		    //voteResultReceived = true;
		    
		    isTie = true;
		    
		    sendAllMessagesSentNotification();
		}
		
		if(message.data == ALL_MESSAGES_SENT_TEXT && message.destination == rank)
		{
		    messagesSent[message.source] = 1;
		    
		    if(messageSendingComplete())
		    {
			if(canDismantle())
			    dismantle();
		    }
		}
		
//  		if(message.destination == rank || message.destination == MESSAGE_DESTINATION_BROADCAST)
// 		{
// 		    cerr<<endl<<rank<<" a primit de la "<<message.source<<": ";
// 		    message.print();
// 		}
		    
		
		break;
	    }
		
	    case CUSTOM_TAG_DISMANTLE:
		isDismantled[status.MPI_SOURCE] = 1;
		
		if(messageSendingComplete()/* || allNeighborsDismantled()*/)
		{
		    if(canDismantle())
			dismantle();
		}
		    
		break;
		
// 	    case CUSTOM_TAG_ALL_MESSAGES_SENT:
// 		allMessagesSent[status.MPI_SOURCE] = 1;
// 		break;
		
	    case CUSTOM_TAG_TOPOLOGY_INFO:
		break;
		
	    default:
		cerr<<endl<<"Unexpected packet with tag = "<<status.MPI_TAG;
		break;
	}
    }
    
    void sendMessageToAll(string &data)
    {
	for(int i = 0; i < size; i++)
	{
	    if(i == rank)
		continue;
	    
	    stringstream s;
	    
	    s<<rank<<" "<<i<<" "<<data;
	    
	    Message m(s.str());
	    
	    sendMessage(m);
	}
    }
    
    void sendVoteResult()
    {
	isTie = false;
	
	mainLeader = 0;
	
	for(int i = 0; i < size; i++)
	    if(mainLeaderVotes[i] > mainLeaderVotes[mainLeader])
		mainLeader = i;
	    
	secondaryLeader = 0;
	
	for(int i = 0; i < size; i++)
	    if(secondaryLeaderVotes[i] > secondaryLeaderVotes[secondaryLeader])
		secondaryLeader = i;
	    
	for(int i = 0; i < size; i++)
	    if(mainLeaderVotes[i] == mainLeaderVotes[mainLeader] && i != mainLeader)
		isTie = true;
	    
	for(int i = 0; i < size; i++)
	    if(secondaryLeaderVotes[i] == secondaryLeaderVotes[secondaryLeader] && i != secondaryLeader)
		isTie = true;
	    
	if(isTie)
	{
	    string data(MESSAGE_TEXT_TIE_VOTE);
	    
	    sendMessageToAll(data);
	}
	else
	{
	    stringstream s;
	    s<<MESSAGE_TEXT_VOTE_RESULT<<" "<<mainLeader<<" "<<secondaryLeader;
	    
	    string data = s.str();
	    
	    sendMessageToAll(data);
	}
	
	sendAllMessagesSentNotification();
    }
    
/*
    void receiveVotes()
    {

	int* buf = new int[4];

	MPI_Status status;
	MPI_Recv(buf, 4, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	
	switch(status.MPI_TAG)
	{
	    case CUSTOM_TAG_VOTE:
		if(rank)
		{
		    int route = routingTable->getRouteTo(0);
		    
		    MPI_Send(&buf[0], 4, MPI_INT, route, CUSTOM_TAG_VOTE, MPI_COMM_WORLD);
		}
		else
		{
		    mainLeaderVotes[buf[2]]++;
		    secondaryLeaderVotes[buf[3]]++;
		    
		    votesReceived++;
		    
		    if(votesReceived == size)
			sendVoteResult();
		}
		break;
	}
    }
*/


    void sendMessages()
    {
	for(int i = 0; i < messages.size(); i++)
	{
	    sendMessage(messages.at(i));
	}
	
	sendAllMessagesSentNotification();
    }
    
    void voteAndSendLeaders()
    {
	int mainLeaderVote;
	int secondaryLeaderVote;
	
	do
	{
	    mainLeaderVote = rand() % size;
	}
	while(mainLeaderVote == rank);
	
	do
	{
	    secondaryLeaderVote = rand() % size;
	}
	while(secondaryLeaderVote == rank);

	if(rank)
	{
	    stringstream s;
	    s<<rank<<" 0 "<<MESSAGE_TEXT_VOTE<<" "<<mainLeaderVote<<" "<<secondaryLeaderVote;
	
	    Message m(s.str());
	
	    sendMessage(m);
	}
	else
	{
	    mainLeaderVotes[mainLeaderVote]++;
	    secondaryLeaderVotes[secondaryLeaderVote]++;
	    
	    votesReceived++;
	}
    }
    
    int getRank()
    {
	return rank;
    }
    
    void printGlobalTopology()
    {
	globalTopology->print();
    }
    
    void printLocalTopology()
    {
	localTopology->print();
    }
    
    void printRoutingTable()
    {
	cerr<<endl<<"Tabela de routare pentru nodul "<<rank<<": ";
	routingTable->print();
    }
    
    int* getGlobalTopologyData()
    {
	return globalTopology->getData();
    }
    
    void sendVotingStart()
    {
	string s(MESSAGE_TEXT_VOTING_STARTED);
	sendMessageToAll(s);
	
	votingStarted[rank] = 1;
    }
    
    int getMainLeader()
    {
	return mainLeader;
    }
    
    int getSecondaryLeader()
    {
	return secondaryLeader;
    }
    
    ~Process()
    {
	delete localTopology;
	delete globalTopology;
	delete routingTable;
	delete[] messagesSent;
	delete[] isDismantled;
	delete[] votingStarted;
    }
};

