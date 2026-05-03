/**
 * @file ticket.h
 * @brief Gestión de tickets (ventas y gastos)
 *
 * Módulo central para el registro de transacciones del bazar.
 * Maneja tanto ingresos (ventas) como egresos (gastos/devoluciones).
 *
 * El mismo struct Ticket se usa para ambos tipos de operación,
 * diferenciados por el campo 'type'. Esto facilita el cálculo
 * de reportes financieros.
 *
 * Formato de almacenamiento:
 * id:date:type:total:item_count:pid1,qty1,price1,name1;pid2,qty2,price2,name2;...
 *
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3
 */

#ifndef TICKET_H
#define TICKET_H

#include "utils.h"

#define TICKETS_FILE "data/tickets.txt"     ///< Archivo de persistencia
#define MAX_ITEMS 4                         ///< Máximo de items por ticket

/**
 * @brief Estructura que representa un item dentro de un ticket
 *
 * Almacena la información de un producto vendido/comprado
 * incluyendo precio unitario en el momento de la transacción.
 */
typedef struct {
    int product_id;       ///< ID del producto (referencia a inventory)
    int quantity;         ///< Cantidad de unidades
    double unit_price;    ///< Precio unitario al momento de la transacción
    char product_name[MAX_NAME]; ///< Nombre del producto (cache)
} TicketItem;

/**
 * @brief Estructura principal de ticket/transacción
 *
 * Representa una operación completa (venta o gasto).
 * El campo 'type' determina el tipo:
 *   - 'I': Ingreso (venta al cliente)
 *   - 'E': Egreso (gasto, devolución, compra a proveedor)
 *
 * La fecha se almacena en formato DDMMYY.
 */
typedef struct {
    int id;                    ///< Primary key (autoincrement)
    char date[8];              ///< Fecha en formato DDMMYY
    char type;                 ///< 'I' = Ingreso, 'E' = Egreso
    double total;              ///< Total de la transacción
    TicketItem items[MAX_ITEMS]; ///< Array de items (máximo 4)
    int item_count;            ///< Cantidad de items en el ticket
} Ticket;

/**
 * @brief Inicializa el módulo de tickets
 *
 * Llama a db_init() para cargar secuencias.
 * @return OP_SUCCESS
 */
int ticket_init(void);

/**
 * @brief Crea un nuevo ticket
 *
 * Calcula el total automáticamente a partir de los items.
 * Si el tipo es 'I' (ingreso), decrementa el stock.
 * Si el tipo es 'E' (egreso), incrementa el stock.
 *
 * @param t Puntero a Ticket con los datos (ID se asigna automáticamente)
 * @return OP_SUCCESS, OP_ERROR_INVALID_DATA, OP_ERROR_NO_STOCK o OP_ERROR_FILE_IO
 */
int ticket_create(Ticket *t);

/**
 * @brief Obtiene un ticket por su ID
 *
 * @param id ID del ticket a buscar
 * @param t Puntero a Ticket donde almacenar el resultado
 * @return OP_SUCCESS o OP_ERROR_NOT_FOUND
 */
int ticket_get_by_id(int id, Ticket *t);

/**
 * @brief Lista todos los tickets registrados
 *
 * @param tickets Array de Ticket para almacenar resultados
 * @param count Puntero a entero para almacenar cantidad
 * @return OP_SUCCESS o OP_ERROR_INVALID_DATA
 */
int ticket_list_all(Ticket tickets[], int *count);

/**
 * @brief Imprime un ticket por su ID en formato visual
 *
 * Muestra el ticket formateado para el cliente en la terminal.
 * @param id ID del ticket a imprimir
 * @return OP_SUCCESS o OP_ERROR_NOT_FOUND
 */
int ticket_print(int id);

/**
 * @brief Imprime un ticket desde estructura
 *
 * Versión que recibe directamente el struct Ticket.
 * Útil para imprimir inmediatamente después de crear.
 *
 * @param t Puntero a Ticket a imprimir
 * @return OP_SUCCESS o OP_ERROR_INVALID_DATA
 */
int ticket_print_full(Ticket *t);

/**
 * @brief Calcula el total de un ticket
 *
 * Suma quantity * unit_price de cada item.
 * @param t Puntero a Ticket
 * @return Total calculado
 */
double ticket_calculate_total(Ticket *t);

/**
 * @brief Obtiene resumen financiero de operaciones
 *
 * Calcula totales de ingresos y egresos, útil para reportes.
 *
 * @param total_ingresos Puntero a double para almacenar suma de ventas
 * @param total_egresos Puntero a double para almacenar suma de gastos
 * @param count_ingresos Puntero a int para cantidad de ventas
 * @param count_egresos Puntero a int para cantidad de gastos
 * @return OP_SUCCESS
 */
int ticket_get_summary(double *total_ingresos, double *total_egresos, 
                       int *count_ingresos, int *count_egresos);

#endif