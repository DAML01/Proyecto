#include <iostream>
#include <string>
#include <fstream>
using namespace std;

bool InicioSesion ()
{
    string usuario, contrasena;
    string user, contra;//para comparar las primeras variables

    cout<<"ingrese su usuario ";
    cin>>usuario;
    cout<<"ingrese su contraseña";
    cin>>contrasena;

    ifstream read(usuario + ".txt");
    getline(read, user);//lee usuario
    getline(read, contra);

    if(user==usuario && contra==contrasena)//this makes the read to see if the user and password matches
    //then gives access
    {
        return true
    }
    else 
    {
        return false
    }
    
}

int main(){
    int opcion;
    cout<< endl;
    cout<< "Menu" <<endl;
    cout<<"---------------" <<enld;
    cout<<"1.registrarse " <<endl;
    cout<<"2.acceder" <<endl;
    cout<< endld;
    cout<<"opción";
    cin>>opcion;

    if (opcion==1)
    {
        string usuario, contrasena;

        cout<<"rellene las opciones " <<endl;
        cout<< endl;
        cout<<"ingrese usuario" <<endl;
        cin>>usuario
        cout<<"ingrese contraseña" <<endl;
        cin>> contrasena;

        ofstream archivo;//crear archivo con información
        archivo.open(usuario +".txt");
        archivo <<usuario <<enld <<contrasena;
        archivo.close();

        cout<< "Bienvenido" <<usuario << "!!" <<endl;
    }
    else if(opcion==2)
    {
        bool estado = InicioSesion();
        
        if(!estado)
        system("clear");
    cout<< endl;
    cout<<"no se pudo acceder a su cuenta"<<endl;
    main ();
    return 0;
    }
    else
    {
        cout<< "Bienvenido!" <<endl;

    }
    return 0;
}