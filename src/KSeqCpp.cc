#include "KSeqCpp.h"
#include "kseq.h"
#include <zlib.h>
#include <iostream>

KSEQ_INIT(gzFile, gzread);

using namespace YiCppLib;

struct KSeqCpp::Impl {
	const char *filename;
	gzFile fp;
	kseq_t *seq;


	Impl(const char *filename) : 
		filename(filename) 
	{
		fp = gzopen(filename, "r");
		if(fp != Z_NULL) seq = kseq_init(fp);
	}

	~Impl() {
		if(fp != Z_NULL) {
			gzclose(fp);
		}
	}
};

KSeqCpp::KSeqCpp(const std::string& filename) : pImpl_(std::unique_ptr<Impl>(new Impl(filename.c_str()))) {
}

KSeqCpp::~KSeqCpp() = default;
KSeqCpp::KSeqCpp(KSeqCpp&& rhs) = default;
KSeqCpp& KSeqCpp::operator=(KSeqCpp::KSeqCpp&& rhs) = default;

bool KSeqCpp::isOpen() const noexcept {
	if(pImpl_.get() == nullptr) return false;
	return pImpl_->fp != Z_NULL;
}

KSeqCpp::FastqRecord KSeqCpp::nextRecord() {
	if(!isOpen()) throw Exception::FileNotOpenException();

	int l = kseq_read(pImpl_->seq);

	if(l < 0) {
		pImpl_.reset(nullptr);
		throw Exception::EndOfFileException();
	}

	return FastqRecord {
		std::string(pImpl_->seq->name.s),
		pImpl_->seq->comment.l ? std::string(pImpl_->seq->comment.s) : "",
		std::string(pImpl_->seq->seq.s),
		pImpl_->seq->qual.l ? std::string(pImpl_->seq->qual.s) : "",
	};
}


