#include "download_helper.h"

#include <iostream>
#include <string>
#include <algorithm>

#include <cppunit/extensions/HelperMacros.h>

#include "RequestGroup.h"
#include "DownloadContext.h"
#include "Option.h"
#include "array_fun.h"
#include "prefs.h"
#include "Exception.h"
#include "util.h"
#include "FileEntry.h"

namespace aria2 {

class DownloadHelperTest : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(DownloadHelperTest);
  CPPUNIT_TEST(testCreateRequestGroupForUri);
  CPPUNIT_TEST(testCreateRequestGroupForUri_parameterized);
  CPPUNIT_TEST(testCreateRequestGroupForUriList);

#ifdef ENABLE_METALINK
  CPPUNIT_TEST(testCreateRequestGroupForUri_Metalink);
  CPPUNIT_TEST(testCreateRequestGroupForMetalink);
#endif // ENABLE_METALINK

  CPPUNIT_TEST_SUITE_END();

private:
  std::shared_ptr<Option> option_;

public:
  void setUp() { option_.reset(new Option()); }

  void tearDown() {}

  void testCreateRequestGroupForUri();
  void testCreateRequestGroupForUri_parameterized();
  void testCreateRequestGroupForUriList();

#ifdef ENABLE_METALINK
  void testCreateRequestGroupForUri_Metalink();
  void testCreateRequestGroupForMetalink();
#endif // ENABLE_METALINK
};

CPPUNIT_TEST_SUITE_REGISTRATION(DownloadHelperTest);

void DownloadHelperTest::testCreateRequestGroupForUri()
{
  std::vector<std::string> uris{"http://alpha/file", "http://bravo/file",
                                "http://charlie/file"};
  option_->put(PREF_SPLIT, "7");
  option_->put(PREF_MAX_CONNECTION_PER_SERVER, "2");
  option_->put(PREF_DIR, "/tmp");
  option_->put(PREF_OUT, "file.out");
  {
    std::vector<std::shared_ptr<RequestGroup>> result;
    createRequestGroupForUri(result, option_, uris);
    CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
    std::shared_ptr<RequestGroup> group = result[0];
    auto xuris = group->getDownloadContext()->getFirstFileEntry()->getUris();
    CPPUNIT_ASSERT_EQUAL((size_t)6, xuris.size());
    for (size_t i = 0; i < 6; ++i) {
      CPPUNIT_ASSERT_EQUAL(uris[i % 3], xuris[i]);
    }
    CPPUNIT_ASSERT_EQUAL(7, group->getNumConcurrentCommand());
    std::shared_ptr<DownloadContext> ctx = group->getDownloadContext();
    CPPUNIT_ASSERT_EQUAL(std::string("/tmp/file.out"), ctx->getBasePath());
  }
  option_->put(PREF_SPLIT, "5");
  {
    std::vector<std::shared_ptr<RequestGroup>> result;
    createRequestGroupForUri(result, option_, uris);
    CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
    std::shared_ptr<RequestGroup> group = result[0];
    auto xuris = group->getDownloadContext()->getFirstFileEntry()->getUris();
    CPPUNIT_ASSERT_EQUAL((size_t)5, xuris.size());
    for (size_t i = 0; i < 5; ++i) {
      CPPUNIT_ASSERT_EQUAL(uris[i % 3], xuris[i]);
    }
  }
  option_->put(PREF_SPLIT, "2");
  {
    std::vector<std::shared_ptr<RequestGroup>> result;
    createRequestGroupForUri(result, option_, uris);
    CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
    std::shared_ptr<RequestGroup> group = result[0];
    auto xuris = group->getDownloadContext()->getFirstFileEntry()->getUris();
    CPPUNIT_ASSERT_EQUAL((size_t)3, xuris.size());
    for (size_t i = 0; i < 3; ++i) {
      CPPUNIT_ASSERT_EQUAL(uris[i % 3], xuris[i]);
    }
  }
  option_->put(PREF_FORCE_SEQUENTIAL, A2_V_TRUE);
  {
    std::vector<std::shared_ptr<RequestGroup>> result;
    createRequestGroupForUri(result, option_, uris);
    CPPUNIT_ASSERT_EQUAL((size_t)3, result.size());
    // for alpha server
    std::shared_ptr<RequestGroup> alphaGroup = result[0];
    auto alphaURIs =
        alphaGroup->getDownloadContext()->getFirstFileEntry()->getUris();
    CPPUNIT_ASSERT_EQUAL((size_t)2, alphaURIs.size());
    for (size_t i = 0; i < 2; ++i) {
      CPPUNIT_ASSERT_EQUAL(uris[0], alphaURIs[i]);
    }
    CPPUNIT_ASSERT_EQUAL(2, alphaGroup->getNumConcurrentCommand());
    std::shared_ptr<DownloadContext> alphaCtx =
        alphaGroup->getDownloadContext();
    // See filename is not assigned yet
    CPPUNIT_ASSERT_EQUAL(std::string(""), alphaCtx->getBasePath());
  }
}

