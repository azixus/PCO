#include "criticalsection.h"
#include <thread>
#include <algorithm>

void WonderfulCriticalSection::initialize(unsigned int nbThreads)
{
    tickets.resize(nbThreads, 0);
    entering.resize(nbThreads, 0);
}

void WonderfulCriticalSection::lock(unsigned int index)
{
    entering[index] = 1;
    tickets[index] = 1 + *std::max_element(tickets.begin(), tickets.end());
    entering[index] = 0;

    for (unsigned int other = 0; other < entering.size(); ++other) {
        while (entering[other]) {
            std::this_thread::yield();
        }

        while(tickets[other] != 0 && (tickets[other] < tickets[index] ||
                                        (tickets[other] == tickets[index] && other < index))) {
            std::this_thread::yield();
        }
    }
}

void WonderfulCriticalSection::unlock(unsigned int index)
{
    tickets[index] = 0;
}
