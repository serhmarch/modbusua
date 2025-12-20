#include <gtest/gtest.h>
#include <core/CnItemLexer.h>

// ============================================================================
// CnItemToken Tests
// ============================================================================

TEST(CnItemTokenTest, CreateEOL)
{
    CnItemToken token = CnItemToken::createEOL();
    
    EXPECT_TRUE(token.isEOL());
    EXPECT_FALSE(token.isInt());
    EXPECT_FALSE(token.isModbus());
    EXPECT_FALSE(token.isSymbol());
    EXPECT_FALSE(token.isWord());
    EXPECT_EQ(token.type(), CnItemToken::TOK_EOL);
}

TEST(CnItemTokenTest, CreateInt)
{
    CnItemToken token = CnItemToken::createInt(12345, 5);
    
    EXPECT_FALSE(token.isEOL());
    EXPECT_TRUE(token.isInt());
    EXPECT_FALSE(token.isModbus());
    EXPECT_FALSE(token.isSymbol());
    EXPECT_FALSE(token.isWord());
    EXPECT_EQ(token.type(), CnItemToken::TOK_INT);
    EXPECT_EQ(token.Int(), 12345);
    EXPECT_EQ(token.pos(), 5);
}

TEST(CnItemTokenTest, CreateModbus)
{
    CnItemToken token = CnItemToken::createModbus(400001, 10);
    
    EXPECT_FALSE(token.isEOL());
    EXPECT_TRUE(token.isInt()); // Modbus is also considered Int
    EXPECT_TRUE(token.isModbus());
    EXPECT_FALSE(token.isSymbol());
    EXPECT_FALSE(token.isWord());
    EXPECT_EQ(token.type(), CnItemToken::TOK_MODBUS);
    EXPECT_EQ(token.Modbus(), 400001);
    EXPECT_EQ(token.Int(), 400001); // Modbus can be retrieved as Int
    EXPECT_EQ(token.pos(), 10);
}

TEST(CnItemTokenTest, CreateSymbol)
{
    CnItemToken token = CnItemToken::createSym(CnCHR(','), 3);
    
    EXPECT_FALSE(token.isEOL());
    EXPECT_FALSE(token.isInt());
    EXPECT_FALSE(token.isModbus());
    EXPECT_TRUE(token.isSymbol());
    EXPECT_TRUE(token.isSymbolComma());
    EXPECT_FALSE(token.isSymbolDot());
    EXPECT_FALSE(token.isWord());
    EXPECT_EQ(token.type(), CnItemToken::TOK_SYMBOL);
    EXPECT_EQ(token.Symbol(), CnCHR(','));
    EXPECT_EQ(token.pos(), 3);
}

TEST(CnItemTokenTest, CreateSymbolDot)
{
    CnItemToken token = CnItemToken::createSym(CnCHR('.'), 7);
    
    EXPECT_TRUE(token.isSymbol());
    EXPECT_FALSE(token.isSymbolComma());
    EXPECT_TRUE(token.isSymbolDot());
    EXPECT_EQ(token.Symbol(), CnCHR('.'));
}

TEST(CnItemTokenTest, CreateWord)
{
    CnItemToken token = CnItemToken::createWord(CnSTR("DEVICE"), 0);
    
    EXPECT_FALSE(token.isEOL());
    EXPECT_FALSE(token.isInt());
    EXPECT_FALSE(token.isModbus());
    EXPECT_FALSE(token.isSymbol());
    EXPECT_TRUE(token.isWord());
    EXPECT_EQ(token.type(), CnItemToken::TOK_WORD);
    EXPECT_EQ(token.Word(), CnSTR("DEVICE"));
    EXPECT_EQ(token.pos(), 0);
}

TEST(CnItemTokenTest, DefaultConstructor)
{
    CnItemToken token;
    
    EXPECT_TRUE(token.isEOL());
    EXPECT_EQ(token.pos(), -1);
}

