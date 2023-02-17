/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2015-2016 University of Cambridge,
 * Copyright (C) 2016-2017 Harvard University,
 * Copyright (C) 2017-2018 University of Cambridge,
 * Copyright (C) 2018-2021 University of Bristol,
 * Copyright (C) 2021-2022 University of British Columbia
 *
 * Author: Thomas Pasquier <tfjmp@cs.ubc.ca>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.
 */

#include <unistd.h>

#define MAX_JSON_BUFFER_EXP     13
#define MAX_JSON_BUFFER_LENGTH  ((1 << MAX_JSON_BUFFER_EXP)*sizeof(uint8_t))
#define BUFFER_LENGTH (MAX_JSON_BUFFER_LENGTH-strnlen(buffer, MAX_JSON_BUFFER_LENGTH))

extern __thread char buffer[MAX_JSON_BUFFER_LENGTH];
extern char date[256];
extern pthread_rwlock_t  date_lock;

// ideally should be derived from jiffies
static void update_time( void ){
  struct tm tm;
  struct timeval tv;

  pthread_rwlock_wrlock(&date_lock);
  gettimeofday(&tv, NULL);
  gmtime_r(&tv.tv_sec, &tm);
  strftime(date, 30,"%Y:%m:%dT%H:%M:%S", &tm);
  pthread_rwlock_unlock(&date_lock);
}

static inline void __add_attribute(const char* name, bool comma){
  if(comma){
    strncat(buffer, ",\"", BUFFER_LENGTH);
  }else{
    strncat(buffer, "\"", BUFFER_LENGTH);
  }
  strncat(buffer, name, BUFFER_LENGTH);
  strncat(buffer, "\":", BUFFER_LENGTH);
}

static inline void __add_uint32_attribute(const char* name, const uint32_t value, bool comma){
  char tmp[32];
  __add_attribute(name, comma);
  strncat(buffer, utoa(value, tmp, DECIMAL), BUFFER_LENGTH);
}


static inline void __add_int32_attribute(const char* name, const int32_t value, bool comma){
  char tmp[32];
  __add_attribute(name, comma);
  strncat(buffer, itoa(value, tmp, DECIMAL), BUFFER_LENGTH);
}

static inline void __add_uint32hex_attribute(const char* name, const uint32_t value, bool comma){
  char tmp[32];
  __add_attribute(name, comma);
  strncat(buffer, "\"0x", BUFFER_LENGTH);
  strncat(buffer, utoa(value, tmp, HEX), BUFFER_LENGTH);
  strncat(buffer, "\"", BUFFER_LENGTH);
}

static inline void __add_uint64_attribute(const char* name, const uint64_t value, bool comma){
  char tmp[64];
  __add_attribute(name, comma);
  strncat(buffer, "\"", BUFFER_LENGTH);
  strncat(buffer, ulltoa(value, tmp, DECIMAL), BUFFER_LENGTH);
  strncat(buffer, "\"", BUFFER_LENGTH);
}

static inline void __add_uint64hex_attribute(const char* name, const uint64_t value, bool comma){
  char tmp[64];
  __add_attribute(name, comma);
  strncat(buffer, "\"", BUFFER_LENGTH);
  strncat(buffer, ulltoa(value, tmp, HEX), BUFFER_LENGTH);
  strncat(buffer, "\"", BUFFER_LENGTH);
}

static inline void __add_int64_attribute(const char* name, const int64_t value, bool comma){
  char tmp[64];
  __add_attribute(name, comma);
  strncat(buffer, "\"", BUFFER_LENGTH);
  strncat(buffer, lltoa(value, tmp, DECIMAL), BUFFER_LENGTH);
  strncat(buffer, "\"", BUFFER_LENGTH);
}

static inline void __add_string_attribute(const char* name, const char* value, bool comma){
  if(value[0]=='\0'){ // value is not set
    return;
  }
  __add_attribute(name, comma);
  strncat(buffer, "\"", BUFFER_LENGTH);
  strncat(buffer, value, BUFFER_LENGTH);
  strncat(buffer, "\"", BUFFER_LENGTH);
}

static inline void __add_date_attribute(bool comma){
  __add_attribute("cf:date", comma);
  strncat(buffer, "\"", BUFFER_LENGTH);
  pthread_rwlock_rdlock(&date_lock);
  strncat(buffer, date, BUFFER_LENGTH);
  pthread_rwlock_unlock(&date_lock);
  strncat(buffer, "\"", BUFFER_LENGTH);
}

#define UUID_STR_SIZE 37
static inline char* uuid_to_str(uint8_t* uuid, char* str, size_t size){
  if(size<37){
    snprintf(str, size, "UUID-ERROR");
    return str;
  }
  snprintf(str, size, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
    uuid[0], uuid[1], uuid[2], uuid[3]
    , uuid[4], uuid[5]
    , uuid[6], uuid[7]
    , uuid[8], uuid[9]
    , uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]);
    return str;
}

static inline void __add_ipv4(uint32_t ip, uint32_t port){
    char tmp[8];
    strncat(buffer, uint32_to_ipv4str(ip), BUFFER_LENGTH);
    strncat(buffer, ":", BUFFER_LENGTH);
    strncat(buffer, utoa(htons(port), tmp, DECIMAL), BUFFER_LENGTH);
}

static inline void __add_ipv4_attribute(const char* name, const uint32_t ip, const uint32_t port, bool comma){
  char tmp[64];
  __add_attribute(name, comma);
  strncat(buffer, "\"", BUFFER_LENGTH);
  __add_ipv4(ip, port);
  strncat(buffer, "\"", BUFFER_LENGTH);
}

static inline void __add_machine_id(uint32_t value, bool comma){
  char tmp[32];
  __add_attribute("cf:machine_id", comma);
  strncat(buffer, "\"cf:", BUFFER_LENGTH);
  strncat(buffer, utoa(value, tmp, DECIMAL), BUFFER_LENGTH);
  strncat(buffer, "\"", BUFFER_LENGTH);
}

static inline void __add_name_id(union prov_identifier* name_id, bool comma){
  char *name;
  int i;
  if (name_id->node_id.type == 0)
    return;
  for (i = 0; i < 10; i++) {
    name = name_id_to_str(name_id);
    if (name != NULL)
      break;
    usleep((i+1)*50000);
  }
  if (name != NULL)
    __add_string_attribute("cf:name", name, comma);
}
