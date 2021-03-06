#ifndef __PUBSUB__
#define __PUBSUB__
#include <unordered_set>
#include <type_traits>
#include <functional>
#include <memory>

namespace pubsub {

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
    void subscribe (std::shared_ptr<Subscriber<T>> sub) {
        Subscriber<T>::listOfSub.push_back (sub);
        subscribe(&*sub); 

    }

    template <typename T>                
    void unsubscribe (std::shared_ptr<Subscriber<T>> sub) {
        
        Subscriber<T>::listOfSub.erase (  
            std::remove (    Subscriber<T>::listOfSub.begin(),    Subscriber<T>::listOfSub.end(), sub),
            Subscriber<T>::listOfSub.end() );
        unsubscribe(&*sub); 

    }
    template <typename T>                
    void subscribe ( std::function<void(const T&)> f){
        struct Sub:public Subscriber<T> {
            void onNotified(const T& e)override  {
                f(e);
            }
            Sub(  std::function<void(const T&)> f): f{f} {}

            std::function<void(const T&)> f;

        };

        auto sub = std::make_shared<Sub> (f);
        static std::vector<std::shared_ptr<Sub>> lambdalistOfSub ; // list to hold the shared_ptr to prevent them from being destroyed when this function returns

        lambdalistOfSub.push_back(sub);
        subscribe<T>(&*sub); 

    }

    template <typename T>                
    void unsubscribe (Subscriber<T>* sub) {
        Subscriber<T>::subscribersSet.erase(sub); 

    }


    template <typename T> 
    class Subscriber {
    private:
        static std::unordered_set<Subscriber<T>*> subscribersSet;
        static std::vector<std::shared_ptr<Subscriber<T>>> listOfSub ; 
            
        template <typename T2>                
        friend void notify (const T2& e); 

        template <typename T2>                
        friend void subscribe (Subscriber<T2>* sub); 

        template <typename T2>                
        friend void subscribe ( std::function<void(const T2&)> f); 

        template <typename T2>                
        friend void unsubscribe (Subscriber<T2>* sub); 


        template <typename T2>                
        friend void subscribe (std::shared_ptr<Subscriber<T2>> sub) ;

        template <typename T2>
        friend void unsubscribe (std::shared_ptr<Subscriber<T2>> sub) ;

    public:
        Subscriber(){}
        virtual ~Subscriber() {
            unsubscribe<T> (this);
        }
            

        virtual void onNotified(const T& e){ // supposed to be pure virtual (=0) but gcc4.9 gives error
            assert(0); //subclass should override  this function
        }
        
    };


    template <typename T>
    std::unordered_set<Subscriber<T>*> Subscriber<T>::subscribersSet;

    template <typename T>                
    std::vector<std::shared_ptr<Subscriber<T>>> Subscriber<T>::listOfSub ; 
} // namespace PubSub


#endif //__PUBSUB__
