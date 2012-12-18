#ifndef ASSOCIATIVE_VECTOR_HXX
# define ASSOCIATIVE_VECTOR_HXX


namespace GenericTools
{

    template<class T, class U, Nature::Value V>
    typename AssociativeVector<T, U, V>::const_iterator
    AssociativeVector<T, U, V>::insert(const Pair& newPair)
    {
        iterator it = std::lower_bound(pUnderlyingVector.begin(), pUnderlyingVector.end(),
                                       newPair.first, Private::PairLess<Pair>());

        // Does not allow duplication if V is single (more severe than a map,
        // that would simply overwrite
        // If V = multi we don't care...
        if (V == Nature::noDuplicatedKey)
        {
            if (it != pUnderlyingVector.end() && (it->first == newPair.first))
                throw AssociativeVectorException();
        }

        pUnderlyingVector.insert(it, newPair);
        return std::move(it);
    }

    template<class T, class U, Nature::Value V>
    template<bool EqualityT>
    typename AssociativeVector<T, U, V>::const_iterator
    AssociativeVector<T, U, V>::lower_bound_helper(const T& key) const
    {
        // Should be sorted, or the container is completely useless!
        assert(std::is_sorted(pUnderlyingVector.cbegin(), pUnderlyingVector.cend(),
                              Private::PairLess<Pair>()));

        auto end = pUnderlyingVector.cend();
        auto ret = std::lower_bound(pUnderlyingVector.cbegin(), end,
                                    key, Private::PairLess<Pair>());

        // Equivalence case
        if (!EqualityT)
            return std::move(ret);

        // Equality case
        return (ret->first == key ? ret : end);
    }




    template<class T, class U, Nature::Value V>
    typename AssociativeVector<T, U, V>::const_iterator
    AssociativeVector<T, U, V>::lower_bound(const T& key) const
    {
        return lower_bound_helper<false>(key);
    }


    template<class T, class U, Nature::Value V>
    typename AssociativeVector<T, U, V>::const_iterator
    AssociativeVector<T, U, V>::find(const T& key) const
    {
        return lower_bound_helper<true>(key);
    }


    template<class T, class U, Nature::Value V>
    typename AssociativeVector<T, U, V>::const_iterator
    AssociativeVector<T, U, V>::upper_bound(const T& key) const
    {

    // Should be sorted, or the container is completely useless!
    assert(std::is_sorted(pUnderlyingVector.cbegin(), pUnderlyingVector.cend(),
                          Private::PairLess<Pair>()));

    return std::upper_bound(pUnderlyingVector.cbegin(), pUnderlyingVector.cend(),
                            key, Private::PairLess<Pair>());
    }



    template<class T, class U, Nature::Value V>
    std::pair<typename AssociativeVector<T, U, V>::const_iterator,
              typename AssociativeVector<T, U, V>::const_iterator>
    AssociativeVector<T, U, V>::equal_range(const T& key) const
    {

    // Should be sorted, or the container is completely useless!
    assert(std::is_sorted(pUnderlyingVector.cbegin(), pUnderlyingVector.cend(),
                          Private::PairLess<Pair>()));

    return std::equal_range(pUnderlyingVector.cbegin(), pUnderlyingVector.cend(),
                            key, Private::PairLess<Pair>());
    }




} // namespace GenericTools

#endif // ASSOCIATIVE_VECTOR_HXX
