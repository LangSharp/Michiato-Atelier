/**
 * @file inventory.h
 * @brief Gestión del inventario de productos
 *
 * Módulo responsable de mantener el catálogo de productos del bazar.
 * Proporciona operaciones CRUD sobre el inventario con validación
 * de datos y gestión de stock.
 *
 * Formato de almacenamiento:
 * id:name:description:price:quantity
 *
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "utils.h"

#define INVENTORY_FILE "data/inventory.txt"  ///< Archivo de persistencia

/**
 * @brief Estructura que representa un producto en el inventario
 *
 * Almacena la información completa de un producto disponible
 * para venta en el bazar.
 */
typedef struct {
    int id;                ///< Primary key (autoincrement)
    char name[MAX_NAME];   ///< Nombre del producto
    char description[MAX_DESC]; ///< Descripción detallada
    double price;          ///< Precio unitario
    int quantity;          ///< Cantidad en stock
} Product;

/**
 * @brief Inicializa el módulo de inventario
 *
 * Llama a db_init() para cargar secuencias.
 * @return OP_SUCCESS
 */
int inventory_init(void);

/**
 * @brief Agrega un nuevo producto al inventario
 *
 * Genera automáticamente el ID usando el contador interno.
 * @param p Puntero a Product con los datos (ID se asigna automáticamente)
 * @return OP_SUCCESS o OP_ERROR_FILE_IO
 */
int inventory_add(Product *p);

/**
 * @brief Edita un producto existente
 *
 * Solo actualiza los campos que no estén vacíos o sean mayores a 0.
 * @param id ID del producto a editar
 * @param p Puntero a Product con los nuevos datos
 * @return OP_SUCCESS, OP_ERROR_NOT_FOUND o OP_ERROR_FILE_IO
 */
int inventory_edit(int id, Product *p);

/**
 * @brief Elimina un producto del inventario
 *
 * @param id ID del producto a eliminar
 * @return OP_SUCCESS o OP_ERROR_NOT_FOUND
 */
int inventory_delete(int id);

/**
 * @brief Obtiene un producto por su ID
 *
 * @param id ID del producto a buscar
 * @param p Puntero a Product donde almacenar el resultado
 * @return OP_SUCCESS o OP_ERROR_NOT_FOUND
 */
int inventory_get_by_id(int id, Product *p);

/**
 * @brief Lista todos los productos del inventario
 *
 * @param products Array de Product para almacenar resultados
 * @param count Puntero a entero para almacenar cantidad
 * @return OP_SUCCESS o OP_ERROR_INVALID_DATA
 */
int inventory_list_all(Product products[], int *count);

/**
 * @brief Actualiza el stock de un producto
 *
 * Incrementa o decrementa la cantidad disponible.
 * No permite stock negativo.
 * @param id ID del producto
 * @param quantity_change Cambio a aplicar (positivo o negativo)
 * @return OP_SUCCESS, OP_ERROR_NOT_FOUND o OP_ERROR_NO_STOCK
 */
int inventory_update_stock(int id, int quantity_change);

/**
 * @brief Obtiene la cantidad actual en stock
 *
 * @param id ID del producto
 * @return Cantidad en stock, o -1 si no existe
 */
int inventory_get_stock(int id);

#endif