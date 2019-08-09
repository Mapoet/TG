

//#include <boost/serialization/archive_input_unordered_map.hpp>
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
#include <tuple>

enum Types
{
    Null_Types = 0x000,
    Node_Types = 0x001,
    Link_Types = 0x002,
    IStr_Types = 0x010,
    CStr_Types = 0x020,
    TStr_Types = 0x040,
    OStr_Types = 0x080
};

inline Types operator|(const Types &t1, const Types &t2) { return (Types)(((unsigned int)t1) | ((unsigned int)t2)); }
inline Types operator^(const Types &t1, const Types &t2) { return (Types)(((unsigned int)t1) ^ ((unsigned int)t2)); }

template <class Key, class Initial, class Kernel, class Generator, class Active, class Transform, class Stream>
struct GTNNA
{
    //private:

    struct tagBase;
    struct tagRoot;
    struct tagNode;
    struct tagLink;

public:
    typedef std::shared_ptr<Kernel> Kernels;
    typedef std::shared_ptr<Generator> Generators;
    typedef std::shared_ptr<Active> Actives;
    typedef std::shared_ptr<Transform> Transforms;
    typedef std::shared_ptr<Stream> Streams;

    typedef std::shared_ptr<tagBase> Base;
    typedef std::weak_ptr<tagRoot> Root;
    typedef std::shared_ptr<tagNode> Node;
    typedef std::shared_ptr<tagLink> Link;

    typedef std::weak_ptr<tagNode> Cell;
    typedef std::weak_ptr<tagLink> Own;
    typedef boost::unordered_map<Node, Generators> Nodes;
    typedef boost::unordered_map<Link, Transforms> Links;
    typedef boost::unordered_map<Base, Streams> DataStreams;
    typedef boost::unordered_map<Base, bool> DataCtrls;

    typedef boost::unordered_map<Key, std::tuple<Key, Kernels>> Cells;
    typedef boost::unordered_map<Key, std::tuple<Actives, Nodes>> Relativities;
    typedef boost::unordered_map<Key, Streams> KeyStreams;
    typedef boost::unordered_map<Key, bool> KeyCtrls;

    //private:
    struct tagBase : std::tuple<Key, Root, Types>
    {
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            std::tuple<Key, Root, Types>& data=*this;
            //ar &std::get<0>(data);
            //ar &std::get<1>(data);
            //ar &std::get<2>(data);
        }
        operator std::tuple<Key, Root, bool>()
        {
            return std::tuple<Key, Root, bool>(*this);
        }
        Key key() const { return std::get<0>(*this); }
        Root root() const { return std::get<1>(*this); }
        Types type() const { return std::get<2>(*this); }
        //private:
        tagBase(const Key &key = Key(), const Root &root = Root(), const Types &type = Null_Types) : std::tuple<Key, Root, Types>(key, root, type) {}

