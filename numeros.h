//
//  numeros.h
//  MAC
//
//  Created by Enrique Lazcorreta Puigmartí on 10/1/17.
//  Copyright © 2017 Enrique Lazcorreta Puigmartí. All rights reserved.
//

#ifndef numeros_h
#define numeros_h

#include <string>
using std::string;

#define SEPARADOR_DE_MILES ","

string Miles(size_t numero)
{
    string numero_con_separaciones;
    char digitos_escritos = 0;
    
    while (numero)
    {
        numero_con_separaciones.insert(0, 1, (char)(numero - (numero / 10) * 10 + 48));
        
        numero /= 10;
        
        digitos_escritos++;
        if (digitos_escritos == 3 && numero)
        {
            numero_con_separaciones.insert(0, SEPARADOR_DE_MILES);
            digitos_escritos = 0;
        }
    }
    
    if (numero_con_separaciones.empty())
        numero_con_separaciones.assign("0");
    
    return numero_con_separaciones;
}

#endif /* numeros_h */