TEST(CnItemTokenTest, CopyConstructor)
{
    CnItemToken original = CnItemToken::createInt(999, 15);
    CnItemToken copy(original);
    
    EXPECT_EQ(copy.type(), original.type());
    EXPECT_EQ(copy.Int(), original.Int());
    EXPECT_EQ(copy.pos(), original.pos());
}

TEST(CnItemTokenTest, EqualityOperator)
{
    CnItemToken token1 = CnItemToken::createInt(100, 5);
    CnItemToken token2 = CnItemToken::createInt(100, 5);
    CnItemToken token3 = CnItemToken::createInt(200, 5);
    CnItemToken token4 = CnItemToken::createInt(100, 10);
    
    EXPECT_TRUE(token1 == token2);
    EXPECT_FALSE(token1 == token3); // Different value
    EXPECT_FALSE(token1 == token4); // Different position
}

TEST(CnItemTokenTest, NonIntReturnsZero)
{
    CnItemToken wordToken = CnItemToken::createWord(CnSTR("TEST"), 0);
    EXPECT_EQ(wordToken.Int(), 0);
    EXPECT_EQ(wordToken.Modbus(), 0);
}

TEST(CnItemTokenTest, NonSymbolReturnsNull)
{
    CnItemToken intToken = CnItemToken::createInt(123, 0);
    EXPECT_EQ(intToken.Symbol(), CnCHR('\0'));
}

TEST(CnItemTokenTest, NonWordReturnsEmpty)
{
    CnItemToken intToken = CnItemToken::createInt(456, 0);
    EXPECT_TRUE(intToken.Word().isEmpty());
}

// ============================================================================
// CnItemTokenList Tests
// ============================================================================

TEST(CnItemTokenListTest, DefaultConstructor)
{
    CnItemTokenList list;
    
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.count(), 0);
}

TEST(CnItemTokenListTest, AppendTokens)
{
    CnItemTokenList list;
    
    list.append(CnItemToken::createInt(10, 0));
    EXPECT_FALSE(list.isEmpty());
    EXPECT_EQ(list.count(), 1);
    
    list.append(CnItemToken::createWord(CnSTR("TEST"), 3));
    EXPECT_EQ(list.count(), 2);
}

TEST(CnItemTokenListTest, CurrentToken)
{
    CnItemTokenList list;
    list.append(CnItemToken::createInt(100, 0));
    list.append(CnItemToken::createInt(200, 5));
    
    const CnItemToken& current = list.current();
    EXPECT_EQ(current.Int(), 100);
}

TEST(CnItemTokenListTest, NextToken)
{
    CnItemTokenList list;
    list.append(CnItemToken::createInt(111, 0));
    list.append(CnItemToken::createInt(222, 5));
    list.append(CnItemToken::createInt(333, 10));
    
    EXPECT_EQ(list.current().Int(), 111);
    
    const CnItemToken& second = list.next();
    EXPECT_EQ(second.Int(), 222);
    
    const CnItemToken& third = list.next();
    EXPECT_EQ(third.Int(), 333);
}

TEST(CnItemTokenListTest, GetNext)
{
    CnItemTokenList list;
    list.append(CnItemToken::createInt(10, 0));
    list.append(CnItemToken::createInt(20, 5));
    
    EXPECT_EQ(list.current().Int(), 10);
    
    const CnItemToken& next = list.getNext();
    EXPECT_EQ(next.Int(), 20);
    
    // getNext doesn't advance position
    EXPECT_EQ(list.current().Int(), 10);
}

TEST(CnItemTokenListTest, Position)
{
    CnItemTokenList list;
    CnItemToken token1 = CnItemToken::createInt(100, 0);
    CnItemToken token2 = CnItemToken::createInt(200, 5);
    CnItemToken token3 = CnItemToken::createInt(300, 10);
    
    list.append(token1);
    list.append(token2);
    list.append(token3);
    
    EXPECT_EQ(list.position(token1), 0);
    EXPECT_EQ(list.position(token2), 1);
    EXPECT_EQ(list.position(token3), 2);
}

