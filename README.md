# Michiato Atelier

CLI para gestión de bazar - Inventario, ventas y tickets.

**Nota**: Los datos se almacenan en archivos de texto plano (data/inventory.txt, data/tickets.txt).

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

## Formato de Datos
- **inventory.txt**: `id:name:description:price:quantity`
- **tickets.txt**: `id:date:type:customer_name:total:item_count:items...`
- **sequences.txt**: `next_inventory_id:next_ticket_id`

## Características
- Almacenamiento persistente en archivos de texto (separados por ":")
- IDs autoincrementales con persistencia en disco
- Validación de stock en ventas
- Registro de cliente requerido para ventas
- Resumen de ingresos/egresos
- Testing automatizado con `make test`
- Interfaz CLI con ASCII art personalizado
