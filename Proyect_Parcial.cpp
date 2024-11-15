#include <iostream>
#include <string>
#include <fstream>
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
                << contrasena;
        archivo.close();
        cout << "¡Usuario registrado con éxito!\n";
    }
    else
    {
        cout << "Error al crear el archivo.\n";
    }
}

// Función para manejar el inicio de sesión
bool IniciarSesion()
{
    string usuario, contrasena;
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
// Funcion retiro de dinero
float retirarDinero(float saldo, int montoRetiro)
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
    return saldo - montoRetiro;
}

// Función principal del menú
void MostrarMenu()
{
    int opcion, opcion1, montoretiro;
    float saldo = 1000.75;
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
            if (IniciarSesion())
            {
                cout << "Menu" << endl;
                cout << "1. Retiro" << endl;
                cin >> opcion1;
                switch (opcion1)
                {
                case 1:
                    cout << "Saldo actual: $" << saldo << endl;
                    cout << "Ingresa el monto a retirar: $";
                    cin >> montoretiro;

                    saldo = retirarDinero(saldo, montoretiro); // Actualiza el saldo después del registro del retiro.

                    cout << "Saldo restante: $" << saldo << endl;
                    break;

                default:
                    break;
                }
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