TEST(CnItemTokenListTest, PositionNotFound)
{
    CnItemTokenList list;
    list.append(CnItemToken::createInt(100, 0));
    
    CnItemToken notInList = CnItemToken::createInt(999, 99);
    EXPECT_EQ(list.position(notInList), -1);
}

TEST(CnItemTokenListTest, Clear)
{
    CnItemTokenList list;
    list.append(CnItemToken::createInt(10, 0));
    list.append(CnItemToken::createInt(20, 5));
    
    EXPECT_EQ(list.count(), 2);
    
    list.clear();
    
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.count(), 0);
}

TEST(CnItemTokenListTest, CopyConstructor)
{
    CnItemTokenList original;
    original.append(CnItemToken::createInt(100, 0));
    original.append(CnItemToken::createWord(CnSTR("TEST"), 5));
    
    CnItemTokenList copy(original);
    
    EXPECT_EQ(copy.count(), original.count());
    EXPECT_EQ(copy.current().Int(), 100);
}

// ============================================================================
// CnItemLexer Tests
// ============================================================================

TEST(CnItemLexerTest, DefaultConstructor)
{
    CnItemLexer lexer;
    
    EXPECT_TRUE(lexer.isEmpty());
    EXPECT_EQ(lexer.countTokens(), 0);
    EXPECT_FALSE(lexer.hasError());
}

TEST(CnItemLexerTest, ParseEmptyString)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR(""));
    
    EXPECT_TRUE(result);
    EXPECT_FALSE(lexer.hasError());
    EXPECT_EQ(lexer.countTokens(), 1); // Only EOL token
    EXPECT_TRUE(lexer.currentToken().isEOL());
}

TEST(CnItemLexerTest, ParseSingleInteger)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("12345"));
    
    EXPECT_TRUE(result);
    EXPECT_FALSE(lexer.hasError());
    EXPECT_EQ(lexer.countTokens(), 2); // Number + EOL
    EXPECT_TRUE(lexer.currentToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 12345);
}

TEST(CnItemLexerTest, ParseSingleWord)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("DEVICE"));
    
    EXPECT_TRUE(result);
    EXPECT_FALSE(lexer.hasError());
    EXPECT_EQ(lexer.countTokens(), 2); // Word + EOL
    EXPECT_TRUE(lexer.currentToken().isWord());
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("DEVICE"));
}

TEST(CnItemLexerTest, ParseWordToUpperCase)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("device"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isWord());
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("DEVICE"));
}

TEST(CnItemLexerTest, ParseWordWithUnderscore)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("_test_word"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isWord());
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("_TEST_WORD"));
}

TEST(CnItemLexerTest, ParseWordWithNumbers)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("WORD123"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isWord());
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("WORD123"));
}

TEST(CnItemLexerTest, ParseModbusAddress0xxxxx)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("000001"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 000001);
}

TEST(CnItemLexerTest, ParseModbusAddress1xxxxx)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("100001"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 100001);
}

TEST(CnItemLexerTest, ParseModbusAddress3xxxxx)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("300001"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 300001);
}

TEST(CnItemLexerTest, ParseModbusAddress4xxxxx)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("400001"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 400001);
}

TEST(CnItemLexerTest, ParseModbusAddressMaximum)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("465536"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 465536);
}

TEST(CnItemLexerTest, ParseInvalidModbusAddress2xxxxx)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("200001"));
    
    EXPECT_TRUE(result);
    EXPECT_FALSE(lexer.currentToken().isModbus());
    EXPECT_TRUE(lexer.currentToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 200001);
}

TEST(CnItemLexerTest, ParseInvalidModbusAddress5xxxxx)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("500001"));
    
    EXPECT_TRUE(result);
    EXPECT_FALSE(lexer.currentToken().isModbus());
    EXPECT_TRUE(lexer.currentToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 500001);
}

TEST(CnItemLexerTest, ParseInvalidModbusAddressZero)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("000000"));
    
    EXPECT_TRUE(result);
    EXPECT_FALSE(lexer.currentToken().isModbus());
    EXPECT_TRUE(lexer.currentToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 0);
}

