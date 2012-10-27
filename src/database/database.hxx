#ifndef PICT_STOCK_DATABASE_HXX
# define PICT_STOCK_DATABASE_HXX


namespace PictStock
{
namespace Database
{

    inline void Database::addNewPhotographer(const TablePhotographers::FirstName::WrappedType& firstName,
        const TablePhotographers::LastName::WrappedType& lastName,
        const TablePhotographers::Abbreviation::WrappedType& abbreviation)
    {
        pPhotographers->addNewPhotographer(firstName, lastName, abbreviation);
    }


    template<class StringT>
    inline void Database::identifyCameraOwner(
        const StringT& abbreviation,
        Photographer::Ptr& photographer) const
    {
        pCameras->identifyPhotographerAbbr(abbreviation, photographer);
    }


    inline const Cameras& Database::cameras() const
    {
        assert(pCameras);
        return *pCameras;
    }


    inline const YString& Database::path() const
    {
        return pPath;
    }




} // namespace Database
} // namespace PictStock


#endif // DATABASE_HXX
