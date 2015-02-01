#include <gmock/gmock.h>
#include "KSeqCpp.h"

using namespace YiCppLib;
using namespace ::testing;

class KSeqCppFixture : public Test {
	protected:
		KSeqCpp::u_ptr kSeqCppPlain;
		KSeqCpp::u_ptr kSeqCppGzip;
		KSeqCpp::u_ptr kSeqCppNonexist;

		KSeqCppFixture() : 
			kSeqCppPlain(KSeqCpp::u_ptr(new KSeqCpp(TEST_DATADIR "/small.fastq"))),
			kSeqCppGzip(KSeqCpp::u_ptr(new KSeqCpp(TEST_DATADIR "/small2.fastq.gz"))),
			kSeqCppNonexist(KSeqCpp::u_ptr(new KSeqCpp(TEST_DATADIR "/nonexist"))) {
				;
			}
};

TEST_F(KSeqCppFixture, CanCreateObject) {
	ASSERT_TRUE(kSeqCppPlain.get());
}

TEST_F(KSeqCppFixture, CanOpenExistingPlainFile) {
	ASSERT_TRUE(kSeqCppPlain->isOpen());
}

TEST_F(KSeqCppFixture, CanOpenExistingGzipFile) {
	ASSERT_TRUE(kSeqCppGzip->isOpen());
}

TEST_F(KSeqCppFixture, CanDetectUnexistingFile) {
	ASSERT_FALSE(kSeqCppNonexist->isOpen());
}

TEST_F(KSeqCppFixture, CanReturnOneSeqFromPlain) {
	auto seq = kSeqCppPlain->nextRecord();
	ASSERT_THAT(seq->sequence.size(), Gt(0));
}

TEST_F(KSeqCppFixture, CanReturnOneSeqFromGzip) {
	auto seq = kSeqCppGzip->nextRecord();
	ASSERT_THAT(seq->sequence.size(), Gt(0));
}

TEST_F(KSeqCppFixture, WillThrowFileNotOpenException) {
	EXPECT_THROW({
			kSeqCppNonexist->nextRecord();
			}, Exceptions::FileNotOpenException);
}

TEST_F(KSeqCppFixture, WillThrowEndOfFileException) {
	EXPECT_THROW({
			while(true) kSeqCppPlain->nextRecord();
			}, Exceptions::EndOfFileException);
}
