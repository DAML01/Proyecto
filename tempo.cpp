#include <iostream>
#include <string>
#include <fstream> // Para manejo de archivos
#include <ctime>   // Para usar std::clock
#include <cstdlib> // Para la función exit()

using namespace std;

// Función auxiliar para manejar el temporizador
bool TemporizadorInactividad() {
    clock_t inicio = clock();
    string respuesta;

    while (true) {
        // Comprueba si han pasado más de 5 segundos
        if (static_cast<double>(clock() - inicio) / CLOCKS_PER_SEC >= 5) {
            cout << "\nHan pasado 5 segundos sin actividad.\n";
            cout << "¿Desea permanecer en el programa? (si/no): ";
            cin >> respuesta;

            if (respuesta == "no") {
                cout << "Saliendo del programa...\n";
                exit(0); // Finaliza el programa
            } else if (respuesta == "si") {
                cout << "El temporizador se ha reiniciado.\n";
                return true; // El usuario decide continuar
            } else {
                cout << "Respuesta no válida. Intente de nuevo.\n";
                inicio = clock(); // Reinicia el temporizador
                return false; // Reinicia el temporizador y vuelve a mostrar el menú de usuario
            }
        }

        // Pausa para evitar sobrecargar la CPU
        for (volatile int i = 0; i < 100000; ++i) {} // Pausa activa mínima
    }
}

// Función para registrar movimientos en un archivo de texto
void RegistrarMovimiento(const string &tipo, const string &usuario, float monto = 0) {
    ofstream archivo("movimientos.txt", ios::app);
    if (archivo.is_open()) {
        archivo << "Tipo: " << tipo << ", Usuario: " << usuario;
        if (monto > 0) {
            archivo << ", Monto: $" << monto;
        }
        archivo << endl;
        archivo.close();
    } else {
        cout << "Error al registrar el movimiento.\n";
    }
}

// Función para guardar el saldo de un usuario en su archivo específico
void GuardarSaldo(const string &usuario, float saldo) {
    ofstream archivo(usuario + "_saldo.txt");
    if (archivo.is_open()) {
        archivo << saldo;
        archivo.close();
    } else {
        cout << "Error al guardar el saldo del usuario.\n";
    }
}

// Función para cargar el saldo de un usuario desde su archivo
float CargarSaldo(const string &usuario) {
    ifstream archivo(usuario + "_saldo.txt");
    float saldo = 0.0;
    if (archivo.is_open()) {
        archivo >> saldo;
        archivo.close();
    } else {
        saldo = 1000.75;
    }
    return saldo;
}

// Función para registrar un nuevo usuario
void RegistrarUsuario() {
    string usuario, contrasena;

    cout << "Ingrese usuario: ";
    cin >> usuario;
    cout << "Ingrese contraseña: ";
    cin >> contrasena;

    ofstream archivo(usuario + ".txt");
    if (archivo.is_open()) {
        archivo << usuario << endl << contrasena;
        archivo.close();
        GuardarSaldo(usuario, 1000.75);
        cout << "¡Usuario registrado con éxito!\n";
        RegistrarMovimiento("Registro de usuario", usuario);
    } else {
        cout << "Error al crear el archivo.\n";
    }
}

// Función para iniciar sesión
bool IniciarSesion(string &usuario) {
    string contrasena, user, contra;

    cout << "Ingrese su usuario: ";
    cin >> usuario;
    cout << "Ingrese su contraseña: ";
    cin >> contrasena;

    ifstream archivo(usuario + ".txt");
    if (!archivo.is_open()) {
        cout << "Usuario no encontrado.\n";
        return false;
    }

    getline(archivo, user);
    getline(archivo, contra);
    archivo.close();

    if (user == usuario && contra == contrasena) {
        cout << "¡Inicio de sesión exitoso! Bienvenido, " << usuario << "!\n";
        return true;
    } else {
        cout << "Usuario o contraseña incorrectos.\n";
        return false;
    }
}

// Función para realizar un retiro de dinero
float RetirarDinero(const string &usuario, float saldo, int montoRetiro) {
    if (montoRetiro > saldo) {
        cout << "No tienes suficiente saldo para este retiro.\n";
        return saldo;
    }
    if (montoRetiro % 5 != 0) {
        cout << "Solo puedes retirar montos que terminen en 5 o 0.\n";
        return saldo;
    }
    if (montoRetiro > 500) {
        cout << "Para cantidades mayores a $500, acércate a una sucursal.\n";
        return saldo;
    }

    float nuevoSaldo = saldo - montoRetiro;
    RegistrarMovimiento("Retiro", usuario, montoRetiro);
    GuardarSaldo(usuario, nuevoSaldo);

    return nuevoSaldo;
}

// Función para realizar un depósito de dinero
float DepositarDinero(const string &usuario, float saldo, float montoDeposito) {
    if (montoDeposito <= 0) {
        cout << "El monto a depositar debe ser mayor a 0.\n";
        return saldo;
    }

    float nuevoSaldo = saldo + montoDeposito;
    RegistrarMovimiento("Depósito", usuario, montoDeposito);
    GuardarSaldo(usuario, nuevoSaldo);

    return nuevoSaldo;
}

// Función para el menú de usuario
void MenuUsuario(const string &usuario) {
    int opcion1, montoRetiro;
    float montoDeposito;
    float saldo = CargarSaldo(usuario);

    do {
        cout << "\nMenu de usuario\n";
        cout << "1. Retiro\n";
        cout << "2. Depósito\n";
        cout << "3. Salir\n";
        cout << "Opción: ";

        // El temporizador se activa aquí
        if (TemporizadorInactividad()) {
            continue; // Si el temporizador se reinicia, vuelve a mostrar el menú de usuario
        }

        cin >> opcion1;

        switch (opcion1) {
        case 1:
            cout << "Saldo actual: $" << saldo << endl;
            cout << "Ingresa el monto a retirar: $";
            cin >> montoRetiro;
            saldo = RetirarDinero(usuario, saldo, montoRetiro);
            cout << "Saldo restante: $" << saldo << endl;
            break;

        case 2:
            cout << "Saldo actual: $" << saldo << endl;
            cout << "Ingresa el monto a depositar: $";
            cin >> montoDeposito;
            saldo = DepositarDinero(usuario, saldo, montoDeposito);
            cout << "Nuevo saldo: $" << saldo << endl;
            break;

        case 3:
            cout << "Saliendo del menú de usuario...\n";
            break;

        default:
            cout << "Opción no válida.\n";
            break;
        }
    } while (opcion1 != 3);
}

// Función para el menú principal
void MostrarMenu() {
    int opcion;
    string usuario;

    do {
        cout << "\nMenu\n";
        cout << "---------------\n";
        cout << "1. Registrarse\n";
        cout << "2. Iniciar Sesión\n";
        cout << "3. Salir\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            RegistrarUsuario();
            break;
        case 2:
            if (IniciarSesion(usuario)) {
                MenuUsuario(usuario);  // Menú de usuario después de iniciar sesión
            }
            break;

        case 3:
            cout << "¡Adiós!\n";
            return;

        default:
            cout << "Opción no válida. Intente de nuevo.\n";
            break;
        }
    } while (true);
}

int main() {
    MostrarMenu();
    return 0;
}
