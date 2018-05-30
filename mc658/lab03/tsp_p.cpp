/*******************************************************************************
 * VERSION: 1.0
 * MC658 - Projeto e Análise de Algoritmos III - 1s2018
 * Prof: Flavio Keidi Miyazawa
 * PED: Francisco Jhonatas Melo da Silva
 * Usa ideias e código de Mauro Mulati e Flávio Keidi Miyazawa 
  ******************************************************************************/

/* Atenção: Qualquer alteração neste arquivo não terá efeito no projeto a ser 
 * testado no momento da avaliação. */

#include <climits>  // For INT_MAX
#include <set>
#include <ctime>  // For CLOCKS_PER_SEC
#if __cplusplus >= 201103L
#include <unordered_map>
#else
#include <tr1/unordered_map>
#endif
#include <lemon/list_graph.h>
#include "mygraphlib.h"
#include "myutils.h"
#include "tsp_p.h"
#include "tsp_p_algs.h"

using namespace lemon;
using namespace std;

//------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
   Params params;
   readCheckParams(params, argc, argv);

   // Variables that represent the TSP-P
   Digraph        g;
   DNodeStringMap vname(g);
   ArcValueMap    weight(g);
   DNodeValueMap  weight_node(g);
   DNodePosMap    posx(g);
   DNodePosMap    posy(g);
       
   DNode          depot;// node s
   

   // Read the directed graph of the provided instance
   if(!readListDigraphTspP(params.inputFile, g, vname, weight, posx, posy, false, depot, weight_node)){
     cerr << "Erro na leitura do arquivo de entrada " << params.inputFile << endl;
     exit(1);
   }
   
   // Initialize the TSP-P instance with the read values
   Tsp_P_Instance l(g, vname, weight, posx, posy, depot, weight_node);
   
   // Initialize a solution
   Tsp_P_Solution ts;

    
   if(params.verbosity == VERB){  // if(params.verbosity == VERB || params.verbosity == GRAPH){
      cout << "INPUT" << endl;
      cout << instanceAsString(l) << endl;
   }
   else if(params.verbosity == GRAPH){
      cout << "INPUT" << endl;
      cout << instanceDescriptionAsString(l) << endl;
   }
   
   double  elapsedTime = DBL_MAX;
   clock_t before  = clock();
   
   bool optimal = false;
   
   switch(params.alg){
      case CONSTR_HEUR:{
         optimal = constrHeur(l, ts, params.timeLimit);
         break;
      }
      case META_HEUR_VIZ:{
         optimal = metaHeur(l, ts, params.timeLimit);
         break;
      }
      case BRKGA:{
         optimal = brkga(l, ts, params.timeLimit);
         break;
      }
      case EXACT:{
         optimal = exact(l, ts, params.timeLimit);
         break;
      }
   }
   
   clock_t after = clock();
   elapsedTime = (double) (after - before) / CLOCKS_PER_SEC;

  
   // Check if it is indeed a solution for the instance
   SOLUTION_STATUS solutionStatus = checkSolutionStatus(l, ts, optimal);
   
   // Print the result in the outputFile
   ofstream ofsOutputFile;
   ofsOutputFile.open(params.outputFile);
   ofsOutputFile << resultAsString(l, ts, params, optimal, solutionStatus, elapsedTime) << flush;
   ofsOutputFile.close();
   
   if(params.verbosity == VERB || params.verbosity == GRAPH){
      // Print the result in the screen
      cout << "OUTPUT" << endl;
      cout << resultAsString(l, ts, params, optimal, solutionStatus, elapsedTime) << flush;
      // Show a graphical solution
      if(params.verbosity == GRAPH){
         solutionAsGraphical(l, ts, params.inputFile);
      }
      cout << endl;
   }
   
   return solutionStatus;
   
  
}
//------------------------------------------------------------------------------
Tsp_P_Instance::Tsp_P_Instance(ListDigraph    &pg,
                               DNodeStringMap &pvname,
                               ArcValueMap    &pweight,
                               DNodePosMap    &pposx,
                               DNodePosMap    &pposy,
                               DNode           pdepot,
                               DNodeValueMap  &pweight_node):
                               g(pg),
                               vname(pvname),
                               aname(pg),
                               weight(pweight),
                               posx(pposx),
                               posy(pposy),
                               weight_node(pweight_node),
                               depot(pdepot)
{
   n = countNodes(g);
   m = countArcs(g);

}
//------------------------------------------------------------------------------
Tsp_P_Solution::Tsp_P_Solution()
{
   tour.clear();
   lowerBound = 0.0;
   cost       = DBL_MAX;
   upperBound = DBL_MAX;
}
//------------------------------------------------------------------------------
void readCheckParams(Params &params, int argc, char *argv[])
{
   params.alg        = NONE;
   params.timeLimit  = 0;
   params.verbosity  = QUIET;
   params.inputFile  = "";
   params.outputFile = "";

   // Read
   for(int i = 1; i < argc; i++){
      const string arg(argv[i]);
      string next;
      if((i+1) < argc){
         next = string(argv[i+1]);
      }
      else{
         next = string("");
      }
      
      if(params.alg != NONE && (arg.find("-c") == 0 || arg.find("-n") == 0 || arg.find("-e") == 0 || arg.find("-b") == 0)){
         cerr << "Erro ao ler parametro \"" << arg << "\": pode haver somente um parametro de modo de execucao" << endl;
         showUsage();
         exit(1);
      }
      else if(arg.find("-e") == 0){
         params.alg = EXACT;
         continue;
      }
      else if(arg.find("-c") == 0){
         params.alg = CONSTR_HEUR;
         continue;
      }
      else if(arg.find("-b") == 0){
         params.alg = BRKGA;
         continue;
      }
      else if(arg.find("-m") == 0){
         params.alg = META_HEUR_VIZ;
         continue;
      }
      
      if(arg.find("-t") == 0 && next.size() > 0){
         params.timeLimit = atoi(next.c_str()); 
         i++;
         continue;
      }
      
      if(params.verbosity != QUIET && (arg.find("-v") == 0 || arg.find("-g") == 0)){
         cerr << "Erro ao ler parametro \"" << arg << "\": pode haver somente um parametro de modo de execucao" << endl;
         showUsage();
         exit(1);
      }
      else if(arg.find("-v") == 0){
         params.verbosity = VERB;
         continue;
      }
      else if(arg.find("-g") == 0){
         params.verbosity = GRAPH;
         continue;
      }
      
      if(arg.find("-i") == 0 && next.size() > 0){
         params.inputFile = next; 
         i++; 
         continue;
      }
      
      if( arg.find("-o") == 0 && next.size() > 0){
         params.outputFile = next; 
         i++; 
         continue;
      }
      
      cerr << "Parametro invalido: \"" << arg << "\"" << " (ou parametro faltando)" << endl;
      showUsage();
      exit(1);
   }

   // Check
   if(params.alg == NONE){
      cerr << "Deve ser selecionado exatamente um algoritmo" << endl;
      showUsage(); 
      exit(1);
   }
   if(params.inputFile.size() < 1){
      cerr << "Nome de arquivo de entrada invalido" << endl;
      showUsage(); 
      exit(1);
   }
   if(params.outputFile.size() < 1){
      cerr << "Nome de arquivo de saida invalido" << endl;
      showUsage(); 
      exit(1);
   }
   if(params.timeLimit == 0){
      params.timeLimit = 30; 
   }
}
//------------------------------------------------------------------------------
void showUsage()
{
   cout << "Uso: \n"
        << "./tsp_p.e (-c|-m|-b|-e) -i <in> -o <out> [-t <time>] [-v|-g]\n"
        << "Onde:\n"
        << "-c|-m|-b|-e  Seleciona exclusivamente:\n"
        << "          -c: heurística construtiva\n"
        << "          -m: estratégias heurísticas baseadas em grafo de vizinhança e/ou busca local\n"
        << "          -b: BRKGA\n"
        << "          -e: algoritmo exato\n"
        << "-t <time> Informa o tempo limite em segundos dado por <time>. Caso não seja informado, considera-se 30s.\n"
        << "-i <in>   Informa arquivo de entrada <in>\n"
        << "-o <out>  Informa arquivo de saida <out>\n"
        << "-v|-g     Adicionalmente, produz saídas detalhadas: -v para apenas texto ou -g para também mostrar gráfico.\n"
        << flush;
}
//------------------------------------------------------------------------------
bool readListDigraphTspP(string          filename,
                           ListDigraph    &g,
                           DNodeStringMap &vname,
                           ArcValueMap    &weight,
                           DNodePosMap    &posx,
                           DNodePosMap    &posy,
                           const bool      dupla,
                           DNode          &depot,
                           DNodeValueMap   &weight_node)
{
  ifstream ifile;
  int i,n,m;
  double peso;
  Arc a;
  char nomeu[100],nomev[100];
  string STR;
  DNode u,v;
#if __cplusplus >= 201103L
  std::unordered_map<string,DNode> string2node,test;
#else
  std::tr1::unordered_map<string,DNode> string2node;
#endif

  ifile.open(filename.c_str());
  if (!ifile) {cerr << "File '" << filename << "' does not exist.\n"; exit(0);}
  PulaBrancoComentario(ifile);
  ifile >> n;    ifile >> m; // first line have number of nodes and number of arcs
  if (m<0 || ifile.eof())
    { cerr<<"File "<<filename<<" is not a digraph given by arcs.\n"; exit(0);}

  for (i=0;i<n;i++) {
    getline(ifile,STR);
    if (ifile.eof()) {cerr<<"Reached unexpected end of file "<<filename<<".\n";exit(0);}
    while (STR=="") getline(ifile,STR);
    {
      string token;
      istringstream ins; // Declare an input string stream.
      ins.str(STR);        // Specify string to read.
      int nt = 0;
      while( getline(ins, token, ' ') ) {
   // format: <node_name>  <pos_x>  <pos_y>  < priority_v >
   if (nt==0) {
     auto test = string2node.find(token);

     if (test!=string2node.end()){cerr<<"ERROR: Repeated node: "<<nomev<<endl;exit(0);}
     v = g.addNode(); string2node[token] = v; vname[v] = token;}
   else if (nt==1) { posx[v] = atof(token.c_str());}
   else if (nt==2) { posy[v] = atof(token.c_str());}
   else if (nt==3) { weight_node[v] = atof(token.c_str());}
   nt++;
      }
    }
  }
  for (i=0;i<m;i++) {
    // format: <node_source>   <node_target>   <arc_weight>
    ifile >> nomeu;  ifile >> nomev; ifile >> peso;
    if (ifile.eof()) 
      {cerr << "Reached unexpected end of file " <<filename << ".\n"; exit(0);}
    auto test = string2node.find(nomeu);
    if (test == string2node.end()) {cerr<<"ERROR: Unknown node: "<<nomeu<<endl;exit(0);}
    else u = string2node[nomeu];
    
    test = string2node.find(nomev);
    if (test == string2node.end()) {cerr<<"ERROR: Unknown node: "<<nomev<<endl;exit(0);}
    else v = string2node[nomev];
    a = g.addArc(u,v); weight[a] = peso;
    if (dupla) {a = g.addArc(v,u);   weight[a] = peso;}
  }
  
  // Begin of specific parts of the TSP-P
  ifile >> nomeu;
  auto t = string2node.find(nomeu);
  if(t == string2node.end()){
    cerr << "ERROR: Unknown node: " << nomeu << endl;
    exit(1);
  }
  else{
    depot = string2node[nomeu];
  }

    
  // End
  
  ifile.close();
  return(true);
}


