/**
 * @file utils.h
 * @brief Utilidades generales del sistema
 *
 * Proporciona funciones auxiliares para:
 * - Entrada/salida de datos por terminal
 * - Manipulación de cadenas
 * - Manejo de fechas
 * - Interfaz de usuario (menús, separadores)
 *
 * @author Luis Armando Fernandez Chavez
 * @license GPL v3
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_NAME 64        ///< Longitud máxima para nombres de producto
#define MAX_DESC 128       ///< Longitud máxima para descripción
#define MAX_LINE 512       ///< Longitud máxima de línea leída
#define DATE_FORMAT_LEN 8  ///< Tamaño para fecha DDMMYY + null

/**
 * @brief Códigos de resultado para operaciones
 *
 * Utilizados por todos los módulos para indicar éxito o tipo de error
 */
typedef enum {
    OP_SUCCESS = 0,           ///< Operación exitosa
    OP_ERROR_NOT_FOUND,       ///< Registro no encontrado
    OP_ERROR_DUPLICATE_KEY,   ///< Clave primaria duplicada
    OP_ERROR_INVALID_DATA,    ///< Datos inválidos proporcionados
    OP_ERROR_NO_STOCK,        ///< Stock insuficiente
    OP_ERROR_FILE_IO,         ///< Error de lectura/escritura
    OP_ERROR_FULL,            ///< Estructura llena
    OP_ERROR_INVALID_ID       ///< ID inválido
} OperationResult;

/**
 * @brief Elimina espacios en blanco al inicio y final de una cadena
 * @param str Cadena a modificar (in-place)
 */
void utils_trim(char *str);

/**
 * @brief Obtiene la fecha actual en formato DDMMYY
 * @param date_buffer Buffer de salida (debe tener DATE_FORMAT_LEN bytes)
 */
void utils_get_current_date(char *date_buffer);

/**
 * @brief Lee un entero desde stdin con prompt
 * @param prompt Mensaje a mostrar al usuario
 * @return Entero leído
 */
int utils_get_int_input(const char *prompt);

/**
 * @brief Lee un double desde stdin con prompt
 * @param prompt Mensaje a mostrar al usuario
 * @return Double leído
 */
double utils_get_double_input(const char *prompt);

/**
 * @brief Lee una cadena desde stdin con prompt
 * @param prompt Mensaje a mostrar al usuario
 * @param buffer Buffer de salida
 * @param max_len Tamaño máximo del buffer
 */
void utils_get_string_input(const char *prompt, char *buffer, size_t max_len);

/**
 * @brief Solicita confirmación sí/no al usuario
 * @param message Mensaje de confirmación
 * @return 1 si el usuario respondió 's' o 'S', 0 en caso contrario
 */
int utils_confirm(const char *message);

/**
 * @brief Pausa la ejecución hasta que el usuario presione Enter
 */
void utils_pause(void);

/**
 * @brief Limpia la terminal
 */
void utils_clear_screen(void);

/**
 * @brief Imprime un encabezado centrado con separadores
 * @param title Título a mostrar
 */
void utils_print_header(const char *title);

/**
 * @brief Imprime un separador visual (40 caracteres '=')
 */
void utils_print_separator(void);

#endif