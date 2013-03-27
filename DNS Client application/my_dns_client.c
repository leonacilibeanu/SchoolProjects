#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dns.h"

#define QCLASS_IN 1
#define QCLASS_CS 2
#define QCLASS_CH 3
#define QCLASS_HS 4

FILE* logfile;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int send_query(int client_socket_fd, struct sockaddr_in* pserv_addr, char* param, char* request_type)
{
    dns_header_t query_header;
    memset(&query_header, 0, sizeof(query_header));
    
    query_header.id = htons(1);
    query_header.qr = 0; //0 = este un query
    query_header.opcode = 0; //0 = standard query, 1 = inverse query
    query_header.aa = 0;
    query_header.tc = 0;
    query_header.rd = 1;
    query_header.ra = 0;
    query_header.z = 0;
    query_header.rcode = 0;
    query_header.qdcount = htons(1);
    query_header.ancount = 0;
    query_header.nscount = 0;
    query_header.arcount = 0;
    
    dns_question_t query_question;
    
    int type = -1;
    
    if(!strcasecmp(request_type, "A"))
	type = A;
    
    if(!strcasecmp(request_type, "MX"))
	type = MX;
    
    if(!strcasecmp(request_type, "NS"))
	type = NS;
    
    if(!strcasecmp(request_type, "CNAME"))
	type = CNAME;
    
    if(!strcasecmp(request_type, "PTR"))
    {
	type = PTR;
	
	struct sockaddr_in sa;
	inet_pton(AF_INET, param, &(sa.sin_addr));
	
	unsigned char* ip = (unsigned char*)&sa.sin_addr.s_addr;
	
	param = (char*)malloc(0xFF);
	
	sprintf(param, "%u.%u.%u.%u.in-addr.arpa", (unsigned int)ip[3], (unsigned int)ip[2], (unsigned int)ip[1], (unsigned int)ip[0]);
	//printf("\nPARAM = %s", param);
    }
    
    if(type == -1)
    {
	error("Unknown request type !");
    }
    
    query_question.qtype = htons(type);
    query_question.qclass = htons(QCLASS_IN);
    
    int buf_len = sizeof(dns_header_t) + 1 + strlen(param) + 1 + sizeof(query_question);
    char* buf = (char*)malloc(buf_len);
	
    memcpy(buf, &query_header, sizeof(query_header));

    int param_pos = sizeof(query_header);
    
    char* tmp = param;
    while(*tmp)
    {
	int param_len = 0;
	while(tmp[param_len] && tmp[param_len] != '.')
	    param_len++;
	
	//printf("\nPARAM = %s|%d", tmp, param_len); fflush(stdout);
	buf[param_pos] = param_len;
	strncpy(&buf[param_pos + 1], tmp, param_len);
	param_pos += param_len + 1;
	
	if(!tmp[param_len])
	    break;
	
	tmp = &tmp[param_len + 1];
    }
    
    memcpy(&buf[sizeof(dns_header_t) + 1 + strlen(param) + 1], &query_question, sizeof(query_question));
    
    if (sendto(client_socket_fd, buf, buf_len, 0, (const struct sockaddr *)pserv_addr, sizeof(struct sockaddr_in)) == -1)
	error("sendto(..)");
    
    free(buf);
    
    int param_len = strlen(param);
    
    if(type == PTR)
	free(param);
    
    return param_len;
}

int read_name(char* name, char* buf, int pos)
{
    strcpy(name, "");
    int name_len = 0;
    
    int jumped = 0;
    int result_pos = pos;

    while(buf[pos] != 0)
    {
	unsigned char len = buf[pos];
	int is_ptr = len & 0xC0; //verifica daca incepe cu 11 -> pointerii incep cu 11

	if(!is_ptr)
	{
	    strncat(name, &buf[pos + 1], len);
	    pos += len + 1;
	    
	    name_len += len;
	    name[name_len] = '\0';
	
	    strcat(name, ".");
	    name_len++;
	}
	
	if(is_ptr)
	{
	    unsigned short ptr_pos = (((unsigned short)(len & 0x3F))<<8) + (unsigned char)buf[pos + 1]; //inlatura 11 si formeaza pointerul
	    pos = ptr_pos;
	    //printf("\nPTR_POS = %d", (unsigned int)ptr_pos);
	    if(!jumped)
	    {
		jumped = 1;
		result_pos += 2;
	    }
	}
	
	if(!jumped)
	{
	    result_pos = pos;
	}
	
	//printf("\nResult pos: %d", result_pos);
    }
    
    return result_pos;
}

