#ifndef __PUBSUB__
#define __PUBSUB__
#include <unordered_set>
#include <type_traits>

namespace PubSub {


    template <typename T> 
    class Subscriber {
    public:
        Subscriber(){}
        virtual ~Subscriber() {}

        virtual void onNotified(const T& e)=0;
        
        template<typename T2> 
        void subscribe() {
          subscribersSet.insert(this); 
        }
        template<typename T2> 
        void unsubscribe() {
          subscribersSet.erase(this); 
        }
    private:
        static std::unordered_set<Subscriber<T>*> subscribersSet;
            
        template <typename T2>                
        friend void notify (const T2& e); 
    };


    template <typename T>
    std::unordered_set<Subscriber<T>*> Subscriber<T>::subscribersSet;

    template <typename T> 
    void notify(const T& e) {

        for (auto& sub: Subscriber<T>::subscribersSet) {
            sub->onNotified(e);
        }

    }

} // namespace PubSub


#endif //__PUBSUB__
