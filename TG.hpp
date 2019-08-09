#ifndef __TG__
#define __TG__
#include <memory>
#include <valarray>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <exception>
#include <algorithm>
namespace TG
{

enum StreamType
{
    Stream_Input,
    Stream_Output
};
namespace tree
{

template <typename Key, typename Flow>
struct Control;

template <typename Key, typename Flow>
struct Active;

template <typename Key, typename Flow>
struct Transmit;

template <typename Key, typename Value, typename Active>
struct Cell;

template <typename Key, typename Value, typename Active, typename Transmit, typename Control>
struct Form;

template <int n, int m, typename Key, typename Value, typename Active, typename Transmit, typename Control>
struct Node;

template <typename Key, typename Flow>
struct Control
{
    std::map<Key, Flow> _args;
    template <typename C>
    bool predict(C &cs)
    {
        return false;
    }
    template <typename C>
    bool update(C &cs)
    {
        return false;
    }
};
template <typename Key, typename Flow>
struct Active
{
    std::map<Key, Flow> _args;
    template <typename Links>
    bool predict(Links &input, Links &output)
    {
        return false;
    }
    template <typename Links>
    bool update(Links &input, Links &output)
    {
        return false;
    }
};
template <typename Key, typename Flow>
struct Transmit
{
    std::map<Key, Flow> _args;
    Flow _errs;
    template <typename S>
    bool predict(S input, S output)
    {
        return false;
    }
    template <typename S>
    bool update(S input, S output)
    {
        return false;
    }
};
template <typename Key, typename Value, typename Active>
struct Cell
{
    template <typename Keys, typename Values, typename Actives, typename Transmits, typename Controls>
    friend struct Knot;

    template <int n, int m, typename Keys, typename Values, typename Actives, typename Transmits, typename Controls>
    friend struct Node;

private:
    Key _key;
    Value _val;
    Active _act;

public:
    Cell(const Value &val = Value(), const Active &act = Active()) : _val(val), _act(act) {}
    template <typename Values, typename Actives>
    Cell(const Values &val, const Actives &act) : _val(std::make_shared<Values>(val)), _act(std::make_shared<Actives>(act)) {}
};

template <typename Key, typename Value, typename Active, typename Transmit, typename Control>
struct Knot : Cell<Key, Value, Active>
{
    typedef std::shared_ptr<Knot<Key, Value, Active, Transmit, Control>> P;
    typedef Knot<Key, Value, Active, Transmit, Control> *S;
    typedef std::map<S, Transmit> L;
    typedef std::map<P, Control> W;

private:
    L _istr;
    L _ostr;
    W _node;
    S _root;

public:
    bool insertC(const P &c, const Control &w)
    {
        return _node.emplace(c, w) != nullptr;
    }
    template <typename Ps, typename Controls>
    bool insertC(const Ps &c, const Controls &w)
    {
        return _node.emplace(std::make_shared<Ps>(c), std::make_shared<Controls>(w)) != nullptr;
    }

    bool insertI(const S c, const Transmit &w)
    {
        return _istr.emplace(c, w) != nullptr;
    }
    template <typename Ss, typename Transmits>
    bool insertI(const Ss c, const Transmits &w)
    {
        return _istr.emplace(std::make_shared<Ss>(c), std::make_shared<Transmits>(w)) != nullptr;
    }
    bool insertO(const S c, const Transmit &w)
    {
        return _ostr.emplace(c, w) != nullptr;
    }
    template <typename Ss, typename Transmits>
    bool insertO(const Ss c, const Transmits &w)
    {
        return _ostr.emplace(std::make_shared<Ss>(c), std::make_shared<Transmits>(w)) != nullptr;
    }

    void removeC(const Key &key)
    {
        auto it = std::find_if(_node.begin(), _node.end(), [&](const typename W::iterator &it) { return it.first->_key == key; });
        if (it != _node.end())
            _node.erase(it);
    }
    void removeC(const P &c)
    {
        auto it = std::find_if(_node.begin(), _node.end(), [&](const typename W::iterator &it) { return it.first == c; });
        if (it != _node.end())
            _node.erase(it);
    }
    void removeI(const Key &key)
    {
        auto it = std::find_if(_istr.begin(), _istr.end(), [&](const typename L::iterator &it) { return it.first->_key == key; });
        if (it != _istr.end())
            _istr.erase(it);
    }
    void removeI(const P &c)
    {
        auto it = std::find_if(_istr.begin(), _istr.end(), [&](const typename L::iterator &it) { return it.first == c; });
        if (it != _istr.end())
            _istr.erase(it);
    }

