#include "gmock/gmock.h"
#include "pubsub.h"

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
    a.subscribe<Event> ();
    ASSERT_THAT(a.receivedVal,Eq(0));

    notify<Event>(e);
    ASSERT_THAT(a.receivedVal,Eq(e.val));

    a.receivedVal = 0;
    a.unsubscribe<Event>(); 

    notify<Event>(e);
    ASSERT_THAT(a.receivedVal,Eq(0));

}


int main(int argc, char *argv[])
{
	testing::InitGoogleMock (&argc, argv);
	return RUN_ALL_TESTS();
}
