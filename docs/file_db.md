/**
 * @file file_db.h
 * @brief Capa de acceso a datos (emulación de base de datos)
 *
 * Proporciona una abstracción de base de datos utilizando fiche-
 * ros de texto plano con campos delimitados por ':'.
 *
 * Características:
 * - Primary keys autoincrement mediante secuencia
 * - CRUD completo (Create, Read, Update, Delete)
 * - Formato: id:campo1:campo2:...
 *
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3
 */

#ifndef FILE_DB_H
#define FILE_DB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define DATA_PATH "data"                    ///< Directorio de datos
#define MAX_RECORDS 1000                    ///< Máximo de registros en memoria
#define MAX_LINE_LENGTH 1024                ///< Longitud máxima de línea

/**
 * @brief Estructura para almacenar un registro genérico
 *
 * Se utiliza para transportar datos entre la capa de archivo
 * y las capas de negocio.
 */
typedef struct {
    int id;                 ///< Primary key del registro
    char data[MAX_LINE_LENGTH]; ///< Datos completos de la línea
} Record;

/**
 * @brief Estructura para gestionar contadores autoincrement
 *
 * Mantiene los IDs siguientes para cada tabla, permitiendo
 * generar primary keys únicas sin colisiones.
 */
typedef struct {
    int next_inventory_id;  ///< Próximo ID para inventory
    int next_ticket_id;     ///< Próximo ID para tickets
} SequenceDB;

/**
 * @brief Inicializa el sistema de base de datos
 *
 * Lee los contadores actuales desde sequences.txt.
 * Si el archivo no existe, lo crea con valores por defecto.
 * @return OP_SUCCESS siempre
 */
int db_init(void);

/**
 * @brief Obtiene los contadores actuales
 * @return Puntero a SequenceDB con los contadores
 */
SequenceDB* db_get_sequences(void);

/**
 * @brief Persiste los contadores al archivo
 * @param seq Puntero a SequenceDB con los nuevos valores
 * @return OP_SUCCESS o OP_ERROR_FILE_IO
 */
int db_save_sequences(SequenceDB *seq);

/**
 * @brief Obtiene el siguiente ID para una tabla
 *
 * Incrementa automáticamente el contador interno.
 * @param table_name Nombre de la tabla ("inventory" o "tickets")
 * @return siguiente ID, o -1 si la tabla no existe
 */
int db_get_next_id(const char *table_name);

/**
 * @brief Establece el siguiente ID para una tabla
 *
 * Útil para reconstruir índices o importar datos.
 * @param table_name Nombre de la tabla
 * @param id Nuevo valor del contador
 */
void db_set_next_id(const char *table_name, int id);

/**
 * @brief Lee todos los registros de un archivo
 *
 * Lee línea por línea y almacena en array.
 * @param filename Ruta del archivo
 * @param records Array de salida (debe tener MAX_RECORDS)
 * @param count Puntero a entero para almacenar cantidad de registros
 * @return OP_SUCCESS siempre
 */
int db_select_all(const char *filename, char records[][MAX_LINE_LENGTH], int *count);

/**
 * @brief Busca un registro por su ID
 *
 * @param filename Ruta del archivo
 * @param id ID a buscar
 * @param result Buffer de salida para la línea encontrada
 * @return OP_SUCCESS si se encuentra, OP_ERROR_NOT_FOUND si no
 */
int db_select_by_id(const char *filename, int id, char *result);

/**
 * @brief Inserta un nuevo registro
 *
 * @param filename Ruta del archivo
 * @param data Cadena con formato id:campo1:campo2:...
 * @return OP_SUCCESS o OP_ERROR_FILE_IO
 */
int db_insert(const char *filename, const char *data);

/**
 * @brief Actualiza un registro existente
 *
 * @param filename Ruta del archivo
 * @param id ID del registro a actualizar
 * @param new_data Nuevos datos (formato completo)
 * @return OP_SUCCESS, OP_ERROR_NOT_FOUND o OP_ERROR_FILE_IO
 */
int db_update(const char *filename, int id, const char *new_data);

/**
 * @brief Elimina un registro por su ID
 *
 * @param filename Ruta del archivo
 * @param id ID del registro a eliminar
 * @return OP_SUCCESS, OP_ERROR_NOT_FOUND o OP_ERROR_FILE_IO
 */
int db_delete(const char *filename, int id);

/**
 * @brief Verifica si existe un registro con el ID dado
 *
 * @param filename Ruta del archivo
 * @param id ID a verificar
 * @return 1 si existe, 0 si no
 */
int db_exists(const char *filename, int id);

#endif