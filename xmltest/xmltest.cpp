#include "xmltest.h"

using namespace tinyxml2;
using namespace std;


bool xmltest::XMLTest (const char* testString, const char* expected, const char* found, bool echo, bool extraNL )
{
	bool pass;
	if ( !expected && !found )
		pass = true;
	else if ( !expected || !found )
		pass = false;
	else
		pass = !strcmp( expected, found );
	if ( pass )
		printf ("[pass]");
	else
		printf ("[fail]");

	if ( !echo ) {
		printf (" %s\n", testString);
	}
	else {
		if ( extraNL ) {
			printf( " %s\n", testString );
			printf( "%s\n", expected );
			printf( "%s\n", found );
		}
		else {
			printf (" %s [%s][%s]\n", testString, expected, found);
		}
	}

	xmltest::allResults.result(pass);
	return pass;
}

bool xmltest::XMLTest(const char* testString, tinyxml2::XMLError expected, tinyxml2::XMLError found, bool echo, bool extraNL)
{
    return xmltest::XMLTest(testString, tinyxml2::XMLDocument::ErrorIDToName(expected), tinyxml2::XMLDocument::ErrorIDToName(found), echo, extraNL);
}

bool xmltest::XMLTest(const char* testString, bool expected, bool found, bool echo, bool extraNL)
{
    return xmltest::XMLTest(testString, expected ? "true" : "false", found ? "true" : "false", echo, extraNL);
}

template< class T > 
bool xmltest::XMLTest( const char* testString, T expected, T found, bool echo )
{
	bool pass = ( expected == found );
	if ( pass )
		printf ("[pass]");
	else
		printf ("[fail]");

	if ( !echo )
		printf (" %s\n", testString);
	else {
		char expectedAsString[64];
		tinyxml2::XMLUtil::ToStr(expected, expectedAsString, sizeof(expectedAsString));

		char foundAsString[64];
		tinyxml2::XMLUtil::ToStr(found, foundAsString, sizeof(foundAsString));

		printf (" %s [%s][%s]\n", testString, expectedAsString, foundAsString );
	}

	xmltest::allResults.result(pass);
	return pass;
}


// Explicit instantiations for static lib
template bool xmltest::XMLTest<int>(const char* testString, int expected, int found, bool echo=true);
template bool xmltest::XMLTest<long>(const char* testString, long expected, long found, bool echo=true);
template bool xmltest::XMLTest<unsigned int>(const char* testString, unsigned int expected, unsigned int found, bool echo=true);
template bool xmltest::XMLTest<unsigned long>(const char* testString, unsigned long expected, unsigned long found, bool echo=true);
template bool xmltest::XMLTest<double>(const char* testString, double expected, double found, bool echo=true);
template bool xmltest::XMLTest<float>(const char* testString, float expected, float found, bool echo=true);



void xmltest::NullLineEndings( char* p )
{
	while( p && *p ) {
		if ( *p == '\n' || *p == '\r' ) {
			*p = 0;
			return;
		}
		++p;
	}
}


int xmltest::example_1()
{
	XMLDocument doc;
	doc.LoadFile( "resources/dream.xml" );

	return doc.ErrorID();
}
/** @page Example_1 Load an XML File
 *  @dontinclude ./xmltest.cpp
 *  Basic XML file loading.
 *  The basic syntax to load an XML file from
 *  disk and check for an error. (ErrorID()
 *  will return 0 for no error.)
 *  @skip example_1()
 *  @until }
 */


int xmltest::example_2()
{
	static const char* xml = "<element/>";
	XMLDocument doc;
	doc.Parse( xml );

	return doc.ErrorID();
}
/** @page Example_2 Parse an XML from char buffer
 *  @dontinclude ./xmltest.cpp
 *  Basic XML string parsing.
 *  The basic syntax to parse an XML for
 *  a char* and check for an error. (ErrorID()
 *  will return 0 for no error.)
 *  @skip example_2()
 *  @until }
 */