void DownloadHelperTest::testCreateRequestGroupForUri_parameterized()
{
  std::vector<std::string> uris{"http://{alpha, bravo}/file",
                                "http://charlie/file"};
  option_->put(PREF_MAX_CONNECTION_PER_SERVER, "1");
  option_->put(PREF_SPLIT, "3");
  option_->put(PREF_DIR, "/tmp");
  option_->put(PREF_OUT, "file.out");
  option_->put(PREF_PARAMETERIZED_URI, A2_V_TRUE);
  {
    std::vector<std::shared_ptr<RequestGroup>> result;

    createRequestGroupForUri(result, option_, uris);

    CPPUNIT_ASSERT_EQUAL((size_t)1, result.size());
    std::shared_ptr<RequestGroup> group = result[0];
    auto uris = group->getDownloadContext()->getFirstFileEntry()->getUris();
    CPPUNIT_ASSERT_EQUAL((size_t)3, uris.size());

    CPPUNIT_ASSERT_EQUAL(std::string("http://alpha/file"), uris[0]);
    CPPUNIT_ASSERT_EQUAL(std::string("http://bravo/file"), uris[1]);
    CPPUNIT_ASSERT_EQUAL(std::string("http://charlie/file"), uris[2]);

    CPPUNIT_ASSERT_EQUAL(3, group->getNumConcurrentCommand());
    std::shared_ptr<DownloadContext> ctx = group->getDownloadContext();
    CPPUNIT_ASSERT_EQUAL(std::string("/tmp/file.out"), ctx->getBasePath());
  }
}

#ifdef ENABLE_METALINK
void DownloadHelperTest::testCreateRequestGroupForUri_Metalink()
{
  std::vector<std::string> uris{"http://alpha/file", "http://bravo/file",
                                "http://charlie/file", A2_TEST_DIR "/test.xml"};
  option_->put(PREF_MAX_CONNECTION_PER_SERVER, "1");
  option_->put(PREF_SPLIT, "2");
  option_->put(PREF_DIR, "/tmp");
  option_->put(PREF_OUT, "file.out");
  {
    std::vector<std::shared_ptr<RequestGroup>> result;

    createRequestGroupForUri(result, option_, uris);

// group1: http://alpha/file, ...
// group2-6: 5 file entries in Metalink
    CPPUNIT_ASSERT_EQUAL((size_t)6, result.size());

    std::shared_ptr<RequestGroup> group = result[0];
    auto xuris = group->getDownloadContext()->getFirstFileEntry()->getUris();
    CPPUNIT_ASSERT_EQUAL((size_t)3, xuris.size());
    for (size_t i = 0; i < 3; ++i) {
      CPPUNIT_ASSERT_EQUAL(uris[i], xuris[i]);
    }
    CPPUNIT_ASSERT_EQUAL(2, group->getNumConcurrentCommand());
    std::shared_ptr<DownloadContext> ctx = group->getDownloadContext();
    CPPUNIT_ASSERT_EQUAL(std::string("/tmp/file.out"), ctx->getBasePath());

    std::shared_ptr<RequestGroup> aria2052Group = result[1];
    CPPUNIT_ASSERT_EQUAL(1, // because of maxconnections attribute
                         aria2052Group->getNumConcurrentCommand());
    std::shared_ptr<DownloadContext> aria2052Ctx =
        aria2052Group->getDownloadContext();
    CPPUNIT_ASSERT_EQUAL(std::string("/tmp/aria2-0.5.2.tar.bz2"),
                         aria2052Ctx->getBasePath());

    std::shared_ptr<RequestGroup> aria2051Group = result[2];
    CPPUNIT_ASSERT_EQUAL(2, aria2051Group->getNumConcurrentCommand());
  }
}
#endif // ENABLE_METALINK