//------------------------------------------------------------------------------
SOLUTION_STATUS checkSolutionStatus(Tsp_P_Instance &instance,
                                    Tsp_P_Solution &sol,
                                    bool optimal)
/* 
   (0) Verifica sobre factibilidade e otimalidade retornada. Note que, se for informada que a 
 *     solução não é factível, verifica-se o flag optimal, mas nada mais é verificado.
 *     Mesmo assim, o programa vai mostrar a solução, na esperança de ajudar a realizar 
 *     algum debug ou melhoria.
 *     Portanto, o tour da solução gerada nesse caso não precisa estar vazio, 
 *     pode ter um tour parcial para ser visualizado.
 *     Outra opção, apenas para debug, é manter o sol.cost infactível encontrado, 
 *     e ver que erro é retornado. Entretanto, pode ocorrer erro na execução do programa, 
 *     se o tour fornecido não fizer sentido.
 * (1) Rota $R$ iniciando e terminando no vértice $s$:
 *     - Verificar se primeiro vértice do tour é o vértice s
 *     - Verificar se existe arco na instancia ligando vértice do tour com seu seguinte
 *     - Em ambas, verifica se o vértice é válido
 * (2) O custo da rota é igual ao valor de p(C); e,
 * (3) Verifica se o custo (cost) retornado é compatı́vel com o lowerBound e o upperBound e com otimalidade.
 */
{


   // (0)
   if(sol.cost >= DBL_MAX - MY_EPS){
      if(!optimal){
         return NOT_FOUND_FEASIBLE_SOLUTION;
      }
      else{
         return INCOMPATIBLES_COST_AND_OPTIMAL;
      }
   }

   // (1)
   if(!instance.g.valid(sol.tour.front())) return INVALID_DNODE;
   if(sol.tour.front() != instance.depot){
      return FIRST_IS_NOT_DEPOT;
   }
   
   for(int i = 0; i < (int)sol.tour.size(); i++){
      if(!instance.g.valid(sol.tour[(i+1) % (int)sol.tour.size()])) return INVALID_DNODE;
      OutArcIt o(instance.g, sol.tour[i]);
      for(; o != INVALID; ++o) if(instance.g.target(o) == sol.tour[(i+1) % (int)sol.tour.size()]) break;
      if(o == INVALID) return ARC_MISSING;
   }
   
    
   while((int)sol.tour.size() < instance.n){
   
        for (OutArcIt a(instance.g, sol.tour.back()); a != INVALID; ++a){
        
        int  i = 0;
         while(i < (int)sol.tour.size() && instance.g.target(a) != sol.tour[i]) i++;
         if(i < (int)sol.tour.size()) return REPEATED_NODES_IN_TOUR;
         }
            
    }  
  

   // (2)
   double calcCost = 0.0;
   double sum_ta=0.0;
   for(int i = 0; i < (int)sol.tour.size(); i++){
      for(OutArcIt o(instance.g, sol.tour[i]); o != INVALID; ++o){
         if(instance.g.target(o) == sol.tour[(i+1) % (int)sol.tour.size()]){
            sum_ta += instance.weight[o];
             // cerr << "Priority of node = " << instance.weight_node[sol.tour[(i+1) % (int)sol.tour.size()]] << endl;
            calcCost += instance.weight_node[sol.tour[(i+1) % (int)sol.tour.size()]] * sum_ta;
           //      cerr << "calcCost = " << calcCost << endl;
            break;
         }
      }
   }
   if(!(calcCost - MY_EPS <= sol.cost && sol.cost <= calcCost + MY_EPS)){
     cerr << "calcCost = " << calcCost << endl;
      return COST_ERROR;
   }

   // (3)
   if(!(sol.lowerBound <= sol.cost && sol.cost <= sol.upperBound)){
      return COST_BOUND_ERROR;
   }
 /*  if(optimal){
      if(!(sol.lowerBound - MY_EPS <= sol.cost && sol.cost <= sol.upperBound + MY_EPS &&
           sol.lowerBound - MY_EPS <= sol.upperBound && sol.upperBound <= sol.lowerBound + MY_EPS)){
         return INVALID_BOUNDS_OPT;
      }
   }*/

   return OK;
   
   
  
}
 
