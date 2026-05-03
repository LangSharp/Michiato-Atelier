# Documentación Técnica - Michiato Atelier

## Visión General

Michiato Atelier es un sistema CLI para gestión de bazar desarrollado en C.
No utiliza base de datos, toda la información se almacena en fiche-
ros de texto plano con formato delimitado por `:`.

## Arquitectura

```
src/
├── main.c           # Punto de entrada y menús
├── include/         # Archivos de cabecera (.h)
│   ├── utils.h      # Utilidades generales
│   ├── file_db.h    # Capa de acceso a datos
│   ├── inventory.h  # Gestión de inventario
│   └── ticket.h     # Gestión de tickets
└── src/             # Implementaciones (.c)
    ├── utils.c
    ├── file_db.c
    ├── inventory.c
    └── ticket.c
```

## Flujo de Datos

```
Usuario → main.c (menú)
            ↓
    ┌───────┴────────┐
    ↓                ↓
inventory.c    ticket.c
    ↓                ↓
file_db.c ←────┘
    ↓
data/*.txt (persistencia)
```

## Módulos

### 1. utils.h / utils.c
Funciones auxiliares:
- Entrada/salida (utils_get_int_input, utils_get_double_input, etc.)
- Manejo de cadenas (utils_trim)
- Fechas (utils_get_current_date)
- Interfaz (utils_clear_screen, utils_print_header)

### 2. file_db.h / file_db.c
Capa de abstracción de datos:
- `db_init()`: Inicializa contadores
- `db_select_all()`: Lee todos los registros
- `db_select_by_id()`: Busca por ID
- `db_insert()`: Crea registro
- `db_update()`: Actualiza registro
- `db_delete()`: Elimina registro

### 3. inventory.h / inventory.c
Gestión de productos:
- `inventory_add()`: Alta de producto
- `inventory_edit()`: Modificación
- `inventory_delete()`: Baja
- `inventory_list_all()`: Listado
- `inventory_update_stock()`: Ajuste de stock

### 4. ticket.h / ticket.c
Gestión de transacciones:
- `ticket_create()`: Nueva venta/gasto
- `ticket_get_by_id()`: Consulta ticket
- `ticket_print()`: Impresión formateada
- `ticket_get_summary()`: Reporte financiero

## Tipos de Datos

### Product
```c
typedef struct {
    int id;
    char name[MAX_NAME];
    char description[MAX_DESC];
    double price;
    int quantity;
} Product;
```

### TicketItem
```c
typedef struct {
    int product_id;
    int quantity;
    double unit_price;
    char product_name[MAX_NAME];
} TicketItem;
```

### Ticket
```c
typedef struct {
    int id;
    char date[8];
    char type;  // 'I' = Ingreso, 'E' = Egreso
    double total;
    TicketItem items[MAX_ITEMS];
    int item_count;
} Ticket;
```

## Formato de Archivos

### inventory.txt
```
id:name:description:price:quantity
1:Veladora Lavender:Aroma relajante:45.50:20
2:Veladora Vainilla:Vainilla natural:45.50:15
```

### tickets.txt
```
id:date:type:total:item_count:pid,qty,price,name;...
1:200426:I:136.50:2:1,2,45.50,Veladora Lavender;2,1,45.50,Veladora Vainilla
```

### sequences.txt
```
inventory_next_id:ticket_next_id
3:2
```

## Constantes

| Constante | Valor | Descripción |
|-----------|-------|-------------|
| MAX_NAME | 64 | Longitud máxima de nombre |
| MAX_DESC | 128 | Longitud máxima de descripción |
| MAX_LINE | 512 | Longitud máxima de línea |
| MAX_RECORDS | 1000 | Máximo de registros en memoria |
| MAX_LINE_LENGTH | 1024 | Longitud máxima de línea en BD |
| MAX_ITEMS | 4 | Máximo de items por ticket |
| DATE_FORMAT_LEN | 8 | Tamaño para fecha DDMMYY |

## Códigos de Error

| Código | Significado |
|--------|-------------|
| OP_SUCCESS | Operación exitosa |
| OP_ERROR_NOT_FOUND | Registro no encontrado |
| OP_ERROR_DUPLICATE_KEY | Clave duplicada |
| OP_ERROR_INVALID_DATA | Datos inválidos |
| OP_ERROR_NO_STOCK | Stock insuficiente |
| OP_ERROR_FILE_IO | Error de archivo |
| OP_ERROR_FULL | Estructura llena |
| OP_ERROR_INVALID_ID | ID inválido |

## Compilación

```bash
make        # Compilar
make run    # Ejecutar
make clean  # Limpiar binarios
```

## Licencia

GPL v3 - Ver archivo LICENSE

## Autor

Luis Armando Fernandez Chavez