    public:
    };

    struct tagRoot : std::tuple<Initial,           //initial data
                                std::vector<Node>, //nodes
                                std::vector<Link>, //links
                                DataStreams,       //DataStreams
                                DataCtrls          //start or end
                                >
    {
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            std::tuple<Initial,           //initial data
                                std::vector<Node>, //nodes
                                std::vector<Link>, //links
                                DataStreams,       //DataStreams
                                DataCtrls          //start or end
                                > &data=*this;
            ar &std::get<0>(data);
            ar &std::get<1>(data);
            ar &std::get<2>(data);
            ar &std::get<3>(data);
            ar &std::get<4>(data);
        }
        operator std::tuple<Initial,           //initial data
                            std::vector<Node>, //nodes
                            std::vector<Link>, //links
                            DataStreams,       //DataStreams
                            DataCtrls          //start or end
                            >()
        {
            return std::tuple<Initial,           //initial data
                              std::vector<Node>, //nodes
                              std::vector<Link>, //links
                              DataStreams,       //DataStreams
                              DataCtrls          //start or end
                              >(*this);
        }
        Initial initial() const { return std::get<0>(*this); }
        std::vector<Node> nodes() const { return std::get<1>(*this); }
        std::vector<Link> links() const { return std::get<2>(*this); }
        DataStreams streams() const { return std::get<3>(*this); }
        DataCtrls ctrls() const { return std::get<4>(*this); }
        Initial &initial() { return std::get<0>(*this); }
        std::vector<Node> &nodes() { return std::get<1>(*this); }
        std::vector<Link> &links() { return std::get<2>(*this); }
        DataStreams &streams() { return std::get<3>(*this); }
        DataCtrls &ctrls() { return std::get<4>(*this); }
        //private:
        tagRoot(const Initial &initial = Initial(),
                const std::vector<Node> &nodes = std::vector<Node>(),
                const std::vector<Link> &links = std::vector<Link>(),
                const DataStreams &streams = DataStreams(),
                const DataCtrls &ctrls = DataCtrls()) : std::tuple<Initial,           //initial data
                                                                   std::vector<Node>, //nodes
                                                                   std::vector<Link>, //links
                                                                   DataStreams,       //DataStreams
                                                                   DataCtrls          //start or end
                                                                   >(initial, nodes, links, streams, ctrls)
        {
        }

    public:
    };

    struct tagNode : tagBase,
                     std::tuple<Own,     // of link
                                Kernels, // Kernels
                                Links,   //istr
                                Links    //ostr
                                >
    {
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            std::tuple<Own,     // of link
                            Kernels, // Kernels
                            Links,   //istr
                            Links    //ostr
                            >&data=*this;
            tagBase::serialize(ar, version);
            ar &std::get<0>(data);
            ar &std::get<1>(data);
            ar &std::get<2>(data);
            ar &std::get<3>(data);
        }
        operator std::tuple<Own,     // of link
                            Kernels, // Kernels
                            Links,   //istr
                            Links    //ostr
                            >()
        {
            return std::tuple<Own,     // of link
                              Kernels, // Kernels
                              Links,   //istr
                              Links    //ostr
                              >(*this);
        }
        operator tagBase()
        {
            return tagBase(*this);
        }
        Own own() const { return std::get<0>(*this); }
        Kernels kernel() const { return std::get<1>(*this); }
        Links istrs() const { return std::get<2>(*this); }
        Links ostrs() const { return std::get<3>(*this); }
        Own &own() { return std::get<0>(*this); }
        Kernels &kernel() { return std::get<1>(*this); }
        Links &istrs() { return std::get<2>(*this); }
        Links &ostrs() { return std::get<3>(*this); }
        //private:
        tagNode(const Key &key = Key(), const Root &root = Root(), const Own &own = Own(),
                const Kernels &kernel = Kernels(),
                const Links &istrs = Links(),
                const Links &ostrs = Links()) : tagBase(key, root, Node_Types), std::tuple<Own,     // of link
                                                                                           Kernels, // Kernels
                                                                                           Links,   //istr
                                                                                           Links    //ostr
                                                                                           >(own, kernel, istrs, ostrs)
        {
        }

    public:
    };

    struct tagLink : tagBase,
                     std::tuple<Links,   //istr
                                Actives, // Kernels
                                Links,   //ostr
                                Nodes    //nodes
                                >
    {
        friend class boost::serialization::access;
        template <class Archive>
        void serialize(Archive &ar, const unsigned int version)
        {
            std::tuple<Links,   //istr
                                Actives, // Kernels
                                Links,   //ostr
                                Nodes    //nodes
                                >&data=*this;
            tagBase::serialize(ar, version);
            ar &std::get<0>(data);
            ar &std::get<1>(data);
            ar &std::get<2>(data);
            ar &std::get<3>(data);
        }
        operator std::tuple<Links,   //istr
                            Actives, // Kernels
                            Links,   //ostr
                            Nodes    //nodes
                            >()
        {
            return std::tuple<Links,   //istr
                              Actives, // Kernels
                              Links,   //ostr
                              Nodes    //nodes
                              >(*this);
        }
        operator tagBase()
        {
            return tagBase(*this);
        }
        Links istrs() const { return std::get<0>(*this); }
        Actives active() const { return std::get<1>(*this); }
        Links ostrs() const { return std::get<2>(*this); }
        Nodes nodes() const { return std::get<3>(*this); }
        Links &istrs() { return std::get<0>(*this); }
        Actives &active() { return std::get<1>(*this); }
        Links &ostrs() { return std::get<2>(*this); }
        Nodes &nodes() { return std::get<3>(*this); }
        //private:
        tagLink(const Key &key = Key(), const Root &root = Root(), const Links &istrs = Links(),
                const Actives &active = Actives(),
                const Links &ostrs = Links(),
                const Nodes &nodes = Nodes()) : tagBase(key, root, Link_Types), std::tuple<Links,   //istr
                                                                                           Actives, // Kernels
                                                                                           Links,   //ostr
                                                                                           Nodes    //nodes
                                                                                           >(istrs, active, ostrs, nodes)
        {
        }

    public:
    };

public:
};


int main()
{
    typedef GTNNA<int, int, int, int, int, int, int> Ints;
    Ints::Root root;
    auto troot = std::make_shared<Ints::tagRoot>();
    root = troot;
    root.lock()->nodes().push_back(Ints::Node());
    auto tbase = std::make_shared<Ints::tagBase>(2, root);
    Ints::Base base = tbase;
    std::ofstream out("out.txt");
    boost::archive::text_oarchive oa(out);
    tbase->serialize(oa,1);
    out.close();
    return 0;
}