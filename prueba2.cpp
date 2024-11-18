#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

// Función para manejar el registro de usuarios
void RegistrarUsuario()
{
    string usuario, contrasena;

    cout << "Ingrese usuario: ";
    cin >> usuario;
    cout << "Ingrese contraseña: ";
    cin >> contrasena;

    ofstream archivo(usuario + ".txt");
    if (archivo.is_open())
    {
        archivo << usuario << endl
                << contrasena << endl
                << "1000.75"; // Saldo inicial
        archivo.close();
        cout << "¡Usuario registrado con éxito!\n";
    }
    else
    {
        cout << "Error al crear el archivo.\n";
    }
}

// Función para cargar el saldo de un usuario
float CargarSaldo(const string &usuario)
{
    ifstream archivo(usuario + ".txt");
    string linea, saldoStr;
    float saldo = 0.0;

    if (archivo.is_open())
    {
        getline(archivo, linea);        // Usuario
        getline(archivo, linea);        // Contraseña
        if (getline(archivo, saldoStr)) // Intenta leer el saldo
        {
            try
            {
                saldo = stof(saldoStr); // Convertir a float
            }
            catch (const invalid_argument &e)
            {
                cerr << "Error: El saldo en el archivo no es válido.\n";
                saldo = 0.0; // Saldo predeterminado
            }
            catch (const out_of_range &e)
            {
                cerr << "Error: El saldo en el archivo está fuera de rango.\n";
                saldo = 0.0; // Saldo predeterminado
            }
        }
        else
        {
            cerr << "Error: No se pudo leer el saldo.\n";
        }
        archivo.close();
    }
    else
    {
        cout << "Error al cargar el archivo del usuario.\n";
    }

    return saldo;
}

// Función para guardar el saldo de un usuario
void GuardarSaldo(const string &usuario, float saldo)
{
    fstream archivo(usuario + ".txt", ios::in | ios::out);
    string linea;

    if (archivo.is_open())
    {
        getline(archivo, linea);        // Usuario
        getline(archivo, linea);        // Contraseña
        archivo.seekp(archivo.tellg()); // Posicionar al inicio del saldo
        archivo << saldo;               // Sobrescribir el saldo
        archivo.close();
    }
    else
    {
        cout << "Error al guardar el saldo.\n";
    }
}

// Función para manejar el inicio de sesión
bool IniciarSesion(string &usuario)
{
    string contrasena;
    string user, contra;

    cout << "Ingrese su usuario: ";
    cin >> usuario;
    cout << "Ingrese su contraseña: ";
    cin >> contrasena;

    ifstream archivo(usuario + ".txt");
    if (!archivo.is_open())
    {
        cout << "Usuario no encontrado.\n";
        return false;
    }
    getline(archivo, user);
    getline(archivo, contra);
    archivo.close();

    if (user == usuario && contra == contrasena)
    {
        cout << "¡Inicio de sesión exitoso! Bienvenido, " << usuario << "!\n";
        return true;
    }
    else
    {
        cout << "Usuario o contraseña incorrectos.\n";
        return false;
    }
}

// Función para registrar una transacción en un archivo
void RegistrarTransaccion(const string &usuario, const string &tipo, float monto, float saldoRestante)
{
    ofstream archivo(usuario + "_transacciones.txt", ios::app); // Modo de agregar (append)
    if (archivo.is_open())
    {
        archivo << tipo << ": $" << monto << ", Saldo restante: $" << saldoRestante << endl;
        archivo.close();
        cout << tipo << " registrado correctamente.\n";
    }
    else
    {
        cout << "Error al registrar la transacción.\n";
    }
}

// Función para manejar el retiro de dinero
float RetirarDinero(const string &usuario, float saldo, int montoRetiro)
{
    if (montoRetiro > saldo)
    {
        cout << "No tienes suficiente saldo para este retiro." << endl;
        return saldo;
    }
    if (montoRetiro % 5 != 0)
    {
        cout << "Solo puedes retirar montos que terminen en 5 o 0." << endl;
        return saldo;
    }
    if (montoRetiro > 500)
    {
        cout << "Para cantidades mayores a $500, por favor acérquese a la sucursal más cercana." << endl;
        return saldo;
    }

    float nuevoSaldo = saldo - montoRetiro;
    RegistrarTransaccion(usuario, "Retiro", montoRetiro, nuevoSaldo);
    GuardarSaldo(usuario, nuevoSaldo); // Guardar el saldo actualizado
    return nuevoSaldo;
}

// Función para manejar el depósito de dinero
float AgregarDinero(const string &usuario, float saldo, float montoDeposito)
{
    if (montoDeposito <= 0)
    {
        cout << "El monto a depositar debe ser mayor a 0." << endl;
        return saldo;
    }

    float nuevoSaldo = saldo + montoDeposito;
    RegistrarTransaccion(usuario, "Depósito", montoDeposito, nuevoSaldo);
    GuardarSaldo(usuario, nuevoSaldo); // Guardar el saldo actualizado
    return nuevoSaldo;
}

// Función principal del menú
void MostrarMenu()
{
    int opcion, opcion1;
    float saldo, monto;
    string usuario;

    do
    {
        cout << "\nMenu\n";
        cout << "---------------\n";
        cout << "1. Registrarse\n";
        cout << "2. Iniciar Sesión\n";
        cout << "3. Salir\n";
        cout << "Opción: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            RegistrarUsuario();
            break;
        case 2:
            if (IniciarSesion(usuario))
            {
                saldo = CargarSaldo(usuario); // Cargar saldo persistente
                do
                {
                    cout << "\nMenu de usuario\n";
                    cout << "1. Retiro\n";
                    cout << "2. Dep\xA2sito\n";
                    cout << "3. Salir\n";
                    cout << "Opción: ";
                    cin >> opcion1;

                    switch (opcion1)
                    {
                    case 1:
                        cout << "Saldo actual: $" << saldo << endl;
                        cout << "Ingresa el monto a retirar: $";
                        cin >> monto;

                        saldo = RetirarDinero(usuario, saldo, monto);
                        cout << "Saldo restante: $" << saldo << endl;
                        break;

                    case 2:
                        cout << "Saldo actual: $" << saldo << endl;
                        cout << "Ingresa el monto a depositar: $";
                        cin >> monto;

                        saldo = AgregarDinero(usuario, saldo, monto);
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
            break;
        case 3:
            cout << "¡Adiós!\n";
            break;
        default:
            cout << "Opción no válida. Intente de nuevo.\n";
            break;
        }
    } while (opcion != 3);
}

int main()
{
    MostrarMenu();
    return 0;
}
