#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "inventory.h"
#include "ticket.h"

int test_inventory() {
    printf("Test inventario... ");
    inventory_init();

    Product p = {0};
    strcpy(p.name, "Test");
    p.price = 100;
    p.quantity = 5;
    if (inventory_add(&p) != OP_SUCCESS) { printf("FAIL add\n"); return 0; }

    Product p2;
    if (inventory_get_by_id(1, &p2) != OP_SUCCESS || strcmp(p2.name, "Test")) { printf("FAIL get\n"); return 0; }

    Product new_p = {0};
    strcpy(new_p.name, "Test2");
    new_p.quantity = -1;
    inventory_edit(1, &new_p);
    inventory_get_by_id(1, &p2);
    if (strcmp(p2.name, "Test2") || p2.quantity != 5) { printf("FAIL edit\n"); return 0; }

    inventory_update_stock(1, -3);
    if (inventory_get_stock(1) != 2) { printf("FAIL stock\n"); return 0; }

    printf("PASS\n");
    return 1;
}

int test_ticket() {
    printf("Test tickets... ");
    inventory_init();
    ticket_init();

    Product p = {0};
    strcpy(p.name, "Item");
    p.price = 50;
    p.quantity = 10;
    inventory_add(&p);

    Ticket t = {0};
    t.type = 'I';
    utils_get_current_date(t.date);
    strcpy(t.customer_name, "Juan");
    t.items[0] = (TicketItem){1, 2, 50, "Item"};
    t.item_count = 1;

    if (ticket_create(&t) != OP_SUCCESS) { printf("FAIL create\n"); return 0; }
    if (inventory_get_stock(1) != 8) { printf("FAIL stock update\n"); return 0; }

    Ticket t2;
    ticket_get_by_id(1, &t2);
    if (strcmp(t2.customer_name, "Juan") || t2.total != 100) { printf("FAIL data\n"); return 0; }

    printf("PASS\n");
    return 1;
}

int test_stack() {
    printf("Test stack (items)... ");
    Ticket t = {0};
    t.items[t.item_count++] = (TicketItem){1, 1, 10, "A"};
    t.items[t.item_count++] = (TicketItem){2, 1, 20, "B"};
    if (t.item_count != 2) { printf("FAIL push\n"); return 0; }
    t.item_count--;
    if (t.item_count != 1) { printf("FAIL pop\n"); return 0; }
    printf("PASS\n");
    return 1;
}

int main() {
    int passed = 0;
    passed += test_inventory();
    passed += test_ticket();
    passed += test_stack();
    printf("\n%d/3 tests passed\n", passed);
    return passed == 3 ? 0 : 1;
}
