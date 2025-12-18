//Gestion de Tienda de Abarrotes Camila
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#ifdef _WIN32
#include <direct.h>
#else
#include <sys/stat.h>
#endif
#include "Producto.h"
#include "Venta.h"
#include "Compra.h"

using namespace std;

vector<Producto*> inventario;

// Forward declarations for section handlers
void sectionProductsAndPurchases();
void sectionAddRemove();
void sectionSell();
void sectionStocks();

// Colores ANSI
static const string C_RESET = "\033[0m";
static const string C_BOLD = "\033[1m";
static const string C_RED = "\033[31m";
static const string C_GREEN = "\033[32m";
static const string C_YELLOW = "\033[33m";
static const string C_CYAN = "\033[36m";
static const string C_MAGENTA = "\033[35m";

static void printBoxHeader(const string &title) {
    string line(50, '-');
    cout << C_MAGENTA << "+" << line << "+" << C_RESET << "\n";
    cout << C_MAGENTA << "| " << C_RESET << C_BOLD << title << C_RESET;
    int pad = 50 - (int)title.size();
    for(int i=0;i<pad;i++) cout << ' ';
    cout << C_MAGENTA << " |" << C_RESET << "\n";
    cout << C_MAGENTA << "+" << line << "+" << C_RESET << "\n";
}

static void printMenuOption(int idx, const string &text) {
    cout << C_YELLOW << idx << "." << C_RESET << " " << text << "\n";
}
// Espera portable a que el usuario presione Enter
static void waitForKey() {
    cout << "Presiona Enter para continuar...";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.get();
}

// Menú no interactivo: elegir por número
static int interactiveMenu(const vector<string> &options) {
    for(size_t i=0;i<options.size();i++) printMenuOption((int)i+1, options[i]);
    cout << C_CYAN << "Opcion: " << C_RESET;
    int sel;
    if(!(cin >> sel)) {
        cin.clear();
        string dummy; getline(cin, dummy);
        return 1;
    }
    return sel;
}

static void ensure_data_dir() {
#ifdef _WIN32
    _mkdir("data");
#else
    mkdir("data", 0755);
#endif
}

void guardarInventario() {
    ensure_data_dir();
    std::ofstream f("data/inventario.txt");
    if(!f.is_open()) return;
    for(auto p : inventario) {
        f << p->getNombre() << "|" << p->getStock() << "|" << p->getPrecio() << "|" << p->getStockCritico() << "\n";
    }
    f.close();
}

void cargarInventario() {
    std::ifstream f("data/inventario.txt");
    if(!f.is_open()) {
        // Si no existe, crear inventario por defecto y guardar (con stock critico)
        inventario.push_back(new Producto("Arroz", 50, 4.5, 10));
        inventario.push_back(new Producto("Azucar", 30, 3.8, 5));
        inventario.push_back(new Producto("Aceite", 20, 8.0, 5));
        guardarInventario();
        return;
    }

    string line;
    while(getline(f, line)) {
        if(line.empty()) continue;
        std::stringstream ss(line);
        string nombre;
        string stock_s;
        string precio_s;
        string crit_s;
        if(!getline(ss, nombre, '|')) continue;
        if(!getline(ss, stock_s, '|')) continue;
        if(!getline(ss, precio_s, '|')) continue;
        int stock = stoi(stock_s);
        float precio = stof(precio_s);
        int crit = 5; // valor por defecto
        if(getline(ss, crit_s, '|')) {
            try { crit = stoi(crit_s); } catch(...) { crit = 5; }
        }
        inventario.push_back(new Producto(nombre, stock, precio, crit));
    }
    f.close();
}

int main() {
    cargarInventario();
    int op;

    do {
        cout << "\n";
        printBoxHeader("TIENDA CAMILA");
        vector<string> opts = {
            "Productos y Compras",
            "Agregar/Editar/Eliminar Productos",
            "Vender",
            "Stocks",
            "Salir"
        };
        op = interactiveMenu(opts);

        if(op == 1) {
            sectionProductsAndPurchases();
        }
        else if(op == 2) {
            sectionAddRemove();
        }
        else if(op == 3) {
            sectionSell();
        }
        else if(op == 4) {
            sectionStocks();
        }

    } while(op != 5);

    return 0;
}

// ----- Secciones del menú (handlers) -----
void sectionProductsAndPurchases() {
    vector<string> opts = {"Ver Productos", "Comprar Producto (tienda)", "Volver"};
    int sel;
    do {
        sel = interactiveMenu(opts);
        if(sel == 1) {
            for(auto p : inventario) p->mostrar();
            waitForKey();
        } else if(sel == 2) {
            int idx2, cant2; float precioCompra;
            for(int i=0;i<inventario.size();i++){ cout<<i+1<<". "; inventario[i]->mostrar(); }
            cout<<"Seleccione producto a comprar: "; cin>>idx2;
            cout<<"Cantidad a comprar: "; cin>>cant2;
            cout<<"Precio por unidad de compra: "; cin>>precioCompra;
            if(idx2>0 && idx2<=inventario.size()) {
                Compra::registrarCompra(inventario[idx2-1], cant2, precioCompra);
                guardarInventario();
            } else cout<<"Producto invalido.\n";
        }
    } while(sel != 3);
}

