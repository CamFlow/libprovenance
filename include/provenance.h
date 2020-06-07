/*
*
* Author: Thomas Pasquier <thomas.pasquier@bristol.ac.uk>
*
* Copyright (C) 2015-2016 University of Cambridge
* Copyright (C) 2016-2017 Harvard University
* Copyright (C) 2017-2018 University of Cambridge
* Copyright (C) 2018-2019 University of Bristol
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2, as
* published by the Free Software Foundation.
*
*/
#ifndef __PROVENANCELIB_H
#define __PROVENANCELIB_H


#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/provenance.h>
#include <linux/provenance_fs.h>
#include <linux/provenance_utils.h>
#include <linux/provenance_types.h>

#define xstr(s) str(s)
#define str(s) # s

#define PROVLIB_VERSION_MAJOR 0
#define PROVLIB_VERSION_MINOR 4
#define PROVLIB_VERSION_PATCH 10
#define PROVLIB_VERSION_STR   "v"xstr(PROVLIB_VERSION_MAJOR)\
    "."xstr(PROVLIB_VERSION_MINOR)\
    "."xstr(PROVLIB_VERSION_PATCH)\

#define PROVLIB_COMMIT "55e4384675b471d21ed15fd55222e297aac6722c"

struct provenance_ops{
  void (*init)(void);
  bool (*filter)(prov_entry_t* msg);
  void (*received_prov)(union prov_elt*);
  void (*received_long_prov)(union long_prov_elt*);
  /* relation callback */
  void (*log_derived)(struct relation_struct*);
  void (*log_generated)(struct relation_struct*);
  void (*log_used)(struct relation_struct*);
  void (*log_informed)(struct relation_struct*);
  void (*log_influenced)(struct relation_struct*);
  void (*log_associated)(struct relation_struct*);
  /* nodes callback */
  void (*log_proc)(struct proc_prov_struct*);
  void (*log_task)(struct task_prov_struct*);
  void (*log_inode)(struct inode_prov_struct*);
  void (*log_str)(struct str_struct*);
  void (*log_act_disc)(struct disc_node_struct*);
  void (*log_agt_disc)(struct disc_node_struct*);
  void (*log_ent_disc)(struct disc_node_struct*);
  void (*log_msg)(struct msg_msg_struct*);
  void (*log_shm)(struct shm_struct*);
  void (*log_packet)(struct pck_struct*);
  void (*log_address)(struct address_struct*);
  void (*log_file_name)(struct file_name_struct*);
  void (*log_iattr)(struct iattr_prov_struct*);
  void (*log_xattr)(struct xattr_prov_struct*);
  void (*log_packet_content)(struct pckcnt_struct*);
  void (*log_arg)(struct arg_struct*);
  void (*log_machine)(struct machine_struct*);
  /* callback for library errors */
  void (*log_error)(char*);
  /* is it filter only? for query framework */
  bool is_query;
};

void prov_record(union prov_elt* msg);
void long_prov_record(union long_prov_elt* msg);

/*
* Function return boolean value corresponding to the presence or not of the
* provenance module in the kernel.
*/
bool provenance_is_present(void);

/*
* Function return boolean value representing if either or not provenance
* has ever been written by the kernel.
*/
bool provenance_was_written(void);

/*
* @ops structure containing audit callbacks
* start and register callback. Note that there is no concurrency guarantee made.
* The application developper is expected to deal with concurrency issue.
*/
int provenance_relay_register(struct provenance_ops* ops, const char* name);

/*
* shutdown tightly the things that are running behind the scene.
*/
void provenance_relay_stop(void);

/* security file manipulation */

/*
* @v boolean value
* enable or disable provenance data capture depending on the value of v. Will
* fail if the current process is not root.
*/
int provenance_set_enable(bool v);

/*
* return either or not the provenance capture is active.
*/
bool provenance_get_enable(void);

/*
* @v boolean value
* activate provenance on all kernel objects. WARNING the computer may slow down
* dramatically and the amount of data generated may be excessively large. Will
* fail if current process is not root.
*/
int provenance_set_all(bool v);

/*
* return either or not provenance on all kernel object is active.
*/
bool provenance_get_all(void);

/*
* @v boolean value
* activate provenance node compression.
*/
int provenance_should_compress_node(bool v);

/*
* return either or not nodes are compressed.
*/
bool provenance_does_compress_node(void);

