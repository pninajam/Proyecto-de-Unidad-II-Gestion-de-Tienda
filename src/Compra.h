#ifndef COMPRA_H
#define COMPRA_H

#include "Producto.h"

class Compra {
public:
    static void registrarCompra(Producto* p, int cantidad, float precioCompra);
};

#endif