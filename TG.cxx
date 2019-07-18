

//'Droid Sans Mono', 'monospace', monospace, 'Droid Sans Fallback'
#include "TG.hpp"
int main(){
    typedef TG::Root<3,std::string,std::valarray<double>> Root;
    typedef TG::branch<3,2,std::string,std::valarray<double>> branch32;
    Root root;
    root->_node[0].first->_ostr.begin()->second->predict(const_cast<branch32*>(root->_node[0].first.get()),const_cast<branch32*>(root->_node[0].first->_ostr.begin()->first));
    const_cast<branch32*>(root->_node[0].first.get())->_node; 
}
