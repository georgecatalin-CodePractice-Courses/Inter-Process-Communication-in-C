/*
 * common.h
 *
 *  Created on: Jun 2, 2021
 *      Author: georgeca
 *
 *  ***********************
 *  typedef <existing_type> <the_new_alias>
 */

typedef struct test_struct_t
{
	unsigned int a;
	unsigned int b;
} test_struct_t;

/* ***
* will be using
* 'test_struct_t struct_t1'  when declaring struct_t1 instead of 'struct test_struct_t struct_t1'
* ***/

typedef struct result_struct_t
{
	unsigned int c;
} result_struct_t;
