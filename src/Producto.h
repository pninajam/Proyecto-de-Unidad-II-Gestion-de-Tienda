#ifndef PRODUCTO_H
#define PRODUCTO_H

#include <string>
using namespace std;

class Producto {
private:
    string nombre;
    int stock;
    float precio;
    int stockCritico;

public:
    Producto(string nombre, int stock, float precio, int stockCritico = 0);
    string getNombre();
    int getStock();
    float getPrecio();
    int getStockCritico();
    void setStockCritico(int sc);
    void descontar(int cantidad);
    void aumentarStock(int cantidad);
    void mostrar();
};
#endif