/*******************************************************************************
 * VERSION: 1.0
 * MC658 - Projeto e Análise de Algoritmos III - 1s2018
 * Prof: Flavio Keidi Miyazawa
 * PED: Francisco Jhonatas Melo da Silva
 * Usa ideias e código de Mauro Mulati e Flávio Keidi Miyazawa 
 ******************************************************************************/

/* Atenção: Qualquer alteração neste arquivo não terá efeito no projeto a ser 
 * testado no momento da avaliação. */

#ifndef TSP_P_ALGS_H
#define TSP_P_ALGS_H

#include "tsp_p.h"

bool constrHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl);
bool metaHeur(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl);
bool exact(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl);
bool brkga(const Tsp_P_Instance &l, Tsp_P_Solution  &s, int tl);

#endif