/*
* @v boolean value
* activate provenance edge compression.
*/
int provenance_should_compress_edge(bool v);

/*
* return either or not edges are compressed.
*/
bool provenance_does_compress_edge(void);

/*
* @v boolean value
* activate provenance duplication.
*/
int provenance_should_duplicate(bool v);

/*
* return either or not provenance duplication is enabled.
*/
bool provenance_does_duplicate(void);

/*
* @v boolean value
* Hide the current process from provenance capture. Should be mostly used by the
* provenance capture service itself. Will fail if the current process is not
* root.
*/
int provenance_set_opaque(bool v);

/*
* return if current process is opaque or not.
*/
bool provenance_get_opaque(void);

/*
* @v boolean value
* Request the current process to be part of the provenance record (even if 'all'
* is not set).
*/
int provenance_set_tracked(bool v);

/*
* return if current process is tracked or not.
*/
bool provenance_get_tracked(void);

/*
* @v boolean value
* Request the current process to propagate tracking.
*/
int provenance_set_propagate(bool v);

/*
* return if current process propagate tracking or not.
*/
bool provenance_get_propagate(void);

/*
* apply label to current process.
*/
int provenance_label(const char *label);

/*
* @v uint32_t value
* Assign an ID to the current machine. Will fail if the current process is not
* root.
*/
int provenance_set_machine_id(uint32_t v);

/*
* @v pointer to uint32_t value
* Read the machine ID corresponding to the current machine.
*/
int provenance_get_machine_id(uint32_t* v);

/*
* @v uint32_t value
* Assign an ID to the current boot. Will fail if the current process is not
* root.
*/
int provenance_set_boot_id(uint32_t v);

/*
* @v pointer to uint32_t value
* Read the boot ID corresponding to the current machine.
*/
int provenance_get_boot_id(uint32_t* v);

/*
* @node node data structure to be recorded
* API to dsiclose a provenance node. Some values should be left blank and Will
* be updated by the kernel.
*/
int provenance_disclose_node(struct disc_node_struct* node);

/*
* @node node data structure to be retrieved
* API to retrieve the last disclosed node.
*/
int provenance_last_disclosed_node(struct disc_node_struct* node);

/*
* @relation relation data structure to be recorded
* API to dsiclose a provenance relation. Some values should be left blank and Will
* be updated by the kernel.
*/
int provenance_disclose_relation(struct relation_struct* relation);

/*
* @self point to a node data structure
* self if filled with the provenance information corresponding to the current
* process.
*/
int provenance_self(struct task_prov_struct* self);

/*
* flush the current relay subuffers.
*/
int provenance_flush(void);

int provenance_change_epoch(void);

/*
* @name file name
* @inode_info point to an inode_info structure
* retrieve provenance information of the file associated with name.
*/
int provenance_read_file(const char path[PATH_MAX], union prov_elt* inode_info);

int provenance_file_id(const char path[PATH_MAX], char* buff, size_t len);

int fprovenance_read_file(int fd, union prov_elt* inode_info);

int fprovenance_file_id(int fd, char* buff, size_t len);

/*
* @name file name
* @track boolean either to track or not the file
* set tracking option corresponding to the file associated with name
*/
int provenance_track_file(const char name[PATH_MAX], bool track);

/*
* @fd file descriptor
* @track boolean either to track or not the file
* set tracking option corresponding to the file associated with fd
*/
int fprovenance_track_file(int fd, bool track);

/*
* @name file name
* @opaque boolean either to make opaque or not the file
* make the file opaque to provenance tracking.
*/
int provenance_opaque_file(const char name[PATH_MAX], bool opaque);

/*
* @fd file descriptor
* @opaque boolean either to make opaque or not the file
* make the file opaque to provenance tracking.
*/
int fprovenance_opaque_file(int fd, bool opaque);

/*
* @name file name
* @propagate boolean either to propagate tracking or not
* set propagate option corresponding to the file associated with name
*/
int provenance_propagate_file(const char name[PATH_MAX], bool propagate);

/*
* @fd file descriptor
* @propagate boolean either to propagate tracking or not
* set propagate option corresponding to the file associated with fd
*/
int fprovenance_propagate_file(int fd, bool propagate);

/*
* @name file name
* @label label to be applied to the file
* add label to the file corresponding to name
*/
int provenance_label_file(const char name[PATH_MAX], const char *label);

/*
* @fd file descriptor
* @label label to be applied to the file
* add label to the file corresponding to fd
*/
int fprovenance_label_file(int fd, const char *label);

