#ifndef __PUBSUB__
#define __PUBSUB__
#include <unordered_set>
#include <type_traits>

namespace PubSub {

    template <typename T>
    class Subscriber ;


    template <typename T> 
    void notify(const T& e) {

        for (auto& sub: Subscriber<T>::subscribersSet) {
            sub->onNotified(e);
        }

    }

    template <typename T>                
    void subscribe (Subscriber<T>* sub) {
        Subscriber<T>::subscribersSet.insert(sub); 

    }
    template <typename T>                
    void unsubscribe (Subscriber<T>* sub) {
        Subscriber<T>::subscribersSet.erase(sub); 

    }


    template <typename T> 
    class Subscriber {
    private:
        static std::unordered_set<Subscriber<T>*> subscribersSet;
            
        template <typename T2>                
        friend void notify (const T2& e); 

        template <typename T2>                
        friend void subscribe (Subscriber<T2>* sub); 


        template <typename T2>                
        friend void unsubscribe (Subscriber<T2>* sub); 
    public:
        Subscriber(){}
        virtual ~Subscriber() {
            unsubscribe<T> (this);
        }
            

        virtual void onNotified(const T& e){}
        
    };


    template <typename T>
    std::unordered_set<Subscriber<T>*> Subscriber<T>::subscribersSet;

} // namespace PubSub


#endif //__PUBSUB__
