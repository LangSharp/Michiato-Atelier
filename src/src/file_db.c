/**
 * @file file_db.c
 * @brief Implementación del sistema de base de datos basada en archivos
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3 - Ver archivo LICENSE
 */
#include "file_db.h"

static SequenceDB sequences = {1, 1};

/**
 * @brief Inicializa la base de datos
 * @details Lee sequences.txt si existe, sino lo crea con valores por defecto {1,1}
 * @return OP_SUCCESS siempre (crea archivo si no existe)
 */
int db_init(void) {
    FILE *f;
    char line[64];
    char line_copy[64];
    char *token;
    int inv_id = 1, tick_id = 1;

    f = fopen(DATA_PATH "/sequences.txt", "r");
    if (f != NULL) {
        if (fgets(line, sizeof(line), f) != NULL) {
            strncpy(line_copy, line, sizeof(line_copy)-1);
            line_copy[sizeof(line_copy)-1] = 0;
            token = strtok(line_copy, ":");
            if (token) inv_id = atoi(token);
            token = strtok(NULL, ":");
            if (token) tick_id = atoi(token);
        }
        fclose(f);
    }
    
    sequences.next_inventory_id = inv_id;
    sequences.next_ticket_id = tick_id;
    
    if (f == NULL) {
        db_save_sequences(&sequences);
    }
    return OP_SUCCESS;
}

SequenceDB* db_get_sequences(void) {
    return &sequences;
}

/**
 * @brief Guarda las secuencias en disco
 * @param seq Puntero a SequenceDB con los valores a guardar
 * @return OP_SUCCESS si se guardó, OP_ERROR_INVALID_DATA si seq es NULL
 */
int db_save_sequences(SequenceDB *seq) {
    FILE *f;
    if (seq == NULL) return OP_ERROR_INVALID_DATA;
    f = fopen(DATA_PATH "/sequences.txt", "w");
    if (f == NULL) return OP_ERROR_FILE_IO;
    fprintf(f, "%d:%d\n", seq->next_inventory_id, seq->next_ticket_id);
    fclose(f);
    sequences.next_inventory_id = seq->next_inventory_id;
    sequences.next_ticket_id = seq->next_ticket_id;
    return OP_SUCCESS;
}

/**
 * @brief Obtiene el siguiente ID disponible y lo incrementa automáticamente
 * @details También guarda el cambio en disco para persistencia
 * @param table_name "inventory" o "tickets"
 * @return ID asignado, o -1 si la tabla no existe
 */
int db_get_next_id(const char *table_name) {
    int ret = -1;
    if (strcmp(table_name, "inventory") == 0) {
        ret = sequences.next_inventory_id++;
    } else if (strcmp(table_name, "tickets") == 0) {
        ret = sequences.next_ticket_id++;
    }
    if (ret != -1) {
        db_save_sequences(&sequences);
    }
    return ret;
}

void db_set_next_id(const char *table_name, int id) {
    if (strcmp(table_name, "inventory") == 0) {
        if (id >= sequences.next_inventory_id) {
            sequences.next_inventory_id = id + 1;
        }
    } else if (strcmp(table_name, "tickets") == 0) {
        if (id >= sequences.next_ticket_id) {
            sequences.next_ticket_id = id + 1;
        }
    }
    db_save_sequences(&sequences);
}

/**
 * @brief Lee todos los registros de un archivo
 * @param filename Ruta completa del archivo
 * @param records Array bidimensional para guardar las líneas
 * @param count Puntero donde se devolverá la cantidad de registros
 * @return OP_SUCCESS siempre (si archivo no existe, count=0)
 */
int db_select_all(const char *filename, char records[][MAX_LINE_LENGTH], int *count) {
    FILE *f;
    char line[MAX_LINE_LENGTH];
    int i = 0;
    *count = 0;
    f = fopen(filename, "r");
    if (f == NULL) {
        return OP_SUCCESS;
    }
    while (fgets(line, sizeof(line), f) != NULL && i < MAX_RECORDS) {
        utils_trim(line);
        if (strlen(line) > 0) {
            strncpy(records[i], line, MAX_LINE_LENGTH - 1);
            records[i][MAX_LINE_LENGTH - 1] = 0;
            i++;
        }
    }
    fclose(f);
    *count = i;
    return OP_SUCCESS;
}

/**
 * @brief Busca un registro por ID (el ID debe ser el primer campo antes de ":")
 * @param filename Ruta del archivo
 * @param id ID a buscar
 * @param result Buffer donde se copiará la línea completa
 * @return OP_SUCCESS si se encontró, OP_ERROR_NOT_FOUND si no
 */
