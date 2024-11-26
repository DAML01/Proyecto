#include <iostream>  // biblioteca entrada y salida (cout y cin)
#include <chrono>    // biblioteca para manipular tiempo y duraciones
#include <string>    // biblioteca para manipulación de cadenas de texto

#ifdef _WIN32  // si estamos compilando para un entorno Windows
#include <windows.h>  // Incluye el archivo de cabecera de la API de Windows (para usar Sleep() en Windows)
#endif

using namespace std; 

// función para preguntar si el usuario quiere continuar en el programa o salir
bool askToContinue() {
    string choice;
    cout << "¿Desea continuar en el programa? (si/no): ";
    cin >> choice;
    return (choice == "si");  // retorna verdadero si elige "si"; falso de otro modo
}

// función para mostrar el menú basado en un rol predefinido
// e iniciar el temporizador de inactividad de 5 segundos
bool showMenu() {
    // Asumimos un rol predeterminado de "usuario"
    cout << "\nBienvenido, Usuario. Opciones del menú:\n";
    cout << "1. Opciones de usuario\n";

    // inicia un temporizador para medir la inactividad
    auto start = chrono::steady_clock::now();
    bool inactive = true;

    // bucle que espera una opción del usuario o hasta que pase el tiempo de inactividad
    while (true) {
        // Verifica si han pasado 5 segundos desde que se inició el temporizador
        if (chrono::steady_clock::now() - start >= chrono::seconds(5)) { // Ajustar el tiempo según sea necesario
            break;  // Termina el bucle si han pasado 5 segundos sin actividad
        }
        
        // verifica si el usuario ingresó algún valor en el buffer de entrada
        if (cin.rdbuf()->in_avail() > 0) { 
            int option;
            cin >> option;  // Lee la opción seleccionada por el usuario
            cout << "Opción " << option << " seleccionada.\n";
            inactive = false; // marca como activo si se seleccionó una opción
            break;
        }

        // pausa brevemente para evitar que el bucle ocupe todo el procesador
        #ifdef _WIN32
        Sleep(100);  // Para Windows, Sleep en milisegundos
        #endif
    }

    // retorna verdadero si el usuario no seleccionó ninguna opción en 5 segundos
    return inactive;
}

int main() {
    // bucle que controla la ejecución del programa
    while (true) {
        // llama a la función showMenu y verifica si hubo inactividad
        if (showMenu()) {
            cout << "\nNo se seleccionó ninguna opción en 5 segundos.\n";
            
            // pregunta al usuario si desea continuar o salir
            if (!askToContinue()) {
                cout << "Saliendo del programa...\n";
                break;  // sale del bucle principal si elige "no"
            } else {
                cout << "Continuando en el programa...\n";
            }
        }
    }
    
    return 0;  
}