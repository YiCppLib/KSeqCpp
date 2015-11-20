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

    Impl():
        filename(NULL)
    {
        fp = gzdopen(fileno(stdin), "r");
        if(fp != Z_NULL) seq = kseq_init(fp);
    }

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


KSeqCpp::KSeqCpp() : pImpl_(std::unique_ptr<Impl>(new Impl())) {
}

KSeqCpp::KSeqCpp(const std::string& filename) : pImpl_(std::unique_ptr<Impl>(new Impl(filename.c_str()))) {
}

KSeqCpp::~KSeqCpp() = default;
KSeqCpp::KSeqCpp(KSeqCpp&& rhs) = default;
KSeqCpp& KSeqCpp::operator=(KSeqCpp&& rhs) = default;

bool KSeqCpp::isOpen() const noexcept {
	if(pImpl_.get() == nullptr) return false;
	return pImpl_->fp != Z_NULL;
}

KSeqCpp::FastqRecord::u_ptr KSeqCpp::nextRecord() {
	if(!isOpen()) throw Exceptions::FileNotOpenException();

	int l = kseq_read(pImpl_->seq);

	if(l < 0) {
		throw Exceptions::EndOfFileException();
	}

	return FastqRecord::u_ptr(new FastqRecord {
		.name = std::string(pImpl_->seq->name.s),
		.comment = pImpl_->seq->comment.l ? std::string(pImpl_->seq->comment.s) : "",
		.sequence = std::string(pImpl_->seq->seq.s),
		.quality = pImpl_->seq->qual.l ? std::string(pImpl_->seq->qual.s) : ""});
}


