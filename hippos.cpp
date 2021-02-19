#include <hippomocks.h>
#include <iostream>
#include "B.h"

class A {
public:
    int doSomethingToB(int arg1, int arg2);
    void setB(B* b);
private:
    std::shared_ptr<B> b;
};

void 
A::setB(B* b) {
    //if(!b) {
    //    this->b.reset();
    //    return;
    //}
    std::shared_ptr<B> newB(b);
    this->b = newB;
}

int
A::doSomethingToB(int arg1, int arg2) {
    return b->sendMessage(arg1, arg2);
}

int main(int argc, char* argv[]) {

    MockRepository mocks;

    B* mockB = mocks.Mock< B >();
    try {
        A a;

        a.setB(mockB);

        mocks.ExpectCall(mockB, B::sendMessage).With(1,2).Return(-255);
        mocks.ExpectCall(mockB, B::sendMessage).With(3,4).Return(-123);
	// add a call to the destructor because the shared pointer will try to delete the mock
        mocks.ExpectCallDestructor(mockB);

        int r = a.doSomethingToB(1,2);
        std::cout << "a.doSomethingToB returned " << r << std::endl;
        r = a.doSomethingToB(3,4);
        std::cout << "a.doSomethingToB returned " << r << std::endl;

    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    mocks.VerifyAll();
    
    return 0;
}
