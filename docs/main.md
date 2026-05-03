/**
 * @file main.c
 * @brief Punto de entrada y menú principal del CLI
 *
 * Controla el flujo principal de la aplicación mediante un sistema
 * de menús interactivos. Es el módulo que interactúa directamente
 * con el usuario.
 *
 * Estructura de menús:
 * - Menú Principal (4 opciones)
 *   - Submenú Inventario (CRUD completo)
 *   - Submenú Ventas (crear ventas)
 *   - Submenú Tickets (ver tickets y resumen)
 *
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "inventory.h"
#include "ticket.h"
#include "file_db.h"

extern int inventory_init(void);
extern int ticket_init(void);

void menu_inventory(void);
void menu_sales(void);
void menu_ticket(void);

void submenu_inventory_list(void);
void submenu_inventory_add(void);
void submenu_inventory_edit(void);
void submenu_inventory_delete(void);

void submenu_sales_create(void);

void submenu_ticket_view(void);
void submenu_ticket_summary(void);

/**
 * @brief Función principal
 *
 * Inicializa los módulos de inventario y tickets, luego entra
 * en el ciclo principal del menú.
 *
 * @return 0 al salir
 */
int main(void) {
    int option;

    inventory_init();
    ticket_init();

    while (1) {
        utils_clear_screen();
        utils_print_header("MICHIATO ATELIER");
        printf("\n");
        printf("  1. Gestion de Inventario\n");
        printf("  2. Registro de Venta\n");
        printf("  3. Generar/Ver Ticket\n");
        printf("  4. Salir\n");
        printf("\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (option) {
            case 1:
                menu_inventory();
                break;
            case 2:
                menu_sales();
                break;
            case 3:
                menu_ticket();
                break;
            case 4:
                utils_clear_screen();
                printf("\n");
                utils_print_separator();
                printf("  Gracias por usar Michiato Atelier\n");
                printf("         Hasta luego!\n");
                utils_print_separator();
                printf("\n");
                return 0;
            default:
                printf("\nOpcion invalida.\n");
                utils_pause();
        }
    }

    return 0;
}

/**
 * @brief Submenú de gestión de inventario
 *
 * Opciones:
 * 1. Listar productos (tabla formateada)
 * 2. Agregar producto (formulario interactivo)
 * 3. Editar producto (selección por ID, campos opcionales)
 * 4. Eliminar producto (confirmación requerida)
 * 5. Volver al menú principal
 */
void menu_inventory(void) {
    int option;

    while (1) {
        utils_clear_screen();
        utils_print_header("GESTION DE INVENTARIO");
        printf("\n");
        printf("  1. Listar productos\n");
        printf("  2. Agregar producto\n");
        printf("  3. Editar producto\n");
        printf("  4. Eliminar producto\n");
        printf("  5. Volver al menu principal\n");
        printf("\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (option) {
            case 1:
                submenu_inventory_list();
                break;
            case 2:
                submenu_inventory_add();
                break;
            case 3:
                submenu_inventory_edit();
                break;
            case 4:
                submenu_inventory_delete();
                break;
            case 5:
                return;
            default:
                printf("\nOpcion invalida.\n");
                utils_pause();
        }
    }
}

/**
 * @brief Lista todos los productos en formato tabular
 *
 * Muestra: ID, Nombre, Descripción, Precio, Stock
 * Usa inventory_list_all() para obtener los datos.
 */
void submenu_inventory_list(void) {
    Product products[MAX_RECORDS];
    int count, i;

    utils_clear_screen();
    utils_print_header("LISTADO DE PRODUCTOS");

    if (inventory_list_all(products, &count) != OP_SUCCESS || count == 0) {
        printf("No hay productos en el inventario.\n");
        utils_pause();
        return;
    }

    printf("\n%-4s %-20s %-15s %10s %8s\n", "ID", "Nombre", "Descripcion", "Precio", "Stock");
    utils_print_separator();

    for (i = 0; i < count; i++) {
        printf("%-4d %-20s %-15s %9.2f $ %6d\n",
               products[i].id,
               products[i].name,
               products[i].description,
               products[i].price,
               products[i].quantity);
    }

    printf("\nTotal de productos: %d\n", count);
    utils_pause();
}

/**
 * @brief Formulario para agregar nuevo producto
 *
 * Solicita: nombre, descripción, precio, cantidad inicial.
 * Valida que nombre no esté vacío y precio > 0.
 */
