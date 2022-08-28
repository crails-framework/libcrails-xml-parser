#include <boost/property_tree/xml_parser.hpp>
#include <crails/context.hpp>
#include "xml_parser.hpp"

using namespace std;
using namespace Crails;
using namespace boost::property_tree;

void RequestXmlParser::operator()(Context& context, function<void(RequestParser::Status)> callback) const
{
  static const regex is_xml("(application|text)/xml", regex_constants::extended | regex_constants::optimize);
  const HttpRequest& request = context.connection->get_request();

  if (request.method() != HttpVerb::get && content_type_matches(request, is_xml))
  {
    wait_for_body(context, [callback]()
    {
      callback(RequestParser::Stop);
    });
  }
  else
    callback(RequestParser::Continue);
}

void RequestXmlParser::body_received(Context& context, const string& body) const
{
  if (body.size() > 0)
  {
    context.params["document"] = 0;
    context.params["document"].merge(DataTree().from_xml(body));
  }
}

