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
#include <boost/weak_ptr.hpp>

namespace TGA
{
template<typename V>
using shared_ptr=boost::shared_ptr<V>;
template<typename V>
using weak_ptr=boost::weak_ptr<V>;
template<typename K,typename V>
using unordered_map=boost::unordered_map<K,V>;


} // namespace TGA

template<typename V>
using shared_ptr=boost::shared_ptr<V>;



int main(int argc,char**argv){


 return 0;
}