void submenu_inventory_add(void) {
    Product p;

    utils_clear_screen();
    utils_print_header("AGREGAR PRODUCTO");

    memset(&p, 0, sizeof(Product));

    utils_get_string_input("Nombre: ", p.name, MAX_NAME);
    utils_get_string_input("Descripcion: ", p.description, MAX_DESC);
    p.price = utils_get_double_input("Precio: ");
    p.quantity = utils_get_int_input("Cantidad inicial: ");

    if (strlen(p.name) == 0 || p.price <= 0 || p.quantity < 0) {
        printf("\nDatos invalidos.\n");
        utils_pause();
        return;
    }

    if (inventory_add(&p) == OP_SUCCESS) {
        printf("\nProducto agregado exitosamente (ID: %d).\n", p.id);
    } else {
        printf("\nError al agregar producto.\n");
    }

    utils_pause();
}

/**
 * @brief Formulario para editar producto existente
 *
 * Solicita ID del producto, muestra datos actuales,
 * luego permite modificar campos individualmente.
 * Campos vacíos o en 0 se mantienen sin cambio.
 */
void submenu_inventory_edit(void) {
    Product p, new_p;
    int id;

    utils_clear_screen();
    utils_print_header("EDITAR PRODUCTO");

    id = utils_get_int_input("Ingrese ID del producto: ");

    if (inventory_get_by_id(id, &p) != OP_SUCCESS) {
        printf("\nProducto no encontrado.\n");
        utils_pause();
        return;
    }

    printf("\nProducto actual:\n");
    printf("  Nombre: %s\n", p.name);
    printf("  Descripcion: %s\n", p.description);
    printf("  Precio: %.2f\n", p.price);
    printf("  Cantidad: %d\n", p.quantity);

    printf("\nIngrese nuevos datos (Enter para mantener actual):\n");

    memset(&new_p, 0, sizeof(Product));
    utils_get_string_input("Nombre: ", new_p.name, MAX_NAME);
    utils_get_string_input("Descripcion: ", new_p.description, MAX_DESC);
    new_p.price = utils_get_double_input("Precio: ");
    new_p.quantity = utils_get_int_input("Cantidad: ");

    if (inventory_edit(id, &new_p) == OP_SUCCESS) {
        printf("\nProducto actualizado exitosamente.\n");
    } else {
        printf("\nError al actualizar producto.\n");
    }

    utils_pause();
}

/**
 * @brief Eliminación de producto con confirmación
 *
 * Solicita ID, muestra datos del producto,
 * pide confirmación antes de eliminar.
 */
void submenu_inventory_delete(void) {
    Product p;
    int id;

    utils_clear_screen();
    utils_print_header("ELIMINAR PRODUCTO");

    id = utils_get_int_input("Ingrese ID del producto: ");

    if (inventory_get_by_id(id, &p) != OP_SUCCESS) {
        printf("\nProducto no encontrado.\n");
        utils_pause();
        return;
    }

    printf("\nProducto a eliminar:\n");
    printf("  ID: %d\n", p.id);
    printf("  Nombre: %s\n", p.name);
    printf("  Descripcion: %s\n", p.description);

    if (utils_confirm("Esta seguro de eliminar este producto")) {
        if (inventory_delete(id) == OP_SUCCESS) {
            printf("\nProducto eliminado exitosamente.\n");
        } else {
            printf("\nError al eliminar producto.\n");
        }
    } else {
        printf("\nOperacion cancelada.\n");
    }

    utils_pause();
}

/**
 * @brief Submenú de registro de ventas
 *
 * Opciones:
 * 1. Nueva venta (crear ticket tipo 'I')
 * 2. Volver al menú principal
 */
void menu_sales(void) {
    int option;

    while (1) {
        utils_clear_screen();
        utils_print_header("REGISTRO DE VENTAS");
        printf("\n");
        printf("  1. Nueva venta\n");
        printf("  2. Volver al menu principal\n");
        printf("\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (option) {
            case 1:
                submenu_sales_create();
                break;
            case 2:
                return;
            default:
                printf("\nOpcion invalida.\n");
                utils_pause();
        }
    }
}

/**
 * @brief Crea una nueva venta
 *
 * Proceso:
 * 1. Lista productos disponibles (con stock > 0)
 * 2. Permite seleccionar productos por ID (máximo 4)
 * 3. Para cada producto, pide cantidad
 * 4. Muestra resumen y confirmación
 * 5. Crea ticket tipo 'I' (decrementa stock)
 * 6. Imprime ticket formateado
 *
 * Valida stock disponible antes de confirmar.
 */
