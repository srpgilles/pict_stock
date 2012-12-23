//#include <yuni/core/getopt.h>
//#include "database/database.hpp"
//#include "extended_photo/extended_photo.hpp"

#include <iostream>
#include <vector>
#include <cassert>


/*!
 * \brief Find the index of all rows for which a given column is not empty
 *
 * \param[in] iColumn Column considered
 * \param[in] ia Array of index of first nonzero element of row i
 * \param[in] ja Array of column index of each A element
 *
 * \param[out] out List of all rows with non-zero values for the given column
 * First parameter is the index of the row, second is the index in ja array that yielded this result
 */
void findNonZeroRows(std::vector<std::pair<int, std::vector<int>::size_type> >& out, int iColumn, const std::vector<int>& ia, const std::vector<int>& ja)
{
    out.clear();
    int rowIndex = 0;
    int maxRowIndex = ia.size(); // which is number of rows of the dense matrix + 1
    assert(maxRowIndex > 0);
    --maxRowIndex;

    for (std::vector<int>::size_type i = 0, nbColumnsJa = ja.size(); i < nbColumnsJa; ++i)
    {
        // Consider only indexes in ja that means the chosen column is non zero
        if (ja[i] != iColumn)
            continue;

        //std::cout << "Index to consider = " << i << '\n';

        // Determine the row to which it belongs through the use of ia
        while (ia[rowIndex + 1] <= i)
        {
            ++rowIndex;
            assert(rowIndex < maxRowIndex);
        }

        //std::cout << "Result = " << rowIndex << '\n';
        out.push_back(std::make_pair(rowIndex, i));
    }
}


void nullifyNonDiagonalTerms(std::vector<double>& values, int iColumn, const std::vector<int>& ia, const std::vector<int>& ja)
{
    std::vector<std::pair<int, std::vector<int>::size_type> > nonNullRows;
    findNonZeroRows(nonNullRows, iColumn, ia, ja);

    // assert(values.size() > iColumn);

    for (auto it = nonNullRows.cbegin(), end = nonNullRows.cend(); it != end; ++it)
    {
        if (it->first == iColumn)
            continue;

        values[it->second] = 0.;

    }
}


int main(int argc, char* argv[])
{
    (void) argc;
	(void) argv;


    //[ 10 20  0  0  0  0 ]
    //[  0 30  0 40  0  0 ]
    //[  0  0 50 60 70  0 ]
    //[  0  0  0  0  0 80 ]

    //ExtendedPhoto::ExtendedPhoto photo(logs, database, "/tmp/Baptême/Photo.JPG");

    std::vector<double> values = {10., 20., 30., 40., 50., 60., 70., 80.};
    std::vector<int> ia = {0, 2, 4, 7, 8};
    std::vector<int> ja = {0, 1, 1, 3, 2, 3, 4, 5 };

    std::vector<int> result;

//    for (unsigned int i = 0; i < 6; ++i)
//    {
//        findNonZeroRows(result, i, ia, ja);
//        std::cout << "For column " << i << ": ";
//        for (auto it = result.cbegin(), end = result.cend(); it != end; ++it)
//            std::cout << *it << '\t';
//        std::cout << '\n';
//    }


    nullifyNonDiagonalTerms(values, 5, ia, ja);
    std::cout << "Nullify: ";
    for (auto it = values.cbegin(), end = values.cend(); it != end; ++it)
        std::cout << *it << '\t';
    std::cout << '\n';


	return 0;
}
