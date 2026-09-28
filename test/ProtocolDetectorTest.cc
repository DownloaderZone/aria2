#include "ProtocolDetector.h"

#include <cppunit/extensions/HelperMacros.h>

#include "Exception.h"
#include "util.h"

namespace aria2 {

class ProtocolDetectorTest : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(ProtocolDetectorTest);
  CPPUNIT_TEST(testIsStreamProtocol);
  CPPUNIT_TEST(testGuessMetalinkFile);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}

  void tearDown() {}

  void testIsStreamProtocol();
  void testGuessMetalinkFile();
};

CPPUNIT_TEST_SUITE_REGISTRATION(ProtocolDetectorTest);

void ProtocolDetectorTest::testIsStreamProtocol()
{
  ProtocolDetector detector;
  CPPUNIT_ASSERT(detector.isStreamProtocol("http://localhost/index.html"));
  CPPUNIT_ASSERT(detector.isStreamProtocol("ftp://localhost/index.html"));
  CPPUNIT_ASSERT(!detector.isStreamProtocol("/home/web/localhost/index.html"));
}

void ProtocolDetectorTest::testGuessMetalinkFile()
{
  ProtocolDetector detector;
  CPPUNIT_ASSERT(detector.guessMetalinkFile(A2_TEST_DIR "/test.xml"));
  CPPUNIT_ASSERT(!detector.guessMetalinkFile("http://localhost/test.xml"));
  CPPUNIT_ASSERT(
      !detector.guessMetalinkFile(A2_TEST_DIR "/gzip_decode_test.gz"));
}

} // namespace aria2