int xmltest::example_3()
{
	static const char* xml =
		"<?xml version=\"1.0\"?>"
		"<!DOCTYPE PLAY SYSTEM \"play.dtd\">"
		"<PLAY>"
		"<TITLE>A Midsummer Night's Dream</TITLE>"
		"</PLAY>";

	XMLDocument doc;
	doc.Parse( xml );

	XMLElement* titleElement = doc.FirstChildElement( "PLAY" )->FirstChildElement( "TITLE" );
	const char* title = titleElement->GetText();
	printf( "Name of play (1): %s\n", title );

	XMLText* textNode = titleElement->FirstChild()->ToText();
	title = textNode->Value();
	printf( "Name of play (2): %s\n", title );

	return doc.ErrorID();
}
/** @page Example_3 Get information out of XML
	@dontinclude ./xmltest.cpp
	In this example, we navigate a simple XML
	file, and read some interesting text. Note
	that this example doesn't use error
	checking; working code should check for null
	pointers when walking an XML tree, or use
	XMLHandle.

	(The XML is an excerpt from "dream.xml").

	@skip example_3()
	@until </PLAY>";

	The structure of the XML file is:

	<ul>
		<li>(declaration)</li>
		<li>(dtd stuff)</li>
		<li>Element "PLAY"</li>
		<ul>
			<li>Element "TITLE"</li>
			<ul>
			    <li>Text "A Midsummer Night's Dream"</li>
			</ul>
		</ul>
	</ul>

	For this example, we want to print out the
	title of the play. The text of the title (what
	we want) is child of the "TITLE" element which
	is a child of the "PLAY" element.

	We want to skip the declaration and dtd, so the
	method FirstChildElement() is a good choice. The
	FirstChildElement() of the Document is the "PLAY"
	Element, the FirstChildElement() of the "PLAY" Element
	is the "TITLE" Element.

	@until ( "TITLE" );

	We can then use the convenience function GetText()
	to get the title of the play.

	@until title );

	Text is just another Node in the XML DOM. And in
	fact you should be a little cautious with it, as
	text nodes can contain elements.

	@verbatim
	Consider: A Midsummer Night's <b>Dream</b>
	@endverbatim

	It is more correct to actually query the Text Node
	if in doubt:

	@until title );

	Noting that here we use FirstChild() since we are
	looking for XMLText, not an element, and ToText()
	is a cast from a Node to a XMLText.
*/


bool xmltest::example_4()
{
	static const char* xml =
		"<information>"
		"	<attributeApproach v='2' />"
		"	<textApproach>"
		"		<v>2</v>"
		"	</textApproach>"
		"</information>";

	XMLDocument doc;
	doc.Parse( xml );

	int v0 = 0;
	int v1 = 0;

	XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement( "attributeApproach" );
	attributeApproachElement->QueryIntAttribute( "v", &v0 );

	XMLElement* textApproachElement = doc.FirstChildElement()->FirstChildElement( "textApproach" );
	textApproachElement->FirstChildElement( "v" )->QueryIntText( &v1 );

	printf( "Both values are the same: %d and %d\n", v0, v1 );

	return !doc.Error() && ( v0 == v1 );
}
/** @page Example_4 Read attributes and text information.
	@dontinclude ./xmltest.cpp

	There are fundamentally 2 ways of writing a key-value
	pair into an XML file. (Something that's always annoyed
	me about XML.) Either by using attributes, or by writing
	the key name into an element and the value into
	the text node wrapped by the element. Both approaches
	are illustrated in this example, which shows two ways
	to encode the value "2" into the key "v":

	@skip example_4()
	@until "</information>";

	TinyXML-2 has accessors for both approaches.

	When using an attribute, you navigate to the XMLElement
	with that attribute and use the QueryIntAttribute()
	group of methods. (Also QueryFloatAttribute(), etc.)

	@skip XMLElement* attributeApproachElement
	@until &v0 );

	When using the text approach, you need to navigate
	down one more step to the XMLElement that contains
	the text. Note the extra FirstChildElement( "v" )
	in the code below. The value of the text can then
	be safely queried with the QueryIntText() group
	of methods. (Also QueryFloatText(), etc.)

	@skip XMLElement* textApproachElement
	@until &v1 );
*/
