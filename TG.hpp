#ifndef __TG__
#define __TG__
#include <memory>
#include <valarray>
#include <vector>
#include <map>
namespace TG
{
namespace tree
{

template <typename Value, typename Active>
struct Cell;

template <int n, int m, typename Key, typename Value, typename Active, typename Transform, typename Control>
struct Node;

template <typename Key, typename Flow>
struct Control
{
    std::map<Key, Flow> _args;
    template <typename C>
    virtual bool predict(C& cs){
        return false;
    }
    template <typename C>
    virtual bool update(C& cs){
        return false;
    }

};
template <typename Key, typename Flow>
struct Active
{
    std::map<Key, Flow> _args;
/*     template <int n, int m, typename Active, typename Transform, typename Control>
    virtual bool predict(typename Node<n,m,Key,Flow,Active,Transform,Control>::L& input,typename Node<n,m,Key,Flow,Active,Transform,Control>::L& output){
        return false;
    }
    template <int n, int m, typename Active, typename Transform, typename Control>
    virtual bool update(typename Node<n,m,Key,Flow,Active,Transform,Control>::L& input,typename Node<n,m,Key,Flow,Active,Transform,Control>::L& output){
        return false;
    } */
    template <typename Links>
    virtual bool predict(Links& input,Links& output){
        return false;
    }
    template <typename Links>
    virtual bool update(Links& input,Links& output){
        return false;
    }
};
template <typename Key, typename Flow>
struct Transform
{
    std::map<Key, Flow> _args;
    Flow                _errs;
/*     template <int n, int m, typename Active, typename Transform, typename Control>
    virtual bool predict(typename Node<n,m,Key,Flow,Active,Transform,Control>::Self& input,typename Node<n,m,Key,Flow,Active,Transform,Control>::Self& output){
        return false;
    }
    template <int n, int m, typename Active, typename Transform, typename Control>
    virtual bool update(typename Node<n,m,Key,Flow,Active,Transform,Control>::Self& input,typename Node<n,m,Key,Flow,Active,Transform,Control>::Self& output){
        return false;
    } */
    template <typename Self>
    virtual bool predict(Self input,Self output){
        return false;
    }
    template <typename Self>
    virtual bool update(Self input,Self output){
        return false;
    } 
};
template <typename Value, typename Active>
struct Cell
{
    Value  _val;
    Active _act;
};
template <int n, int m, typename Key, typename Value, typename Active, typename Transform, typename Control>
struct Node : Cell<Value, Active>
{
    typedef Node<n, m, Key, Value, Active, Transform, Control> *Self;
    typedef std::map<Self, Transform> L;
    typedef std::vector<std::pair<std::shared_ptr<Node<n, m - 1, Key, Value, Active, Transform, Control>>, Control>> C;
    typedef Node<n, m + 1, Key, Value, Active, Transform, Control> *R;
    L _istr;
    L _ostr;
    C _node;
    R _root;
};
template <int n, typename Key, typename Value, typename Active, typename Transform, typename Control>
struct Node<n, n, Key, Value, Active, Transform, Control> : Cell<Value, Active>
{
    typedef Node<n, n, Key, Value, Active, Transform, Control> *Self;
    typedef std::vector<std::pair<std::shared_ptr<Node<n, n - 1, Key, Value, Active, Transform, Control>>, Control>> C;
    C _node;
};
template <int n, typename Key, typename Value, typename Active, typename Transform, typename Control>
struct Node<n, 0, Key, Value, Active, Transform, Control> : Cell<Value, Active>
{
    typedef Node<n, 0, Key, Value, Active, Transform, Control> *Self;
    typedef std::map<Self, Transform> L;
    typedef Node<n, 1, Key, Value, Active, Transform, Control> *R;
    L _istr;
    L _ostr;
    R _root;
};

} // namespace tree
template <int n, int m, typename Key, typename Value>
using branch = tree::Node<n, m, Key, Value, std::shared_ptr<tree::Active<Key, Value>>, std::shared_ptr<tree::Transform<Key, Value>>, std::shared_ptr<tree::Control<Key, Value>>>;
template <int n, typename Key, typename Value>
using root = tree::Node<n, n, Key, Value, std::shared_ptr<tree::Active<Key, Value>>, std::shared_ptr<tree::Transform<Key, Value>>, std::shared_ptr<tree::Control<Key, Value>>>;
template <int n, typename Key, typename Value>
using leaf = tree::Node<n, 0, Key, Value, std::shared_ptr<tree::Active<Key, Value>>, std::shared_ptr<tree::Transform<Key, Value>>, std::shared_ptr<tree::Control<Key, Value>>>;
template <int n, int m, typename Key, typename Value>
using Branch = std::shared_ptr<branch<n,m,Key,Value>>;
template <int n, typename Key, typename Value>
using Root = std::shared_ptr<root<n,Key,Value>>;
template <int n, typename Key, typename Value>
using Leaf = std::shared_ptr<leaf<n,Key,Value>>;

} // namespace TG
#endif
