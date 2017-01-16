//
//  funciones.hpp
//  CCCC_v0
//
//  Created by Enrique Lazcorreta Puigmartí on 25/8/16.
//  Copyright © 2016 Enrique Lazcorreta Puigmartí. All rights reserved.
//

#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <ctime>
#define DECLARA_E_INICIA_RELOJ_TOTAL clock_t start_TOTAL = clock();
#define TIEMPO_TRANSCURRIDO_TOTAL ((clock() - start_TOTAL) / double(CLOCKS_PER_SEC))

#define DECLARA_E_INICIA_RELOJ clock_t start = clock();
#define INICIA_RELOJ start = clock();
#define CALCULA_TIEMPO_TRANSCURRIDO ((clock() - start) / double(CLOCKS_PER_SEC))


//#define __MS_WINDOWS__
//#ifdef __MS_WINDOWS__
//#define SEPARADOR_RUTAS "\\"
//#else
#define SEPARADOR_RUTAS "/"

#include <sys/time.h>
#include <sys/resource.h>

#include <string>
using std::string;


double getCPUtime(void);

long long total_cpu_time_consumed_in_usecs(bool for_completed_child_processes = false);

size_t getRAM(void);

string HazLegibleParaHumanos(size_t tamanyo_archivo);


#endif /* FUNCIONES_H */