/*
* @pid process pid
* @process_info point to an process_info structure
* retrieve provenance information of the process associated with pid.
*/
int provenance_read_process(uint32_t pid, union prov_elt* process_info);

/*
* @pid process pid
* @track boolean either to track or not the file
* set tracking option corresponding to the proccess associated with pid
*/
int provenance_track_process(uint32_t pid, bool track);

/*
* @pid process pid
* @opaque boolean either to make opaque or not the file
* make the process opaque to provenance tracking.
*/
int provenance_opaque_process(uint32_t pid, bool opaque);

int provenance_propagate_process(uint32_t pid, bool propagate);

int provenance_label_process(uint32_t pid, const char *label);

int provenance_ingress_ipv4_track(const char* param);
int provenance_ingress_ipv4_propagate(const char* param);
int provenance_ingress_ipv4_record(const char* param);
int provenance_ingress_ipv4_delete(const char* param);
int provenance_ingress_ipv4( struct prov_ipv4_filter* filters, size_t length );

int provenance_egress_ipv4_track(const char* param);
int provenance_egress_ipv4_propagate(const char* param);
int provenance_egress_ipv4_record(const char* param);
int provenance_egress_ipv4_delete(const char* param);
int provenance_egress_ipv4( struct prov_ipv4_filter* filters, size_t length );

int provenance_ingress_ipv6_track(const char* param);
int provenance_ingress_ipv6_propagate(const char* param);
int provenance_ingress_ipv6_record(const char* param);
int provenance_ingress_ipv6_delete(const char* param);
int provenance_ingress_ipv6(prov_ipv6_filter* filters, size_t length);

int provenance_egress_ipv6_track(const char* param);
int provenance_egress_ipv6_propagate(const char* param);
int provenance_egress_ipv6_record(const char* param);
int provenance_egress_ipv6_delete(const char* param);
int provenance_egress_ipv6(prov_ipv6_filter* filters, size_t length);

int provenance_secid_to_secctx( uint32_t secid, char* secctx, uint32_t len);

int provenance_secctx_track(const char* secctx);
int provenance_secctx_propagate(const char* secctx);
int provenance_secctx_opaque(const char* secctx);
int provenance_secctx_delete(const char* secctx);
int provenance_secctx( struct secinfo* filters, size_t length );

int provenance_user_track(const char* name);
int provenance_user_propagate(const char* name);
int provenance_user_opaque(const char* name);
int provenance_user_delete(const char* name);
int provenance_user(struct userinfo* filters, size_t length );

int provenance_group_track(const char* name);
int provenance_group_propagate(const char* name);
int provenance_group_opaque(const char* name);
int provenance_group_delete(const char* name);
int provenance_group(struct groupinfo* filters, size_t length );

int provenance_cgroup_track(const uint32_t cid);
int provenance_cgroup_propagate(const uint32_t cid);
int provenance_cgroup_delete(const uint32_t cid);

int provenance_ns(struct nsinfo* filters, size_t length);

int provenance_policy_hash(uint8_t* buffer, size_t length);

char* relation_id_to_str(uint64_t id);
char* node_id_to_str(uint64_t id);

uint64_t relation_str_to_id(const char* name, uint32_t len);
uint64_t node_str_to_id(const char* name, uint32_t len);

int provenance_version(char* version, size_t len);

int provenance_lib_version(char* version, size_t len);

int provenance_commit(char* commit, size_t len);

int provenance_lib_commit(char* commit, size_t len);

int provenance_create_channel(const char name[PATH_MAX]);



/* HIGH LEVEL DISCLOSING API */
typedef uint64_t agent_t;
typedef uint64_t activity_t;
typedef uint64_t entity_t;

int disclose_init(void);
void disclose_free(uint64_t id);

agent_t disclose_agent(char* json_attributes);
activity_t disclose_activity(char* json_attributes);
entity_t disclose_entity(char* json_attributes);

void disclose_derives(entity_t from, entity_t to);
void disclose_generates(activity_t from, entity_t to);
void disclose_uses(entity_t from, activity_t to);
void disclose_informs(activity_t from, activity_t to);
void disclose_influences(uint64_t activity_t, uint64_t agent_t);
void disclose_associates(uint64_t agent_t, uint64_t activity_t);

entity_t disclose_get_file(const char path[PATH_MAX]);

#endif /* __PROVENANCELIB_H */
