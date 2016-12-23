[![Build Status](https://travis-ci.org/alext234/PubSub.svg?branch=master)](https://travis-ci.org/alext234/PubSub)


# PubSub
Publish-Subscribe pattern with modern C++. Following is sample usage:

```C++
#include <iostream>
#include "pubsub.h"

using namespace std;
using namespace pubsub;

struct Event { };

class A: public Subscriber<Event> {
public:
    void onNotified (const Event& e)override {
        std::cout<< "event received" << std::endl;    
    }

};



int main() {
    A a; 
    subscribe<Event>(&a);

    notify<Event> (Event());

}

```

More examples can be found in [tests.cpp](../master/tests.cpp), such as when *a* is *shared_ptr*. Other usecases include lambdas and subscribing to multiple events.