void print_type(int type)
{
    switch(type)
    {
	case A:
	    printf("A");
	    fprintf(logfile, "A");
	    break;
	case NS:
	    printf("NS");
	    fprintf(logfile, "NS");
	    break;
	case CNAME:
	    printf("CNAME");
	    fprintf(logfile, "CNAME");
	    break;
	case PTR:
	    printf("PTR");
	    fprintf(logfile, "PTR");
	    break;
	case MX:
	    printf("MX");
	    fprintf(logfile, "MX");
	    break;
	case SOA:
	    printf("SOA");
	    fprintf(logfile, "SOA");
	    break;
	    
	default:;
    }
}

void print_class(int qclass)
{
    switch(qclass)
    {
	case QCLASS_IN:
	    printf("IN");
	    fprintf(logfile, "IN");
	    break;
	case QCLASS_CS:
	    printf("CS");
	    fprintf(logfile, "CS");
	    break;
	case QCLASS_CH:
	    printf("CH");
	    fprintf(logfile, "CH");
	    break;
	case QCLASS_HS:
	    printf("HS");
	    fprintf(logfile, "HS");
	    break;
    }
}

uint32_t read_u32(char* data, int pos)
{
    uint32_t result = (((uint32_t)data[pos])<<24) + (((uint32_t)data[pos + 1])<<16) + (((uint32_t)data[pos + 2])<<8) + ((uint32_t)data[pos + 3]);
    
    return result;
}

void print_dns_rr(char* name, dns_rr_t* dns_rr, char* buf, int rrdata_pos)
{
    char tmp[256];
    
    printf("\n%-48s", name); 
    fprintf(logfile, "\n%-48s", name);
    
    printf("\t%d", dns_rr->ttl); 
    fprintf(logfile, "\t%d", dns_rr->ttl);
    
    printf("\t"); 
    fprintf(logfile, "\t");
    print_class(dns_rr->class);
    
    printf("\t"); 
    fprintf(logfile, "\t");
    print_type(dns_rr->type);
    
    unsigned short priority;
    
    switch(dns_rr->type)
    {
	case A:
	    printf("\t%d.%d.%d.%d", (unsigned char)buf[rrdata_pos], (unsigned char)buf[rrdata_pos + 1], (unsigned char)buf[rrdata_pos + 2], (unsigned char)buf[rrdata_pos + 3]);
	    fprintf(logfile, "\t%d.%d.%d.%d", (unsigned char)buf[rrdata_pos], (unsigned char)buf[rrdata_pos + 1], (unsigned char)buf[rrdata_pos + 2], (unsigned char)buf[rrdata_pos + 3]);
	    break;
	case NS:
	    read_name(&tmp[0], buf, rrdata_pos);
	    printf("\t%s", tmp);
	    fprintf(logfile, "\t%s", tmp);
	    break;
	case MX:
	    priority = (((unsigned short)buf[rrdata_pos])<<8) + buf[rrdata_pos + 1];
	    printf("\t%u", (unsigned int) priority);
	    fprintf(logfile, "\t%u", (unsigned int) priority);
	    read_name(&tmp[0], buf, rrdata_pos + 2);
	    printf("\t%s", tmp);
	    fprintf(logfile, "\t%s", tmp);
	    break;
	case CNAME:
	    read_name(&tmp[0], buf, rrdata_pos);
	    printf("\t%s", tmp);
	    fprintf(logfile, "\t%s", tmp);
	    break;
	case PTR:
	    read_name(&tmp[0], buf, rrdata_pos);
	    printf("\t%s", tmp);
	    fprintf(logfile, "\t%s", tmp);
	    break;
	case SOA:
	    rrdata_pos = read_name(&tmp[0], buf, rrdata_pos);
	    
	    printf("\t%s", tmp);
	    fprintf(logfile, "\t%s", tmp);
	    
	    rrdata_pos = read_name(&tmp[0], buf, rrdata_pos);
	    
	    printf("\t%s %u %u %u %u %u", tmp, read_u32(buf, rrdata_pos), read_u32(buf, rrdata_pos + 4), read_u32(buf, rrdata_pos + 8), read_u32(buf, rrdata_pos + 12), read_u32(buf, rrdata_pos + 16));
	    fprintf(logfile, "\t%s %u %u %u %u %u", tmp, read_u32(buf, rrdata_pos), read_u32(buf, rrdata_pos + 4), read_u32(buf, rrdata_pos + 8), read_u32(buf, rrdata_pos + 12), read_u32(buf, rrdata_pos + 16));
	    break;
	default:
	    printf("\nUnknown type %d", dns_rr->type);
	    fprintf(logfile, "\nUnknown type %d", dns_rr->type);
    }
}

