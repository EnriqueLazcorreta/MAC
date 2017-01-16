//
//  main.cpp
//  MAC
//
//  Created by Enrique Lazcorreta Puigmartí on 10/1/17.
//  Copyright © 2017 Enrique Lazcorreta Puigmartí. All rights reserved.
//

#include "funciones.h"
#include "ac.h"
#include "numeros.h"

#include <iostream>

#include <string>
using std::string;



void UsoCorrecto(const char *nombre_aplicacion)
{
    string nombre(nombre_aplicacion);
    
    cout << "Uso:\n\n\t" << nombre.substr(nombre.find_last_of("/") + 1)
         << " nombre-dataset"
         << " [--sm:X]"
         << " [--D-en-RAM]"
         << " [--parlanchin]"
         << endl << endl
         << "Argumentos:\n\n"
         << "\tnombre-dataset \tNombre del dataset a analizar\n\n"
         << "\t--sm:X \tX es el soporte mínimo relativo, un número real entre 0 y 100 (5 por defecto)\n\n"
         << "\t--D-en-RAM \tSe guarda el dataset en un mapa en memoria RAM, cuidado con datasets muy grandes\n\n"
         << "\t--parlanchin \tLa aplicación muestra en pantalla los resultados que va obteniendo (no lo hace por defecto)\n\n"
         << endl;
}





bool ProcesaArgumentos(int argc,
                       const char * argv[],
                       string *nombre_dataset,
                       double *soporte_minimo,
                       bool *D_en_RAM,
                       bool *parlanchin)
{
    if (argc == 1)
        return false;
    
    nombre_dataset->assign(argv[1]);
    
    for (int arg = 2; arg < argc; arg++)
    {
        string argumento(argv[arg]);

        if (!argumento.compare("--verbose"))
            *parlanchin = true;
        else if (!argumento.compare("--D-en-RAM"))
            *D_en_RAM = true;
        else if (!argumento.substr(0, 4).compare("--s:"))
            //TODO:Leo el soporte mínimo como número real, añadir control de errores
            *soporte_minimo = stod(argumento.substr(4));
    }
    
    if ((*soporte_minimo) < 0. || (*soporte_minimo > 100.))
    {
        cout << "\n\nEl soporte ha de ser un número real entre 0 y 100\n\n";
        return false;
    }
    
    cout << *soporte_minimo << endl;
    
    return true;
}










int main(int argc, const char * argv[])
{
    string nombre_dataset;
    bool parlanchin = false,
         D_en_RAM = false;
    double soporte_minimo = 0.01;
    
    if (!ProcesaArgumentos(argc,
                           argv,
                           &nombre_dataset,
                           &soporte_minimo,
                           &D_en_RAM,
                           &parlanchin))
    {
        UsoCorrecto(argv[0]);
        return 1;
    }
    
    DECLARA_E_INICIA_RELOJ_TOTAL
    DECLARA_E_INICIA_RELOJ
    
    acArchivo *archivo;
    try
    {
        archivo = new acArchivo(nombre_dataset,
                                soporte_minimo,
                                D_en_RAM,
                                parlanchin);
    }
    catch (string &e)
    {
        cout << e << endl;
        return 1;
    }
    
//    INICIA_RELOJ
//    cout << "Tiene " << Miles(archivo->GetNumLineas()) << " líneas";
//    cout << " [" << CALCULA_TIEMPO_TRANSCURRIDO << " sg.]" << endl;
    
//    INICIA_RELOJ
//    cout << "Tiene " << Miles(archivo->GuardaRegistrosEnRAM()) << " registros, " << Miles(archivo->GetNumRegistrosDistintos()) << " distintos";
//    cout << " [" << CALCULA_TIEMPO_TRANSCURRIDO << " sg.]" << endl;
    
    INICIA_RELOJ
    cout << "Tiene " << Miles(archivo->GetC0()) << " registros, " << Miles(archivo->GetNumRegistrosDistintos(0)) << " distintos";
    cout << " [" << CALCULA_TIEMPO_TRANSCURRIDO << " sg.]" << endl;
    
    cout << endl << "C0" << endl;
    INICIA_RELOJ
    size_t atributo_mejor = archivo->MuestraC0();
    cout << endl << "[" << CALCULA_TIEMPO_TRANSCURRIDO << " sg.]" << endl;
    
    /*
    if (atributo_mejor)
    {
        INICIA_RELOJ
        //    cout << archivo->GetNumRegistrosDistintos() << " registros en D tras eliminar los " << archivo->ReduceD(5) << " registros discriminados por A" << 5 << endl;
        
        
        size_t nuevo_tamaño_D = archivo->ReduceD(atributo_mejor);
        cout << Miles(nuevo_tamaño_D) << " registros en D tras eliminar los registros discriminados por A" << atributo_mejor << " (reducido " << 100. - nuevo_tamaño_D * 100. / archivo->GetNumRegistros(0) << "% su tamaño original)" << endl;
        cout << endl << "[" << CALCULA_TIEMPO_TRANSCURRIDO << " sg.]" << endl;
        
        cout << endl << "Los " << Miles(archivo->GetNumRegistros(0)) << " registros iniciales se convierten en " << Miles(archivo->GetNumRegistros(1)) << endl;
        cout << endl << "Los " << Miles(archivo->GetNumRegistrosDistintos(0)) << " registros DISTINTOS iniciales se convierten en " << Miles(archivo->GetNumRegistrosDistintos(1)) << endl;
    }
     */
    
    cout << endl << "Valores discriminantes:" << endl;
    archivo->MuestraValoresDiscriminantes();
    cout << endl << "Valores discriminantes FUERTES:" << endl;
    archivo->MuestraValoresDiscriminantesFuertes();
    
    archivo->MuestraCaracteristicasDCS();
    
    
    
    cout << "Códigos:" << endl;
    archivo->MuestraCodigos();
    
    archivo->CodificaD();
    
    
    
    cout << endl << TIEMPO_TRANSCURRIDO_TOTAL << " sg. [total]" << endl;

    delete archivo;

    return 0;
}