//------------------------------------------------------------------------------
string decodeSolutionStatus(SOLUTION_STATUS solutionStatus)
{  
   stringstream ss;
   switch(solutionStatus){
      case NOT_FOUND_FEASIBLE_SOLUTION:{
         ss << "NOT_FOUND_FEASIBLE_SOLUTION";
         break;
      }
      case REPEATED_NODES_IN_TOUR:{
         ss << "REPEATED_NODES_IN_TOUR";
         break;
      }
      case INCOMPATIBLES_COST_AND_OPTIMAL:{
         ss << "INCOMPATIBLES_COST_AND_OPTIMAL";
         break;
      }
      case INVALID_DNODE:{
         ss << "INVALID_DNODE";
         break;
      }
      case FIRST_IS_NOT_DEPOT:{
         ss << "FIRST_IS_NOT_DEPOT";
         break;
      }
      case ARC_MISSING:{
         ss << "ARC_MISSING";
         break;
      }
      case NEGATIVE_LOAD_ERROR:{
         ss << "NEGATIVE_LOAD_ERROR";
         break;
      }
      case REMAINING_LOAD_ERROR:{
         ss << "REMAINING_LOAD_ERROR";
         break;
      }
      case COST_ERROR:{
         ss << "COST_ERROR";
         break;
      }
      case COST_BOUND_ERROR:{
         ss << "COST_BOUND_ERROR";
         break;
      }
      case INVALID_BOUNDS_OPT:{
         ss << "INVALID_BOUNDS_OPT";
         break;
      }
      case OK:{
         ss << "OK";
         break;
      }
   }
   return ss.str();
}
//------------------------------------------------------------------------------
string instanceDescriptionAsString(Tsp_P_Instance &instance)
{
   stringstream ss;
   
   ss << "n          : " << instance.n << endl;
   ss << "m          : " << instance.m << endl;
   ss << "depot      : " << instance.vname[instance.depot] << endl;
   
   return ss.str();
}
//------------------------------------------------------------------------------
string instanceAsString(Tsp_P_Instance &instance)
{
   stringstream ss;
   
   ss << instanceDescriptionAsString(instance);
   ss << dnodesAndItemsAsString(instance);
   
   return ss.str();
}
//------------------------------------------------------------------------------
string dnodesAndItemsAsString(Tsp_P_Instance &instance)
{
   stringstream ss;
   ss << "dnodes     :";
   for(DNodeIt v(instance.g); v != INVALID; ++v){
   //   ss << " " << instance.vname[v] << "." << vti(v,instance);
   }
   ss << endl;
   return ss.str();
}
//------------------------------------------------------------------------------
string arcsAndItemsAsString(Tsp_P_Instance &instance)
{
   stringstream ss;
   ss << "arcs       :";
   for(ArcIt a(instance.g); a != INVALID; ++a){
//      ss << " (" << instance.vname[instance.g.source(a)] << "." << vti(instance.g.source(a), instance) << ", " << instance.vname[instance.g.target(a)] << "." << vti(instance.g.target(a), instance) << "; " << instance.weight[a] << ") ";
   }
   ss << endl;
   return ss.str();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
string solutionAsString(Tsp_P_Instance &instance, Tsp_P_Solution  &sol)
{
   stringstream ss;
   ss << instanceDescriptionAsString(instance);
   ss << valuesAsString(sol);
   ss << tourAsString(instance, sol);
   //ss << tourAndItemsAsString(instance, sol);
   return ss.str();
}
//------------------------------------------------------------------------------
string tourAsString(Tsp_P_Instance &instance, Tsp_P_Solution  &sol)
{
   stringstream ss;
   ss << "tour       :";
   for(auto v = sol.tour.begin(); v != sol.tour.end(); ++v){
      ss << " " << instance.vname[*v];
   }
   ss << endl;
   return ss.str();
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
string valuesAsString(Tsp_P_Solution &sol)
{
   stringstream ss;
  // ss << "lowerBound : " << sol.lowerBound << endl;
   ss << "cost       : " << sol.cost << endl;
  // ss << "upperBound : " << sol.upperBound << endl;
   return ss.str();
}
//------------------------------------------------------------------------------
string resultAsString(Tsp_P_Instance  &Tsp_P_Instance,
                      Tsp_P_Solution  &Tsp_P_Solution,
                      Params          &params,
                      bool             optimal,
                      SOLUTION_STATUS  solutionStatus,
                      int              elapsedTime)
{
   stringstream ss;

   ss << "algorithm  : " << decodeAlg(params.alg) << endl;
   ss << "instance   : " << params.inputFile << endl;
   ss << "elapsedTime: " << elapsedTime << " s" << endl;
   ss << "timeLimit  : " << params.timeLimit << " s" <<  endl;
   ss << "optimal    : " <<  (optimal?"Yes":"No") << endl;
   ss << "sol. status: " << decodeSolutionStatus(solutionStatus) << endl;
   if(solutionStatus != OK && solutionStatus != NOT_FOUND_FEASIBLE_SOLUTION){
      ss << "Solution returned has not passed in the verification." << endl;
   }
   ss << solutionAsString(Tsp_P_Instance, Tsp_P_Solution);



   return ss.str();
}
//------------------------------------------------------------------------------
void solutionAsGraphical(Tsp_P_Instance &instance, Tsp_P_Solution  &sol, string inputFile)
{
   Digraph h;
   DNodeStringMap h_vname(h);  // node names
   ListDigraph::NodeMap<ListDigraph::Node> g2h(instance.g);  // maps a node of g to a node of h
   DNodePosMap h_posx(h);
   DNodePosMap h_posy(h);
   DNodeColorMap vcolor(h);   // color of the vertices
   ArcColorMap acolor(h);  // color of edges
   // ArcStringMap aname(h);  // name of edges
   
   for(ListDigraph::NodeIt v(instance.g); v != INVALID; ++v){
      ListDigraph::Node hv = h.addNode();
      g2h[v] = hv;
      h_posx[hv] = instance.posx[v];
       //cerr << instance.posx[v] << endl;
      h_posy[hv] = instance.posy[v];
      //cerr << instance.posy[v] << endl;
      h_vname[hv] = instance.vname[v];// + "." + IntToString(instance.s[v] + instance.t[v]);  // + ".\"" + vti(v, l) +"\"";
      if(v == instance.depot){
         vcolor[hv] = BLUE;
      }
    ///  else if(instance.s[v] != 0){
    //     vcolor[hv] = GREEN;
    //  }
      else{
         vcolor[hv] = RED;
      }
   }
   
   for(int i = 0; i < (int)sol.tour.size(); i++){
      if((i+1 < instance.n && i+1 < (int)sol.tour.size()) || (i+1 == instance.n)){
         DNode u, v;
         Arc a;
         u = sol.tour[i];
         v = sol.tour[(i+1) % instance.n];
         a = h.addArc(g2h[u] , g2h[v]);
         // aname[a] = "";
         acolor[a] = BLACK;
      }
   }
       
   ViewListDigraph(h, h_vname,  h_posx, h_posy, vcolor, acolor, "TSP-P. Instance: " + inputFile + ". Tour with cost: " + DoubleToString(sol.cost));
}
//------------------------------------------------------------------------------
string decodeAlg(ALG alg)
{
   stringstream ss;
   switch(alg){
      case NONE:{
         ss << "NONE";
         break;
      }
      case CONSTR_HEUR:{
         ss << "CONSTR_HEUR";
         break;
      }
      case META_HEUR_VIZ:{
         ss << "META_HEUR_VIZ";
         break;
      }
      case BRKGA:{
         ss << "BRKGA";
         break;
      }
      case EXACT:{
         ss << "EXACT";
         break;
      }
   }
   return ss.str();
}
//------------------------------------------------------------------------------
