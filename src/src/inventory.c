#include "inventory.h"

static Product s_inventory[MAX_RECORDS];
static int s_inventory_count = 0;
static int s_next_inv_id = 1;

int inventory_init(void) {
    s_inventory_count = 0;
    s_next_inv_id = 1;
    return OP_SUCCESS;
}

int inventory_add(Product *p) {
    if (!p || s_inventory_count >= MAX_RECORDS) return OP_ERROR_FULL;
    p->id = s_next_inv_id++;
    s_inventory[s_inventory_count++] = *p;
    return OP_SUCCESS;
}

int inventory_get_by_id(int id, Product *p) {
    if (!p) return OP_ERROR_INVALID_DATA;
    for (int i = 0; i < s_inventory_count; i++) {
        if (s_inventory[i].id == id) {
            *p = s_inventory[i];
            return OP_SUCCESS;
        }
    }
    return OP_ERROR_NOT_FOUND;
}

int inventory_list_all(Product products[], int *count) {
    if (!products || !count) return OP_ERROR_INVALID_DATA;
    *count = s_inventory_count;
    for (int i = 0; i < s_inventory_count; i++) products[i] = s_inventory[i];
    return OP_SUCCESS;
}

int inventory_edit(int id, Product *p) {
    if (!p) return OP_ERROR_INVALID_DATA;
    for (int i = 0; i < s_inventory_count; i++) {
        if (s_inventory[i].id == id) {
            if (p->name[0]) strncpy(s_inventory[i].name, p->name, MAX_NAME-1);
            if (p->description[0]) strncpy(s_inventory[i].description, p->description, MAX_DESC-1);
            if (p->price > 0) s_inventory[i].price = p->price;
            if (p->quantity != -1) s_inventory[i].quantity = p->quantity;
            return OP_SUCCESS;
        }
    }
    return OP_ERROR_NOT_FOUND;
}

int inventory_delete(int id) {
    for (int i = 0; i < s_inventory_count; i++) {
        if (s_inventory[i].id == id) {
            for (int j = i; j < s_inventory_count-1; j++) s_inventory[j] = s_inventory[j+1];
            s_inventory_count--;
            return OP_SUCCESS;
        }
    }
    return OP_ERROR_NOT_FOUND;
}

int inventory_update_stock(int id, int quantity_change) {
    for (int i = 0; i < s_inventory_count; i++) {
        if (s_inventory[i].id == id) {
            s_inventory[i].quantity += quantity_change;
            if (s_inventory[i].quantity < 0) {
                s_inventory[i].quantity -= quantity_change;
                return OP_ERROR_NO_STOCK;
            }
            return OP_SUCCESS;
        }
    }
    return OP_ERROR_NOT_FOUND;
}

int inventory_get_stock(int id) {
    for (int i = 0; i < s_inventory_count; i++)
        if (s_inventory[i].id == id) return s_inventory[i].quantity;
    return -1;
}
