#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "inventory.h"
#include "ticket.h"

int test_inventory() {
    printf("Test inventario... ");
    // Limpiar archivos
    remove("data/inventory.txt");
    remove("data/tickets.txt");
    remove("data/sequences.txt");
    
    inventory_init();
    
    Product p = {0};
    strcpy(p.name, "Test");
    p.price = 100;
    p.quantity = 5;
    
    printf("\n  - Agregando producto...\n");
    int result = inventory_add(&p);
    printf("    inventory_add result: %d, p.id: %d\n", result, p.id);
    
    printf("  - Leyendo inventory.txt:\n    ");
    fflush(stdout);
    system("cat data/inventory.txt; echo ''");
    
    Product p2 = {0};
    printf("  - Buscando ID %d...\n", p.id);
    result = inventory_get_by_id(p.id, &p2);
    printf("    inventory_get_by_id result: %d\n", result);
    printf("    p2.id: %d\n", p2.id);
    printf("    p2.name: '%s'\n", p2.name);
    printf("    p2.price: %.2f\n", p2.price);
    printf("    p2.quantity: %d\n", p2.quantity);
    
    if (result != OP_SUCCESS || strcmp(p2.name, "Test") != 0) { 
        printf("  FAIL get\n"); 
        return 0; 
    }
    
    printf("  PASS\n");
    return 1;
}

int test_ticket() {
    printf("Test tickets... ");
    // Limpiar archivos
    remove("data/inventory.txt");
    remove("data/tickets.txt");
    remove("data/sequences.txt");
    
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
    
    printf("\n  - Creando ticket...\n");
    int result = ticket_create(&t);
    printf("    ticket_create result: %d, t.id: %d\n", result, t.id);
    
    if (result != OP_SUCCESS) { 
        printf("  FAIL create\n"); 
        return 0; 
    }
    
    if (inventory_get_stock(1) != 8) { 
        printf("  FAIL stock update\n"); 
        return 0; 
    }
    
    Ticket t2;
    ticket_get_by_id(1, &t2);
    if (strcmp(t2.customer_name, "Juan") != 0 || t2.total != 100) { 
        printf("  FAIL data\n"); 
        return 0; 
    }
    
    printf("  PASS\n");
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