void sectionAddRemove() {
    vector<string> opts = {"Agregar Producto", "Editar Producto", "Eliminar Producto", "Volver"};
    int sel;
    do {
        sel = interactiveMenu(opts);
        if(sel == 1) {
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            string nombre; int stock; float precio; int critico;
            cout<<"Nombre del producto: "; getline(cin, nombre);
            cout<<"Stock inicial: "; cin>>stock;
            cout<<"Precio: "; cin>>precio;
            cout<<"Stock critico: "; cin>>critico;
            inventario.push_back(new Producto(nombre, stock, precio, critico));
            guardarInventario(); cout<<"Producto agregado.\n";
        }
        else if(sel == 2) {
            // Editar producto
            int idxe;
            for(int i=0;i<inventario.size();i++){ cout<<i+1<<". "; inventario[i]->mostrar(); }
            cout<<"Seleccione producto a editar: "; cin>>idxe;
            if(idxe>0 && idxe<=inventario.size()){
                // limpiar resto de linea
                cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                Producto* p = inventario[idxe-1];
                cout<<"Dejar campo vacio y presionar Enter mantiene el valor actual.\n";
                cout<<"Nombre actual: "<<p->getNombre()<<"\n";
                cout<<"Nuevo nombre: ";
                string line;
                getline(cin, line);
                if(!line.empty()) {
                    // reemplazar nombre: crear temporal y reasignar
                    string newName = line;
                    // No setter en Producto para nombre; recreate object preserving other fields
                    int curStock = p->getStock();
                    float curPrecio = p->getPrecio();
                    int curCrit = p->getStockCritico();
                    delete p;
                    inventario[idxe-1] = new Producto(newName, curStock, curPrecio, curCrit);
                    p = inventario[idxe-1];
                }
                cout<<"Stock actual: "<<p->getStock()<<"\n";
                cout<<"Nuevo stock (Enter para mantener): ";
                getline(cin, line);
                if(!line.empty()){
                    try{ int ns = stoi(line); p->aumentarStock(ns - p->getStock()); } catch(...){}
                }
                cout<<"Precio actual: "<<p->getPrecio()<<"\n";
                cout<<"Nuevo precio (Enter para mantener): ";
                getline(cin, line);
                if(!line.empty()){
                    try{ float np = stof(line); // no setter for price, recreate object
                        int cs = p->getStock(); int cc = p->getStockCritico(); string nm = p->getNombre();
                        delete p; inventario[idxe-1] = new Producto(nm, cs, np, cc); p = inventario[idxe-1];
                    } catch(...){}
                }
                cout<<"Stock critico actual: "<<p->getStockCritico()<<"\n";
                cout<<"Nuevo stock critico (Enter para mantener): ";
                getline(cin, line);
                if(!line.empty()){
                    try{ int nsc = stoi(line); p->setStockCritico(nsc); } catch(...){}
                }
                guardarInventario(); cout<<"Producto actualizado.\n";
            } else cout<<"Producto invalido.\n";
        }
        else if(sel == 3) {
            int idxd;
            for(int i=0;i<inventario.size();i++){ cout<<i+1<<". "; inventario[i]->mostrar(); }
            cout<<"Seleccione producto a eliminar: "; cin>>idxd;
            if(idxd>0 && idxd<=inventario.size()){
                delete inventario[idxd-1];
                inventario.erase(inventario.begin() + (idxd-1));
                guardarInventario(); cout<<"Producto eliminado.\n";
            } else cout<<"Producto invalido.\n";
        }
    } while(sel != 4);
}

void sectionSell() {
    vector<string> opts = {"Vender Producto", "Ver Productos", "Volver"};
    int sel;
    do {
        sel = interactiveMenu(opts);
        if(sel == 1) {
            int idx, cant;
            for(int i=0;i<inventario.size();i++){ cout<<i+1<<". "; inventario[i]->mostrar(); }
            cout<<"Seleccione producto: "; cin>>idx;
            cout<<"Cantidad: "; cin>>cant;
            if(idx>0 && idx<=inventario.size() && inventario[idx-1]->getStock()>=cant){
                int ventaId = Venta::registrarVenta(inventario[idx-1], cant);
                cout<<"ID de venta: "<<ventaId<<"\n";
                guardarInventario();
            } else cout<<"Stock insuficiente.\n";
        } else if(sel == 2) {
            for(auto p: inventario) p->mostrar();
            waitForKey();
        }
    } while(sel != 3);
}

void sectionStocks() {
    vector<string> opts = {"Ver stock critico", "Ver todos los stocks", "Volver"};
    int sel;
    do {
        sel = interactiveMenu(opts);
        if(sel == 1) {
            cout<<"-- Productos con stock critico (<= critico) --\n";
            for(auto p: inventario) if(p->getStock()<=p->getStockCritico()) p->mostrar();
            waitForKey();
        } else if(sel == 2) {
            for(auto p: inventario) p->mostrar();
            waitForKey();
        }
    } while(sel != 3);
}
