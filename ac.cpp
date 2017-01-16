//
//  ac.cpp
//  MAC
//
//  Created by Enrique Lazcorreta Puigmartí on 10/1/17.
//  Copyright © 2017 Enrique Lazcorreta Puigmartí. All rights reserved.
//

#include "ac.h"

#include <cmath>



//TODO:Eliminar, ni siquiera comprueba que está al principio del archivo.
size_t acArchivo::GetNumLineas()
{
    string registro;
    //Leemos la primera línea
    std::getline(*this, registro);
    
    size_t num_lineas = 1;
    while (good())
    {
        num_lineas++;
        std::getline(*this, registro);
    }
    
    
    return num_lineas;
}



size_t acArchivo::GuardaRegistrosEnRAM()
{
    close();
    open(m_nombre);
    
    size_t num_registros = 0;
    
    string registro;
    
    //Saltamos metadatos en formato KEEL (@) o UCI (#)
    do
    {
        std::getline(*this, registro);
        
    } while (good() && (registro[0] == '@' || registro[0] == '#'));
    
    //Hemos leído el primer registro
    do
    {
        if (registro.length())
        {
            num_registros++;
            if (registro.back() == '\r')
                registro.pop_back();
            m_D[registro]++;
        }
        
        std::getline(*this, registro);
    } while (good());
    
    close();
    m_num_registros.push_back(num_registros);
    m_num_registros_distintos.push_back(m_D.size());
    
    return num_registros;
}









/**
 Lee el dataset
 **/
size_t acArchivo::GetC0()
{
    close();
    open(m_nombre);
    
    size_t num_registros = 0;
    
    string registro;
    
    //Saltamos metadatos en formato KEEL (@) o UCI (#)
    do
    {
        std::getline(*this, registro);
        
    } while (good() && (registro[0] == '@' || registro[0] == '#'));
    
    //Hemos leído el primer registro
    do
    {
        if (registro.length())
        {
            num_registros++;
            if (registro.back() == '\r')
                registro.pop_back();
            if (m_D_en_RAM)
                m_D[registro]++;
            
            //TODO:Ampliar a UCI, en algunos datasets la clase es el primer valor de cada registro
            //La clase es el último valor del registro (en KEEL)
            size_t posicion_ultima_coma = registro.find_last_of(',');
            string clase(registro.substr(posicion_ultima_coma + 1));
            
            string caracterizacion(registro.substr(0, posicion_ultima_coma));
            acCaracterizacion caracterizacion_VECTOR(caracterizacion, m_num_atributos);
            
            size_t atributo = 1;
            m_C1[0].m_rango[clase][clase]++;
            for (auto &valor: caracterizacion_VECTOR)
                m_C1[atributo++].m_rango[valor][clase]++;
        }
        
        std::getline(*this, registro);
    } while (good());
    
    close();
    
    m_num_registros.push_back(num_registros);
    m_num_registros_distintos.push_back(m_D.size());
    
    //TODO:Añadir YA soporte mínimo como argumento de la aplicación.
    m_soporte_minimo = ceil(num_registros * m_soporte_minimo_relativo);
    //Un soporte mínimo nulo obligaría a más comprobaciones en la fase de poda.
    if (!m_soporte_minimo)
        m_soporte_minimo = 1;
    //TODO:Necesito vector<size_t> m_soporte_minimo_de_clase para poder aplicar a cada clase un soporte mínimo diferente. Ver modelización en documentación.
//    if (m_soporte_minimo < 3)
//        m_soporte_minimo = 3;
    
    return num_registros;
}










size_t acArchivo::ReduceD(size_t atributo)
{
    for (auto registro = m_D.begin(); registro != m_D.end(); )
    {
        string contenido(registro->first);
        //La clase es el último valor del registro (en KEEL)
        size_t posicion_ultima_coma = contenido.find_last_of(',');
        string clase(contenido.substr(posicion_ultima_coma + 1)),
               caracterizacion(contenido.substr(0, posicion_ultima_coma));
        
        acCaracterizacion caracterizacion_VECTOR(caracterizacion, m_num_atributos);

//
        if (m_C1[atributo].m_rango[caracterizacion_VECTOR[atributo - 1]].size() == 1)
//        map<string, size_t> mapa_de_clases_hijas = m_C0[atributo].m_C0[caracterizacion_VECTOR[atributo - 1]];
//        if (mapa_de_clases_hijas.size() == 1)
//        {
//            if (mapa_de_clases_hijas.begin()->second >= m_soporte_minimo)
                registro = m_D.erase(registro);
//        }
        else
            ++registro;
    }
    
    size_t num_registros = CuentaNumRegistros();
    m_num_registros.push_back(num_registros);
    m_num_registros_distintos.push_back(m_D.size());
    
    return num_registros;
//    return m_D.size();
}