    void removeO(const Key &key)
    {
        auto it = std::find_if(_ostr.begin(), _ostr.end(), [&](const typename L::iterator &it) { return it.first->_key == key; });
        if (it != _ostr.end())
            _ostr.erase(it);
    }
    void removeO(const P &c)
    {
        auto it = std::find_if(_ostr.begin(), _ostr.end(), [&](const typename L::iterator &it) { return it.first == c; });
        if (it != _ostr.end())
            _ostr.erase(it);
    }

    Control operator[](const Key &key) const
    {
        for (auto it : _node)
        {
            if (it.first->_key == key)
                return it.second;
        }
        return Control();
    }
    Control &operator[](const Key &key)
    {
        for (auto &it : _node)
        {
            if (it.first->_key == key)
                return it.second;
        }
    }
    P operator()(const Key &key) const
    {
        for (auto it : _node)
        {
            if (it.first->_key == key)
                return it.first;
        }
        return P();
    }
    P operator()(const Key &key)
    {
        for (auto &it : _node)
        {
            if (it.first->_key == key)
                return it.first;
        }
    }
    Transmit operator[](const std::pair<StreamType, Key> &skey) const
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        return Transmit();
    }
    Transmit &operator[](const std::pair<StreamType, Key> &skey)
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
    }
    S operator()(const std::pair<StreamType, Key> &skey) const
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.first;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.first;
            }
        }
        return nullptr;
    }
    S operator()(const std::pair<StreamType, Key> &skey)
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return const_cast<S>(it.first);
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return const_cast<S>(it.first);
            }
        }
    }
};

template <int n, int m, typename Key, typename Value, typename Active, typename Transmit, typename Control>
struct Node : Cell<Key, Value, Active>
{
    typedef std::shared_ptr<Node<n, m, Key, Value, Active, Transmit, Control>> P;
    typedef Node<n, m, Key, Value, Active, Transmit, Control> *S;
    typedef std::map<S, Transmit> L;
    typedef std::shared_ptr<Node<n, m - 1, Key, Value, Active, Transmit, Control>> C;
    typedef std::map<C, Control> W;
    typedef Node<n, m + 1, Key, Value, Active, Transmit, Control> *R;

private:
    L _istr;
    L _ostr;
    W _node;
    R _root;

public:
    bool insertC(const C &c, const Control &w)
    {
        return _node.emplace(c, w) != nullptr;
    }
    template <typename Cs, typename Controls>
    bool insertC(const Cs &c, const Controls &w)
    {
        return _node.emplace(std::make_shared<Cs>(c), std::make_shared<Controls>(w)) != nullptr;
    }

    bool insertI(const S c, const Transmit &w)
    {
        return _istr.emplace(c, w) != nullptr;
    }
    template <typename Ss, typename Transmits>
    bool insertI(const Ss c, const Transmits &w)
    {
        return _istr.emplace(std::make_shared<Ss>(c), std::make_shared<Transmits>(w)) != nullptr;
    }
    bool insertO(const S c, const Transmit &w)
    {
        return _ostr.emplace(c, w) != nullptr;
    }
    template <typename Ss, typename Transmits>
    bool insertO(const Ss c, const Transmits &w)
    {
        return _ostr.emplace(std::make_shared<Ss>(c), std::make_shared<Transmits>(w)) != nullptr;
    }

    void removeC(const Key &key)
    {
        auto it = std::find_if(_node.begin(), _node.end(), [&](const typename W::iterator &it) { return it.first->_key == key; });
        if (it != _node.end())
            _node.erase(it);
    }
    void removeC(const C &c)
    {
        auto it = std::find_if(_node.begin(), _node.end(), [&](const typename W::iterator &it) { return it.first == c; });
        if (it != _node.end())
            _node.erase(it);
    }
    void removeI(const Key &key)
    {
        auto it = std::find_if(_istr.begin(), _istr.end(), [&](const typename L::iterator &it) { return it.first->_key == key; });
        if (it != _istr.end())
            _istr.erase(it);
    }
    void removeI(const S &c)
    {
        auto it = std::find_if(_istr.begin(), _istr.end(), [&](const typename L::iterator &it) { return it.first == c; });
        if (it != _istr.end())
            _istr.erase(it);
    }