void submenu_sales_create(void) {
    Product products[MAX_RECORDS];
    Ticket t;
    int product_count, i;
    int choice;

    utils_clear_screen();
    utils_print_header("NUEVA VENTA");

    if (inventory_list_all(products, &product_count) != OP_SUCCESS || product_count == 0) {
        printf("No hay productos disponibles.\n");
        utils_pause();
        return;
    }

    printf("\nProductos disponibles:\n");
    printf("%-4s %-20s %10s %8s\n", "ID", "Nombre", "Precio", "Stock");
    utils_print_separator();
    for (i = 0; i < product_count; i++) {
        if (products[i].quantity > 0) {
            printf("%-4d %-20s %9.2f $ %6d\n",
                   products[i].id,
                   products[i].name,
                   products[i].price,
                   products[i].quantity);
        }
    }

    memset(&t, 0, sizeof(Ticket));
    t.type = 'I';
    utils_get_current_date(t.date);

    printf("\n--- Agregar productos (max %d) ---\n", MAX_ITEMS);

    while (t.item_count < MAX_ITEMS) {
        printf("\nProducto %d:\n", t.item_count + 1);
        choice = utils_get_int_input("  ID del producto (0 para terminar): ");

        if (choice == 0) break;

        for (i = 0; i < product_count; i++) {
            if (products[i].id == choice) break;
        }

        if (i >= product_count) {
            printf("  Producto no encontrado.\n");
            continue;
        }

        if (products[i].quantity <= 0) {
            printf("  Producto sin stock.\n");
            continue;
        }

        int qty = utils_get_int_input("  Cantidad: ");

        if (qty <= 0 || qty > products[i].quantity) {
            printf("  Cantidad invalida o insuficiente.\n");
            continue;
        }

        t.items[t.item_count].product_id = choice;
        t.items[t.item_count].quantity = qty;
        t.items[t.item_count].unit_price = products[i].price;
        strncpy(t.items[t.item_count].product_name, products[i].name, MAX_NAME - 1);
        t.items[t.item_count].product_name[MAX_NAME - 1] = 0;

        t.item_count++;
        printf("  Agregado: %s x%d\n", products[i].name, qty);
    }

    if (t.item_count == 0) {
        printf("\nNo se agregaron productos.\n");
        utils_pause();
        return;
    }

    t.total = ticket_calculate_total(&t);

    printf("\n--- Resumen de venta ---\n");
    for (i = 0; i < t.item_count; i++) {
        printf("  %s x%d = $%.2f\n",
               t.items[i].product_name,
               t.items[i].quantity,
               t.items[i].quantity * t.items[i].unit_price);
    }
    printf("  TOTAL: $%.2f\n", t.total);

    if (!utils_confirm("\nConfirmar venta")) {
        printf("\nVenta cancelada.\n");
        utils_pause();
        return;
    }

    if (ticket_create(&t) == OP_SUCCESS) {
        printf("\nVenta registrada exitosamente!\n");
        printf("Ticket #: %d\n", t.id);
        ticket_print_full(&t);
    } else {
        printf("\nError al registrar venta. Stock insuficiente?\n");
    }

    utils_pause();
}

/**
 * @brief Submenú de tickets
 *
 * Opciones:
 * 1. Ver ticket por ID (impresión formateada)
 * 2. Resumen de ingresos/gastos (reportes)
 * 3. Volver al menú principal
 */
void menu_ticket(void) {
    int option;

    while (1) {
        utils_clear_screen();
        utils_print_header("TICKETS");
        printf("\n");
        printf("  1. Ver ticket por ID\n");
        printf("  2. Resumen de ingresos/gastos\n");
        printf("  3. Volver al menu principal\n");
        printf("\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (option) {
            case 1:
                submenu_ticket_view();
                break;
            case 2:
                submenu_ticket_summary();
                break;
            case 3:
                return;
            default:
                printf("\nOpcion invalida.\n");
                utils_pause();
        }
    }
}

/**
 * @brief Visualiza un ticket específico
 *
 * Solicita ID y muestra el ticket formateado
 * usando ticket_print().
 */
void submenu_ticket_view(void) {
    int id;

    utils_clear_screen();
    utils_print_header("VER TICKET");

    id = utils_get_int_input("Ingrese ID del ticket: ");

    ticket_print(id);
    utils_pause();
}

/**
 * @brief Muestra resumen financiero
 *
 * Calcula y muestra:
 * - Total de ingresos (ventas)
 * - Total de egresos (gastos)
 * - Cantidad de operaciones de cada tipo
 * - Balance final (ingresos - egresos)
 *
 * Usa ticket_get_summary() para obtener los datos.
 */
void submenu_ticket_summary(void) {
    double ingresos, egresos;
    int count_ing, count_eg;

    utils_clear_screen();
    utils_print_header("RESUMEN DE OPERACIONES");

    ticket_get_summary(&ingresos, &egresos, &count_ing, &count_eg);

    printf("\n");
    printf("  INGRESOS (Ventas):\n");
    printf("    Cantidad: %d\n", count_ing);
    printf("    Total:    $%.2f\n", ingresos);
    printf("\n");
    printf("  EGRESOS (Gastos):\n");
    printf("    Cantidad: %d\n", count_eg);
    printf("    Total:    $%.2f\n", egresos);
    printf("\n");
    printf("  BALANCE: $%.2f\n", ingresos - egresos);
    printf("\n");

    utils_pause();
}