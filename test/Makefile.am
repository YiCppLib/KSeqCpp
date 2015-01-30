GMOCK_DIR = /opt/gmock-1.7.0
GTEST_DIR = ${GMOCK_DIR}/gtest

AM_CPPFLAGS = -isystem ${GMOCK_DIR}/include \
			  -I${GMOCK_DIR} \
			  -isystem ${GTEST_DIR}/include \
			  -I${GTEST_DIR} \
			  -I$(srcdir)/../src \
			  -D TEST_DATADIR='"$(srcdir)/data"'

LDADD = ../src/libyi_kseqcpp.la \
		${GMOCK_DIR}/lib/libgmock.la \
		${GTEST_DIR}/lib/libgtest.la \
		${GMOCK_DIR}/lib/libgmock_main.la \
		-lz

TESTS = testKSeqCpp

check_PROGRAMS = ${TESTS}
testKSeqCpp_SOURCES=testKSeqCpp.cc


