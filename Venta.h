#ifndef VENTA_H
#define VENTA_H

#include "Producto.h"
#include <vector>

class Venta {
public:
    static int registrarVenta(Producto* p, int cantidad);
};

#endif