    void removeO(const Key &key)
    {
        auto it = std::find_if(_ostr.begin(), _ostr.end(), [&](const typename L::iterator &it) { return it.first->_key == key; });
        if (it != _ostr.end())
            _ostr.erase(it);
    }
    void removeO(const S &c)
    {
        auto it = std::find_if(_ostr.begin(), _ostr.end(), [&](const typename L::iterator &it) { return it.first == c; });
        if (it != _ostr.end())
            _ostr.erase(it);
    }

    Control operator[](const Key &key) const
    {
        for (auto it : _node)
        {
            if (it.first->_key == key)
                return it.second;
        }
        return Control();
    }
    Control &operator[](const Key &key)
    {
        for (auto &it : _node)
        {
            if (it.first->_key == key)
                return it.second;
        }
    }
    C operator()(const Key &key) const
    {
        for (auto it : _node)
        {
            if (it.first->_key == key)
                return it.first;
        }
        return C();
    }
    C operator()(const Key &key)
    {
        for (auto &it : _node)
        {
            if (it.first->_key == key)
                return it.first;
        }
    }
    Transmit operator[](const std::pair<StreamType, Key> &skey) const
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        return Transmit();
    }
    Transmit &operator[](const std::pair<StreamType, Key> &skey)
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
    }
    S operator()(const std::pair<StreamType, Key> &skey) const
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.first;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.first;
            }
        }
        return nullptr;
    }
    S operator()(const std::pair<StreamType, Key> &skey)
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return const_cast<S>(it.first);
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return const_cast<S>(it.first);
            }
        }
    }
};
template <int n, typename Key, typename Value, typename Active, typename Transmit, typename Control>
struct Node<n, n, Key, Value, Active, Transmit, Control> : Cell<Key, Value, Active>
{
    typedef std::shared_ptr<Node<n, n, Key, Value, Active, Transmit, Control>> P;
    typedef Node<n, n, Key, Value, Active, Transmit, Control> *S;
    typedef std::shared_ptr<Node<n, n - 1, Key, Value, Active, Transmit, Control>> C;
    typedef std::map<C, Control> W;

private:
    W _node;

public:
    bool insertC(const C &c, const Control &w)
    {
        return _node.emplace(c, w) != nullptr;
    }
    template <typename Cs, typename Controls>
    bool insertC(const Cs &c, const Controls &w)
    {
        return _node.emplace(std::make_shared<Cs>(c), std::make_shared<Controls>(w)) != nullptr;
    }

    void removeC(const Key &key)
    {
        auto it = std::find_if(_node.begin(), _node.end(), [&](const typename W::iterator &it) { return it.first->_key == key; });
        if (it != _node.end())
            _node.erase(it);
    }
    void removeC(const C &c)
    {
        auto it = std::find_if(_node.begin(), _node.end(), [&](const typename W::iterator &it) { return it.first == c; });
        if (it != _node.end())
            _node.erase(it);
    }

    Control operator[](const Key &key) const
    {
        for (auto it : _node)
        {
            if (it.first->_key == key)
                return it.second;
        }
        return Control();
    }
    Control &operator[](const Key &key)
    {
        for (auto &it : _node)
        {
            if (it.first->_key == key)
                return it.second;
        }
    }
    C operator()(const Key &key) const
    {
        for (auto it : _node)
        {
            if (it.first->_key == key)
                return it.first;
        }
        return C();
    }
    C operator()(const Key &key)
    {
        for (auto &it : _node)
        {
            if (it.first->_key == key)
                return it.first;
        }
    }
};
template <int n, typename Key, typename Value, typename Active, typename Transmit, typename Control>
struct Node<n, 0, Key, Value, Active, Transmit, Control> : Cell<Key, Value, Active>
{
    typedef std::shared_ptr<Node<n, 0, Key, Value, Active, Transmit, Control>> P;
    typedef Node<n, 0, Key, Value, Active, Transmit, Control> *S;
    typedef std::map<S, Transmit> L;
    typedef Node<n, 1, Key, Value, Active, Transmit, Control> *R;

private:
    L _istr;
    L _ostr;
    R _root;

public:
    bool insertI(const S c, const Transmit &w)
    {
        return _istr.emplace(c, w) != nullptr;
    }
    template <typename Ss, typename Transmits>
    bool insertI(const Ss c, const Transmits &w)
    {
        return _istr.emplace(static_cast<S>(c), std::make_shared<Transmits>(w)) != nullptr;
    }
    bool insertO(const S c, const Transmit &w)
    {
        return _ostr.emplace(c, w) != nullptr;
    }
    template <typename Ss, typename Transmits>
    bool insertO(const Ss c, const Transmits &w)
    {
        return _ostr.emplace(static_cast<S>(c), std::make_shared<Transmits>(w)) != nullptr;
    }

