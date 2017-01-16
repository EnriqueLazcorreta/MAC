//
//  ac.h
//  MAC
//
//  Created by Enrique Lazcorreta Puigmartí on 10/1/17.
//  Copyright © 2017 Enrique Lazcorreta Puigmartí. All rights reserved.
//

#ifndef ac_h
#define ac_h

#include <iostream>
using std::cout;

#include <fstream>
using std::ifstream;

#include <string>
using std::string;
using std::endl;

#include <map>
using std::map;
using std::pair;

#include <vector>
using std::vector;


//#include "numeros.h"









class acApriori;
class acL;
class acNodoL
{
    string m_item;
    acL *m_hijos;
};

class acL
{
    size_t m_soporte;
    vector<acNodoL> m_nodos;
    
public:
    acL(size_t soporte_inicial = 1) : m_soporte(soporte_inicial)
    {}
    
    //Busca un itemset en L
    const acL *Busca(const vector<string> &itemset, vector<string>::const_iterator item) const;
    
    //Incrementa el soporte de los candidatos presentes en L
    void Incrementa(vector<string>::const_iterator it_basket_upper_bound,
                    vector<string>::const_iterator it_basket,
                    size_t incremento = 1);
    
friend class acApriori;
};

class acApriori
{
    acL m_L;
};










class acClase : public string
{
    
};









/**
 Clase para guardar la caracterización, leída en el dataset como un string de valores separados por comas.
 **/
class acCaracterizacion : public vector<string>
{
public:
    acCaracterizacion(string caracterizacion, size_t num_atributos)
    {
        resize(num_atributos);
        
        size_t inicio_valor = caracterizacion.find_first_not_of(','),
               fin_valor = caracterizacion.find_first_of(',');

        for (size_t atributo = 0; atributo < num_atributos; atributo++)
        {
            string valor = caracterizacion.substr(inicio_valor, fin_valor - inicio_valor);
            //TODO:Un iterador o puntero sería más eficiente
            at(atributo) = valor;
            inicio_valor = fin_valor + 1;
            fin_valor = caracterizacion.find_first_of(',', inicio_valor);
        }
    }
    acCaracterizacion(size_t num_atributos)
    {
        resize(num_atributos);
    }
};










class acRegistro : public pair<acCaracterizacion, acClase>
{
public:
//    acRegistro(acCaracterizacion caracterizacion, acClase clase)
//    {
//        first = caracterizacion;
//        second = clase;
//    }
    
};










class acArchivo;
class acAtributo
{
friend class acArchivo;
    string m_nombre;
    map<string, map<string, size_t>> m_rango;
    size_t m_num_valores_discriminantes = 0,
           m_num_VDF = 0,               //Valores Discriminantes Fuertes
           m_num_registros_discriminados = 0,
           m_num_RDVDF = 0;             //Registros Discriminados por VDF
};










class acArchivo : public ifstream
{
    string m_nombre;

    bool m_parlanchin = false;
    
    size_t m_num_atributos = 0;
    
    //Conforme avanza el algoritmo va decreciendo el tamaño de D.
    vector<size_t> m_num_registros_distintos,
                   m_num_registros;
    
    bool m_D_en_RAM = false;
    map<string, size_t> m_D;
    map<vector<long>, size_t> m_D_codificado_para_ARM;
//    map<acRegistro, size_t> m_D_bis;
    vector<acAtributo> m_C1;
    
    size_t m_soporte_minimo;
    double m_soporte_minimo_relativo;
    
    vector<map<string, long>> m_codigo_del_valor;
    vector<map<long, string>> m_valor_del_codigo;
    bool m_codificado = false;
    
public:
    acArchivo(string nombre,
              double soporte_minimo_relativo = 5,
              bool D_en_RAM = false,
              bool parlanchin = true) : m_nombre(nombre),
                                        m_soporte_minimo_relativo(soporte_minimo_relativo),
                                        m_D_en_RAM(D_en_RAM),
                                        m_parlanchin(parlanchin)
    {
        open(m_nombre);
        if (!is_open())
            throw ("No he podido abrir el archivo " + m_nombre);
        
        if (m_parlanchin)
            cout << "Archivo " << m_nombre << " abierto" << endl;
        
        //Obtengo el número de atributos
        string registro;
        //Saltamos metadatos en formato KEEL
        do
        {
            std::getline(*this, registro);
            
        } while (good() && registro[0] == '@');
        m_num_atributos = 0;
        for (auto &c: registro)
            if (c == ',')
                m_num_atributos++;
        if (m_parlanchin)
            cout << "Tiene " << m_num_atributos << " atributos" << endl;
        
        //Guardamos en m_C1 el rango de la clase y de todos los atributos
        m_C1.resize(m_num_atributos + 1);
        
        close();
    }
    
    ~acArchivo()
    {
        close();
    }
    
    size_t GetNumLineas();
    
    size_t GuardaRegistrosEnRAM();
    
    size_t GetNumRegistros(const size_t nivel) { return m_num_registros[nivel]; }
    size_t GetNumRegistrosDistintos(const size_t nivel) { return m_num_registros_distintos[nivel]; }

    size_t GetC0();
    
    //TODO:Añadir (ostream &salida = cout) como parámetro en todas las funciones MuestraX() para poder pasar un ofstream si lo quiero escribir en disco.
    size_t MuestraC0();
    void MuestraValoresDiscriminantes();
    void MuestraValoresDiscriminantesFuertes();
    void MuestraCaracteristicasDCS();
    
    size_t ReduceD(size_t atributo);
    
    void CodificaClasesYValores();
    bool CodificaD();
    void MuestraCodigos();

    
private:
    size_t CuentaNumRegistros();
};








#endif /* ac_h */
