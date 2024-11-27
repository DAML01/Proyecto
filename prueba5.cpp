#include <iostream>
#include <string>
#include <fstream> // Para manejar archivos
#include <limits>  // Para limpiar entradas no válidas
using namespace std;

// Función que pide un número entero al usuario y valida la entrada
// Es útil para asegurarse de que no se ingresen letras u otros caracteres
int ObtenerEntero(const string &mensaje)
{
    int valor; // Aquí se guarda el número que el usuario ingrese
    while (true)
    {
        cout << mensaje; // Mostramos el mensaje que indica qué se espera
        cin >> valor;    // Intentamos leer el número

        // Validamos si la entrada fue un número
        if (cin.fail()) // Si no fue un número (por ejemplo, letras)
        {
            cin.clear();                                         // Limpiamos el estado de error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignoramos la entrada inválida
            cout << "Por favor, ingresa un número entero válido: ";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiamos lo que quedó en el buffer
            return valor;                                        // Todo bien, regresamos el número
        }
    }
}

// Función para pedir un número decimal al usuario
// Sirve para cosas como montos de dinero que tienen decimales
float ObtenerFloat(const string &mensaje)
{
    float valor; // Aquí guardaremos el número flotante ingresado
    while (true)
    {
        cout << mensaje; // Mostramos el mensaje para que el usuario sepa qué hacer
        cin >> valor;    // Intentamos leer el número

        // Verificamos si es válido
        if (cin.fail()) // Si no fue un número (por ejemplo, texto)
        {
            cin.clear();                                         // Limpiamos el error
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignoramos lo malo
            cout << "Por favor, ingresa un número válido: ";
        }
        else
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Limpiamos el buffer
            return valor;                                        // Devolvemos el número válido
        }
    }
}

// Función para registrar los movimientos del usuario en un archivo
// Esto nos permite llevar un historial (por ejemplo, retiros y depósitos)
void RegistrarMovimiento(const string &tipo, const string &usuario, float monto = 0)
{
    ofstream archivo("movimientos.txt", ios::app); // Abrimos el archivo en modo agregar
    if (archivo.is_open())                         // Si se abrió bien, seguimos
    {
        archivo << "Tipo: " << tipo << ", Usuario: " << usuario; // Escribimos el tipo de operación y usuario
        if (monto > 0)
        {
            archivo << ", Monto: $" << monto; // Agregamos el monto si aplica
        }
        archivo << endl; // Salto de línea para el próximo registro
        archivo.close(); // Cerramos el archivo
    }
    else
    {
        cout << "No se pudo registrar el movimiento.\n"; // Error si no se puede escribir
    }
}

// Función para guardar el saldo del usuario en un archivo
// Es importante para que el saldo no se pierda al cerrar el programa
void GuardarSaldo(const string &usuario, float saldo)
{
    ofstream archivo(usuario + "_saldo.txt"); // Crea un archivo que lleva el nombre del usuario
    if (archivo.is_open())                    // Si se abrió correctamente
    {
        archivo << saldo; // Guardamos el saldo en el archivo
        archivo.close();  // Cerramos el archivo
    }
    else
    {
        cout << "No se pudo guardar el saldo.\n"; // Si algo falla, avisamos
    }
}

// Función para leer el saldo del usuario desde su archivo
// Si el archivo no existe, asignamos un saldo inicial predeterminado
float CargarSaldo(const string &usuario)
{
    ifstream archivo(usuario + "_saldo.txt"); // Intentamos abrir el archivo del usuario
    float saldo = 0.0;                        // Aquí guardaremos el saldo
    if (archivo.is_open())                    // Si el archivo existe y se abre bien
    {
        archivo >> saldo; // Leemos el saldo del archivo
        archivo.close();  // Cerramos el archivo
    }
    else
    {
        saldo = 1000.75; // Si el archivo no existe, asignamos un saldo base
    }
    return saldo; // Devolvemos el saldo al programa
}

