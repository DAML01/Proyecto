#include <iostream>
using namespace std;
int main(){
    int opcion=0;
    char error;
    do
    {
        cout<<"ingrese dato\n";
        cout<<"1.menu\n";
        cout<<"2.salir\n";
        cin>>opcion;
        if(opcion==error)
        {
            cout<<"caracter invalido\n";
        }
         break;
            switch (opcion)
            {     
            case 1:
                cout<<"menu\n";/* code */
                break;
            case 2:
                cout<<"salir\n";/* code */
                break;
            default:
                cout<<"opcion invalida\n";
                break;
            }
    }while (opcion==opcion);
    {
        cout<<"salio\n";/* code */
    }
    return 0;
}