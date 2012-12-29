#include <map>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include <unistd.h>

#include <yuni/core/string.h>
#include <yuni/core/logs.h>
#include <yuni/core/math/random/range.h>
#include <yuni/core/math/random/default.h>
#include <yuni/datetime/timestamp.h>

#include "associative_vector.hpp"


typedef Yuni::Logs::Logger<> LoggingFacility;

typedef std::chrono::steady_clock Clock; // in microseconds

template<class T>
void InitAssocContainer(T& container, unsigned int NB, Clock::duration& duration)
{
    auto t0 = Clock::now();
    for (unsigned int i = 0u; i < NB; ++i)
        container.insert(std::make_pair(i, cos(i) * sin(i)));
    duration = Clock::now()- t0;
}


template<class T>
void LookUpAssocContainer(const T& container, typename T::const_iterator& iterator, unsigned int key, Clock::duration& duration)
{
    auto t0 = Clock::now();
    iterator = container.find(key);
    duration += Clock::now()- t0;
}


int main()
{
    static const unsigned int NB = 1000;

	LoggingFacility logs;

    Clock::duration durationInitMap, durationInitHash, durationInitVec;

    // Init std::unordered_map
    std::unordered_map<unsigned int, double> UnorderedMap;
    UnorderedMap.reserve(NB);
    InitAssocContainer(UnorderedMap, NB, durationInitHash);

    // Init associative vector
    GenericTools::AssociativeVector<unsigned int, double> AssociativeVec;
    AssociativeVec.reserve(NB);
    InitAssocContainer(AssociativeVec, NB, durationInitVec);

    // Init std::map
    std::map<unsigned int, double> StdMap;
    InitAssocContainer(StdMap, NB, durationInitMap);

    std::cout << "Map -> " << durationInitMap.count() << std::endl;
    std::cout << "Unordered Map -> " << durationInitHash.count() << std::endl;
    std::cout << "Assoc vector -> " << durationInitVec.count() << std::endl;

    Yuni::Math::Random::Range<Yuni::Math::Random::Default, 0, NB, unsigned int> randomRange;

    Clock::duration durationMap, durationHash, durationVec;

    auto iterStd = StdMap.cbegin();
    auto iterUnord = UnorderedMap.cbegin();
    auto iterVec = AssociativeVec.cbegin();

    for (unsigned int i = 0u; i < 10000u; ++i)
    {
        auto key = randomRange();
        LookUpAssocContainer(UnorderedMap, iterUnord, key, durationHash);
        LookUpAssocContainer(StdMap, iterStd, key, durationMap);
        LookUpAssocContainer(AssociativeVec, iterVec, key, durationVec);

        assert(Yuni::Math::Equals(iterStd->second, iterUnord->second));
        assert(Yuni::Math::Equals(iterStd->second, iterVec->second));
    }

    std::cout << "Map -> " << durationMap.count() << std::endl;
    std::cout << "Unordered Map -> " << durationHash.count() << std::endl;
    std::cout << "Assoc vector -> " << durationVec.count() << std::endl;




	return 0;
}
