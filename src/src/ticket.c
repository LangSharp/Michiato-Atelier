/**
 * @file ticket.c
 * @brief Implementación del módulo de tickets
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3 - Ver archivo LICENSE
 */
#include "ticket.h"
#include "inventory.h"
#include "file_db.h"
#include <string.h>

int ticket_init(void) {
    db_init();
    return OP_SUCCESS;
}

static int parse_ticket_line(const char *line, Ticket *t) {
    char *copy = strdup(line);
    if (!copy) return OP_ERROR_FILE_IO;
    
    char *token;
    int field = 0;
    
    token = strtok(copy, ":");
    while (token != NULL && field < 6) {
        switch (field) {
            case 0: t->id = atoi(token); break;
            case 1: strncpy(t->date, token, 7); t->date[7] = 0; break;
            case 2: t->type = token[0]; break;
            case 3: strncpy(t->customer_name, token, MAX_NAME-1); t->customer_name[MAX_NAME-1] = 0; break;
            case 4: t->total = atof(token); break;
            case 5: t->item_count = atoi(token); break;
        }
        field++;
        token = strtok(NULL, ":");
    }
    
    // Parse items
    if (field >= 6 && t->item_count > 0) {
        char *items_str = strtok(NULL, "");
        if (items_str) {
            int i = 0;
            char *item = strtok(items_str, ";");
            while (item != NULL && i < MAX_ITEMS) {
                char *p_id = strtok(item, ",");
                char *qty = strtok(NULL, ",");
                char *price = strtok(NULL, ",");
                char *name = strtok(NULL, ",");
                
                if (p_id && qty && price && name) {
                    t->items[i].product_id = atoi(p_id);
                    t->items[i].quantity = atoi(qty);
                    t->items[i].unit_price = atof(price);
                    strncpy(t->items[i].product_name, name, MAX_NAME-1);
                    t->items[i].product_name[MAX_NAME-1] = 0;
                    i++;
                }
                item = strtok(NULL, ";");
            }
            t->item_count = i;
        }
    }
    
    free(copy);
    return (field >= 6) ? OP_SUCCESS : OP_ERROR_INVALID_DATA;
}

int ticket_create(Ticket *t) {
    char buffer[2048];
    char items_str[1024];
    int i;

    if (!t) return OP_ERROR_INVALID_DATA;

    // Validar stock primero (ventas)
    if (t->type == 'I') {
        for (i = 0; i < t->item_count; i++) {
            if (inventory_get_stock(t->items[i].product_id) < t->items[i].quantity)
                return OP_ERROR_NO_STOCK;
        }
    }

    // Asignar ID y calcular total
    int id = db_get_next_id("tickets");
    if (id == -1) return OP_ERROR_INVALID_ID;
    t->id = id;
    t->total = ticket_calculate_total(t);

    // Construir cadena de items
    items_str[0] = 0;
    for (i = 0; i < t->item_count; i++) {
        char item_part[256];
        snprintf(item_part, sizeof(item_part), "%d,%d,%.2f,%s;",
                 t->items[i].product_id, t->items[i].quantity,
                 t->items[i].unit_price, t->items[i].product_name);
        strcat(items_str, item_part);
    }

    // Guardar ticket en archivo
    snprintf(buffer, sizeof(buffer), "%d:%s:%c:%s:%.2f:%d:%s",
             t->id, t->date, t->type, t->customer_name, t->total, t->item_count, items_str);

    if (db_insert(TICKETS_FILE, buffer) != OP_SUCCESS) {
        return OP_ERROR_FILE_IO;
    }

    // Actualizar stock
    if (t->type == 'I') {
        for (i = 0; i < t->item_count; i++)
            inventory_update_stock(t->items[i].product_id, -t->items[i].quantity);
    } else if (t->type == 'E') {
        for (i = 0; i < t->item_count; i++)
            inventory_update_stock(t->items[i].product_id, t->items[i].quantity);
    }

    return OP_SUCCESS;
}

int ticket_get_by_id(int id, Ticket *t) {
    char line[MAX_LINE_LENGTH];
    
    if (!t) return OP_ERROR_INVALID_DATA;
    
    if (db_select_by_id(TICKETS_FILE, id, line) != OP_SUCCESS) {
        return OP_ERROR_NOT_FOUND;
    }
    
    return parse_ticket_line(line, t);
}

int ticket_list_all(Ticket tickets[], int *count) {
    char records[MAX_RECORDS][MAX_LINE_LENGTH];
    int i, total;
    
    if (!tickets || !count) return OP_ERROR_INVALID_DATA;
    
    db_select_all(TICKETS_FILE, records, &total);
    
    *count = 0;
    for (i = 0; i < total; i++) {
        if (parse_ticket_line(records[i], &tickets[*count]) == OP_SUCCESS) {
            (*count)++;
        }
    }
    
    return OP_SUCCESS;
}

int ticket_print(int id) {
    Ticket t;

    if (ticket_get_by_id(id, &t) != OP_SUCCESS) {
        printf("Ticket #%d no encontrado.\n", id);
        return OP_ERROR_NOT_FOUND;
    }

    return ticket_print_full(&t);
}

int ticket_print_full(Ticket *t) {
    int i;

    if (!t) return OP_ERROR_INVALID_DATA;

    utils_print_separator();
    printf("  MICHIATO ATELIER\n");
    printf("  Bazar y Regalos\n");
    utils_print_separator();
    printf("Ticket #: %d\n", t->id);
    printf("Fecha:    %s\n", t->date);
    printf("Tipo:     %s\n", t->type == 'I' ? "INGRESO (Venta)" : "EGRESO (Gasto)");
    printf("Cliente:  %s\n", t->customer_name);
    utils_print_separator();

    for (i = 0; i < t->item_count; i++) {
        double subtotal = t->items[i].quantity * t->items[i].unit_price;
        printf("%-30s x%d\n", t->items[i].product_name, t->items[i].quantity);
        printf("  $%.2f c/u               $%.2f\n", t->items[i].unit_price, subtotal);
    }

    utils_print_separator();
    printf("TOTAL:                    $%.2f\n", t->total);
    utils_print_separator();
    printf("      Gracias por su compra!\n\n");

    return OP_SUCCESS;
}

double ticket_calculate_total(Ticket *t) {
    double total = 0;
    int i;

    if (!t) return 0;

    for (i = 0; i < t->item_count; i++) {
        total += t->items[i].quantity * t->items[i].unit_price;
    }

    return total;
}

int ticket_get_summary(double *total_ingresos, double *total_egresos, int *count_ingresos, int *count_egresos) {
    char records[MAX_RECORDS][MAX_LINE_LENGTH];
    int total, i;
    char *token;

    *total_ingresos = 0;
    *total_egresos = 0;
    *count_ingresos = 0;
    *count_egresos = 0;

    db_select_all(TICKETS_FILE, records, &total);

    for (i = 0; i < total; i++) {
        token = strtok(records[i], ":");
        if (!token) continue;
        
        token = strtok(NULL, ":"); // date
        if (!token) continue;
        
        token = strtok(NULL, ":"); // type
        if (!token) continue;
        
        if (token[0] == 'I') {
            token = strtok(NULL, ":"); // customer_name
            token = strtok(NULL, ":"); // total
            if (token) {
                *total_ingresos += atof(token);
                (*count_ingresos)++;
            }
        } else if (token[0] == 'E') {
            token = strtok(NULL, ":"); // customer_name
            token = strtok(NULL, ":"); // total
            if (token) {
                *total_egresos += atof(token);
                (*count_egresos)++;
            }
        }
    }

    return OP_SUCCESS;
}
