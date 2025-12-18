#include "Compra.h"
#include <fstream>
#include <iostream>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

using namespace std;

static void ensure_data_dir() {
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0755);
#endif
}

void Compra::registrarCompra(Producto* p, int cantidad, float precioCompra) {
    ensure_data_dir();

    float total = cantidad * precioCompra;

    ofstream file("data/compras.txt", ios::app);
    if(file.is_open()) {
        file << p->getNombre() << "|" << cantidad << "|" << precioCompra << "|" << total << endl;
        file.close();
    }

    // Aumentar stock
    p->aumentarStock(cantidad);
    cout << "Compra registrada. Stock aumentado en " << cantidad << ". Costo: S/." << total << endl;
}