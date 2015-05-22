#include <vector>

#include "azer/util/xml/xml_parser.h"
#include "azer/util/xml/xpath.h"
#include "testing/gtest/include/gtest/gtest.h"2

using util::xml::Node;
using util::xml::TextNode;
using util::xml::XPath;
using util::xml::XPathContext;
using util::xml::XMLParser;
using util::xml::XMLContext;
using util::xml::TreeDumper;

TEST(XMLParser, Parser) {
  const char* xml = "<!--   Copyright w3school.com.cn  -->"
      "<note>"
      "<to>George</to>"
      "<from>John</from>"
      "<heading>Reminder</heading>"
      "<body>Don't forget the meeting!</body>"
      "</note>";
  XMLContext context;
  XMLParser parser;
  ASSERT_TRUE(parser.Parse(xml, &context));

  TreeDumper dumper;
  dumper.Dump(context.root());
}

TEST(XPathTest, Element) {
  const char* xml = ""
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<bookstore>"
      "<book>"
      "<title lang=\"eng\">Harry Potter</title>"
      "<price>29.99</price>"
      "</book>"
      "<book>"
      "<title lang=\"eng\">Learning XML</title>"
      "<price><![CDATA[39.95]]></price>"
      "</book>"
      "</bookstore>";
  XMLContext context;
  XMLParser parser;
  ASSERT_TRUE(parser.Parse(xml, &context));

  XPathContext xpathcontext;
  XPath path("//book");
  context.execute(path, &xpathcontext);
  const std::vector<Node*>& res = xpathcontext.GetResult();
  ASSERT_EQ(res.size(), 2u);
  ASSERT_EQ(res[0]->type(), Node::kElementNode);
  ASSERT_EQ(res[1]->type(), Node::kElementNode);
  ASSERT_EQ(res[0]->ToElement()->tag_name(), "book");
  ASSERT_EQ(res[1]->ToElement()->tag_name(), "book");

  TreeDumper dumper;
  dumper.Dump(context.root());
}

TEST(XPathTest, Text) {
  const char* xml = ""
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<bookstore>"
      "<book>"
      "<title lang=\"eng\">Harry Potter</title>"
      "<price>29.99</price>"
      "</book>"
      "<book>"
      "<title lang=\"eng\">Learning XML</title>"
      "<price><![CDATA[39.95]]></price>"
      "</book>"
      "</bookstore>";
  XMLContext context;
  XMLParser parser;
  ASSERT_TRUE(parser.Parse(xml, &context));

  XPathContext xpathcontext;
  XPath path("//price");
  context.execute(path, &xpathcontext);
  const std::vector<Node*>& res = xpathcontext.GetResult();
  ASSERT_EQ(res.size(), 2u);
  ASSERT_EQ(res[0]->type(), Node::kElementNode);
  ASSERT_EQ(res[1]->type(), Node::kElementNode);
  TextNode* text1 = (TextNode*)res[0]->first_child();  // NOLINT
  TextNode* text2 = (TextNode*)res[1]->first_child();  // NOLINT
  ASSERT_EQ(text1->type(), Node::kTextNode);
  ASSERT_EQ(text2->type(), Node::kCDataSectionNode);
  ASSERT_EQ(text1->text(), "29.99");
  ASSERT_EQ(text2->text(), "39.95");

  TreeDumper dumper;
  dumper.Dump(context.root());
}

TEST(XPathTest, RelativePath) {
  const char* xml = ""
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<bookstore>"
      "<book>"
      "<title lang=\"eng\">Harry Potter</title>"
      "<price>29.99</price>"
      "</book>"
      "<book>"
      "<title lang=\"eng\">Learning XML</title>"
      "<price><![CDATA[39.95]]></price>"
      "</book>"
      "</bookstore>";
  XMLContext context;
  XMLParser parser;
  ASSERT_TRUE(parser.Parse(xml, &context));

  XPathContext xpathcontext0;
  XPath path0("/book");
  context.execute(path0, &xpathcontext0);
  const std::vector<Node*>& res0 = xpathcontext0.GetResult();
  ASSERT_EQ(res0.size(), 0u);

  XPathContext xpathcontext1;
  XPath path1("//book");
  context.execute(path1, &xpathcontext1);
  const std::vector<Node*>& res1 = xpathcontext1.GetResult();
  ASSERT_EQ(res1.size(), 2u);
  ASSERT_EQ(res1[0]->type(), Node::kElementNode);
  ASSERT_EQ(res1[1]->type(), Node::kElementNode);
  
  XPathContext xpathcontext2;
  XPath path2("/price");
  util::xml::Element* element = res1[0]->ToElement();
  element->execute(path2, &xpathcontext2);
  const std::vector<Node*>& res2 = xpathcontext2.GetResult();
  ASSERT_EQ(res2.size(), 1u);
  ASSERT_EQ(res2[0]->type(), Node::kElementNode);

  TreeDumper dumper;
  dumper.Dump(context.root());
}

TEST(XPathTest, Attribute) {
  const char* xml = ""
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<bookstore>"
      "<book>"
      "<title lang=\"eng\">Harry Potter</title>"
      "<price>29.99</price>"
      "</book>"
      "<book>"
      "<title lang=\"eng\">Learning XML</title>"
      "<price><![CDATA[39.95]]></price>"
      "</book>"
      "</bookstore>";

  XMLContext context;
  XMLParser parser;
  ASSERT_TRUE(parser.Parse(xml, &context));

  XPathContext xpathcontext;
  XPath path("//title[@lang='eng']");
  context.execute(path, &xpathcontext);
  const std::vector<Node*>& res = xpathcontext.GetResult();
  ASSERT_EQ(res.size(), 2u);
  ASSERT_EQ(res[0]->type(), Node::kElementNode);
  ASSERT_EQ(res[1]->type(), Node::kElementNode);
  ASSERT_EQ(res[0]->ToElement()->GetAttr("lang"), "eng");
}