TEST(CnItemLexerTest, ParseSymbolComma)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR(","));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isSymbol());
    EXPECT_TRUE(lexer.currentToken().isSymbolComma());
    EXPECT_EQ(lexer.currentToken().Symbol(), CnCHR(','));
}

TEST(CnItemLexerTest, ParseSymbolDot)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("."));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isSymbol());
    EXPECT_TRUE(lexer.currentToken().isSymbolDot());
    EXPECT_EQ(lexer.currentToken().Symbol(), CnCHR('.'));
}

TEST(CnItemLexerTest, ParseMultipleSymbols)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("()[]{}"));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(lexer.countTokens(), 7); // 6 symbols + EOL
    
    EXPECT_EQ(lexer.currentToken().Symbol(), CnCHR('('));
    EXPECT_EQ(lexer.nextToken().Symbol(), CnCHR(')'));
    EXPECT_EQ(lexer.nextToken().Symbol(), CnCHR('['));
    EXPECT_EQ(lexer.nextToken().Symbol(), CnCHR(']'));
    EXPECT_EQ(lexer.nextToken().Symbol(), CnCHR('{'));
    EXPECT_EQ(lexer.nextToken().Symbol(), CnCHR('}'));
}

TEST(CnItemLexerTest, ParseMixedTokens)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("DEVICE 123 , PORT"));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(lexer.countTokens(), 5); // DEVICE, 123, comma, PORT, EOL
    
    EXPECT_TRUE(lexer.currentToken().isWord());
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("DEVICE"));
    
    EXPECT_TRUE(lexer.nextToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 123);
    
    EXPECT_TRUE(lexer.nextToken().isSymbolComma());
    
    EXPECT_TRUE(lexer.nextToken().isWord());
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("PORT"));
}

TEST(CnItemLexerTest, ParseModbusAndInteger)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("400001 12345"));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(lexer.countTokens(), 3); // Modbus, Int, EOL
    
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 400001);
    
    EXPECT_TRUE(lexer.nextToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 12345);
}

TEST(CnItemLexerTest, ParseWithMultipleSpaces)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("   WORD1    WORD2   "));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(lexer.countTokens(), 3); // WORD1, WORD2, EOL
    
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("WORD1"));
    EXPECT_EQ(lexer.nextToken().Word(), CnSTR("WORD2"));
}

TEST(CnItemLexerTest, ParseWithTabs)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("\tWORD1\t123\t"));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(lexer.countTokens(), 3); // WORD1, 123, EOL
}

TEST(CnItemLexerTest, ParseComplexExpression)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("DEVICE:PORT1,400001.VALUE"));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(lexer.countTokens(), 8); // DEVICE, :, PORT1, comma, 400001, dot, VALUE, EOL
    
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("DEVICE"));
    EXPECT_EQ(lexer.nextToken().Symbol(), CnCHR(':'));
    EXPECT_EQ(lexer.nextToken().Word(), CnSTR("PORT1"));
    EXPECT_TRUE(lexer.nextToken().isSymbolComma());
    EXPECT_EQ(lexer.nextToken().Modbus(), 400001);
    EXPECT_TRUE(lexer.nextToken().isSymbolDot());
    EXPECT_EQ(lexer.nextToken().Word(), CnSTR("VALUE"));
}

TEST(CnItemLexerTest, TokenNavigation)
{
    CnItemLexer lexer;
    lexer.parse(CnSTR("ONE 123 TWO"));
    
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("ONE"));
    
    const CnItemToken& next = lexer.getNextToken();
    EXPECT_EQ(next.Int(), 123);
    
    // currentToken should not change after getNextToken
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("ONE"));
    
    // nextToken advances position
    lexer.nextToken();
    EXPECT_EQ(lexer.currentToken().Int(), 123);
    
    lexer.nextToken();
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("TWO"));
}

