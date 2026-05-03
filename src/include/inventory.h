#ifndef INVENTORY_H
#define INVENTORY_H

#include "utils.h"

#define INVENTORY_FILE "data/inventory.txt"

typedef struct {
    int id;
    char name[MAX_NAME];
    char description[MAX_DESC];
    double price;
    int quantity;
} Product;

int inventory_init(void);
int inventory_add(Product *p);
int inventory_edit(int id, Product *p);
int inventory_delete(int id);
int inventory_get_by_id(int id, Product *p);
int inventory_list_all(Product products[], int *count);
int inventory_update_stock(int id, int quantity_change);
int inventory_get_stock(int id);

#endif
