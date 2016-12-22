#include "gmock/gmock.h"
#include "pubsub.h"
#include <memory>

using namespace std;
using namespace PubSub;
using namespace testing;

TEST(PubSub, registerAndNotify ) {

    struct Event {
        Event(int v) : val{v}{}
        int val;
    };

    class A:public Subscriber<Event> {
    public:
        A() : Subscriber<Event>()
        {
            
        }
        void onNotified (const Event& e)override {
            receivedVal=e.val;
        }

        int receivedVal=0;
    };

   
    A a; 

    Event e{100};
    subscribe<Event> (&a);
    ASSERT_THAT(a.receivedVal,Eq(0));

    notify<Event>(e);
    ASSERT_THAT(a.receivedVal,Eq(e.val));

    a.receivedVal = 0;
    unsubscribe<Event>(&a); 

    notify<Event>(e);
    ASSERT_THAT(a.receivedVal,Eq(0));

}
template<int n>
struct Event {
    Event(int v) : val{v}{}
    int val;
};
TEST(PubSub, registerAndNotifyMultipleEvents ) {


    class A:public Subscriber<Event<1>>, public Subscriber<Event<2>> {
    public:
        void onNotified (const Event<1>& e)override {
            receivedVal1=e.val;
        }

        void onNotified (const Event<2>& e)override {
            receivedVal2=e.val;
        }
        int receivedVal1=0;
        int receivedVal2=0;
    };


    A a;
    subscribe<Event<1>>(&a); 
    subscribe<Event<2>>(&a); 
    notify(Event<1>{100});
    ASSERT_THAT(a.receivedVal1, Eq(100));
    ASSERT_THAT(a.receivedVal2, Eq(0));
    notify(Event<2>{200});
    ASSERT_THAT(a.receivedVal1, Eq(100));
    ASSERT_THAT(a.receivedVal2, Eq(200));


}

TEST(PubSub, multipleObjectsMultipleEvents ) {


    class A:public Subscriber<Event<1>> {
    public:
        void onNotified (const Event<1>& e)override {
            receivedVal=e.val;
        }

        int receivedVal=0;
    };

    class B:public Subscriber<Event<2>> {
    public:
        void onNotified (const Event<2>& e)override {
            receivedVal=e.val;
        }

        int receivedVal=0;
    };

    A a;
    subscribe<Event<1>>(&a); 
    B b;
    subscribe<Event<2>>(&b); 


    notify(Event<1>{100}); // should not crash here as a is already destroyed
    notify(Event<2>{200}); // should not crash here as a is already destroyed

    ASSERT_THAT(a.receivedVal, Eq(100));
    ASSERT_THAT(b.receivedVal, Eq(200));



}

TEST(PubSub, registerThenDestroy ) {


    class A:public Subscriber<Event<1>> {
    public:
        void onNotified (const Event<1>& e)override {
            receivedVal1=e.val;
        }

        int receivedVal1=0;
    };

    {
        A a;
        subscribe<Event<1>>(&a); 
    } // exit scope, a is destroyed
    notify(Event<1>{200}); // should not crash here as a is already destroyed


}

TEST(PubSub, registerSharedPtr ) {


    class A:public Subscriber<Event<1>> {
    public:
        void onNotified (const Event<1>& e)override {
            receivedVal=e.val;
        }

        int receivedVal=0;
    };

    auto  a = make_shared<A>();
    subscribe<Event<1>>(&*a);  // not nice trick still

    notify(Event<1>{200}); 
    ASSERT_THAT(a->receivedVal, Eq(200));


}
int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
