#ifndef ASSOCIATED_VECTOR_HPP
# define ASSOCIATED_VECTOR_HPP

# include <vector>
# include <algorithm>
# include <functional>

namespace GenericTools
{

    namespace Nature
    {
        enum Value
        {
            noDuplicatedKey,
            multi
        };
    }

    namespace Private
    {
        template<class Pair>
        class PairLess
        {
        private:

            typedef typename Pair::first_type first_type;

        public:

            inline bool operator()(const Pair& lhs, const Pair& rhs) const
            {
                return keyLess(lhs.first, rhs.first);
            }


            inline bool operator()(const Pair& lhs, const first_type& rhs) const
            {
                return keyLess(lhs.first, rhs);
            }


            inline bool operator()(const first_type& lhs, const Pair& rhs) const
            {
                return keyLess(lhs, rhs.first);
            }

        private:

            inline bool keyLess(const first_type& lhs, const first_type& rhs) const
            {
                return lhs < rhs;
            }
        };

    }


    template<class T, class U, Nature::Value = Nature::noDuplicatedKey>
    class AssociativeVector
    {
    private:

        typedef typename std::pair<T, U> Pair;

        typedef typename std::vector<Pair> UnderlyingVector;

    public:

        typedef typename UnderlyingVector::const_iterator const_iterator;

        typedef typename UnderlyingVector::size_type size_type;

    private:

        typedef typename UnderlyingVector::iterator iterator;

    public:

        //! Constructor
        AssociativeVector()
        { }

        //! Copy constructor
        AssociativeVector(const AssociativeVector& rhs)
            : pUnderlyingVector(rhs.pUnderlyingVector)
        { }

        //! Move constructor
        AssociativeVector(AssociativeVector&& rhs)
            : pUnderlyingVector(rhs.pUnderlyingVector)
        { }


        //! Affectation
        AssociativeVector& operator=(const AssociativeVector& rhs)
        {
            pUnderlyingVector = rhs.pUnderlyingVector;
            return *this;
        }

        //! Begin const iterator
        inline const_iterator cbegin() const { return pUnderlyingVector.cbegin(); }

        //! End const iterator
        inline const_iterator cend() const { return pUnderlyingVector.cend(); }

        //! Size
        inline size_type size() const { return pUnderlyingVector.size(); }

        //! Reserve
        inline void reserve(size_type n) { pUnderlyingVector.reserve(n); }

        //! Resize
        inline void resize(size_type n) { pUnderlyingVector.resize(n); }

        /*!
         * \brief Insert a new pair
         *
         * Remember: this operation is costly and should be done very few, in
         * initialization part. If not the case, std::map or std::unordered_map
         * are very likely more effective choices...
         */
        const_iterator insert(const Pair& newPair);

        /*!
         * \brief Look-up for a given value, or the position it would have
         *
         * TODO Improve so that T for integral types instead of const T& (see traits...)
         */
        inline const_iterator lower_bound(const T& key) const;

        inline const_iterator upper_bound(const T& key) const;

        std::pair<const_iterator, const_iterator> equal_range(const T& key) const;

        //! Same as lower_bound, except end() is returned if the exact match is not found
        inline const_iterator find(const T& key) const;


    private:

        //! Lower-bound and find helper
        template<bool IsEquality>
        inline const_iterator lower_bound_helper(const T& key) const;


    private:

        //! Underlying sorted vector
        UnderlyingVector pUnderlyingVector;

    };


    class AssociativeVectorException : public std::exception
    {



    };



} // namespace GenericTools


# include "associative_vector.hxx"

#endif // ASSOCIATED_VECTOR_HPP