TEST(CnItemLexerTest, SourceString)
{
    CnItemLexer lexer;
    CnString source = CnSTR("TEST 123");
    lexer.parse(source);
    
    EXPECT_EQ(lexer.source(), source);
}

TEST(CnItemLexerTest, TokenPositions)
{
    CnItemLexer lexer;
    lexer.parse(CnSTR("WORD 123 ,"));
    
    EXPECT_EQ(lexer.currentToken().pos(), 0); // WORD starts at 0
    EXPECT_EQ(lexer.nextToken().pos(), 5);     // 123 starts at 5
    EXPECT_EQ(lexer.nextToken().pos(), 9);     // comma at 9
}

TEST(CnItemLexerTest, LargeNumber)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("999999999"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 999999999U);
}

TEST(CnItemLexerTest, ZeroValue)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("0"));
    
    EXPECT_TRUE(result);
    EXPECT_TRUE(lexer.currentToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 0);
}

TEST(CnItemLexerTest, MultipleConsecutiveNumbers)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("123 456 789"));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(lexer.countTokens(), 4); // 3 numbers + EOL
    
    EXPECT_EQ(lexer.currentToken().Int(), 123);
    EXPECT_EQ(lexer.nextToken().Int(), 456);
    EXPECT_EQ(lexer.nextToken().Int(), 789);
}

TEST(CnItemLexerTest, NoSpaceBetweenTokens)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("WORD123,456"));
    
    EXPECT_TRUE(result);
    EXPECT_EQ(lexer.countTokens(), 4); // WORD123, comma, 456, EOL
    
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("WORD123"));
    EXPECT_TRUE(lexer.nextToken().isSymbolComma());
    EXPECT_EQ(lexer.nextToken().Int(), 456);
}

TEST(CnItemLexerTest, RepeatParse)
{
    CnItemLexer lexer;
    
    lexer.parse(CnSTR("FIRST"));
    EXPECT_EQ(lexer.countTokens(), 2);
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("FIRST"));
    
    lexer.parse(CnSTR("SECOND"));
    EXPECT_EQ(lexer.countTokens(), 2);
    EXPECT_EQ(lexer.currentToken().Word(), CnSTR("SECOND"));
}

TEST(CnItemLexerTest, TokensAccessor)
{
    CnItemLexer lexer;
    lexer.parse(CnSTR("TEST 123"));
    
    const CnItemTokenList& tokens = lexer.tokens();
    EXPECT_EQ(tokens.count(), 3); // TEST, 123, EOL
}

TEST(CnItemLexerTest, IsEmptyCheck)
{
    CnItemLexer lexer;
    
    EXPECT_TRUE(lexer.isEmpty());
    
    lexer.parse(CnSTR("TEST"));
    EXPECT_FALSE(lexer.isEmpty());
}

TEST(CnItemLexerTest, AllModbusRanges)
{
    CnItemLexer lexer;
    
    // Test 0xxxxx range
    lexer.parse(CnSTR("065536"));
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 65536);
    
    // Test 1xxxxx range
    lexer.parse(CnSTR("165536"));
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 165536);
    
    // Test 3xxxxx range
    lexer.parse(CnSTR("365536"));
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 365536);
    
    // Test 4xxxxx range
    lexer.parse(CnSTR("465536"));
    EXPECT_TRUE(lexer.currentToken().isModbus());
    EXPECT_EQ(lexer.currentToken().Modbus(), 465536);
}

TEST(CnItemLexerTest, SevenDigitNumber)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("1234567"));
    
    EXPECT_TRUE(result);
    EXPECT_FALSE(lexer.currentToken().isModbus());
    EXPECT_TRUE(lexer.currentToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 1234567);
}

TEST(CnItemLexerTest, FiveDigitNumber)
{
    CnItemLexer lexer;
    bool result = lexer.parse(CnSTR("12345"));
    
    EXPECT_TRUE(result);
    EXPECT_FALSE(lexer.currentToken().isModbus());
    EXPECT_TRUE(lexer.currentToken().isInt());
    EXPECT_EQ(lexer.currentToken().Int(), 12345);
}
