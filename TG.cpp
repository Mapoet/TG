

//'Droid Sans Mono', 'monospace', monospace, 'Droid Sans Fallback'
#include "TG.hpp"
typedef std::vector<int> vs;
#include <Eigen/CXX11/Tensor>
#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/SparseExtra>
#include <iostream>
#include <jsoncpp/json/json.h>
#include <zlib.h>
//#include <glibmm-2.4/glibmm.h>
void func(){

}
int main(){
  typedef Eigen::SparseMatrix<double> SpMat;
    auto x=Eigen::MatrixXd::Random(5,2);
    typedef Eigen::Triplet<double> T;
    std::vector<T> tripletList;
    for(int i=0;i<5;i++)
    {
      tripletList.push_back(T(i,i,i+0.5));
//      tripletList.push_back(T((i+1)%5,i,i-1));
//      tripletList.push_back(T((i+4)%5,i,i+1));
    }
    Eigen::SparseMatrix<double> m(5,5);
    m.setFromTriplets(tripletList.begin(), tripletList.end());
    m.makeCompressed();
    Eigen::SparseLU<SpMat,Eigen::COLAMDOrdering<int>> lu;
    Eigen::ConjugateGradient<SpMat> solve;
    auto y=m*x;
    lu.analyzePattern(m);
    lu.factorize(m);
    auto v1=lu.solve(y);
    auto v2=solve.compute(m).solve(y);
    std::cout<<m<<lu.determinant()<<std::endl;
    std::cout<<x<<std::endl;
    std::cout<<v1-x<<std::endl;
    std::cout<<v2-x<<std::endl;
    Eigen::saveMarket(m,"m.mtx");
    Json::Value data;
    data.resize(3);
    data[0]["type"]="double";
    data[0]["value"]=2.3;
    data[0]["att"]["name"]="first";
    data[0]["att"]["type"]="int";
    data[0]["att"]["value"]=3;
    data[1]=data[0];
    data[2].resize(5);
    for(int i=0;i<5;i++)data[2][i]=i;
    data[2];
    std::ofstream out("data.json");

    Json::StreamWriterBuilder writebuilder;
    writebuilder["commentStyle"] = "None";
    writebuilder["indentation"] = "\t";  // or whatever you like
    std::unique_ptr<Json::StreamWriter> writer(
    writebuilder.newStreamWriter());
    writer->write(data, &out);
    out<<std::endl;
    out.close();
    
    data.clear();

    std::ifstream in("data.json");
    std::string err;
    Json::CharReaderBuilder readbuilder;
    readbuilder["commentStyle"] = "None";
    readbuilder["indentation"] = "\t";  // or whatever you like
    Json::parseFromStream(readbuilder,in,&data,&err);
    in.close();
    std::cout<<data.toStyledString()<<std::endl;
    Eigen::Tensor<double,3> ts(5,6,7);
    ts.setRandom();
    auto s=ts.slice(std::array<int,3>({1,2,3}),std::array<int,3>({1,2,3}));
    std::cout<<ts<<std::endl;
    Eigen::Tensor<double,3> s2=s.eval();
    std::cout<<s2<<std::endl;


    return 0;
    
/*     Eigen::Tensor<double,3> t;
    t.resize(4,5,6);
    t.setRandom();


    t(1,2,4)=1.2;
    std::cout<<t<<std::endl;
    std::cout<<t.slice(Eigen::Sizes(0,0,0),Eigen::Sizes(4,1,1))<<std::endl;
    return 0; */
    // typedef TG::Root<3,std::string,vs,std::valarray<double>> Root;
    // typedef TG::flow::branch<3,2,vs,std::string,std::valarray<double>> branch32;
    // Root root;
    // root;
    // //root->_node.begin()->first->_ostr.begin()->second->predict(static_cast<branch32*>(root->_node.begin()->first.get()),static_cast<branch32*>(root->_node.begin()->first->_ostr.begin()->first));
    // (*(*root)("bgtb"))("vtg");//->insertI();
    // auto v2=(*(*root)("bgtb"))({TG::Stream_Input,"vtg"});
   /* 
   // typedef TG::Root<3,int,double,double> Root;
   // typedef TG::flow::branch<3,2,int,double,double> branch32;
   // Root root;
   // root;
    //root->_node.begin()->first->_ostr.begin()->second->predict(static_cast<branch32*>(root->_node.begin()->first.get()),static_cast<branch32*>(root->_node.begin()->first->_ostr.begin()->first));
   // (*(*root)(1))[2]->_args;//->insertI();
   // auto v2=(*(*root)(1))({TG::Stream_Input,5});
   */

   // typedef TG::Knot<int,double,double> Knot;
   // Knot knot;
   // knot->Insert(Knot::MakeKnot(),Knot::MakeControl());
   // (*(*knot)(1))[2]->_args;
}
