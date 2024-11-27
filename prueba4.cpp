#include <iostream>
#include <string>
#include <fstream> // Para manejo de archivos
using namespace std;

// Función para registrar movimientos en un archivo de texto
// Tipo puede ser "Depósito", "Retiro", etc.
// La Función identifica cual usuario realiza el movimiento
// Monto es opcional (se registra solo si es mayor a 0)
void RegistrarMovimiento(const string &tipo, const string &usuario, float monto = 0)
{
    ofstream archivo("movimientos.txt", ios::app); // Abre el archivo en modo "agregar"
    if (archivo.is_open())                         // Verifica si el archivo se abrió correctamente
    {
        // Escribe el tipo de movimiento, usuario, y monto (si aplica)
        archivo << "Tipo: " << tipo << ", Usuario: " << usuario;
        if (monto > 0)
        {
            archivo << ", Monto: $" << monto;
        }
        archivo << endl; // Agrega un salto de línea al final
        archivo.close(); // Cierra el archivo
    }
    else
    {
        cout << "Error al registrar el movimiento.\n"; // Mensaje de error si no se pudo abrir el archivo
    }
}

// Función para guardar el saldo de un usuario en su archivo específico
// Esto asegura que el saldo persista incluso después de cerrar el programa
void GuardarSaldo(const string &usuario, float saldo)
{
    ofstream archivo(usuario + "_saldo.txt"); // Crea/abre un archivo con el nombre del usuario
    if (archivo.is_open())                    // Verifica si el archivo se abrió correctamente
    {
        archivo << saldo; // Escribe el saldo actual del usuario
        archivo.close();  // Cierra el archivo
    }
    else
    {
        cout << "Error al guardar el saldo del usuario.\n"; // Mensaje de error si no se pudo abrir el archivo y leer el dato para almacenar
    }
}

// Función para cargar el saldo de un usuario desde su archivo
// Si no existe el archivo, se devuelve un saldo predeterminado
float CargarSaldo(const string &usuario)
{
    ifstream archivo(usuario + "_saldo.txt"); // Abre el archivo del saldo del usuario
    float saldo = 0.0;                        // Variable para almacenar el saldo leído
    if (archivo.is_open())                    // Verifica si el archivo existe y se abrió correctamente
    {
        archivo >> saldo; // Lee el saldo desde el archivo
        archivo.close();  // Cierra el archivo
    }
    else
    {
        saldo = 1000.75; // Si el archivo no existe, se asigna un saldo inicial predeterminado
    }
    return saldo; // Retorna el saldo leído o el predeterminado
}

// Función para registrar un nuevo usuario
// Crea un archivo con el nombre del usuario que almacena sus credenciales
void RegistrarUsuario()
{
    string usuario, contrasena;
    int n=164, exclam=173, e=130;

    cout << "Ingrese usuario: ";
    cin >> usuario;
    cout << "Ingrese contrase" <<char(n); cout<<"a: ";//ASCII ñ se lee "ingres contraseña"
    cin >> contrasena;

    ofstream archivo(usuario + ".txt"); // Crea un archivo para el usuario con sus credenciales
    if (archivo.is_open())
    {
        archivo << usuario << endl
                << contrasena; // Escribe el usuario y contraseña en el archivo
        archivo.close();       // Cierra el archivo

        // Guarda un saldo inicial en un archivo separado
        GuardarSaldo(usuario, 1000.75);

        cout<<char(exclam); cout << "Usuario registrado con ";cout <<char(e)<<"xito!\n"; //ASCII tilde e
        RegistrarMovimiento("Registro de usuario", usuario); // Registra el movimiento en el log general, este caso usuario
    }
    else
    {
        cout << "Error al crear el archivo.\n"; // Mensaje de error si no se pudo crear el archivo
    }
}

// Función para iniciar sesión con usuario y contraseña
// Verifica que el archivo del usuario exista y que las credenciales coincidan
bool IniciarSesion(string &usuario)
{
    string contrasena, user, contra;
    int n=164,o=162, exclam=173;//ASCII para letra o tilde

    cout << "Ingrese su usuario: ";
    cin >> usuario;
    cout << "Ingrese su contrase"<<char(n); cout<<"a: ";//ASCII ñ se lee "ingrese su contraseña"
    cin >> contrasena;

    ifstream archivo(usuario + ".txt"); // Intenta abrir y leer el archivo del usuario
    if (!archivo.is_open())             // Si no existe el archivo, muestra un mensaje y retorna false
    {
        cout << "Usuario no encontrado.\n";
        cout << "Porfavor Registrarse\n";
        return false;
    }

    // Lee el usuario y la contraseña almacenados en el archivo existente
    getline(archivo, user);
    getline(archivo, contra);
    archivo.close();

    if (user == usuario && contra == contrasena) // Verifica que las credenciales coincidan
    {
         cout<<char(exclam);cout <<"Inicio de sesi"<<char(o); cout<<"n exitoso!"; 
         cout<<char(exclam); cout<<"Bienvenido, "; cout<< usuario << "!\n";//ASCII inicio para signo de exclamacion entrada y 'o' tildada
        return true;
    }
    else
    {
        cout << "Usuario o contrase"<<char(n);cout<<"incorrectos.\n";//ASCII ñ
        return false;
    }
}