void DownloadHelperTest::testCreateRequestGroupForUriList()
{
  option_->put(PREF_MAX_CONNECTION_PER_SERVER, "3");
  option_->put(PREF_SPLIT, "3");
  option_->put(PREF_INPUT_FILE, A2_TEST_DIR "/input_uris.txt");
  option_->put(PREF_DIR, "/tmp");
  option_->put(PREF_OUT, "file.out");

  std::vector<std::shared_ptr<RequestGroup>> result;

  createRequestGroupForUriList(result, option_);

  CPPUNIT_ASSERT_EQUAL((size_t)2, result.size());

  std::shared_ptr<RequestGroup> fileGroup = result[0];
  auto fileURIs =
      fileGroup->getDownloadContext()->getFirstFileEntry()->getUris();
  CPPUNIT_ASSERT_EQUAL(std::string("http://alpha/file"), fileURIs[0]);
  CPPUNIT_ASSERT_EQUAL(std::string("http://bravo/file"), fileURIs[1]);
  CPPUNIT_ASSERT_EQUAL(std::string("http://charlie/file"), fileURIs[2]);
  CPPUNIT_ASSERT_EQUAL(3, fileGroup->getNumConcurrentCommand());
  std::shared_ptr<DownloadContext> fileCtx = fileGroup->getDownloadContext();
  CPPUNIT_ASSERT_EQUAL(std::string("/mydownloads/myfile.out"),
                       fileCtx->getBasePath());

  std::shared_ptr<RequestGroup> fileISOGroup = result[1];
  std::shared_ptr<DownloadContext> fileISOCtx =
      fileISOGroup->getDownloadContext();
  // PREF_OUT in option_ must be ignored.
  CPPUNIT_ASSERT_EQUAL(std::string(), fileISOCtx->getBasePath());
}

#ifdef ENABLE_METALINK
void DownloadHelperTest::testCreateRequestGroupForMetalink()
{
  option_->put(PREF_SPLIT, "5");
  option_->put(PREF_METALINK_FILE, A2_TEST_DIR "/test.xml");
  option_->put(PREF_DIR, "/tmp");
  option_->put(PREF_OUT, "file.out");
  {
    std::vector<std::shared_ptr<RequestGroup>> result;

    createRequestGroupForMetalink(result, option_);

    CPPUNIT_ASSERT_EQUAL((size_t)5, result.size());
    std::shared_ptr<RequestGroup> group = result[0];
    auto uris = group->getDownloadContext()->getFirstFileEntry()->getUris();
    std::sort(uris.begin(), uris.end());
    CPPUNIT_ASSERT_EQUAL((size_t)2, uris.size());
    CPPUNIT_ASSERT_EQUAL(std::string("ftp://ftphost/aria2-0.5.2.tar.bz2"),
                         uris[0]);
    CPPUNIT_ASSERT_EQUAL(std::string("http://httphost/aria2-0.5.2.tar.bz2"),
                         uris[1]);
    // See numConcurrentCommand is 1 because of maxconnections attribute.
    CPPUNIT_ASSERT_EQUAL(1, group->getNumConcurrentCommand());
  }
}
#endif // ENABLE_METALINK

} // namespace aria2
