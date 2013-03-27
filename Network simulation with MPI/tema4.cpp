#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <set>
#include <ctime>

#include <stdint.h>
#include <unistd.h>

#include <mpi.h>

#define MESSAGE_DESTINATION_BROADCAST -1

#define CUSTOM_TAG_TOPOLOGY_PROBE 1
#define CUSTOM_TAG_TOPOLOGY_ECHO 2
#define CUSTOM_TAG_TOPOLOGY_NULL_ECHO 3
#define CUSTOM_TAG_TOPOLOGY_INFO 4
#define CUSTOM_TAG_MESSAGE 5
#define CUSTOM_TAG_DISMANTLE 6
#define ALL_MESSAGES_SENT_TEXT "[SPECIAL] ALL MESSAGES SENT!"
#define MESSAGE_TEXT_VOTE "[SPECIAL] VOTE:"
#define MESSAGE_TEXT_TIE_VOTE "[SPECIAL] TIE VOTE!"
#define MESSAGE_TEXT_VOTE_RESULT "[SPECIAL] VOTE RESULT:"
#define MESSAGE_TEXT_VOTING_STARTED "[SPECIAL] VOTING STARTED!"

using namespace std;

#include "Matrix.h"
#include "LocalTopology.h"
#include "GlobalTopology.h"
#include "RoutingTable.h"
#include "Message.h"
#include "Process.h"

int main(int argc, char* argv[])
{
    int rank, size;
    MPI_Status status;
    
    if(argc != 3)
    {
	cerr<<endl<<"Usage: "<<argv[0]<<" fisier_topologie fisier_mesaje"<<endl;
	return 0;
    }
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    const char* topology_filename = argv[1];
    const char* message_filename = argv[2];
    
    Process process(rank, size, topology_filename, message_filename);
    
// if(rank)
// {
//     int i = 0;
//     char hostname[256];
//     gethostname(hostname, sizeof(hostname));
//     printf("PID %d on %s ready for attach\n", getpid(), hostname);
//     fflush(stdout);
//     while (0 == i)
//         sleep(5);
// }
    
    if(process.getRank() == 0)
    {
	//coordonatorul trimite mesaje de sondare catre toti vecinii
	process.sendProbeToNeighbors();
    }

    while(!process.discoveryComplete())
    {
	process.receiveDiscoveryPacket();
    }
    
    if(process.getRank() == 0)
    {
	process.printGlobalTopology();
	
	//trimite topologia catre toate nodurile
	process.sendTopologyToNeighbors();	    
    }
    
    while(!process.hasReceivedFullTopologyInfo())
    {
	process.receiveDiscoveryPacket();
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    
    //afisare secventiala a tabelelor de routare
    for(int i = 0; i < size; i++)
    {
	if(rank == i)
	    process.printRoutingTable();
	
	MPI_Barrier(MPI_COMM_WORLD);
    }
    
    //trimiterea mesajelor
    process.sendMessages();

    //receptionarea mesajelor
    while(!process.messageSendingComplete())
    {
	process.receiveMessagePacket();
    }
    
    process.routingOnly();
    
    //cerr<<endl<<"Process "<<rank<<" received all messages";
    
    //folosit mai mult pentru a obtine o afisare ok a textelor
    MPI_Barrier(MPI_COMM_WORLD);
    
    //stabilirea unor structuri de conducere    
    srand(time(NULL) + rank);

    do
    {
	MPI_Barrier(MPI_COMM_WORLD);
	
	process.resetDismantling();
	process.resetVotes();
	process.resetMessagesSent();

	if(rank == 0)
	    process.sendVotingStart();
	else
	{
	    while(!process.votingIsStarted())
		process.receiveMessagePacket();	    
	}
	
	//cerr<<endl<<"Process "<<rank<<" started voting";
	
	process.voteAndSendLeaders();

	while(!process.messageSendingComplete())
	    process.receiveMessagePacket();
	
	process.routingOnly();
	
	if(process.isTieVote() && rank == 0)
	    cerr<<endl<<"##### IS TIE VOTE .. VOTING AGAIN !!! #####";
	
    }
    while(process.isTieVote());
    
    cerr<<endl<<"Process "<<rank<<" completed voting, result: "<<process.getMainLeader()<<" "<<process.getSecondaryLeader()<<flush;
    
    //while(1)
	//process.receiveMessagePacket();
    
    MPI_Finalize();
  
    return 0;
}
