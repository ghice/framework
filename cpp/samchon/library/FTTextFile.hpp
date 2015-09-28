#pragma once
#include <samchon/API.hpp>

#include <samchon/library/FTFile.hpp>

namespace samchon
{
	namespace library
	{
		/**
		 * @brief A text file
		 * @details A file having data as text.
		 *
		 * @image html cpp/library_file_tree.png
		 * @image latex cpp/library_file_tree.png
		 *
		 * @author Jeongho Nam
		 */
		class SAMCHON_FRAMEWORK_API FTTextFile
			: public FTFile
		{
		private:
			typedef protocol::Entity FTFile;

		protected:
			/**
			 * @brief Text recorded in the file
			 */
			std::string data;

		public:
			/**
			 * @copydoc FTFile::FTFile()
			 */
			FTTextFile(FTFolder*);
			virtual ~FTTextFile() = default;

			virtual void construct(std::shared_ptr<library::XML> xml);

			/**
			 * @brief Get string content.
			 */
			auto getData() const -> std::string;

			virtual auto toXML() const->std::shared_ptr<library::XML>;
		};
	};
};