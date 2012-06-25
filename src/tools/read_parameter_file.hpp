#ifndef READ_PARAMETER_FILE_HPP_
# define READ_PARAMETER_FILE_HPP_

# include "../pict_stock.hpp"
# include <yuni/core/string.h>

namespace GenericTools
{


	/*!
	 * \brief Read the parameter file and check the expected fields are there
	 *
	 * \param File Parameters file. It is expected to fill values inputFolder,
	 * outputFolder and logFile
	 * \param mandatoryKeys Keys that are expected in the parameters file
	 * \param[out] parameters Parameters read

	 * \return True if the values were all correctly read
	 */
	class ReadParameterFile
	{
	public:

		// Type of keys in the parameters file
		typedef Yuni::CString<30, false> KeyString;
			// 30 for a safety margin: current lenghtiest key is 13-characters long

		/*!
		 ** Constructor
		 **
		 ** It read the parameter files, load into the class the values that were expected
		 ** and finally check all expected values were actually filled
		 */
		ReadParameterFile(LoggingFacility& logs, const YString& file);

		//! Access to parameters; return false if one is not known
		const YString& operator[](const KeyString& key) const;


	public:

		//! Logs
		LoggingFacility& logs;


	private:

		//! Check #pParameters is correctly filled
		bool checkParameters() const;



	private:

		//! Parameter file
		const YString& pFile;

		/*!
		** \brief Map containing all relevant values read and their associated values
		**
		*/
		std::map<KeyString, YString> pParameters;


	};

} // namespace GenericTools


#endif /* READ_PARAMETER_FILE_HPP_ */