size_t acArchivo::CuentaNumRegistros()
{
    size_t num_registros = 0;
    for (auto &registro: m_D)
        num_registros += registro.second;
    return num_registros;
}










size_t acArchivo::MuestraC0()
{
    size_t num_atributo = 0;
    
    for (auto &atributo: m_C1)
    {
        size_t num_valores_discriminantes = 0,
               num_registros_discriminados = 0,
               num_VDF = 0,
               num_RDVDF = 0,
               num_ramas = 0;
        
        cout << 'A' << num_atributo++ << endl;
        for (auto &valor: atributo.m_rango)
        {
            if (m_parlanchin)
                cout << valor.first;
            for (auto &clase: valor.second)
            {
                if (m_parlanchin)
                    cout << '\t' << clase.first << " (" << clase.second << ')' << endl;
//                if (valor.second.size() >= m_soporte_minimo)
                if (valor.second.size() == 1)
                {
                    num_valores_discriminantes++;
                    num_registros_discriminados += clase.second;
                    
                    map<string, size_t>::const_iterator clase_apuntada = valor.second.begin();
                    if (clase_apuntada->second >= m_soporte_minimo)
                    {
                        num_VDF++;
                        num_RDVDF += clase.second;
                    }
                }
                
                num_ramas++;
            }
        }
        
        atributo.m_num_valores_discriminantes = num_valores_discriminantes;
        atributo.m_num_registros_discriminados = num_registros_discriminados;
        atributo.m_num_VDF = num_VDF;
        atributo.m_num_RDVDF = num_RDVDF;
        
        cout << "\t" << num_valores_discriminantes << " valores discriminantes (de " << atributo.m_rango.size() << ") que afectan a " << num_registros_discriminados << " registros" << endl;
        cout << "\t\t" << num_valores_discriminantes * 100. / atributo.m_rango.size() << "% de sus valores" << endl;
        cout << "\t\t" << num_registros_discriminados * 100. / m_num_registros[0] << "% de los registros de D"<< endl;

        cout << "\t" << num_VDF << " valores discriminantes fuertes (de " << atributo.m_rango.size() << ") que afectan a " << num_RDVDF << " registros" << endl;
        cout << "\t\t" << num_VDF * 100. / atributo.m_rango.size() << "% de sus valores" << endl;
        cout << "\t\t" << num_RDVDF * 100. / m_num_registros[0] << "% de los registros de D"<< endl;
        
        cout << '\t' << num_ramas << " ramas de un total teórico de " << atributo.m_rango.size() * m_C1[0].m_rango.size() << endl;
        cout << "\t\t" << num_ramas * 100. / (atributo.m_rango.size() * m_C1[0].m_rango.size()) << "%" << endl;
    }
    
    size_t max_num_registros_discriminados = 0,
    atributo_mejor = 0;
    num_atributo = -1;
    for (auto &atributo: m_C1)
    {
        num_atributo++;
        //        cout << 'A' << num_atributo << " :" << atributo.m_num_registros_discriminados << endl;
        //Me salto la clase, que lo discrimina todo.
        if (!num_atributo)
            continue;
        
        if (max_num_registros_discriminados < atributo.m_num_registros_discriminados)
        {
            max_num_registros_discriminados = atributo.m_num_registros_discriminados;
            atributo_mejor = num_atributo;
        }
    }
    
    if (atributo_mejor)
        cout << "El mejor atributo es A" << atributo_mejor << " que elimina " << max_num_registros_discriminados << " registros para la siguiente rama del árbol L" << endl;
    else
        cout << "No hay ningún valor discriminante en D, habrá que usar el criterio del número de Reglas de Asociación Negativas" << endl;
    
    return atributo_mejor;
}










void acArchivo::MuestraValoresDiscriminantes()
{
    size_t num_atributo = 0,
           num_valores_discriminantes = 0;
    
    for (auto &atributo: m_C1)
    {
        if (!num_atributo)
        {
            num_atributo++;
            continue;
        }

        cout << 'A' << num_atributo++ << endl;
        for (auto &valor: atributo.m_rango)
        {
            if (valor.second.size() == 1)
            {
                cout << valor.first;
                auto clase = valor.second.begin();
                cout << '\t' << clase->first << " (" << clase->second << ')' << endl;
                
                num_valores_discriminantes++;
            }
        }
    }
    
    cout << num_valores_discriminantes << " valores discriminantes" << endl;
}