    void removeI(const Key &key)
    {
        auto it = std::find_if(_istr.begin(), _istr.end(), [&](const typename L::iterator &it) { return it.first->_key == key; });
        if (it != _istr.end())
            _istr.erase(it);
    }
    void removeI(const S &c)
    {
        auto it = std::find_if(_istr.begin(), _istr.end(), [&](const typename L::iterator &it) { return it.first == c; });
        if (it != _istr.end())
            _istr.erase(it);
    }

    void removeO(const Key &key)
    {
        auto it = std::find_if(_ostr.begin(), _ostr.end(), [&](const typename L::iterator &it) { return it.first->_key == key; });
        if (it != _ostr.end())
            _ostr.erase(it);
    }
    void removeO(const S &c)
    {
        auto it = std::find_if(_ostr.begin(), _ostr.end(), [&](const typename L::iterator &it) { return it.first == c; });
        if (it != _ostr.end())
            _ostr.erase(it);
    }
    Transmit operator[](const std::pair<StreamType, Key> &skey) const
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        return Transmit();
    }
    Transmit &operator[](const std::pair<StreamType, Key> &skey)
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.second;
            }
        }
    }
    S operator()(const std::pair<StreamType, Key> &skey) const
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return it.first;
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return it.first;
            }
        }
        return nullptr;
    }
    S operator()(const std::pair<StreamType, Key> &skey)
    {
        if (skey.first == Stream_Input)
        {
            for (auto it : _istr)
            {
                if (it.first->_key == skey.second)
                    return const_cast<S>(it.first);
            }
        }
        if (skey.first == Stream_Output)
        {
            for (auto it : _ostr)
            {
                if (it.first->_key == skey.second)
                    return const_cast<S>(it.first);
            }
        }
    }
};

} // namespace tree

namespace flow
{
template <typename Key, typename Object, typename Value>
struct knot : tree::Knot<Key, Object, std::shared_ptr<tree::Active<Key, Value>>, std::shared_ptr<tree::Transmit<Key, Value>>, std::shared_ptr<tree::Control<Key, Value>>>
{
};
template <int n, int m, typename Key, typename Object, typename Value>
struct branch : tree::Node<n, m, Key, Object, std::shared_ptr<tree::Active<Key, Value>>, std::shared_ptr<tree::Transmit<Key, Value>>, std::shared_ptr<tree::Control<Key, Value>>>
{
};

template <int n, typename Key, typename Object, typename Value>
struct root : tree::Node<n, n, Key, Object, std::shared_ptr<tree::Active<Key, Value>>, std::shared_ptr<tree::Transmit<Key, Value>>, std::shared_ptr<tree::Control<Key, Value>>>
{
};
template <int n, typename Key, typename Object, typename Value>
struct leaf : tree::Node<n, 0, Key, Object, std::shared_ptr<tree::Active<Key, Value>>, std::shared_ptr<tree::Transmit<Key, Value>>, std::shared_ptr<tree::Control<Key, Value>>>
{
};

} // namespace flow
template <typename Key, typename Object, typename Value>
using Knot = std::shared_ptr<flow::knot<Key, Object, Value>>;
template <int n, int m, typename Key, typename Object, typename Value>
using Branch = std::shared_ptr<flow::branch<n, m, Key, Object, Value>>;
template <int n, typename Key, typename Object, typename Value>
using Root = std::shared_ptr<flow::root<n, Key, Object, Value>>;
template <int n, typename Key, typename Object, typename Value>
using Leaf = std::shared_ptr<flow::leaf<n, Key, Object, Value>>;

} // namespace TG
#endif
