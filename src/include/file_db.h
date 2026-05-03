/**
 * @file file_db.h
 * @brief Sistema de base de datos basada en archivos de texto plano
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3 - Ver archivo LICENSE
 */
#ifndef FILE_DB_H
#define FILE_DB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define DATA_PATH "data"
#define MAX_RECORDS 1000
#define MAX_LINE_LENGTH 1024

/**
 * @brief Estructura para almacenar un registro genérico
 * @param id Identificador único del registro
 * @param data Línea completa de datos en formato texto
 */
typedef struct {
    int id;
    char data[MAX_LINE_LENGTH];
} Record;

/**
 * @brief Estructura para control de IDs autoincrementales
 * @param next_inventory_id Siguiente ID para inventario
 * @param next_ticket_id Siguiente ID para tickets
 */
typedef struct {
    int next_inventory_id;
    int next_ticket_id;
} SequenceDB;

int db_init(void);
SequenceDB* db_get_sequences(void);
int db_save_sequences(SequenceDB *seq);
int db_get_next_id(const char *table_name);
void db_set_next_id(const char *table_name, int id);
int db_select_all(const char *filename, char records[][MAX_LINE_LENGTH], int *count);
int db_select_by_id(const char *filename, int id, char *result);
int db_insert(const char *filename, const char *data);
int db_update(const char *filename, int id, const char *new_data);
int db_delete(const char *filename, int id);
int db_exists(const char *filename, int id);

#endif