void acArchivo::MuestraValoresDiscriminantesFuertes()
{
    size_t num_atributo = 0,
           num_valores_discriminantes_fuertes = 0;
    
    for (auto &atributo: m_C1)
    {
        if (!num_atributo)
        {
            num_atributo++;
            continue;
        }

        cout << 'A' << num_atributo++ << endl;
        for (auto &valor: atributo.m_rango)
        {
            if (valor.second.size() == 1)
            {
                auto clase = valor.second.begin();
                if (clase->second >= m_soporte_minimo)
                {
                    cout << '\t' << valor.first;
                    cout << '\t' << clase->first << " (" << clase->second << ')' << endl;
                    
                    num_valores_discriminantes_fuertes++;
                }
            }
        }
    }
    
    cout << num_valores_discriminantes_fuertes << " valores discriminantes fuertes" << endl;
}










void acArchivo::MuestraCaracteristicasDCS()
{
    cout << "El dataset " << m_nombre << " tiene " << endl;
    cout << '\t' << m_num_registros[0] << " registros (" << m_num_registros_distintos[0] << " distintos)" << endl;
    cout << '\t' << m_num_atributos << " atributos" << endl;
    cout << '\t' << m_C1[0].m_rango.size() << " clases" << endl;
    
    size_t num_clases_sin_soporte_minimo = 0;
    for (auto &clase: m_C1[0].m_rango)
    {
        auto soporte = clase.second.begin();
        cout << clase.first << "\t\t(" << soporte->second << ')' << endl;
        
        if (soporte->second < m_soporte_minimo)
            num_clases_sin_soporte_minimo++;
    }
    
    if (num_clases_sin_soporte_minimo)
        cout << num_clases_sin_soporte_minimo << " de sus " << m_C1[0].m_rango.size() << " clases no tienen soporte mínimo (fijado en " << m_soporte_minimo << " registros por el analista)" << endl;
    else
        cout << "Todas sus clases tienen soporte mínimo (fijado en " << m_soporte_minimo << " registros por el analista)" << endl;
    
    size_t num_atributo = 0;
    size_t num_total_valores = 0;
    for (auto &atributo: m_C1)
    {
        if (!num_atributo)
        {
            num_atributo++;
            continue;
        }
        
        cout << 'A' << num_atributo++ << endl;
        
        cout << '\t' << atributo.m_rango.size() << " valores" << endl;
        num_total_valores += atributo.m_rango.size();
        cout << '\t' << atributo.m_num_valores_discriminantes << " valores discriminantes (" << atributo.m_num_VDF << " son fuertes)" << endl;
        cout << '\t' << atributo.m_num_registros_discriminados << " registros discriminados (" << atributo.m_num_RDVDF << " por valores fuertes)" << endl;

        size_t num_ramas = 0;
        for (auto &valor: atributo.m_rango)
            for (auto &clase: valor.second)
                num_ramas++;
        
        cout << '\t' << num_ramas << " ramas de un total teórico de " << atributo.m_rango.size() * m_C1[0].m_rango.size() << " (" << num_ramas * 100. / (atributo.m_rango.size() * m_C1[0].m_rango.size()) << "%)" << endl;
    }
    
    cout << num_total_valores << " valores distintos en el dataset" << endl;

}










/***
 Se trata de asignar a cada clase y a cada par atributo=valor un código entero de modo que todos los ítems sean diferentes y puedan ser analizados mediante Minería de Reglas de Asociación.
 
 He de hacerlo en un vector de atributos porque puede repetirse un valor en distintos atributos (verdadero/falso, números enteros...).
 **/
void acArchivo::CodificaClasesYValores()
{
    if (m_codificado)
        return;
    
    int num_codigo = 1;
    size_t num_atributo = 0;
    
    for (auto &atributo: m_C1)
    {
        m_codigo_del_valor.resize(m_num_atributos + 1);
        m_valor_del_codigo.resize(m_num_atributos + 1);
        
        map<string, long> *vector_de_codigos = &(m_codigo_del_valor.at(num_atributo));
        map<long, string> *vector_de_valores = &(m_valor_del_codigo.at(num_atributo++));
        
        for (map<string, map<string, size_t>>::const_iterator it_C0 = atributo.m_rango.begin(); it_C0 != atributo.m_rango.end(); ++it_C0)
        {
            (*vector_de_codigos)[it_C0->first] = num_codigo;
            (*vector_de_valores)[num_codigo] = it_C0->first;
            num_codigo++;
        }
    }

    m_codificado = true;
}










