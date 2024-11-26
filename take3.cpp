#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// Función para registrar movimientos
void RegistrarMovimiento(const string &tipo, const string &usuario, float monto = 0)
{
    ofstream archivo("movimientos.txt", ios::app); // Abre el archivo en modo de agregar
    if (archivo.is_open())
    {
        archivo << "Tipo: " << tipo << ", Usuario: " << usuario;
        if (monto > 0)
        {
            archivo << ", Monto: $" << monto;
        }
        archivo << endl;
        archivo.close();
    }
    else
    {
        cout << "Error al registrar el movimiento.\n";
    }
}

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
                << contrasena;
        archivo.close();
        cout << "¡Usuario registrado con éxito!\n";

        // Registrar el movimiento
        RegistrarMovimiento("Registro de usuario", usuario);
    }
    else
    {
        cout << "Error al crear el archivo.\n";
    }
}

// Función para manejar el inicio de sesión
bool IniciarSesion(string &usuario)
{
    string contrasena, user, contra;

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

// Función para manejar retiros
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
        cout << "Para cantidades mayores a $500, por favor acerquese a sucursal mas cercana." << endl;
        return saldo;
    }

    float nuevoSaldo = saldo - montoRetiro;

    // Registrar el movimiento
    RegistrarMovimiento("Retiro", usuario, montoRetiro);

    return nuevoSaldo;
}

// Función para manejar depósitos
float DepositarDinero(const string &usuario, float saldo, float montoDeposito)
{
    if (montoDeposito <= 0)
    {
        cout << "El monto a depositar debe ser mayor a 0.\n";
        return saldo;
    }

    float nuevoSaldo = saldo + montoDeposito;

    // Registrar el movimiento
    RegistrarMovimiento("Depósito", usuario, montoDeposito);

    return nuevoSaldo;
}

// Función de administrador para ver los movimientos
void Administrador()
{
    ifstream archivo("movimientos.txt");
    string linea;

    if (archivo.is_open())
    {
        cout << "\n--- Movimientos registrados ---\n";
        while (getline(archivo, linea))
        {
            cout << linea << endl;
        }
        archivo.close();
    }
    else
    {
        cout << "No se encontraron movimientos registrados.\n";
    }
}

// Función principal del menú
void MostrarMenu()
{
    int opcion, opcion1, montoretiro;
    float saldo = 1000.75, montoDeposito;
    string usuario;

    do
    {
        cout << "\nMenu\n";
        cout << "---------------\n";
        cout << "1. Registrarse\n";
        cout << "2. Iniciar Sesión\n";
        cout << "3. Modo Administrador\n";
        cout << "4. Salir\n";
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
                do
                {
                    cout << "\nMenu de usuario\n";
                    cout << "1. Retiro\n";
                    cout << "2. Depósito\n";
                    cout << "3. Salir\n";
                    cout << "Opción: ";
                    cin >> opcion1;

                    switch (opcion1)
                    {
                    case 1:
                        cout << "Saldo actual: $" << saldo << endl;
                        cout << "Ingresa el monto a retirar: $";
                        cin >> montoretiro;
                        saldo = RetirarDinero(usuario, saldo, montoretiro);
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
            break;

        case 3:
            Administrador();
            break;

        case 4:
            cout << "¡Adiós!\n";
            break;

        default:
            cout << "Opción no válida. Intente de nuevo.\n";
            break;
        }
    } while (opcion != 4);
}

int main()
{
    MostrarMenu();
    return 0;
}
