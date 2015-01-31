#ifndef YICPPLIB_KSEQCPP_H
#define YICPPLIB_KSEQCPP_H

#include <memory>
#include <string>

namespace YiCppLib {
	class KSeqCpp {
		public:
			struct FastqRecord {
				using u_ptr = std::unique_ptr<FastqRecord>;
				const std::string name;
				const std::string comment;
				const std::string sequence;
				const std::string quality;
			};

			using u_ptr = std::unique_ptr<KSeqCpp>;

			KSeqCpp(const std::string& filename);

			// following three function needs to be 
			// synthized in implementation file
			virtual ~KSeqCpp();
			KSeqCpp(KSeqCpp&& rhs);
			KSeqCpp& operator=(KSeqCpp&& rhs);

			bool isOpen() const noexcept;
			FastqRecord::u_ptr nextRecord();
			
		protected:
			struct Impl;
			std::unique_ptr<Impl> pImpl_;
	};

	namespace Exceptions {
		class FileNotOpenException : public std::runtime_error {
			public:
				FileNotOpenException() : std::runtime_error("FASTA / FASTQ File Not Open") {;}
		};

		class EndOfFileException: public std::runtime_error {
			public:
				EndOfFileException() : std::runtime_error("At the end of the file") {;}
		};	
	}
}

#endif