void acArchivo::MuestraCodigos()
{
    if (!m_codificado)
        CodificaClasesYValores();

    size_t num_atributo = 0;
    
    for (auto &atributo: m_C1)
    {
        if (num_atributo)
            cout << 'A' << num_atributo << endl;
        else
            cout << "Clase" << endl;
        
        map<string, long> *vector_de_codigos = &(m_codigo_del_valor.at(num_atributo++));
        
        for (auto &valor: *vector_de_codigos)
            cout << '\t' << valor.first << " -> " << valor.second << endl;
    }
}










/***
 Se trata de transformar cada registro de D en una transacción, sustituyendo cada clase o par atributo=valor por su código, obtenido a partir de C0, que diferencie a todos los ítems sin importar en qué posición están situados. Utilizo códigos enteros.
 **/
bool acArchivo::CodificaD()
{
    bool hecho = false;
    
    string extension(m_nombre.substr(m_nombre.find_last_of('.') + 1));
    string nombre_archivo_codificado(m_nombre.substr(0, m_nombre.find_last_of('.')));
    nombre_archivo_codificado.append("-ARM.");
    nombre_archivo_codificado.append(extension);
    
    cout << "Archivo codificado en " << nombre_archivo_codificado << endl;
    
    std::ofstream archivo_codificado(nombre_archivo_codificado);
    if (!archivo_codificado.is_open())
        throw "ERROR: No he podido crear el archivo " + nombre_archivo_codificado + "\n\n";

    //Metadatos
    size_t posicion_nombre = m_nombre.find_last_of("\\/") + 1;
    size_t posicion_ultimo_punto = m_nombre.find_last_of('.');
    archivo_codificado << "@relation " << m_nombre.substr(posicion_nombre, posicion_ultimo_punto - posicion_nombre) << endl;
    
    archivo_codificado << "@registros " << m_num_registros[0] << " (";
    if (m_num_registros[0] == m_num_registros_distintos[0])
        archivo_codificado << "TODOS";
    else
        archivo_codificado << m_num_registros_distintos[0];
    archivo_codificado << " distintos)" << endl;
    
    for (size_t atributo = 1; atributo <= m_num_atributos; atributo++)
        archivo_codificado << "@A" << atributo << " \t" << m_codigo_del_valor[atributo].begin()->second << "..." << m_codigo_del_valor[atributo].rbegin()->second << "   \t(" << m_codigo_del_valor[atributo].size() << " valores)" << endl;
    archivo_codificado << "@Clase \t" << m_codigo_del_valor[0].begin()->second << "..." << m_codigo_del_valor[0].rbegin()->second << "   \t(" << m_codigo_del_valor[0].size() << " valores)" << endl;
    
    if (m_D_en_RAM)
    {
        if (!m_codificado)
            CodificaClasesYValores();

        for (auto registro_con_soporte = m_D.begin(); registro_con_soporte != m_D.end(); ++registro_con_soporte)
        {
            string registro(registro_con_soporte->first);
            size_t soporte(registro_con_soporte->second);
            
            //TODO:He de crear método que separe clase de valores de diferentes formatos
            //La clase es el último valor del registro (en KEEL)
            size_t posicion_ultima_coma = registro.find_last_of(',');
            string clase(registro.substr(posicion_ultima_coma + 1));
            long clase_codificada = m_codigo_del_valor[0][clase];
            
            string caracterizacion(registro.substr(0, posicion_ultima_coma));
            acCaracterizacion caracterizacion_VECTOR(caracterizacion, m_num_atributos);
            vector<long> caracterizacion_VECTOR_codificada;
            
            int num_atributo = 1;
            for (auto &valor: caracterizacion_VECTOR)
            {
                long valor_codificado = m_codigo_del_valor[num_atributo++][valor];
                caracterizacion_VECTOR_codificada.push_back(valor_codificado);
            }
//            caracterizacion_VECTOR_codificada.push_back(clase_codificada);
            
            m_D_codificado_para_ARM[caracterizacion_VECTOR_codificada] = soporte;
            
            for (int num_repeticiones = 0; num_repeticiones < soporte; num_repeticiones++)
            {
                for (auto &codigo: caracterizacion_VECTOR_codificada)
                    archivo_codificado << codigo << ',';
                archivo_codificado << clase_codificada << endl;
//                //TODO:Corregir, estoy borrando la última coma
//                archivo_codificado << '\b';
//                archivo_codificado << endl;
            }
            
//            ++registro;
        }
    }
//    else
//    {
//        GetC0();
//    }
    
    archivo_codificado.close();
    
    return hecho;
}
