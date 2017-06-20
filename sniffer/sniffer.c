/*
* gcc -Wall -I/usr/include/json-c/ -o sniffer sniffer.c -ljson-c
*/
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdbool.h>
#include<stdlib.h>    //malloc
#include<string.h>    //strlen
#include <json.h> //json-c 
#include<netinet/ip_icmp.h>   //Provides declarations for icmp header
#include<netinet/udp.h>   //Provides declarations for udp header
#include<netinet/tcp.h>   //Provides declarations for tcp header
#include<netinet/ip.h>    //Provides declarations for ip header
#include<netinet/if_ether.h>  //For ETH_P_ALL
#include<net/ethernet.h>  //For ether_header
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/ioctl.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>
 
void ProcessPacket(unsigned char* , int);
void process_ip_header(unsigned char* , int);
void process_tcp_packet(unsigned char * , int );
void process_udp_packet(unsigned char * , int );
void process_icmp_packet(unsigned char* , int );
void PrintData (unsigned char* , int);
bool checkPacketExists();
char* getFieldStringValue( struct json_object * obj, char *field); 
int getFieldIntValue( struct json_object * obj, char *field);
void sendPacket();
FILE *logfile;
struct sockaddr_in source,dest;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j,count; 
int clientSocket, portNum, nBytes;
    
struct sockaddr_in serverAddr;
socklen_t addr_size;
struct json_object *jobj, *json_object_arr, *medi_array_obj, *obj_temp, *json_parser;
long currentTime;
int  i, len, port_src, port_dest;
char *str, *protocol;
char ip_src[100], ip_dest[100];
time_t start_t, end_t;
int diff_t; //second
int packetTimeout = 5;
long BUFFER_SIZE = 1024 * 64;
int SERVER_PORT = 7891;
char SERVER_HOST[] = "127.0.0.1";
char group[20];
int main(int argc, char *argv[])
{   
    int saddr_size , data_size;
    struct sockaddr saddr;
    if (argv[1] != "\0"){
       strcpy (group,argv[1]);
    }
    unsigned char *buffer = (unsigned char *) malloc(BUFFER_SIZE); //Its Big!
    
    /*Create UDP socket*/
    clientSocket = socket(PF_INET, SOCK_DGRAM, 0);
    
    /*Configure settings in address struct*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
    /*Initialize size variable to be used later on*/
    addr_size = sizeof serverAddr;
     
    int sock_raw = socket( AF_PACKET , SOCK_RAW , htons(ETH_P_ALL)) ;
    //setsockopt(sock_raw , SOL_SOCKET , SO_BINDTODEVICE , "eth0" , strlen("eth0")+ 1 );
     
    if(sock_raw < 0)
    {
        //Print the error with proper message
        perror("Socket Error");
        return 1;
    }
    
    json_object_arr = json_object_new_array();
    time(&start_t);
    while(1)
    {
        saddr_size = sizeof saddr;
        //Receive a packet
        data_size = recvfrom(sock_raw , buffer , BUFFER_SIZE , 0 , &saddr , (socklen_t*)&saddr_size);
        if(data_size <0 )
        {
            printf("Recvfrom error , failed to get packets\n");
            return 1;
        }
        //Now process the packet
        ProcessPacket(buffer , data_size);
        sendPacket();  
        
        
    }
    close(sock_raw);
    printf("Finished");
    return 0;
} //end Main


void sendPacket() {
    unsigned char *dataBuffer = (unsigned char *) malloc(BUFFER_SIZE); //Its Big!
    int nDataBuffer;
    
    time(&end_t);
    
    diff_t = end_t - start_t;
    
    if(diff_t >= packetTimeout) {
        str = json_object_to_json_string_ext(json_object_arr, JSON_C_TO_STRING_PRETTY);
        
        nDataBuffer = strlen(str) + 1;
        
        strcpy(dataBuffer, str);
        sendto(clientSocket, dataBuffer, nDataBuffer, 0, (struct sockaddr *)&serverAddr,addr_size);
        printf("TCP : %d   UDP : %d   ICMP : %d   IGMP : %d   Others : %d   Total : %d \nStart: %d End: %d  Timer: %d Sent at %ld \n%s\n\n\n", tcp , udp , icmp , igmp , others , total, start_t, end_t, diff_t, time(NULL), str);
        
        // Reset
        time(&start_t);
        tcp = udp = icmp = others = igmp = total = 0;
        json_object_arr = json_object_new_array();
    }  
   
  

} 
void ProcessPacket(unsigned char* buffer, int size)
{
    
    jobj = json_object_new_object();
        
    //Get the IP Header part of this packet , excluding the ethernet header
    struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));
    ++total;
    switch (iph->protocol) //Check the Protocol and do accordingly...
    {
        case 1:  //ICMP Protocol
            ++icmp;
            process_icmp_packet( buffer , size);
            break;
         
        case 2:  //IGMP Protocol
            ++igmp;
            break;
         
        case 6:  //TCP Protocol
            ++tcp;
            process_tcp_packet(buffer , size);
            break;
         
        case 17: //UDP Protocol
            ++udp;
            process_udp_packet(buffer , size);
            break;
         
        default: //Some Other Protocol like ARP etc.
            ++others;
            break;
    }
    
    //Remove on production
    if(checkPacketExists() == FALSE) {
        currentTime = (long)time(NULL);
        count = 1;
        json_object_object_add(jobj, "count", json_object_new_int(count));
        json_object_object_add(jobj, "time", json_object_new_int(currentTime));
         if (group != "\0"){
         json_object_object_add(jobj, "group", json_object_new_string(group));
        }
        json_object_array_add(json_object_arr, jobj);
        
    } 
   
}
char* getFieldStringValue( struct json_object * obj, char *field) {
        char* result = "";
        struct json_object *tempt;
        tempt  = json_object_object_get(obj, field);
        if(tempt != NULL) {
            
           result = json_object_get_string(tempt);
           
        }
        
        return result;
}

