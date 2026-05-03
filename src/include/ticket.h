#ifndef TICKET_H
#define TICKET_H

#include "utils.h"

#define MAX_ITEMS 4

typedef struct {
    int product_id;
    int quantity;
    double unit_price;
    char product_name[MAX_NAME];
} TicketItem;

typedef struct {
    int id;
    char date[8];
    char type;
    char customer_name[MAX_NAME];
    double total;
    TicketItem items[MAX_ITEMS];
    int item_count;
} Ticket;

int ticket_init(void);
int ticket_create(Ticket *t);
int ticket_get_by_id(int id, Ticket *t);
int ticket_list_all(Ticket tickets[], int *count);
int ticket_print(int id);
int ticket_print_full(Ticket *t);
double ticket_calculate_total(Ticket *t);
int ticket_get_summary(double *total_ingresos, double *total_egresos, int *count_ingresos, int *count_egresos);

#endif
