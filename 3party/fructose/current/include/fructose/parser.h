/* FRUCTOSE C++ unit test library. 
Copyright (c) 2007 Chris Main and Andrew Peter Marlow. All rights reserved.
http://www.andrewpetermarlow.co.uk.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef INCLUDED_FRUCTOSE_PARSER
#define INCLUDED_FRUCTOSE_PARSER

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace fructose {

class parsed_tests
{
public:
    virtual ~parsed_tests() = 0;
    virtual void add_file(const std::string& name) = 0;
    virtual void add_class(const std::string& name) = 0;
    virtual void add_test(const std::string& name) = 0;
};

void parse(const std::vector<std::string>& file_names,
           parsed_tests& test_collection);

class file_parser
{
public:
    file_parser(const std::string& file_name,
                parsed_tests& test_collection)
        : m_file_name(file_name)
        , m_test_collection(test_collection)
        , m_add_file(&file_parser::first_add_file) {}

    void operator()(const std::string& line);

    void add_class(const std::string& name);
    void add_test(const std::string& name);

private:
    const std::string& m_file_name;
    parsed_tests& m_test_collection;

    typedef void (file_parser::*add_file)();
    add_file m_add_file;

    void first_add_file();
    void no_add_file();
};

class line_parser
{
public:
    line_parser(file_parser& parser,
                const std::string& line);

private:
    file_parser& m_parser;
    const std::string& m_line;

    typedef void (file_parser::*add_name)(const std::string&);

    void parse(const std::string& marker, add_name add);
};

void
parse(std::istream& file, file_parser& parser);

inline
void 
parse(const std::vector<std::string>& file_names,
      parsed_tests& test_collection)
{
    for (std::vector<std::string>::const_iterator file_name_i = file_names.begin(),
                                                  file_name_end = file_names.end();
         file_name_i != file_name_end;
         ++file_name_i)
    {
        std::ifstream file(file_name_i->c_str());
        if (file)
        {
            file_parser parser(*file_name_i, test_collection);
            parse(file, parser);
        }
        else
        {
            std::cerr << "Failed to open " << *file_name_i
                << "; skipping the file" << std::endl;
        }
    }
}

inline
void 
file_parser::operator()(const std::string& line)
{
    line_parser parser(*this, line);
}

inline
void
file_parser::add_class(const std::string& name)
{
    (this->*m_add_file)();
    m_test_collection.add_class(name);
}

inline
void
file_parser::add_test(const std::string& name)
{
    m_test_collection.add_test(name);
}

inline
parsed_tests::~parsed_tests()
{
}

inline
void
file_parser::first_add_file()
{
    m_test_collection.add_file(m_file_name);
    m_add_file = &file_parser::no_add_file;
}

inline
void
file_parser::no_add_file()
{
}

inline
line_parser::line_parser(file_parser& parser,
                         const std::string& line)
    : m_parser(parser)
    , m_line(line) 
{
    parse("FRUCTOSE_CLASS", &file_parser::add_class);
    parse("FRUCTOSE_STRUCT", &file_parser::add_class);
    parse("FRUCTOSE_TEST", &file_parser::add_test);
}

inline
void 
line_parser::parse(const std::string& marker, 
                   add_name add)
{
    const std::string::size_type begin_marker =
        m_line.find(marker);
    if (begin_marker != std::string::npos)
    {
        const std::string::size_type open_parenthesis = 
            m_line.find("(", begin_marker + 1);
        if (open_parenthesis == std::string::npos)
        {
            std::cerr << "Skipping invalid " << marker << ": " << m_line
                << std::endl;
            return;
        }
        const std::string::size_type close_parenthesis = 
            m_line.find(")", open_parenthesis + 1);
        if (close_parenthesis == std::string::npos)
        {
            std::cerr << "Skipping invalid " << marker << ": " << m_line
                << std::endl;
            return;
        }
        std::string::size_type start_name = open_parenthesis + 1;
        while (isspace(m_line[start_name]))
        {
            ++start_name;
        }
        std::string::size_type end_name = close_parenthesis - 1;
        while (isspace(m_line[end_name]))
        {
            --end_name;
        }
        (m_parser.*add)(m_line.substr(start_name,
                                      end_name - start_name + 1));
    }
}

inline
void
parse(std::istream& file, file_parser& parser)
{
    while (!file.eof())
    {
        std::string line;
        std::getline(file, line);
        parser(line);
    }
}

} // fructose namespace

#endif

