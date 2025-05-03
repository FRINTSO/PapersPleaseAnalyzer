#include "pch.h"
#include "test/core/pool_test.h"

#include "paplease/core/pool.h"

namespace test {
    namespace core {

        using namespace paplease::core;

        struct MyStruct
        {
            MyStruct() : myNums(nullptr) {}
            MyStruct(size_t size)
            {
                myNums = new int[size] {};
            }

            ~MyStruct() {
                if (myNums)
                {
                    delete[] myNums;
                }
            }

            int* myNums;
        };

        void test_pool()
        {
            PoolBound<std::string> string1;
            PoolBound<std::string> string2;

            Pool<std::string, 10> stringPool;

            string1 = stringPool.Allocate("Hello this is my string");
            {
                auto string3 = stringPool.Allocate("intermediate string");
                std::cout << string3.Value() << "\n";
            }
            string2 = stringPool.Allocate("Hello, this is my second string");

            std::cout << string1.Value() << "\n";
            std::cout << string2.Value() << "\n";


            std::cin.get();

        }

    }
}