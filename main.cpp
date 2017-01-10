//
//  main.cpp
//  MAC
//
//  Created by Enrique Lazcorreta Puigmartí on 10/1/17.
//  Copyright © 2017 Enrique Lazcorreta Puigmartí. All rights reserved.
//

#include <iostream>
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
using std::cout;
using std::endl;

bool m_parlanchin = true;

class acArchivo : public ifstream
{
    string m_nombre;
public:
    acArchivo(string nombre) : m_nombre(nombre)
    {
        open(m_nombre);
        if (!is_open())
            throw ("No he podido abrir el archivo " + m_nombre);
        if (m_parlanchin) {
            cout << "Archivo " << m_nombre << " abierto" << endl;
        }
    }
    ~acArchivo() { close(); }
};










int main(int argc, const char * argv[])
{
    acArchivo *archivo;
    try
    {
        archivo = new acArchivo("/Users/enriquelazcorretapuigmarti/Dropbox/investigacion-2016/ACDC/datos/_KEEL/abalone.dat");
    }
    catch (string &e)
    {
        cout << e << endl;
        return 1;
    }

    delete archivo;
    
    cout << "Hello, World!\n";
    return 0;
}
