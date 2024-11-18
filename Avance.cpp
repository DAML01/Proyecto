#include <iostream>
using namespace std;

// Función para registrar el retiro de dinero
float retirarDinero(float saldo, int montoRetiro) {
    if (montoRetiro > saldo) {
        cout << "No tienes suficiente saldo para este retiro." << endl;
        return saldo;
    }
    if (montoRetiro % 5 != 0) {
        cout << "Solo puedes retirar montos que terminen en 5 o 0." << endl;
        return saldo;
    }
    if (montoRetiro > 500) {
        cout << "Para cantidades mayores a $500, por favor acerquese a sucursal mas cercana." << endl;
        return saldo;
    }
    return saldo - montoRetiro;
}

int main() {
    float saldo = 1000.75;  // Saldo inicial
    int montoRetiro;

    cout << "Saldo actual: $" << saldo << endl;
    cout << "Ingresa el monto a retirar: $";
    cin >> montoRetiro;

    saldo = retirarDinero(saldo, montoRetiro);  // Actualiza el saldo después del registro del retiro.

    cout << "Saldo restante: $" << saldo << endl;

    return 0;
}
