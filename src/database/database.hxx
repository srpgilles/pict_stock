#ifndef __PICT_STOCK__DATABASE__DATABASE_HXX_
# define __PICT_STOCK__DATABASE__DATABASE_HXX_


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


#endif /* __PICT_STOCK__DATABASE__DATABASE_HXX_ */
