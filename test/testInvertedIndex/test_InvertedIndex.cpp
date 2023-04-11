#include <QtTest/QTest>

#include "InvertedIndex.hpp"



class TestInvertedIndex : public QObject
{
    Q_OBJECT
public:
    TestInvertedIndex();
    ~TestInvertedIndex();
private:
    void TestInvertedIndexFunctionality();
private slots:
    void TestBasic1();
    void TestBasic2();
    void TestInvertedIndexMissingWord(
            std::vector<std::string>&        docs,
            std::vector<std::string>&        requests,
            std::vector<std::vector<Entry>>& expected
        );
};

TestInvertedIndex::TestInvertedIndex()
{
    ///
}

TestInvertedIndex::~TestInvertedIndex()
{
    ///
}



void TestInvertedIndex::TestInvertedIndexFunctionality(
		const std::vector<std::string>&        docs,
		const std::vector<std::string>&        requests,
		const std::vector<std::vector<Entry>>& expected
    )
{
	std::vector<std::vector<Entry>> result;
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);

	for (auto& request : requests)
	{
		std::vector<Entry> wordCount = idx.GetWordCount(request);
		result.push_back(wordCount);
	}
//	ASSERT_EQ(result, expected);
    QVERIFY2(result == expected, "TestBasic1 is ok");
}

void TestInvertedIndex::TestBasic1()
{
	const std::vector<std::string> docs = {
			"london is the capital of great britain",
			"big ben is the nickname for the Great bell of the striking clock"
	};
	const std::vector<std::string> requests = {
			"london",
			"the"
	};
	const std::vector<std::vector<Entry>> expected = {
			{ {0, 1}         }, // "london" - 0"london is..", 1""
			{ {0, 1}, {1, 3} }  // "the" - 0"..is the..", 1"..is the..for the..of the.."
	};
	TestInvertedIndexFunctionality(docs, requests, expected);
}

void TestInvertedIndex::TestBasic2()
{
	const std::vector<std::string> docs = {
			"milk milk milk milk water water water",
			"milk water water",
			"milk milk milk milk milk water water water water water",
			"americano cappuccino"
	};
	const std::vector<std::string> requests = {
			"milk",
			"water",
			"cappuccino"
	};
	const std::vector<std::vector<Entry>> expected = {
			{ {0, 4}, {1, 1}, {2, 5}         },
			{ {0, 3}, {1, 2}, {2, 5}         },
			{                         {3, 1} }
	};
	TestInvertedIndexFunctionality(docs, requests, expected);
}

void TestInvertedIndex::TestInvertedIndexMissingWord()
{
	const std::vector<std::string> docs = {
			"a b c d e f g h i j k l",
			"statement"
	};
	const std::vector<std::string> requests = {
			"m",
			"statement"
	};
	const std::vector<std::vector<Entry>> expected = {
			{                },
			{         {1, 1} }
	};
	TestInvertedIndexFunctionality(docs, requests, expected);
}

QTEST_APPLESS_MAIN(TestInvertedIndex)

#include "test_InvertedIndex.moc"
