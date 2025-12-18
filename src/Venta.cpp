#include "Venta.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif

using namespace std;

// Contador simple para generar IDs de venta
static int venta_id_counter = 1;

static void ensure_data_dir() {
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0755);
#endif
}

int Venta::registrarVenta(Producto* p, int cantidad) {
    // Asegurar que exista la carpeta data
    ensure_data_dir();

    int id = venta_id_counter++;
    float total = p->getPrecio() * cantidad;

    // timestamp simple
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    ofstream file("data/ventas.txt", ios::app);
    if(file.is_open()) {
        file << id << "|" << p->getNombre() << "|" 
             << cantidad << "|" << total << "|" << std::ctime(&t);
        file.close();
    }

    p->descontar(cantidad);
    cout << "Venta registrada (ID " << id << "). Total: S/." << total << endl;
    return id;
}