void dns_rr_ntohs(dns_rr_t* dns_rr)
{
    dns_rr->type = ntohs(dns_rr->type);
    dns_rr->class = ntohs(dns_rr->class);
    dns_rr->ttl = ntohl(dns_rr->ttl);
    dns_rr->rdlength = ntohs(dns_rr->rdlength);
}

char* dns_servers[0xFFF];
int dns_server_count = 0;

void load_dns_servers()
{
    char tmp[0xFF];
    
    dns_server_count = 0;
    
    FILE* f = fopen("dns_servers.conf", "rb");
    
    if(f == NULL)
    {
	error("Can't open dns_servers.conf");
    }
    
    while(!feof(f))
    {
	fgets(&tmp[0], 0xFF, f);
	
	int start = 0;
	while((tmp[start] == ' ' || tmp[start] == '\t') && tmp[start])
	    start++;
	
	if(!tmp[start] || tmp[start] == '#')
	    continue;
	
	int end = strlen(tmp) - 1;
	while(tmp[end] == '\r' || tmp[end] == '\n' || tmp[end] == '\t' || tmp[end] == ' ' && end > 1)
	    end--;
	
	tmp[end + 1] = 0;
	
	if(strlen(tmp) < 1)
	    continue;
	
	dns_servers[dns_server_count] = (char*)malloc(strlen(&tmp[start]) + 1);
	strcpy(dns_servers[dns_server_count], &tmp[start]);
	//printf("\nLoaded '%s'", dns_servers[dns_server_count]);
	
	dns_server_count++;
    }
    
    fclose(f);
    
    if(dns_server_count == 0)
	error("No DNS servers found !");
}

