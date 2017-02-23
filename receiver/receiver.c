/**
 * gcc -Wall -I/usr/local/include/json-c   -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0   -o receiver receiver.c -lpthread -lmongoc-1.0 -lbson-1.0 -ljson-c
 */ 
#include <bson.h>
#include <bcon.h>
#include <mongoc.h>
#include<netinet/in.h>
#include<errno.h>
#include<netdb.h>
#include<stdio.h> //For standard things
#include<stdlib.h>    //malloc
#include<string.h>    //strlen
#include</usr/include/json/json.h>
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
void print_ip_header(unsigned char* , int);
void print_tcp_packet(unsigned char * , int );
void print_udp_packet(unsigned char * , int );
void print_icmp_packet(unsigned char* , int );
void PrintData (unsigned char* , int);
void initMongoDb(); 
void closeMongoDb(); 
void savePacket();

FILE *logfile;
struct sockaddr_in source,dest;
int tcp=0,udp=0,icmp=0,others=0,igmp=0,total=0,i,j; 
mongoc_client_t      *client;
mongoc_database_t    *database;
mongoc_collection_t  *collection;
bson_t               *command, reply, *document;
bson_error_t          error;
char                 *str;
bool                  retval;
long currentTime;
int saddr_size , data_size;
struct sockaddr saddr;
         
int udpSocket, nBytes, len;
char buffer[1024];
struct sockaddr_in serverAddr, clientAddr;
struct sockaddr_storage serverStorage;
socklen_t addr_size, client_addr_size;
int i;
struct json_object *jobj, *json_object_arr, *medi_array_obj, *obj_temp, *json_parser;
int main (int   argc, char *argv[]) {
   
   initMongoDb();
  
   unsigned char *buffer = (unsigned char *) malloc(65536); //Its Big!
   printf("Server listening...: \n");

  /*Create UDP socket*/
  udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

  /*Configure settings in address struct*/
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(7891);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  

  /*Bind socket with address struct*/
  bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

  /*Initialize size variable to be used later on*/
  addr_size = sizeof serverStorage;
  
  while(1){
    /* Try to receive any incoming UDP datagram. Address and port of 
      requesting client will be stored on serverStorage variable */
    nBytes = recvfrom(udpSocket,buffer,65536,0,(struct sockaddr *)&serverStorage, &addr_size);
    
     if(nBytes <0 ) {
         printf("Recvfrom error , failed to get packets\n");
         return 1;
      } else {
          
         
      }//else
      
      ProcessPacket(buffer, nBytes);
  }
   
  
   close(udpSocket);
  
   closeMongoDb();
   return 0;

   
}
/**
 * Init MongoDB
 */
void initMongoDb() {
    
   // Required to initialize libmongoc's internals
   mongoc_init ();

   
   // Create a new client instance
    
   client = mongoc_client_new ("mongodb://localhost:27017");
   
   
    //Get a handle on the database "db_name" and collection "coll_name"
   database = mongoc_client_get_database (client, "network_monitor");
   collection = mongoc_client_get_collection (client, "network_monitor", "traffic");
   
  
   printf("connect to mongodb...\n");
}
/**
 * Close MongoDB connection
 */ 
