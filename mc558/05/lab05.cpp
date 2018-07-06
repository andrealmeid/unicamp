#include "solve.hpp"


using namespace std;

int main() {

    int v;
    vector<atracao> atracoes;

    cin >> v;
    for (int i = 0 ; i < v ; i++){
        int rep;
        int tamSeq;
        atracao *k = new atracao();

        cin >> rep;
        cin >> tamSeq;

        k->repeticoes = rep;

        for (int j = 0 ; j < tamSeq ; j++ ){
            int index;
            cin >> index;
            k->indexNaoSeq.push_back(index);
        }
        atracoes.push_back(*k);
    }

    cout << solve(v, atracoes);
    return 0;
}
