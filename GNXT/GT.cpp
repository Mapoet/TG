#include <fstream>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/boost_unordered_map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>
namespace tt{

template<typename T>
struct point
{
    T _x,_y,_z;
    point(const T&x=T(),const T&y=T(),const T&z=T()):_x(x),_y(y),_z(z){}
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            ar&_x;//BOOST_SERIALIZATION_NVP(_x);
            ar&_y;//BOOST_SERIALIZATION_NVP(_y);
            ar&_z;//BOOST_SERIALIZATION_NVP(_z);
        }
};

template<typename V>
struct points{
    boost::unordered_map<std::string,boost::shared_ptr<point<V>>> _pts;
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            boost::serialization::serialize(ar,_pts,version);
        }
};

}

int main(){

    tt::points<double> pts;
    std::vector<boost::shared_ptr<tt::point<double>>> ps;
    for(int i=0;i<10;i++)ps.emplace_back(boost::make_shared<tt::point<double>>((random()%10000)/100.,(random()%10000)/100.,(random()%10000)/100.));
    for(int i=0;i<20;i++)pts._pts.emplace(std::to_string(i),ps[random()%10]);
    std::ofstream out("out.txt");
    boost::archive::text_oarchive oa(out);
    //oa& ps[0];
   // boost::serialization::serialize(oa,ps[0],1);
    pts.serialize(oa,1);
    //ps[0].get()->serialize(oa,1);
    out.close();

    std::ifstream in("out.txt");
    boost::archive::text_iarchive ia(in);
    //oa& ps[0];
    pts._pts.clear();
   // boost::serialization::serialize(oa,ps[0],1);
    pts.serialize(ia,1);
    //ps[0].get()->serialize(oa,1);
    out.close();

    std::ofstream out2("out2.txt");
    boost::archive::text_oarchive oa2(out2);
    //oa& ps[0];
   // boost::serialization::serialize(oa,ps[0],1);
    pts.serialize(oa2,1);
    //ps[0].get()->serialize(oa,1);
    out2.close();

}