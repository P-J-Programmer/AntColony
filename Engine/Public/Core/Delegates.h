#include <memory>
#include <vector>

/*
 * Delegate Functionality. Note: Currently not thread safe.  
 */
template <typename... Parameters>
class FCallback
{
public:
    virtual void Invoke(Parameters... Params) = 0;
};

template <typename... Parameters>
class FFunctionCallback : public FCallback<Parameters...>
{
public:
    using FFunctionPtr = void (*)(Parameters...);

public:
    FFunctionCallback(FFunctionPtr CallbackFunction)
        : m_Callback(CallbackFunction)
    {}

    virtual void Invoke(Parameters... Params) override
    {
        m_Callback(Params...);
    }

private:
    FFunctionPtr m_Callback;
};

template <typename ObjectType, typename... Parameters>
class FMethodCallback : public FCallback<Parameters...>
{
public:
    using FMethodPtr = void (ObjectType::*)(Parameters...);

public:
    FMethodCallback(ObjectType* Object, FMethodPtr CallbackFunction)
        : m_Object(Object)
        , m_Callback(CallbackFunction)
    {}

    virtual void Invoke(Parameters... Params) override
    {
        (m_Object->*m_Callback)(Params...);
    }

private:
    ObjectType* m_Object;
    FMethodPtr m_Callback;
};

template <typename... Parameters>
class FDelegate
{
private:
    std::vector<std::unique_ptr<FCallback<Parameters...>>> m_Callbacks;

public:

    template <typename ObjectType>
    void Add(ObjectType* Object, typename FMethodCallback<ObjectType, Parameters...>::FMethodPtr Callback)
    {
        m_Callbacks.emplace_back(std::make_unique<FMethodCallback<ObjectType, Parameters...>>(Object, Callback));
    }

    void Add(typename FFunctionCallback<Parameters...>::FFunctionPtr Callback)
    {
        m_Callbacks.emplace_back(std::make_unique<FFunctionCallback<Parameters...>>(Callback));
    }

    void operator()(Parameters... Params)
    {
        for (const std::unique_ptr<FCallback<Parameters...>>& Callback : m_Callbacks)
        {
            Callback->Invoke(Params...);
        }
    }
};