void closeMongoDb() {
    /*
    * Release our handles and clean up libmongoc
    */
   bson_destroy (document);
   mongoc_collection_destroy (collection);
   mongoc_database_destroy (database);
   mongoc_client_destroy (client);
   mongoc_cleanup ();
}
void ProcessPacket(unsigned char* buffer, int size)
{
    
    json_parser = json_tokener_parse(buffer);
    len = json_object_array_length(json_parser);
    for (i = 0; i < len; i++) {
        document = bson_new ();
        
        // get the i-th object in json_parser
        medi_array_obj = json_object_array_get_idx(json_parser, i);
         printf("\n----Package %d----\n",i);
        // get the field attribute in the i-th object
        obj_temp  = json_object_object_get(medi_array_obj, "ip_src");
        if(obj_temp != NULL) {
             printf("ip_src=%s\n", json_object_get_string(obj_temp));
            BSON_APPEND_UTF8 (document, "ip_src", json_object_get_string(obj_temp)); 
        }
        
        
        obj_temp  = json_object_object_get(medi_array_obj, "ip_dest");
        if(obj_temp != NULL) {
            printf("ip_dest=%s\n", json_object_get_string(obj_temp));
            BSON_APPEND_UTF8 (document, "ip_dest", json_object_get_string(obj_temp));  
             
        }
        
        obj_temp  = json_object_object_get(medi_array_obj, "port_src");
        if(obj_temp != NULL) {
            printf("port_src=%s\n", json_object_get_string(obj_temp));
            BSON_APPEND_UTF8 (document, "port_src", json_object_get_string(obj_temp));  
        }
        
        obj_temp  = json_object_object_get(medi_array_obj, "port_dest");
        if(obj_temp != NULL) {
            printf("port_dest=%s\n", json_object_get_string(obj_temp));
            BSON_APPEND_UTF8 (document, "port_dest", json_object_get_string(obj_temp)); 
        }
        
        obj_temp  = json_object_object_get(medi_array_obj, "protocol");
        if(obj_temp != NULL) {
            printf("protocol=%s\n", json_object_get_string(obj_temp));
            BSON_APPEND_UTF8 (document, "protocol", json_object_get_string(obj_temp));  
        }
        
        obj_temp  = json_object_object_get(medi_array_obj, "count");
        if(obj_temp != NULL) {
            printf("count=%s\n", json_object_get_string(obj_temp));
            BSON_APPEND_UTF8 (document, "count", json_object_get_string(obj_temp));  
        }
        
        obj_temp  = json_object_object_get(medi_array_obj, "time");
        if(obj_temp != NULL) {
            printf("time=%s\n", json_object_get_string(obj_temp));
            BSON_APPEND_INT32 (document, "time", json_object_get_int(obj_temp));
        } 
         
        if (!mongoc_collection_insert (collection, MONGOC_INSERT_NONE, document, NULL, &error)) {
          fprintf (stderr, "%s\n", error.message);
        }
    } //end for 
   
}


void print_ethernet_header(unsigned char* Buffer, int Size)
{
    struct ethhdr *eth = (struct ethhdr *)Buffer;
     
    //fprintf(logfile , "\n");
    //fprintf(logfile , "Ethernet Header\n");
    //fprintf(logfile , "   |-Destination Address : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_dest[0] , eth->h_dest[1] , eth->h_dest[2] , eth->h_dest[3] , eth->h_dest[4] , eth->h_dest[5] );
   // fprintf(logfile , "   |-Source Address      : %.2X-%.2X-%.2X-%.2X-%.2X-%.2X \n", eth->h_source[0] , eth->h_source[1] , eth->h_source[2] , eth->h_source[3] , eth->h_source[4] , eth->h_source[5] );
    //fprintf(logfile , "   |-Protocol            : %u \n",(unsigned short)eth->h_proto);
}
 
void print_ip_header(unsigned char* Buffer, int Size)
{
    print_ethernet_header(Buffer , Size);
   
    unsigned short iphdrlen;
         
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr) );
    iphdrlen =iph->ihl*4;
     
    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
     
    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;
    
    BSON_APPEND_UTF8 (document, "ip_src", inet_ntoa(source.sin_addr));  
    printf("Add to Document - Source IP        : %s\n",inet_ntoa(source.sin_addr));
    BSON_APPEND_UTF8 (document, "ip_dest", inet_ntoa(dest.sin_addr));  
    printf("Add to Document - Destination IP        : %s\n", inet_ntoa(dest.sin_addr));
}
 
