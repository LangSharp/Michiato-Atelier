#include "ticket.h"
#include "inventory.h"
#include "utils.h"
#include <string.h>

static Ticket s_tickets[MAX_RECORDS];
static int s_ticket_count = 0;
static int s_next_ticket_id = 1;

int ticket_init(void) {
    s_ticket_count = 0;
    s_next_ticket_id = 1;
    return OP_SUCCESS;
}

int ticket_create(Ticket *t) {
    if (!t || s_ticket_count >= MAX_RECORDS) return OP_ERROR_FULL;

    if (t->type == 'I') {
        for (int i = 0; i < t->item_count; i++) {
            if (inventory_get_stock(t->items[i].product_id) < t->items[i].quantity)
                return OP_ERROR_NO_STOCK;
        }
    }

    t->id = s_next_ticket_id++;
    t->total = ticket_calculate_total(t);
    s_tickets[s_ticket_count++] = *t;

    if (t->type == 'I') {
        for (int i = 0; i < t->item_count; i++)
            inventory_update_stock(t->items[i].product_id, -t->items[i].quantity);
    } else if (t->type == 'E') {
        for (int i = 0; i < t->item_count; i++)
            inventory_update_stock(t->items[i].product_id, t->items[i].quantity);
    }

    return OP_SUCCESS;
}

int ticket_get_by_id(int id, Ticket *t) {
    if (!t) return OP_ERROR_INVALID_DATA;
    for (int i = 0; i < s_ticket_count; i++) {
        if (s_tickets[i].id == id) {
            *t = s_tickets[i];
            return OP_SUCCESS;
        }
    }
    return OP_ERROR_NOT_FOUND;
}

int ticket_list_all(Ticket tickets[], int *count) {
    if (!tickets || !count) return OP_ERROR_INVALID_DATA;
    *count = s_ticket_count;
    for (int i = 0; i < s_ticket_count; i++) tickets[i] = s_tickets[i];
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
    for (int i = 0; i < t->item_count; i++) {
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
    if (!t) return 0;
    double total = 0;
    for (int i = 0; i < t->item_count; i++)
        total += t->items[i].quantity * t->items[i].unit_price;
    return total;
}

int ticket_get_summary(double *total_ingresos, double *total_egresos, int *count_ingresos, int *count_egresos) {
    *total_ingresos = *total_egresos = 0;
    *count_ingresos = *count_egresos = 0;
    for (int i = 0; i < s_ticket_count; i++) {
        if (s_tickets[i].type == 'I') {
            *total_ingresos += s_tickets[i].total;
            (*count_ingresos)++;
        } else if (s_tickets[i].type == 'E') {
            *total_egresos += s_tickets[i].total;
            (*count_egresos)++;
        }
    }
    return OP_SUCCESS;
}
