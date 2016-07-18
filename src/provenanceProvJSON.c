/*
*
* provenancelib.h
*
* Author: Thomas Pasquier <tfjmp@seas.harvard.edu>
*
* Copyright (C) 2016 Harvard University
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2, as
* published by the Free Software Foundation.
*
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <netdb.h>
#include <pthread.h>

#include "provenancelib.h"
#include "provenancePovJSON.h"

static pthread_mutex_t l_out =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static pthread_mutex_t l_activity =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static pthread_mutex_t l_agent =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static pthread_mutex_t l_entity =  PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;

bool writing_out = false;

static inline bool append(char destination[MAX_PROVJSON_BUFFER_LENGTH], char* source){
  if (strlen(source) + 2 > MAX_PROVJSON_BUFFER_LENGTH - strlen(destination)){ // not enough space
    return false;
  }
  // add the comma
  if(destination[0]!='\0')
    strcat(destination, ",");
  strncat(destination, source, MAX_PROVJSON_BUFFER_LENGTH - strlen(destination) - 1); // copy up to free space
  return true;
}

char activity[MAX_PROVJSON_BUFFER_LENGTH];
char agent[MAX_PROVJSON_BUFFER_LENGTH];
char entity[MAX_PROVJSON_BUFFER_LENGTH];

#define JSON_START "{\"prefix\":{"
#define JSON_ACTIVITY "}, \"activity\":{"
#define JSON_AGENT "}, \"agent\":{"
#define JSON_ENTITY "}, \"entity\":{"
#define JSON_END "}}"

#define JSON_LENGTH (strlen(JSON_START)\
                      +strlen(JSON_ACTIVITY)\
                      +strlen(JSON_AGENT)\
                      +strlen(JSON_ENTITY)\
                      +strlen(JSON_END)\
                      +strlen(prefix_json())\
                      +strlen(activity)\
                      +strlen(agent)\
                      +strlen(entity)\
                      +1)
// we create the JSON string to be sent to the call back
static inline char* ready_to_print(){
  char* json = (char*)malloc(JSON_LENGTH * sizeof(char));
  json[0]='\0';
  pthread_mutex_lock(&l_activity);
  pthread_mutex_lock(&l_agent);
  pthread_mutex_lock(&l_entity);
  strcat(json, JSON_START);
  strcat(json, prefix_json());
  strcat(json, JSON_ACTIVITY);
  strcat(json, activity);
  strcat(json, JSON_AGENT);
  strcat(json, agent);
  strcat(json, JSON_ENTITY);
  strcat(json, entity);
  strcat(json, JSON_END);
  pthread_mutex_unlock(&l_entity);
  pthread_mutex_unlock(&l_agent);
  pthread_mutex_unlock(&l_activity);
  return json;
}

void (*print_json)(char* json);

static inline void json_append(pthread_mutex_t* l, char destination[MAX_PROVJSON_BUFFER_LENGTH], char* source){
  char* json;
  bool is_tasked=false;
  pthread_mutex_lock(l);
  // we cannot append buffer is full, need to print json out
  if(!append(destination, source)){
    // we need to check that there is no one already printing the json
    pthread_mutex_lock(&l_out);
    if(!writing_out){
      writing_out = true;
      is_tasked = true;
    }
    pthread_mutex_unlock(&l_out);

    // we are tasked to print the json
    if(is_tasked){
      json = ready_to_print();
      print_json(json);
      pthread_mutex_lock(&l_out);
      writing_out = false;
      pthread_mutex_unlock(&l_out);
    }
    pthread_mutex_unlock(l);
    json_append(l, destination, source);
    return;
  }
  pthread_mutex_unlock(l);
}

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};

static int mod_table[] = {0, 2, 1};

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3) + 1;

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 2 - i] = '=';
    encoded_data[(*output_length)-1]='\0';
    return encoded_data;
}

char* node_info_to_json(char* buffer, struct node_identifier* n){
  sprintf(buffer, "{\"cf:type\": %u, \"cf:id\":%llu, \"cf:boot_id\":%u, \"cf:machine_id\":%u, \"cf:version\":%u}", n->type, n->id, n->boot_id, n->machine_id, n->version);
  return buffer;
}

char* edge_info_to_json(char* buffer, struct edge_identifier* e){
  sprintf(buffer, "{\"cf:type\": %u, \"cf:id\":%llu, \"cf:boot_id\":%u, \"cf:machine_id\":%u}", e->type, e->id, e->boot_id, e->machine_id);
  return buffer;
}

static char* bool_str[] = {"false", "true"};

char* edge_to_json(char* buffer, struct edge_struct* e){
  char edge_info[1024];
  size_t length;
  char* id = base64_encode(e->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  char* sender = base64_encode(e->snd.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);;
  char* receiver = base64_encode(e->rcv.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);;
  sprintf(buffer, "\"cf:%s\":{\"cf:edge_info\":%s, \"cf:type\":\"%s\", \"cf:type\":%s, \"cf:sender\":\"cf:%s\", \"cf:receiver\":\"cf:%s\"}",
    id,
    edge_info_to_json(edge_info, &e->identifier.edge_id),
    edge_str[e->type],
    bool_str[e->allowed],
    sender,
    receiver);
  free(id);
  free(sender);
  free(receiver);
  return buffer;
}

char* disc_to_json(char* buffer, struct disc_node_struct* n){
  char node_info[1024];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : { \"cf:node_info\": %s}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id));
  free(id);
  return buffer;
}

char* task_to_json(char* buffer, struct task_prov_struct* n){
  char node_info[1024];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"node_info\":%s, \"user_id\":%u, \"group_id\":%u}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id),
    n->uid,
    n->gid);
  free(id);
  return buffer;
}

#define UUID_STR_SIZE 37
char* uuid_to_str(uint8_t* uuid, char* str, size_t size){
  if(size<37){
    sprintf(str, "UUID-ERROR");
    return str;
  }
  sprintf(str, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
    uuid[0], uuid[1], uuid[2], uuid[3]
    , uuid[4], uuid[5]
    , uuid[6], uuid[7]
    , uuid[8], uuid[9]
    , uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
    return str;
}

static char STR_UNKNOWN[]= "unknown";
static char STR_BLOCK_SPECIAL[]= "block special";
static char STR_CHAR_SPECIAL[]= "char special";
static char STR_DIRECTORY[]= "directory";
static char STR_FIFO[]= "fifo";
static char STR_LINK[]= "link";
static char STR_FILE[]= "file";
static char STR_SOCKET[]= "socket";

static inline char* get_inode_type(mode_t mode){
  char* type=STR_UNKNOWN;
  if(S_ISBLK(mode))
    type=STR_BLOCK_SPECIAL;
  else if(S_ISCHR(mode))
    type=STR_CHAR_SPECIAL;
  else if(S_ISDIR(mode))
    type=STR_DIRECTORY;
  else if(S_ISFIFO(mode))
    type=STR_FIFO;
  else if(S_ISLNK(mode))
    type=STR_LINK;
  else if(S_ISREG(mode))
    type=STR_FILE;
  else if(S_ISSOCK(mode))
    type=STR_SOCKET;
  return type;
}

char* inode_to_json(char* buffer, struct inode_prov_struct* n){

  char msg_info[1024];
  char node_info[1024];
  char uuid[UUID_STR_SIZE];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : { \"cf:node_info\": %s, \"cf:user_id\":%u, \"cf:group_id\":%u, \"cf:type\":\"%s\", \"cf:mode\":\"0X%04hhX\", \"cf:uuid\":\"%s\"}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id),
    n->uid,
    n->gid,
    get_inode_type(n->mode),
    n->mode,
    uuid_to_str(n->sb_uuid, uuid, UUID_STR_SIZE));
  free(id);
  return buffer;
}

char* sb_to_json(char* buffer, struct sb_struct* n){
  char node_info[1024];
  char uuid[UUID_STR_SIZE];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"cf:node_info\":%s, \"cf:uuid\":\"%s\"}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id),
    uuid_to_str(n->uuid, uuid, UUID_STR_SIZE));
  free(id);
  return buffer;
}

char* msg_to_json(char* buffer, struct msg_msg_struct* n){
  char node_info[1024];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"cf:node_info\":%s, \"cf:type\":%ld}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id),
    n->type);
  free(id);
  return buffer;
}

char* shm_to_json(char* buffer, struct shm_struct* n){
  char node_info[1024];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"cf:node_info\":%s, \"cf:mode\":\"0X%04hhX\"}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id),
    n->mode);
  free(id);
  return buffer;
}

char* sock_to_json(char* buffer, struct sock_struct* n){
  char node_info[1024];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"cf:node_info\":%s, \"cf:type\":%u, \"cf:family\":%u, \"cf:protocol\":%u}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id),
    n->type,
    n->family,
    n->protocol);
  free(id);
  return buffer;
}

char* str_msg_to_json(char* buffer, struct str_struct* n){
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"cf:msg\":\"%s\"}",
    id,
    n->str);
  free(id);
  return buffer;
}

char* sockaddr_to_json(char* buffer, struct sockaddr* addr, size_t length){
  char host[NI_MAXHOST];
  char serv[NI_MAXSERV];

  if(addr->sa_family == AF_INET){
    getnameinfo(addr, length, host, NI_MAXHOST, serv, NI_MAXSERV, 0);
    sprintf(buffer, "{\"type\":\"AF_INET\", \"host\":\"%s\", \"serv\":\"%s\"}", host, serv);
  }else if(addr->sa_family == AF_INET6){
    getnameinfo(addr, length, host, NI_MAXHOST, serv, NI_MAXSERV, 0);
    sprintf(buffer, "{\"type\":\"AF_INET6\", \"host\":\"%s\", \"serv\":\"%s\"}", host, serv);
  }else if(addr->sa_family == AF_UNIX){
    sprintf(buffer, "{\"type\":\"AF_UNIX\", \"path\":\"%s\"}", ((struct sockaddr_un*)addr)->sun_path);
  }else{
    sprintf(buffer, "{\"type\":\"OTHER\"}");
  }

  return buffer;
}

char* addr_to_json(char* buffer, struct address_struct* n){
  char node_info[1024];
  char addr_info[PATH_MAX+1024];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"cf:node_info\":%s, \"cf:address\":%s}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id),
    sockaddr_to_json(addr_info, &n->addr, n->length));
  free(id);
  return buffer;
}

char* pathname_to_json(char* buffer, struct file_name_struct* n){
  char node_info[1024];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"cf:node_info\":%s, \"name\":\"%s\"}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id),
    n->name);
  free(id);
  return buffer;
}

char* ifc_to_json(char* buffer, struct ifc_context_struct* n){
  char node_info[1024];
  size_t length;
  char* id = base64_encode(n->identifier.buffer, PROV_IDENTIFIER_BUFFER_LENGTH, &length);
  sprintf(buffer, "\"cf:%s\" : {\"node_info\":%s, \"ifc\":\"TODO\"}",
    id,
    node_info_to_json(node_info, &n->identifier.node_id));
  free(id);
  return buffer;
}

char* prefix_json(){
  return "\"prov\" : \"http://www.w3.org/ns/prov\", \"cf\":\"http://www.camflow.org\"";
}