// Función para realizar un retiro de dinero
// Verifica condiciones como que el monto no exceda el saldo y que sea múltiplo de 5
float RetirarDinero(const string &usuario, float saldo, int montoRetiro)
{
    int a=160;//ASCII a tildada
    if (montoRetiro > saldo)
    {
        cout << "No se posee suficiente saldo para este retiro." << endl;
        return saldo; // Retorna el saldo sin cambios
    }
    if (montoRetiro % 5 != 0)
    {
        cout << "Solo es posible retirar montos que terminen en 5 o 0." << endl;
        return saldo; // Retorna el saldo sin cambios
    }
    if (montoRetiro > 500)
    {
        cout << "Para cantidades mayores a $500, por favor acerquese a sucursal m"<<char(a);cout<< "s cercana." << endl;
        return saldo; // Retorna el saldo sin cambios
    }

    float nuevoSaldo = saldo - montoRetiro;              // Calcula el nuevo saldo
    RegistrarMovimiento("Retiro", usuario, montoRetiro); // Registra el retiro en el log
    GuardarSaldo(usuario, nuevoSaldo);                   // Actualiza el archivo del saldo del usuario

    return nuevoSaldo; // Retorna el nuevo saldo
}

// Función para realizar un depósito de dinero
// Verifica que el monto a depositar sea válido
float DepositarDinero(const string &usuario, float saldo, float montoDeposito)
{
    if (montoDeposito <= 0)
    {
        cout << "El monto a depositar debe ser mayor a 0.\n";
        return saldo; // Retorna el saldo sin cambios
    }

    float nuevoSaldo = saldo + montoDeposito;                // Calcula el nuevo saldo
    RegistrarMovimiento("Depósito", usuario, montoDeposito); // Registra el depósito en el log
    GuardarSaldo(usuario, nuevoSaldo);                       // Actualiza el archivo del saldo del usuario

    return nuevoSaldo; // Retorna el nuevo saldo
}

// Función para que el administrador visualice los movimientos registrados
// Lee el archivo "movimientos.txt" y muestra su contenido
void Administrador()
{
    ifstream archivo("movimientos.txt");
    string linea;

    if (archivo.is_open())
    {
        cout << "\n--- Movimientos registrados ---\n";
        while (getline(archivo, linea)) // Lee línea por línea
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

// Función para validar credenciales de administrador
// Usuario y contraseña están predefinidos como "admin"
bool ValidarAdministrador()
{
    int n=164;//ASCII n
    string usuario, contrasena;

    cout << "Ingrese usuario administrador: ";
    cin >> usuario;
    cout << "Ingrese contrase"<<char(n); cout<<"a: ";//ASCII ñ se lee "ingrese su contraseña"
    cin >> contrasena;

    if (usuario == "admin" && contrasena == "admin") // Verifica credenciales
    {
        return true;
    }
    else
    {
        cout << "Credenciales incorrectas.\n";
        return false;
    }
}

// Función principal que muestra el menú y controla la lógica general
void MostrarMenu()
{
    int opcion, opcion1, montoretiro;
    float montoDeposito;
    string usuario;
    //caracteres especiales
    int o=162, n=164,a=160;//ASCII a tildada;

    do
    {
        // Menú principal
        cout << "\nMenu\n";
        cout << "----------------------\n";
        cout << "1. Registrarse\n";
        cout << "2. Iniciar Sesi"<<char(o); cout<<"n\n"; //ASCII para imprimir ó
        cout << "3. Modo Administrador\n";
        cout << "4. Salir\n";
        cout << "----------------------\n";
        cout << "Su opci"<<char(o); cout<<"n: ";//ASCII tilde o
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            RegistrarUsuario(); // Llama a la función para registrar un usuario
            break;
        case 2:
            if (IniciarSesion(usuario)) // Si el inicio de sesión es exitoso
            {
                float saldo = CargarSaldo(usuario); // Carga el saldo del usuario
                do
                {
                    // Menú de usuario
                    cout << "\nMenu de usuario\n";
                    cout << "----------------------\n";
                    cout << "\n"; cout<<"Su saldo es:" <<saldo <<"$" "\n"; //mostrar saldo a usuario
                    cout << "----------------------\n";
                    cout << "1. Retiro\n";
                    cout << "2. Dep"<<char(o); cout<<"sito\n";
                    cout << "3. Salir\n";
                    cout << "----------------------\n";
                    cout << "Su opci"<<char(o); cout<<"n: ";
                    cin >> opcion1;

                    switch (opcion1)
                    {
                    case 1:
                        cout << "Saldo actual: $" << saldo << endl;
                        cout << "Ingresa el monto a retirar: $";
                        cin >> montoretiro;
                        saldo = RetirarDinero(usuario, saldo, montoretiro); // Realiza el retiro
                        cout << "Saldo restante: $" << saldo << endl;
                        break;

                    case 2:
                        cout << "Saldo actual: $" << saldo << endl;
                        cout << "Ingresa el monto a depositar: $";
                        cin >> montoDeposito;
                        saldo = DepositarDinero(usuario, saldo, montoDeposito); // Realiza el depósito
                        cout << "Nuevo saldo: $" << saldo << endl;
                        break;

                    case 3:
                        cout << "Saliendo del menú de usuario...\n";
                        break;

                    default:
                        cout << "Opci"<<char(o); cout<< "no v"<<char(a); cout<<"lida.\n";//DetalleASCII
                        break;
                    }
                } while (opcion1 != 3); // Repite hasta que el usuario elija salir
            }
            break;

        case 3:
            if (ValidarAdministrador()) // Si las credenciales son correctas
            {
                Administrador(); // Muestra los movimientos registrados
            }
            break;

        case 4:
            cout << "¡Adiós!\n"; // Sale del programa
            break;

        default:
            cout << "Opción no válida. Intente de nuevo.\n";
            break;
        }
    } while (opcion != 4); // Repite hasta que el usuario elija salir del programa
}

// Función principal del programa
int main()
{
    MostrarMenu(); // Llama al menú principal
    return 0;      // Finaliza el programa
}