int db_select_by_id(const char *filename, int id, char *result) {
    FILE *f;
    char line[MAX_LINE_LENGTH];
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    int record_id;
    if (result == NULL) return OP_ERROR_INVALID_DATA;
    f = fopen(filename, "r");
    if (f == NULL) return OP_ERROR_NOT_FOUND;
    while (fgets(line, sizeof(line), f) != NULL) {
        utils_trim(line);
        if (strlen(line) == 0) continue;
        strncpy(line_copy, line, sizeof(line_copy)-1);
        line_copy[sizeof(line_copy)-1] = 0;
        token = strtok(line_copy, ":");
        if (token == NULL) continue;
        record_id = atoi(token);
        if (record_id == id) {
            strncpy(result, line, MAX_LINE_LENGTH - 1);
            result[MAX_LINE_LENGTH - 1] = 0;
            fclose(f);
            return OP_SUCCESS;
        }
    }
    fclose(f);
    return OP_ERROR_NOT_FOUND;
}

/**
 * @brief Inserta un registro al final del archivo (append)
 * @param filename Ruta del archivo
 * @param data Datos en formato "id:field1:field2:..."
 * @return OP_SUCCESS si se insertó, OP_ERROR_INVALID_DATA si data es NULL
 */
int db_insert(const char *filename, const char *data) {
    FILE *f;
    if (data == NULL) return OP_ERROR_INVALID_DATA;
    f = fopen(filename, "a");
    if (f == NULL) return OP_ERROR_FILE_IO;
    fprintf(f, "%s\n", data);
    fclose(f);
    return OP_SUCCESS;
}

/**
 * @brief Actualiza un registro existente buscando por ID
 * @details Crea un archivo temporal, reescribe todo, y reemplaza el original
 * @param filename Ruta del archivo
 * @param id ID del registro a actualizar
 * @param new_data Nuevos datos (debe incluir el ID al inicio)
 * @return OP_SUCCESS si se actualizó, OP_ERROR_NOT_FOUND si no existe
 */
int db_update(const char *filename, int id, const char *new_data) {
    FILE *f, *temp;
    char line[MAX_LINE_LENGTH];
    char temp_file[256];
    char *token;
    int record_id;
    int found = 0;
    if (new_data == NULL) return OP_ERROR_INVALID_DATA;
    snprintf(temp_file, sizeof(temp_file), "%s.tmp", filename);
    f = fopen(filename, "r");
    if (f == NULL) return OP_ERROR_FILE_IO;
    temp = fopen(temp_file, "w");
    if (temp == NULL) {
        fclose(f);
        return OP_ERROR_FILE_IO;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        utils_trim(line);
        if (strlen(line) == 0) {
            fprintf(temp, "\n");
            continue;
        }
        token = strtok(line, ":");
        if (token == NULL) {
            fprintf(temp, "%s\n", line);
            continue;
        }
        record_id = atoi(token);
        if (record_id == id) {
            fprintf(temp, "%s\n", new_data);
            found = 1;
        } else {
            fprintf(temp, "%s\n", line);
        }
    }
    fclose(f);
    fclose(temp);
    if (!found) {
        remove(temp_file);
        return OP_ERROR_NOT_FOUND;
    }
    remove(filename);
    rename(temp_file, filename);
    return OP_SUCCESS;
}

/**
 * @brief Elimina un registro por ID
 * @details Crea un archivo temporal sin el registro, y reemplaza el original
 * @param filename Ruta del archivo
 * @param id ID del registro a eliminar
 * @return OP_SUCCESS si se eliminó, OP_ERROR_NOT_FOUND si no existe
 */
int db_delete(const char *filename, int id) {
    FILE *f, *temp;
    char line[MAX_LINE_LENGTH];
    char temp_file[256];
    char *token;
    int record_id;
    int found = 0;
    snprintf(temp_file, sizeof(temp_file), "%s.tmp", filename);
    f = fopen(filename, "r");
    if (f == NULL) return OP_ERROR_FILE_IO;
    temp = fopen(temp_file, "w");
    if (temp == NULL) {
        fclose(f);
        return OP_ERROR_FILE_IO;
    }
    while (fgets(line, sizeof(line), f) != NULL) {
        utils_trim(line);
        if (strlen(line) == 0) {
            fprintf(temp, "\n");
            continue;
        }
        token = strtok(line, ":");
        if (token == NULL) {
            fprintf(temp, "%s\n", line);
            continue;
        }
        record_id = atoi(token);
        if (record_id == id) {
            found = 1;
        } else {
            fprintf(temp, "%s\n", line);
        }
    }
    fclose(f);
    fclose(temp);
    if (!found) {
        remove(temp_file);
        return OP_ERROR_NOT_FOUND;
    }
    remove(filename);
    rename(temp_file, filename);
    return OP_SUCCESS;
}

int db_exists(const char *filename, int id) {
    char result[MAX_LINE_LENGTH];
    return db_select_by_id(filename, id, result) == OP_SUCCESS;
}
