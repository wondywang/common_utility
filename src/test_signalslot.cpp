
/*
 * test case for signal slot
 */
#include <map>
#include <memory>
#include <functional>
namespace mp
{
#define make_slot(FUNC,OBJ) mp::easy_bind(FUNC,OBJ)

    template <typename Object, typename ReturnType, typename... Args>
    inline std::function<ReturnType(Args...)> easy_bind(ReturnType(Object::*const MemPtr)(Args...), Object& obj)
    {
        return [=, &obj](Args... args) { (obj.*MemPtr)(args...); };
    }
    template <typename Object, typename ReturnType, typename... Args>
    inline std::function<ReturnType(Args...)> easy_bind(ReturnType(Object::*const MemPtr)(Args...) const, const Object& obj)
    {
        return [=, &obj](Args... args) { (obj.*MemPtr)(args...); };
    }

    template <typename Object, typename ReturnType, typename... Args>
    inline std::function<ReturnType(Args...)> easy_bind(ReturnType(Object::*const MemPtr)(Args...), Object* const obj)
    {
        return [MemPtr, obj](Args... args) { (obj->*MemPtr)(args...); };
    }
    template <typename Object, typename ReturnType, typename... Args>
    inline std::function<ReturnType(Args...)> easy_bind(ReturnType(Object::*const MemPtr)(Args...) const, const Object* const obj)
    {
        return [MemPtr, obj](Args... args) { (obj->*MemPtr)(args...); };
    }

    template <typename Object, typename ReturnType, typename... Args>
    inline std::function<ReturnType(Args...)> easy_bind(ReturnType(*const FunctionPtr)(Args...))
    {
        return [=](Args... args) { (*FunctionPtr)(args...); };
    }

    class connection
    {
    public:
        connection() = default;
        connection(std::function<void(void)>&& deleter)
            : m_deleter(std::move(deleter))
        {
        }
        connection&operator=(std::function<void(void)>&& deleter)
        {
            m_deleter = std::move(deleter);
            return *this;
        }

        void disconnect()
        {
            if (m_deleter)
            {
                m_deleter();
                m_deleter = nullptr;
            }
        }

    private:
        std::function<void(void)> m_deleter;
    };

    class scoped_connection : public connection
    {
    public:
        scoped_connection() = default;
        scoped_connection(connection &&rhs)
        {
            connection::operator=(std::move(rhs));
        }
        scoped_connection& operator=(connection &&rhs)
        {
            disconnect();
            connection::operator=(std::move(rhs));
            return *this;
        }
        scoped_connection(const scoped_connection &rhs) = delete;
        scoped_connection& operator=(const scoped_connection &rhs) = delete;
        scoped_connection(scoped_connection &&rhs)
        {
            connection::operator=(std::move(rhs));
        }
        scoped_connection& operator=(scoped_connection &&rhs)
        {
            disconnect();
            connection::operator=(std::move(rhs));
            return *this;
        }

        ~scoped_connection()
        {
            disconnect();
        }
    };

    template <class ...param_t>
    class signal
    {
    public:
        typedef std::function<void(param_t...)> slot_type;
    private:
        typedef std::map<size_t, slot_type>     slot_list_t;
        std::shared_ptr<slot_list_t>            m_slot_list = std::make_shared<slot_list_t>();
        size_t                                  m_seq = 0;
    public:
        void clear()
        {
            m_slot_list->clear();
        }
        bool empty() const
        {
            return m_slot_list->empty();
        }

        signal() = default;
        connection connect(slot_type&& slot)
        {
            size_t key = ++m_seq;
            m_slot_list->emplace(key, std::move(slot));
            std::weak_ptr<slot_list_t> wp_slot = m_slot_list;
            return std::function<void(void)>([wp_slot, key]{
                auto slot_list = wp_slot.lock();
                if (slot_list)
                    slot_list->erase(key);
            });
        }

        template <typename Object, typename ReturnType>
        inline connection connect(ReturnType(Object::*const MemPtr)(param_t...), Object& obj)
        {
            return connect([=, &obj](param_t... args) { (obj.*MemPtr)(args...); });
        }

        template <typename Object, typename ReturnType>
        inline connection connect(ReturnType(Object::*const MemPtr)(param_t...) const, const Object& obj)
        {
            return connect([=, &obj](param_t... args) { (obj.*MemPtr)(args...); });
        }

        template <typename Object, typename ReturnType>
        inline connection connect(ReturnType(Object::*const MemPtr)(param_t...), Object* const obj)
        {
            return connect([MemPtr, obj](param_t... args) { ((*obj).*MemPtr)(args...); });
        }

        template <typename Object, typename ReturnType>
        inline connection connect(ReturnType(Object::*const MemPtr)(param_t...) const, const Object*const obj)
        {
            return connect([MemPtr, obj](param_t... args) { ((*obj).*MemPtr)(args...); });
        }

        template <typename Object, typename ReturnType>
        inline connection connect(ReturnType(*const FunctionPtr)(param_t...))
        {
            return connect([=](param_t... args) { (*FunctionPtr)(args...); });
        }

        template<typename ...args_t>
        inline void operator() (args_t&&... args) const
        {
            slot_list_t  slot_list = *m_slot_list;
            for (auto&a : slot_list)
            {
                if (m_slot_list->find(a.first) != m_slot_list->end())
                {
                    if (a.second)
                        a.second(std::forward<args_t>(args)...);
                }
            }
        }
    };
} // namespace

#include <iostream>
#include <stdio.h>
class CCC
{
public:
    void fn(const char* var)
    {
        printf("%s CCC::fn\n", var);
    }
private:    
    int bb;
};

void fn(const char* var)
{
    printf("%s fn2\n", var);
}

int main(int argc, char * argv[])
{
    mp::signal<const char*> sig;
    CCC a;
    mp::connection conn = sig.connect(&CCC::fn, &a);
    sig.connect(&fn);
    sig("sig 1");
    conn.disconnect();
    sig("sig 2");
    
    {
         mp::scoped_connection conn2 = sig.connect(&CCC::fn, &a); 
         sig("sig 3");
    }
    
    sig("sig 4");
    
	return 0;
}