int getFieldIntValue( struct json_object * obj, char *field) {
        int result ;
        struct json_object *tempt;
        tempt  = json_object_object_get(obj, field);
        if(tempt != NULL) {
            
           result = json_object_get_int(tempt);
           
        }
        
        return result;
}
/*
* Check Package Exists, increnment number
*/
bool checkPacketExists() {
    
    len = json_object_array_length(json_object_arr);
    
    bool result = FALSE;
    bool iPSrcExist = FALSE, iPDestExist = FALSE, portSrcExist = FALSE, portDestExist = FALSE, protocolExist = FALSE;
    char *strTemp;
    int iTempt;
    for (i = 0; i < len; i++) {
        // get the i-th object in json_object_arr
        medi_array_obj = json_object_array_get_idx(json_object_arr, i);
        
        strTemp = getFieldStringValue(medi_array_obj, "ip_src"); 
        //check ip_src
        if(strcmp(strTemp ,ip_src) == 0) {
            iPSrcExist = TRUE ;
            //printf("map ip src\n");
        }
        else {
            iPSrcExist = FALSE; 
            continue;
        }
       
        
        //check ip_dest
        strTemp = getFieldStringValue(medi_array_obj, "ip_dest"); 
       
        if(strcmp(strTemp ,ip_dest) == 0) {
                iPDestExist = TRUE; 
                //printf("map ip_dest\n");
            
        } else {
            iPDestExist = FALSE;
            continue;
        }
        
        //check port_src
        iTempt = getFieldIntValue(medi_array_obj, "port_src"); 
       
        if(iTempt == port_src) {
                portSrcExist = TRUE; 
                //printf("map port src\n");
            
        } else {
            portSrcExist = FALSE;
            continue;
        }
        
        //check port_dest
        iTempt = getFieldIntValue(medi_array_obj, "port_dest"); 
       
        if(iTempt == port_dest) {
                portDestExist = TRUE; 
                //printf("mapport dest\n");
            
        } else {
            portDestExist = FALSE;
            continue;
        }
        
        //check protocol
        strTemp = getFieldStringValue(medi_array_obj, "protocol"); 
       
        if(strcmp(strTemp ,protocol) == 0) {
                protocolExist = TRUE; 
                //printf("map protocol\n");
            
        } else {
            protocolExist = FALSE;
            continue;
        }
        
        if(iPSrcExist && iPDestExist && portSrcExist && portDestExist && protocolExist) {
            
            //Update Count
            obj_temp  = json_object_object_get(medi_array_obj, "count");
            if(obj_temp == NULL) {
                count = 0;
            } else {
                count = json_object_get_int(obj_temp);
            }
            count ++;
            json_object_object_add(medi_array_obj, "count", json_object_new_int(count));
            
            //update time
            currentTime = (long)time(NULL);
            json_object_object_add(medi_array_obj, "time", json_object_new_int(currentTime));
           // printf("Existed Update count %d: ", count);
            return TRUE; // package exist stop checking
        } 
        
    } //end for 
    
    return result;//return FALSE
} 
void process_ethernet_header(unsigned char* Buffer, int Size)
{
    //struct ethhdr *eth = (struct ethhdr *)Buffer;
     
    /*fprintf(logfile , "\n");
    fprintf(logfile , "Ethernet Header\n");
    fprintf(logfile , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
    fprintf(logfile , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    fprintf(logfile , "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);*/
}
 
void process_ip_header(unsigned char* Buffer, int Size)
{
    process_ethernet_header(Buffer , Size);
   
   // unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
   // iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    
    strcpy(ip_src,inet_ntoa(source.sin_addr));
    strcpy(ip_dest,inet_ntoa(dest.sin_addr));
    json_object_object_add(jobj, "ip_src", json_object_new_string(ip_src));
    json_object_object_add(jobj, "ip_dest", json_object_new_string(ip_dest));
    
}
 
void process_tcp_packet(unsigned char* Buffer, int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
    
         
    process_ip_header(Buffer,Size);
    port_src = ntohs(tcph->source);
    port_dest = ntohs(tcph->dest);
    protocol = "tcp";
    json_object_object_add(jobj, "port_src", json_object_new_int(port_src));
    json_object_object_add(jobj, "port_dest", json_object_new_int (port_dest));
    json_object_object_add(jobj, "protocol", json_object_new_string("tcp"));
   
}
 
void process_udp_packet(unsigned char *Buffer , int Size)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
    
    process_ip_header(Buffer,Size); 
    port_src = ntohs(udph->source);
    port_dest = ntohs(udph->dest);
    protocol = "udp";
    json_object_object_add(jobj, "port_src", json_object_new_int(port_src));
    json_object_object_add(jobj, "port_dest", json_object_new_int (port_dest));
    json_object_object_add(jobj, "protocol", json_object_new_string("udp"));
  
}
 
void process_icmp_packet(unsigned char* Buffer , int Size)
{
    //unsigned short iphdrlen;
     
    //struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
    //iphdrlen = iph->ihl * 4;
     
   // struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    //int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
     
    process_ip_header(Buffer , Size);
    port_src = 0;
    port_dest = 0;
    protocol = "icmp";
    json_object_object_add(jobj, "port_src", json_object_new_int(0));
    json_object_object_add(jobj, "port_dest", json_object_new_int (0));
    json_object_object_add(jobj, "protocol", json_object_new_string("icmp"));
}
 