/*******************************************************************************
 * VERSION: 1.0
 * MC658 - Projeto e Análise de Algoritmos III - 1s2018
 * Prof: Flavio Keidi Miyazawa
 * PED: Francisco Jhonatas Melo da Silva
 * Usa ideias e código de Mauro Mulati e Flávio Keidi Miyazawa
 ******************************************************************************/

/* Atenção: Qualquer alteração neste arquivo não terá efeito no projeto a ser
 * testado no Momento da avaliação. */

#ifndef TSP_P__H
#define TSP_P__H

#include "mygraphlib.h"
typedef ListDigraph::NodeMap<ListDigraph::Node> DNodeNodeMap;
typedef ListDigraph::NodeMap<int> DNodeMap;

typedef enum ENUM_ALG
{
   NONE,
   CONSTR_HEUR,
   META_HEUR_VIZ,
   EXACT,
   META_BRKGA,
} ALG;

typedef enum ENUM_VERBOSITY
{
   QUIET,
   VERB,
   GRAPH
} VERBOSITY;

typedef struct structParams
{
   ALG       alg;
   int       timeLimit;
   VERBOSITY verbosity;
   string    inputFile;
   string    outputFile;
} Params;

typedef struct structItem
{
   int i;
   DNode s;
   DNode t;
   double w;
} Item;

class Tsp_P_Instance
{
public:
   Tsp_P_Instance(Digraph        &pg,
                  DNodeStringMap &pvname,
                  ArcValueMap    &pweight,
                  DNodePosMap    &pposx,
                  DNodePosMap    &pposy,
                  DNode           pdepot,
                  DNodeValueMap  &pweight_node);

   ListDigraph    &g;
   int             n,
                   m;
   DNodeStringMap &vname;
   ArcStringMap    aname;
   ArcValueMap    &weight;
   DNodePosMap    &posx;
   DNodePosMap    &posy;
   DNodeValueMap  &weight_node;

   DNode           depot;

};

class Tsp_P_Solution
{
public:
   Tsp_P_Solution();
   vector<DNode> tour;
   double        lowerBound;
   double        cost;
   double        upperBound;
};

typedef enum ENUM_SOLUTION_STATUS
{
   NOT_FOUND_FEASIBLE_SOLUTION,
   INCOMPATIBLES_COST_AND_OPTIMAL,
   INVALID_DNODE,
   FIRST_IS_NOT_DEPOT,
   ARC_MISSING,
   NEGATIVE_LOAD_ERROR,
   REMAINING_LOAD_ERROR,
   COST_ERROR,
   COST_BOUND_ERROR,
   INVALID_BOUNDS_OPT,
   REPEATED_NODES_IN_TOUR,
   OK
} SOLUTION_STATUS;

void            readCheckParams(Params &params, int argc, char *argv[]);
void            showUsage();
void            PulaBrancoComentario(ifstream &ifile);  // Implemented in mygraphlib.cpp
bool            readListDigraphTspP(string          filename,
                                      ListDigraph    &g,
                                      DNodeStringMap &vname,
                                      ArcValueMap    &weight,
                                      DNodePosMap    &posx,
                                      DNodePosMap    &posy,
                                      const bool      dupla,
                                      DNode          &depot,
                                      DNodeValueMap &weight_node);  // Different version of the one in mygraphlib
SOLUTION_STATUS checkSolutionStatus(Tsp_P_Instance &instance,
                                    Tsp_P_Solution &sol,
                                    bool optimal);
string          decodeSolutionStatus(SOLUTION_STATUS solutionStatus);
void            solutionAsGraphical(Tsp_P_Instance &l, Tsp_P_Solution  &s, string inputFile);
string          instanceAsString(Tsp_P_Instance &Tsp_P_Instance);
string          instanceDescriptionAsString(Tsp_P_Instance &instance);
string          dnodesAndItemsAsString(Tsp_P_Instance &l);
string          arcsAndItemsAsString(Tsp_P_Instance &l);
string          solutionAsString(Tsp_P_Instance &Tsp_P_Instance, Tsp_P_Solution  &Tsp_P_Solution);
string          tourAsString(Tsp_P_Instance &Tsp_P_Instance, Tsp_P_Solution  &Tsp_P_Solution);
string          valuesAsString(Tsp_P_Solution &Tsp_P_Solution);
string          resultAsString(Tsp_P_Instance  &Tsp_P_Instance,
                              Tsp_P_Solution  &Tsp_P_Solution,
                              Params          &params,
                              bool             optimal,
                              SOLUTION_STATUS  solutionStatus,
                              int              elapsedTime);
string          decodeAlg(ALG alg);

#endif
