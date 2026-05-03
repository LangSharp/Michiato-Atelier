/**
 * @file inventory.c
 * @brief Implementación del módulo de inventario
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3 - Ver archivo LICENSE
 */
#include "inventory.h"
#include "file_db.h"
#include <string.h>
#include <stdlib.h>

int inventory_init(void) {
    db_init();
    return OP_SUCCESS;
}

int inventory_add(Product *p) {
    char buffer[512];
    int id;

    if (!p) return OP_ERROR_INVALID_DATA;

    id = db_get_next_id("inventory");
    if (id == -1) return OP_ERROR_INVALID_ID;
    p->id = id;

    snprintf(buffer, sizeof(buffer), "%d:%s:%s:%.2f:%d",
             p->id, p->name, p->description, p->price, p->quantity);

    return db_insert(INVENTORY_FILE, buffer);
}

static int parse_product_line(const char *line, Product *p) {
    // Parse manual: id:name:description:price:quantity
    // Maneja campos vacíos (::)
    int field = 0;
    const char *start = line;
    const char *end;
    
    p->id = 0;
    p->name[0] = 0;
    p->description[0] = 0;
    p->price = 0;
    p->quantity = 0;
    
    while (field < 5) {
        end = strchr(start, ':');
        
        if (field == 0) {
            // ID
            if (end) {
                char temp[32];
                int len = end - start;
                if (len >= 32) len = 31;
                strncpy(temp, start, len);
                temp[len] = 0;
                p->id = atoi(temp);
            } else {
                p->id = atoi(start);
                break;
            }
        } else if (field == 1) {
            // Name
            if (end) {
                int len = end - start;
                if (len >= MAX_NAME) len = MAX_NAME - 1;
                strncpy(p->name, start, len);
                p->name[len] = 0;
            } else {
                strncpy(p->name, start, MAX_NAME - 1);
                p->name[MAX_NAME - 1] = 0;
                break;
            }
        } else if (field == 2) {
            // Description
            if (end) {
                int len = end - start;
                if (len >= MAX_DESC) len = MAX_DESC - 1;
                strncpy(p->description, start, len);
                p->description[len] = 0;
            } else {
                strncpy(p->description, start, MAX_DESC - 1);
                p->description[MAX_DESC - 1] = 0;
                break;
            }
        } else if (field == 3) {
            // Price
            if (end) {
                char temp[32];
                int len = end - start;
                if (len >= 32) len = 31;
                strncpy(temp, start, len);
                temp[len] = 0;
                p->price = atof(temp);
            } else {
                p->price = atof(start);
                break;
            }
        } else if (field == 4) {
            // Quantity
            p->quantity = atoi(start);
            break;
        }
        
        field++;
        if (end) start = end + 1; else break;
    }
    
    return (field >= 4) ? OP_SUCCESS : OP_ERROR_INVALID_DATA;
}

int inventory_get_by_id(int id, Product *p) {
    char line[MAX_LINE_LENGTH];
    
    if (!p) return OP_ERROR_INVALID_DATA;
    
    if (db_select_by_id(INVENTORY_FILE, id, line) != OP_SUCCESS) {
        return OP_ERROR_NOT_FOUND;
    }
    
    return parse_product_line(line, p);
}

int inventory_list_all(Product products[], int *count) {
    char records[MAX_RECORDS][MAX_LINE_LENGTH];
    int i, total;
    
    if (!products || !count) return OP_ERROR_INVALID_DATA;
    
    db_select_all(INVENTORY_FILE, records, &total);
    
    *count = 0;
    for (i = 0; i < total; i++) {
        if (parse_product_line(records[i], &products[*count]) == OP_SUCCESS) {
            (*count)++;
        }
    }
    
    return OP_SUCCESS;
}

int inventory_edit(int id, Product *p) {
    char buffer[512];
    Product existing;
    
    if (!p) return OP_ERROR_INVALID_DATA;
    if (inventory_get_by_id(id, &existing) != OP_SUCCESS) {
        return OP_ERROR_NOT_FOUND;
    }
    
    if (p->name[0]) strncpy(existing.name, p->name, MAX_NAME - 1);
    if (p->description[0]) strncpy(existing.description, p->description, MAX_DESC - 1);
    if (p->price > 0) existing.price = p->price;
    if (p->quantity != -1) existing.quantity = p->quantity;
    
    snprintf(buffer, sizeof(buffer), "%d:%s:%s:%.2f:%d",
             existing.id, existing.name, existing.description, existing.price, existing.quantity);
    
    return db_update(INVENTORY_FILE, id, buffer);
}

int inventory_delete(int id) {
    return db_delete(INVENTORY_FILE, id);
}

int inventory_update_stock(int id, int quantity_change) {
    char buffer[512];
    Product p;
    
    if (inventory_get_by_id(id, &p) != OP_SUCCESS) {
        return OP_ERROR_NOT_FOUND;
    }
    
    p.quantity += quantity_change;
    if (p.quantity < 0) {
        return OP_ERROR_NO_STOCK;
    }
    
    snprintf(buffer, sizeof(buffer), "%d:%s:%s:%.2f:%d",
             p.id, p.name, p.description, p.price, p.quantity);
    
    return db_update(INVENTORY_FILE, id, buffer);
}

int inventory_get_stock(int id) {
    Product p;
    if (inventory_get_by_id(id, &p) == OP_SUCCESS) {
        return p.quantity;
    }
    return -1;
}
