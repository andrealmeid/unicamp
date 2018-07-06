#include "solve.hpp"

using namespace std;


int main(int argv, char *argc[]){
    int v, a;

    cin >> v >> a;

    vector< vector< pair<int, int> > > g(v);

    for(int i = 0; i < a ; i ++ ){
        int ti, tj, td;
        cin >> ti >> tj >> td;
        g[ti].push_back( make_pair(tj, td) );
        g[tj].push_back( make_pair(ti, td) );
    }

    int ne;
    cin >> ne;
    vector< vector <int> > e(ne);
    for(int i = 0 ; i < ne; i++){
        int tv, to;
        cin >> tv >> to;
        e[i].push_back(tv);
        for(int j = 0 ; j < to ; j++){
            int pk;
            cin >> pk;
            e[i].push_back(pk);
        }
    }

    int ns;
    cin >> ns;
    vector <int> s(ns);
    for(int i = 0 ; i < ns ; i ++ ){
        int tv;
        cin >> tv;
        s[i] = tv;
    }

    vector< vector<int> > solution = solve(g, e, s);

    for(unsigned int i = 0; i < solution.size(); i++){
        cout << solution[i].size() << endl;
        for(unsigned int j = 0; j < solution[i].size(); j++){
            cout << solution[i][j] << endl;
        }
    }

    return 0;

}
