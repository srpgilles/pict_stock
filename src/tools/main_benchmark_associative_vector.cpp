#include <map>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <string>

#include <yuni/core/string.h>
#include <yuni/core/logs.h>
#include <yuni/core/math/random/range.h>
#include <yuni/core/math/random/default.h>
#include <yuni/datetime/timestamp.h>

#include <boost/container/flat_map.hpp>

#include "associative_vector.hpp"


typedef Yuni::Logs::Logger<> LoggingFacility;

typedef std::chrono::steady_clock Clock; // in microseconds

const std::vector<std::string> shapes { "point", "segment3", "segment2",
                                  "triangle3", "triagnel6",
                                  "tetraedre5", "tetraedre13",
                                  "prism6", "prism15",
                                  "foo17", "foo19",
                                  "bar24", "bar27" };


template<class T>
void InitAssocContainer(T& container, unsigned int NB, Clock::duration& duration)
{
    auto t0 = Clock::now();
    for (unsigned int i = 0u; i < NB; ++i)
        container.insert(std::make_pair(shapes[i], cos(i) * sin(i)));
    duration = Clock::now()- t0;
}


template<class T>
void LookUpAssocContainer(const T& container, typename T::const_iterator& iterator,  const std::string& key, Clock::duration& duration)
{
    auto t0 = Clock::now();
    iterator = container.find(key);
    duration += Clock::now()- t0;
}


int main()
{




    static const unsigned int NB = 13;

    std::ofstream FILE_Hash("FileHash.txt", std::ios::app);
    std::ofstream FILE_Map("FileMap.txt", std::ios::app);
    std::ofstream FILE_AV("FileAssociatedvector.txt", std::ios::app);

	LoggingFacility logs;

    Clock::duration durationInitMap, durationInitHash, durationInitVec;

    // Init std::unordered_map
    std::unordered_map<std::string, double> UnorderedMap;
    UnorderedMap.reserve(NB);
    InitAssocContainer(UnorderedMap, NB, durationInitHash);

    // Init associative vector
    GenericTools::AssociativeVector<std::string, double> AssociativeVec;
    AssociativeVec.reserve(NB);
   // boost::container::flat_map<std::string, double> AssociativeVec;
    InitAssocContainer(AssociativeVec, NB, durationInitVec);

    // Init std::map
    std::map<std::string, double> StdMap;
    InitAssocContainer(StdMap, NB, durationInitMap);

    //std::cout << "Map -> " << durationInitMap.count() << std::endl;
    //std::cout << "Unordered Map -> " << durationInitHash.count() << std::endl;
    //std::cout << "Assoc vector -> " << durationInitVec.count() << std::endl;

    Yuni::Math::Random::Range<Yuni::Math::Random::Default, 0, NB, unsigned int> randomRange;

    Clock::duration durationMap, durationHash, durationVec;

    auto iterStd = StdMap.cbegin();
    auto iterUnord = UnorderedMap.cbegin();
    auto iterVec = AssociativeVec.cbegin();

    for (unsigned int i = 0u; i < 1000000u; ++i)
    {
        auto key = randomRange();
        std::string keyString = shapes[key];
        LookUpAssocContainer(UnorderedMap, iterUnord, keyString, durationHash);
        LookUpAssocContainer(AssociativeVec, iterVec, keyString, durationVec);
        LookUpAssocContainer(StdMap, iterStd, keyString, durationMap);

        assert(Yuni::Math::Equals(iterStd->second, iterUnord->second));
        assert(Yuni::Math::Equals(iterStd->second, iterVec->second));
    }

    std::cout << "Map -> " << durationMap.count() << std::endl;
    std::cout << "Unordered Map -> " << durationHash.count() << std::endl;
    std::cout << "Assoc vector -> " << durationVec.count() << std::endl;

    FILE_Map << NB << '\t' << durationMap.count() << std::endl;
    FILE_Hash << NB << '\t' << durationHash.count() << std::endl;
    FILE_AV << NB << '\t' << durationVec.count() << std::endl;


	return 0;
}
