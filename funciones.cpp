//
//  funciones.cpp
//  CCCC_v0
//
//  Created by Enrique Lazcorreta Puigmartí on 25/8/16.
//  Copyright © 2016 Enrique Lazcorreta Puigmartí. All rights reserved.
//

#include "funciones.h"

double getCPUtime(void)
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    struct timeval tim = ru.ru_stime;
    double t = (double)tim.tv_sec + (double)tim.tv_usec / 1000.;
    return t;
}

long long total_cpu_time_consumed_in_usecs(bool for_completed_child_processes)
{
    rusage usage ;
    getrusage( for_completed_child_processes ? RUSAGE_CHILDREN : RUSAGE_SELF, &usage ) ;
    enum : long long { USECS_PER_SEC = 1000000 } ;
    long long tiempo = usage.ru_utime.tv_sec * USECS_PER_SEC + usage.ru_utime.tv_usec // user
    + usage.ru_stime.tv_sec * USECS_PER_SEC + usage.ru_stime.tv_usec ; // system
    return tiempo;
}



size_t getRAM(void)
{
    struct rusage ru;
    getrusage(RUSAGE_SELF, &ru);
    
    return (size_t)ru.ru_maxrss;
}



string HazLegibleParaHumanos(size_t tamanyo_archivo)
{
    int medida = 0;  //0: bytes 1:Kb 2:Mb
    float tamanyo = (float)tamanyo_archivo;
    while (tamanyo > 1024 && medida < 4) {
        tamanyo /= 1024;
        medida++;
    }
    
    string resultado;
    char buffer[10];
    if (!medida)
        sprintf(buffer, "%zu ", (size_t)tamanyo); //        itoa(atr, buffer, 10);
    else
        sprintf(buffer, "%.2f ", tamanyo); //        itoa(atr, buffer, 10);
    resultado.append(buffer);
    
    switch (medida) {
        case 0: // Bytes
            resultado.append("Bytes");
            break;
            
        case 1: // KB
            resultado.append("KB");
            break;
            
        case 2: // MB
            resultado.append("MB");
            break;
            
        case 3: // GB
            resultado.append("GB");
            break;
            
        case 4: // TB
            resultado.append("TB");
    }
    
    return resultado;
}


