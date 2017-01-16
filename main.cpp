//
//  main.cpp
//  MAC
//
//  Created by Enrique Lazcorreta Puigmartí on 10/1/17.
//  Copyright © 2017 Enrique Lazcorreta Puigmartí. All rights reserved.
//

#include <iostream>
#include <string>
using std::string;


{
    
    
    {
    }










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
    return 0;
}
