/**************************************************************************/
/* def.h                                                                  */
/* This file contains GENERAL definitions used in several data structures */
/**************************************************************************/

#ifndef _DEF_
#define _DEF_

// a value not supposed in stack and queue
#define		EMPTY	0x7FFFFFFF

// new types
typedef void* DATA;					// a type for data (easy to change) - changed to void* instead of int
typedef enum {False, True} BOOL;	// a boolean type

// Node
typedef struct node 
{
	DATA			key;
	struct node*	next;
}NODE;

#endif