// Función para registrar un nuevo usuario
// Crea un archivo con sus datos y asigna un saldo inicial
void RegistrarUsuario()
{
    string usuario, contrasena;

    cout << "Ingresa tu nombre de usuario: ";
    cin >> usuario;
    cout << "Ingresa tu contraseña: ";
    cin >> contrasena;

    ofstream archivo(usuario + ".txt"); // Archivo con los datos del usuario
    if (archivo.is_open())
    {
        archivo << usuario << endl
                << contrasena; // Guardamos el usuario y la contraseña
        archivo.close();

        // Creamos un archivo separado para el saldo inicial
        GuardarSaldo(usuario, 1000.75);

        cout << "Usuario registrado con éxito.\n";
        RegistrarMovimiento("Registro de usuario", usuario); // Lo registramos en el historial
    }
    else
    {
        cout << "Hubo un problema al registrar el usuario.\n"; // Mensaje de error
    }
}

// Función para iniciar sesión
// Verifica si el usuario existe y si la contraseña es correcta
bool IniciarSesion(string &usuario)
{
    string contrasena, user, contra;

    cout << "Ingresa tu usuario: ";
    cin >> usuario;
    cout << "Ingresa tu contraseña: ";
    cin >> contrasena;

    ifstream archivo(usuario + ".txt"); // Archivo con los datos del usuario
    if (!archivo.is_open())             // Si el archivo no existe, mostramos error
    {
        cout << "Usuario no encontrado.\n";
        return false;
    }

    // Leemos el usuario y la contraseña desde el archivo
    getline(archivo, user);
    getline(archivo, contra);
    archivo.close();

    if (user == usuario && contra == contrasena) // Verificamos si coinciden
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
// Función para realizar un retiro de dinero
// Verifica si el usuario tiene saldo suficiente y si el monto es válido
float RetirarDinero(const string &usuario, float saldo, int montoRetiro)
{
    // Verificamos si el monto supera el saldo
    if (montoRetiro > saldo)
    {
        cout << "No tienes suficiente saldo para este retiro.\n";
        return saldo; // No se modifica el saldo
    }
    // Verificamos si el monto es múltiplo de 5 (requisito del cajero)
    if (montoRetiro % 5 != 0)
    {
        cout << "Solo puedes retirar montos que terminen en 5 o 0.\n";
        return saldo; // No se modifica el saldo
    }
    // Limite máximo permitido para retiros automáticos
    if (montoRetiro > 500)
    {
        cout << "Para cantidades mayores a $500, por favor acércate a la sucursal más cercana.\n";
        return saldo; // No se modifica el saldo
    }

    float nuevoSaldo = saldo - montoRetiro;              // Calculamos el nuevo saldo
    RegistrarMovimiento("Retiro", usuario, montoRetiro); // Guardamos el movimiento
    GuardarSaldo(usuario, nuevoSaldo);                   // Actualizamos el saldo en el archivo

    return nuevoSaldo; // Retornamos el nuevo saldo al usuario
}

// Función para realizar un depósito
// Verifica que el monto sea válido antes de actualizar el saldo
float DepositarDinero(const string &usuario, float saldo, float montoDeposito)
{
    // Verificamos si el monto es mayor a cero
    if (montoDeposito <= 0)
    {
        cout << "El monto a depositar debe ser mayor a 0.\n";
        return saldo; // No se modifica el saldo
    }

    float nuevoSaldo = saldo + montoDeposito;                // Calculamos el nuevo saldo
    RegistrarMovimiento("Depósito", usuario, montoDeposito); // Registramos el depósito
    GuardarSaldo(usuario, nuevoSaldo);                       // Guardamos el nuevo saldo

    return nuevoSaldo; // Retornamos el nuevo saldo actualizado
}

// Función para el administrador: permite revisar todos los movimientos registrados
void Administrador()
{
    ifstream archivo("movimientos.txt"); // Abrimos el archivo de movimientos
    string linea;

    if (archivo.is_open()) // Si el archivo existe y se abre bien
    {
        cout << "\n--- Movimientos registrados ---\n";
        while (getline(archivo, linea)) // Leemos línea por línea
        {
            cout << linea << endl; // Mostramos cada línea (movimiento)
        }
        archivo.close(); // Cerramos el archivo
    }
    else
    {
        cout << "No se encontraron movimientos registrados.\n";
    }
}

// Función para validar credenciales de administrador
// El usuario y contraseña están predefinidos como "admin"
bool ValidarAdministrador()
{
    string usuario, contrasena;

    cout << "Ingrese usuario administrador: ";
    cin >> usuario;
    cout << "Ingrese contraseña: ";
    cin >> contrasena;

    if (usuario == "admin" && contrasena == "admin") // Credenciales correctas
    {
        return true;
    }
    else
    {
        cout << "Credenciales incorrectas.\n";
        return false;
    }
}

// Función principal que controla el flujo del programa
// Incluye menús, opciones, y llamada a las funciones principales
void MostrarMenu()
{
    int opcion, opcion1, montoRetiro;
    float montoDeposito;
    string usuario;

    do
    {
        // Menú principal
        cout << "\nMenú Principal\n";
        cout << "-----------------\n";
        cout << "1. Registrarse\n";
        cout << "2. Iniciar Sesión\n";
        cout << "3. Modo Administrador\n";
        cout << "4. Salir\n";
        cout << "Opción: ";
        opcion = ObtenerEntero(""); // Utilizamos ObtenerEntero para validar la entrada

        switch (opcion)
        {
        case 1:
            RegistrarUsuario(); // Registro de un nuevo usuario
            break;

        case 2:
            if (IniciarSesion(usuario)) // Si el inicio de sesión es exitoso
            {
                float saldo = CargarSaldo(usuario); // Cargamos su saldo desde el archivo
                do
                {
                    // Menú para usuarios
                    cout << "\nMenú de Usuario\n";
                    cout << "1. Retiro\n";
                    cout << "2. Depósito\n";
                    cout << "3. Salir\n";
                    cout << "Opción: ";
                    opcion1 = ObtenerEntero(""); // Validamos entrada numérica

                    switch (opcion1)
                    {
                    case 1:
                        cout << "Saldo actual: $" << saldo << endl;
                        cout << "Ingresa el monto a retirar: $";
                        montoRetiro = ObtenerEntero("");                    // Validamos el monto
                        saldo = RetirarDinero(usuario, saldo, montoRetiro); // Realizamos el retiro
                        cout << "Saldo restante: $" << saldo << endl;
                        break;

                    case 2:
                        cout << "Saldo actual: $" << saldo << endl;
                        cout << "Ingresa el monto a depositar: $";
                        montoDeposito = ObtenerFloat("");                       // Validamos el monto
                        saldo = DepositarDinero(usuario, saldo, montoDeposito); // Realizamos el depósito
                        cout << "Nuevo saldo: $" << saldo << endl;
                        break;

                    case 3:
                        cout << "Saliendo del menú de usuario...\n";
                        break;

                    default:
                        cout << "Opción no válida. Intente de nuevo.\n";
                        break;
                    }
                } while (opcion1 != 3); // Volvemos al menú hasta que elija salir
            }
            break;

        case 3:
            if (ValidarAdministrador()) // Si las credenciales son correctas
            {
                Administrador(); // Mostramos los movimientos registrados
            }
            break;

        case 4:
            cout << "¡Adiós!\n";
            break;

        default:
            cout << "Opción no válida. Intente de nuevo.\n";
            break;
        }
    } while (opcion != 4); // Repetimos el menú principal hasta que elija salir
}

// Función principal del programa
int main()
{
    MostrarMenu(); // Mostramos el menú principal
    return 0;      // Fin del programa
}
