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
#ifndef __PROVENANCEFILTER_H
#define __PROVENANCEFILTER_H

#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <linux/provenance.h>
#include <linux/provenance_types.h>


/*
* @filter pointer to contain filter to read
* read the current state of the node filter.
*/
int provenance_get_node_filter( uint64_t* filter );

/*
* @filter value of node filter
* set node provenance capture filter.
*/
int provenance_add_node_filter( uint64_t filter );
int provenance_remove_node_filter( uint64_t filter );
int provenance_reset_node_filter( void );

/*
* @filter pointer to contain filter to read
* read the current state of the node filter.
*/
int provenance_get_propagate_node_filter( uint64_t* filter );

/*
* @filter value of node filter
* set node provenance propagate filter.
*/
int provenance_add_propagate_node_filter( uint64_t filter );
int provenance_remove_propagate_node_filter( uint64_t filter );
int provenance_reset_propagate_node_filter( void );

/*
* @filter pointer to contain filter to read
* read the current state of the relation filter.
*/
int provenance_get_derived_filter( uint64_t* filter );
int provenance_get_generated_filter( uint64_t* filter );
int provenance_get_used_filter( uint64_t* filter );
int provenance_get_informed_filter( uint64_t* filter );

/*
* @filter value of node filter
* set relation provenance capture filter.
*/
int provenance_add_derived_filter( uint64_t filter );
int provenance_add_generated_filter( uint64_t filter );
int provenance_add_used_filter( uint64_t filter );
int provenance_add_informed_filter( uint64_t filter );

static inline int provenance_add_relation_filter( uint64_t filter ) {
  if (prov_is_derived(filter))
    return provenance_add_derived_filter(filter);
  else if (prov_is_generated(filter))
    return provenance_add_generated_filter(filter);
  else if (prov_is_used(filter))
    return provenance_add_used_filter(filter);
  else if (prov_is_informed(filter))
    return provenance_add_informed_filter(filter);
  return -1;
}

int provenance_remove_derived_filter( uint64_t filter );
int provenance_remove_generated_filter( uint64_t filter );
int provenance_remove_used_filter( uint64_t filter );
int provenance_remove_informed_filter( uint64_t filter );

static inline int provenance_remove_relation_filter( uint64_t filter ) {
  if (prov_is_derived(filter))
    return provenance_remove_derived_filter(filter);
  else if (prov_is_generated(filter))
    return provenance_remove_generated_filter(filter);
  else if (prov_is_used(filter))
    return provenance_remove_used_filter(filter);
  else if (prov_is_informed(filter))
    return provenance_remove_informed_filter(filter);
  return -1;
}

int provenance_reset_derived_filter( void );
int provenance_reset_generated_filter( void );
int provenance_reset_used_filter( void );
int provenance_reset_informed_filter( void );

static inline int provenance_reset_relation_filter( void ) {
  provenance_reset_derived_filter();
  provenance_reset_generated_filter();
  provenance_reset_used_filter();
  provenance_reset_informed_filter();
}

/*
* @filter pointer to contain filter to read
* read the current state of the relation filter.
*/
int provenance_get_propagate_derived_filter( uint64_t* filter );
int provenance_get_propagate_generated_filter( uint64_t* filter );
int provenance_get_propagate_used_filter( uint64_t* filter );
int provenance_get_propagate_informed_filter( uint64_t* filter );

/*
* @filter value of node filter
* set relation provenance capture filter.
*/
int provenance_add_propagate_derived_filter( uint64_t filter );
int provenance_add_propagate_generated_filter( uint64_t filter );
int provenance_add_propagate_used_filter( uint64_t filter );
int provenance_add_propagate_informed_filter( uint64_t filter );

static inline int provenance_add_propagate_relation_filter( uint64_t filter ) {
  if (prov_is_derived(filter))
    return provenance_add_propagate_derived_filter(filter);
  else if (prov_is_generated(filter))
    return provenance_add_propagate_generated_filter(filter);
  else if (prov_is_used(filter))
    return provenance_add_propagate_used_filter(filter);
  else if (prov_is_informed(filter))
    return provenance_add_propagate_informed_filter(filter);
  return -1;
}

int provenance_remove_propagate_derived_filter( uint64_t filter );
int provenance_remove_propagate_generated_filter( uint64_t filter );
int provenance_remove_propagate_used_filter( uint64_t filter );
int provenance_remove_propagate_informed_filter( uint64_t filter );

static inline int provenance_remove_propagate_relation_filter( uint64_t filter ) {
  if (prov_is_derived(filter))
    return provenance_remove_propagate_derived_filter(filter);
  else if (prov_is_generated(filter))
    return provenance_remove_propagate_generated_filter(filter);
  else if (prov_is_used(filter))
    return provenance_remove_propagate_used_filter(filter);
  else if (prov_is_informed(filter))
    return provenance_remove_propagate_informed_filter(filter);
  return -1;
}

int provenance_reset_propagate_derived_filter( void );
int provenance_reset_propagate_generated_filter( void );
int provenance_reset_propagate_used_filter( void );
int provenance_reset_propagate_informed_filter( void );

static inline int provenance_reset_propagate_relation_filter( void ) {
  int rc = 0;
  rc |= provenance_reset_propagate_derived_filter();
  rc |= provenance_reset_propagate_generated_filter();
  rc |= provenance_reset_propagate_used_filter();
  rc |= provenance_reset_propagate_informed_filter();
  return rc;
}

#endif
