#include "Producto.h"
#include <iostream>

// Colores ANSI simples (si la terminal los soporta)
static const std::string COL_RESET = "\033[0m";
static const std::string COL_CYAN = "\033[36m";
static const std::string COL_YELLOW = "\033[33m";
static const std::string COL_GREEN = "\033[32m";
static const std::string COL_RED = "\033[31m";

Producto::Producto(string nombre, int stock, float precio, int stockCritico) {
    this->nombre = nombre;
    this->stock = stock;
    this->precio = precio;
    this->stockCritico = stockCritico;
}

string Producto::getNombre() {
    return nombre;
}

int Producto::getStock() {
    return stock;
}

float Producto::getPrecio() {
    return precio;
}

int Producto::getStockCritico() {
    return stockCritico;
}

void Producto::setStockCritico(int sc) {
    stockCritico = sc;
}

void Producto::descontar(int cantidad) {
    stock -= cantidad;
}

void Producto::aumentarStock(int cantidad) {
    stock += cantidad;
}

void Producto::mostrar() {
    std::string stockColor = (stock <= stockCritico) ? COL_RED : COL_YELLOW;
    cout << COL_CYAN << nombre << COL_RESET
         << " | Stock: " << stockColor << stock << COL_RESET
         << " | Precio: " << COL_GREEN << "S/." << precio << COL_RESET
         << " | Critico: " << COL_RED << stockCritico << COL_RESET
         << endl;
}
