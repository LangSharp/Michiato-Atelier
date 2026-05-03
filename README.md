# Michiato Atelier

CLI para gestión de bazar - Inventario, ventas y tickets.

**Nota**: Todos los datos se almacenan en memoria RAM (temporales, se pierden al cerrar el programa).

## Autor
Luis Armando Fernandez Chavez

## Licencia
GPL v3 - Ver archivo LICENSE

## Compilación y Ejecución
```bash
make
./bin/michiato
```
O simplemente: `make run`

## Menú Principal
1. Gestión de Inventario
2. Registro de Venta
3. Generar/Ver Ticket
4. Salir

## Tipos Estructurados
| Struct | Descripción |
|--------|-------------|
| `Product` | Producto (id, name, description, price, quantity) |
| `TicketItem` | Item de transacción (product_id, quantity, unit_price, product_name) |
| `Ticket` | Ticket/Registro (id, date, type, customer_name, total, items[], item_count) |

## Vista de Ticket

Ejemplo de ticket generado:

```
========================================
  MICHIATO ATELIER
  Bazar y Regalos
========================================
Ticket #: 1
Fecha:    020526
Tipo:     INGRESO (Venta)
Cliente:  Juan Perez
========================================
Camiseta x2
  $150.00 c/u               $300.00
Gorra x1
  $80.00 c/u                $80.00
========================================
TOTAL:                    $380.00
========================================
      Gracias por su compra!
```

## Características
- Almacenamiento 100% en memoria RAM (sin archivos)
- IDs autoincrementales que se mantienen entre operaciones
- Validación de stock en ventas
- Resumen de ingresos/egresos
- Estructuras de datos:
  - Listas (arrays) para inventario y tickets
  - Stacks para gestión de items de ticket
- Registro de cliente requerido para ventas
- Testing automatizado con `make test`