void print_tcp_packet(unsigned char* Buffer, int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)( Buffer  + sizeof(struct ethhdr) );
    iphdrlen = iph->ihl*4;
     
    struct tcphdr *tcph=(struct tcphdr*)(Buffer + iphdrlen + sizeof(struct ethhdr));
             
    int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff*4;
     
    
    BSON_APPEND_UTF8 (document, "protocol", "tcp");  
    printf("Add to Document - Protocol       : TCP");
    
    print_ip_header(Buffer,Size);
    
     
    BSON_APPEND_INT32 (document, "port_src", ntohs(tcph->source));  
    printf("Add to Document - Source Port       : %u\n", ntohs(tcph->source));
    BSON_APPEND_INT32 (document, "port_dest", ntohs(tcph->dest));  
    printf("Add to Document - Destination Port       : %u\n", ntohs(tcph->dest));
    
}
 
void print_udp_packet(unsigned char *Buffer , int Size)
{
     
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer +  sizeof(struct ethhdr));
    iphdrlen = iph->ihl*4;
     
    struct udphdr *udph = (struct udphdr*)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof udph;
     
   
     
    BSON_APPEND_UTF8 (document, "protocol", "udp");  
    printf("Add to Document - Protocol       : UDP");
    
    print_ip_header(Buffer,Size);           
     
    
   // PrintData(Buffer , iphdrlen);
         
    //fprintf(logfile , "UDP Header\n");
    //PrintData(Buffer+iphdrlen , sizeof udph);
         
   // fprintf(logfile , "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
   // PrintData(Buffer + header_size , Size - header_size);
     
   // fprintf(logfile , "\n###########################################################");
    
    
    BSON_APPEND_INT32 (document, "port_src", ntohs(udph->source));  
    printf("Add to Document - Source Port       : %u\n", ntohs(udph->source));
    
    BSON_APPEND_INT32 (document, "port_dest",ntohs(udph->dest));  
    printf("Add to Document - Destination Port       : %u\n", ntohs(udph->dest));
    
}
 
void print_icmp_packet(unsigned char* Buffer , int Size)
{
    unsigned short iphdrlen;
     
    struct iphdr *iph = (struct iphdr *)(Buffer  + sizeof(struct ethhdr));
    iphdrlen = iph->ihl * 4;
     
    struct icmphdr *icmph = (struct icmphdr *)(Buffer + iphdrlen  + sizeof(struct ethhdr));
     
    int header_size =  sizeof(struct ethhdr) + iphdrlen + sizeof icmph;
     
   // fprintf(logfile , "\n\n***********************ICMP Packet*************************\n"); 
     
    print_ip_header(Buffer , Size);
             
   // fprintf(logfile , "\n");
         
   // fprintf(logfile , "ICMP Header\n");
   // fprintf(logfile , "   |-Type : %d",(unsigned int)(icmph->type));
    /*        
    if((unsigned int)(icmph->type) == 11)
    {
        fprintf(logfile , "  (TTL Expired)\n");
    }
    else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY)
    {
        fprintf(logfile , "  (ICMP Echo Reply)\n");
    }
     
    fprintf(logfile , "   |-Code : %d\n",(unsigned int)(icmph->code));
    fprintf(logfile , "   |-Checksum : %d\n",ntohs(icmph->checksum));
    //fprintf(logfile , "   |-ID       : %d\n",ntohs(icmph->id));
    //fprintf(logfile , "   |-Sequence : %d\n",ntohs(icmph->sequence));
    fprintf(logfile , "\n");
 
    fprintf(logfile , "IP Header\n");
    PrintData(Buffer,iphdrlen);
         
    fprintf(logfile , "UDP Header\n");
    PrintData(Buffer + iphdrlen , sizeof icmph);
         
    fprintf(logfile , "Data Payload\n");    
     
    //Move the pointer ahead and reduce the size of string
    PrintData(Buffer + header_size , (Size - header_size) );
     
    fprintf(logfile , "\n###########################################################");
    */
}
 