/*
  Copyright 2013 Statoil ASA.

  This file is part of the Open Porous Media project (OPM).

  OPM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  OPM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with OPM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdexcept>
#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#define BOOST_TEST_MODULE ParserTests
#include <boost/test/unit_test.hpp>

#include "opm/parser/eclipse/Parser/Parser.hpp"
#include "opm/parser/eclipse/RawDeck/RawDeck.hpp"
using namespace Opm;

BOOST_AUTO_TEST_CASE(RawDeckPrintToOStream) {
    boost::filesystem::path singleKeywordFile("testdata/small.data");

    ParserPtr parser(new Parser());

    RawDeckPtr rawDeck = parser->parse(singleKeywordFile.string());
    std::cout << *rawDeck << "\n";
}

BOOST_AUTO_TEST_CASE(Initializing) {
    BOOST_REQUIRE_NO_THROW(Parser parser);
}

BOOST_AUTO_TEST_CASE(ParseWithInvalidInputFileThrows) {
    ParserPtr parser(new Parser());
    BOOST_REQUIRE_THROW(parser->parse("nonexistingfile.asdf"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(ParseWithValidFileSetOnParseCallNoThrow) {

    boost::filesystem::path singleKeywordFile("testdata/small.data");
    ParserPtr parser(new Parser());

    BOOST_REQUIRE_NO_THROW(parser->parse(singleKeywordFile.string()));
}

BOOST_AUTO_TEST_CASE(ParseWithInValidFileSetOnParseCallThrows) {
    boost::filesystem::path singleKeywordFile("testdata/nosuchfile.data");
    ParserPtr parser(new Parser());
    BOOST_REQUIRE_THROW(parser->parse(singleKeywordFile.string()), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(ParseFileWithOneKeyword) {
    boost::filesystem::path singleKeywordFile("testdata/mini.data");

    ParserPtr parser(new Parser());

    RawDeckPtr rawDeck = parser->parse(singleKeywordFile.string());
    BOOST_REQUIRE_EQUAL((unsigned) 1, rawDeck->getNumberOfKeywords());
    RawKeywordPtr rawKeyword = rawDeck->getKeyword("ENDSCALE");
    const std::list<RawRecordPtr>& records = rawKeyword->getRecords();
    BOOST_REQUIRE_EQUAL((unsigned) 1, records.size());
    RawRecordPtr record = records.back();

    const std::string& recordString = record->getRecordString();
    BOOST_REQUIRE_EQUAL("'NODIR'  'REVERS'  1  20", recordString);

    const std::vector<std::string>& recordElements = record->getRecords();
    BOOST_REQUIRE_EQUAL((unsigned) 4, recordElements.size());

    BOOST_REQUIRE_EQUAL("\'NODIR\'", recordElements[0]);
    BOOST_REQUIRE_EQUAL("\'REVERS\'", recordElements[1]);
    BOOST_REQUIRE_EQUAL("1", recordElements[2]);
    BOOST_REQUIRE_EQUAL("20", recordElements[3]);
}

BOOST_AUTO_TEST_CASE(ParseFileWithFewKeywords) {
    boost::filesystem::path singleKeywordFile("testdata/small.data");

    ParserPtr parser(new Parser());

    RawDeckPtr rawDeck = parser->parse(singleKeywordFile.string());
    BOOST_REQUIRE_EQUAL((unsigned) 5, rawDeck->getNumberOfKeywords());

    RawKeywordConstPtr matchingKeyword = rawDeck->getKeyword("OIL");
    std::list<RawRecordPtr> records = matchingKeyword->getRecords();
    BOOST_REQUIRE_EQUAL("OIL", matchingKeyword->getKeyword());
    BOOST_REQUIRE_EQUAL((unsigned) 0, records.size());

    matchingKeyword = rawDeck->getKeyword("INCLUDE");
    BOOST_REQUIRE_EQUAL("INCLUDE", matchingKeyword->getKeyword());
    records = matchingKeyword->getRecords();
    BOOST_REQUIRE_EQUAL((unsigned) 1, records.size());
    RawRecordPtr theRecord = records.front();
    const std::string& recordString = theRecord->getRecordString();
    BOOST_REQUIRE_EQUAL("\'sti til fil/den er her\'", recordString);

    matchingKeyword = rawDeck->getKeyword("GRIDUNIT");
    BOOST_REQUIRE_EQUAL("GRIDUNIT", matchingKeyword->getKeyword());
    records = matchingKeyword->getRecords();
    BOOST_REQUIRE_EQUAL((unsigned) 1, records.size());

    matchingKeyword = rawDeck->getKeyword("RADFIN4");
    records = matchingKeyword->getRecords();
    BOOST_REQUIRE_EQUAL("RADFIN4", matchingKeyword->getKeyword());
    BOOST_REQUIRE_EQUAL((unsigned) 1, records.size());

    matchingKeyword = rawDeck->getKeyword("ABCDAD");
    BOOST_REQUIRE_EQUAL("ABCDAD", matchingKeyword->getKeyword());
    records = matchingKeyword->getRecords();

    BOOST_REQUIRE_EQUAL((unsigned) 2, records.size());
}

//NOTE: needs statoil dataset

BOOST_AUTO_TEST_CASE(ParseFileWithManyKeywords) {
    boost::filesystem::path multipleKeywordFile("testdata/statoil/gurbat_trimmed.DATA");

    ParserPtr parser(new Parser());

    RawDeckPtr rawDeck = parser->parse(multipleKeywordFile.string());
    BOOST_REQUIRE_EQUAL((unsigned) 18, rawDeck->getNumberOfKeywords());
}

//NOTE: needs statoil dataset

BOOST_AUTO_TEST_CASE(ParseFullTestFile) {
    boost::filesystem::path multipleKeywordFile("testdata/statoil/ECLIPSE.DATA");

    ParserPtr parser(new Parser());

    RawDeckPtr rawDeck = parser->parse(multipleKeywordFile.string());
    // Note, cannot check the number of keywords, since the number of
    // records are not defined (yet) for all these keywords.
    // But we can check a copule of keywords, and that they have the correct
    // number of records
    
    RawKeywordConstPtr matchingKeyword = rawDeck->getKeyword("OIL");
    std::list<RawRecordPtr> records = matchingKeyword->getRecords();
    BOOST_REQUIRE_EQUAL("OIL", matchingKeyword->getKeyword());
    
    BOOST_REQUIRE_EQUAL((unsigned) 0, records.size());

    matchingKeyword = rawDeck->getKeyword("VFPPDIMS");
    records = matchingKeyword->getRecords();
    BOOST_REQUIRE_EQUAL("VFPPDIMS", matchingKeyword->getKeyword());
    BOOST_REQUIRE_EQUAL((unsigned) 1, records.size());

    const std::string& recordString = records.front()->getRecordString();
    BOOST_REQUIRE_EQUAL("20  20  15  15  15   50", recordString);
    std::vector<std::string> recordItems = records.front()->getRecords();
    BOOST_REQUIRE_EQUAL((unsigned) 6, recordItems.size());


}


