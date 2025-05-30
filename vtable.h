#pragma once
struct _Costumer;

typedef struct {
	//interface to access member functions
	void    (*print)(const struct _Costumer*);
	void    (*printPrice)(const struct _Costumer*);
	void    (*delete)(struct _Costumer*);
} CustomerVTable;