//todo answer waiting timeout
int main(int argc, char* argv[])
{
    
    if(argc != 3)
    {
	printf("\nUsage: %s parameter query_type\n", argv[0]);
	return 0;
    }
    
    logfile = fopen("logfile", "ab");
    
    if(logfile == NULL)
	error("Failed to open logfile !");
    
    load_dns_servers();
    
    char* param = argv[1];
    char* request_type = argv[2];
    
    printf("\n; Trying: %s %s\n", param, request_type);
    fprintf(logfile, "\n; Trying: %s %s\n", param, request_type);
    
    int client_socket_fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
 
    if(client_socket_fd < 0)
        error("ERROR opening socket");
    
    dns_header_t* dns_header;
    
    char* buf = (char*)malloc(0xFFFF);
    
    int received_reply = 0;
    
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(client_socket_fd, &read_fds);
    
    int param_len;
    int current_dns_server = 0;
    do
    {
	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	
	fd_set tmp_fds = read_fds;
	
	struct sockaddr_in serv_addr;
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(53);
	
	if(inet_aton(dns_servers[current_dns_server], &serv_addr.sin_addr) == 0)
	{
	//    error("inet_aton(..) failed");
	}
	
	param_len = send_query(client_socket_fd, &serv_addr, param, request_type);

	if(select(FD_SETSIZE, &tmp_fds, NULL, NULL, &timeout) == -1)
	    error("ERROR in select");
	    
	if(FD_ISSET(client_socket_fd, &tmp_fds))
	{
	    int slen = sizeof(serv_addr);
	    if(recvfrom(client_socket_fd, buf, 65535, 0, (struct sockaddr*)&serv_addr, &slen) == -1)
	    {
	    //    error("recvfrom(..)");
	    }
	    
	    dns_header = (dns_header_t*)buf;
	    
	    dns_header->ancount = ntohs(dns_header->ancount);
	    dns_header->nscount = ntohs(dns_header->nscount);
	    dns_header->arcount = ntohs(dns_header->arcount);
	    
	    //printf("\nAnswer RRs: %d", dns_header->ancount);
	    //printf("\nAuthority RRs: %d", dns_header->nscount);
	    //printf("\nAdditional RRs: %d", dns_header->arcount);
	    /*
	    if(dns_header->rcode != 0)
	    {
		switch(dns_header->rcode)
		{
		    case 1:
			printf("\nFormat error !");
			fprintf(logfile, "\nFormat error !");
			break;
		    case 2:
			printf("\nServer failure !");
			fprintf(logfile, "\nServer failure !");
			break;
		    case 3:
			printf("\nName error !");
			fprintf(logfile, "\nName error !");
			break;
		    case 4:
			printf("\nNot implemented !");
			fprintf(logfile, "\nNot implemented !");
			break;
		    case 5:
			printf("\nRefused !");
			fprintf(logfile, "\nRefused !");
			break;
		    default:
			printf("\nUnknown rcode !");
			fprintf(logfile, "\nUnknown rcode !");
		}
		
		return 0;
	    }*/
	    //printf("\nRcode = %d | %d ? %d", dns_header->rcode, current_dns_server, dns_server_count);
	    if(dns_header->rcode == 0)
		received_reply = 1;
	    else
		current_dns_server++;
	}
	else
	    current_dns_server++;
    }
    while(!received_reply && current_dns_server < dns_server_count);

    if(current_dns_server == dns_server_count)
	error("Can't get data !");
    
    close(client_socket_fd);
    
    if(dns_header->ancount)
    {
	printf("\n;; ANSWER SECTION:");
	fprintf(logfile, "\n;; ANSWER SECTION:");
    }
    
    int pos = sizeof(dns_header_t) + 1 + param_len + 1 + sizeof(dns_question_t);
	
    int i;
    for(i = 0; i < dns_header->ancount; i++)
    {
	char name[256];

	pos = read_name(name, buf, pos);

	dns_rr_t* dns_rr = (dns_rr_t*)&buf[pos];
	dns_rr_ntohs(dns_rr);

	pos += sizeof(dns_rr_t);
	
	int rrdata_pos = pos;
	
	pos += dns_rr->rdlength;
	
	print_dns_rr(name, dns_rr, buf, rrdata_pos);
    }
    
    if(dns_header->nscount)
    {
	printf("\n\n;; AUTHORITY SECTION:");
	fprintf(logfile, "\n\n;; AUTHORITY SECTION:");
    }
    
    for(i = 0; i < dns_header->nscount; i++)
    {
	char name[256];

	pos = read_name(name, buf, pos);

	dns_rr_t* dns_rr = (dns_rr_t*)&buf[pos];
	dns_rr_ntohs(dns_rr);

	pos += sizeof(dns_rr_t);
	
	int rrdata_pos = pos;
	
	pos += dns_rr->rdlength;
	
	print_dns_rr(name, dns_rr, buf, rrdata_pos);
    }
    
    if(dns_header->arcount)
    {
	printf("\n\n;; ADDITIONAL SECTION:");
	fprintf(logfile, "\n\n;; ADDITIONAL SECTION:");
    }
    
    for(i = 0; i < dns_header->arcount; i++)
    {
	char name[256];

	pos = read_name(name, buf, pos);

	dns_rr_t* dns_rr = (dns_rr_t*)&buf[pos];
	dns_rr_ntohs(dns_rr);

	pos += sizeof(dns_rr_t);
	
	int rrdata_pos = pos;
	
	pos += dns_rr->rdlength;
	
	print_dns_rr(name, dns_rr, buf, rrdata_pos);
    }
    
    free(buf);
    
    printf("\n");
    fprintf(logfile, "\n");
    
    fclose(logfile);
    
    return 0;
}