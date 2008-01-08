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

#ifndef INCLUDED_FRUCTOSE_GENERATOR
#define INCLUDED_FRUCTOSE_GENERATOR

#include "fructose/parser.h"

#include "tclap/CmdLine.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace fructose {

void generate_test_harness(int argc, char *argv[]);

/**
 * 
 */
class harness_generator : public parsed_tests
{
public:
    /*virtual*/ ~harness_generator();
    harness_generator(const std::string& file_name);

    virtual void add_file(const std::string& name);
    virtual void add_class(const std::string& name);
    virtual void add_test(const std::string& name);

private:
    std::string m_includes;
    std::string m_main;
    std::ofstream m_file;
    std::ostream* m_output_p;
    std::string m_current_class;
    std::string m_current_instance;

    void finish_previous_class();
};

/**
 *
 */
class test_files
{
public:
    test_files(const std::vector<std::string>& files);

    void read_from(const std::string& file_name);
    void read_from(std::istream& input);
    void parse(const std::string& harness_name);

    bool empty() const;

private:
    std::vector<std::string> m_test_files;
};

inline
void
generate_test_harness(int argc, char *argv[])
{
    try
    {
        TCLAP::CmdLine cmd("FRUCTOSE test harness generator", ' ', "0.1");
        TCLAP::ValueArg<std::string> input_arg("i", 
                                               "input", 
                                               "File listing test files",
                                               false,
                                               "",
                                               "path",
                                               cmd);
        TCLAP::ValueArg<std::string> output_arg("o",
                                                "output",
                                                "File name for the harness",
                                                false,
                                                "",
                                                "path",
                                                cmd);
        TCLAP::UnlabeledMultiArg<std::string> test_files_arg("files",
                                                             "Test files",
                                                             false,
                                                             "path(s)",
                                                             cmd);
        cmd.parse(argc, argv);

        const std::string& input_file = input_arg.getValue();
        const std::string& harness_name = output_arg.getValue();
        test_files files(test_files_arg.getValue());

        if (!input_file.empty())
        {
            files.read_from(input_file);
        }
        if (files.empty())
        {
            files.read_from(std::cin);
        }
        files.parse(harness_name);
    }
    catch (const TCLAP::ArgException& exception)
    {
        std::cerr << exception.error()
            << " for argument " << exception.argId() << std::endl;
    }
}

inline
harness_generator::~harness_generator()
{
    finish_previous_class();
    *m_output_p << m_includes << "\n" << m_main << std::endl
                << "    return retval;\n}\n";
}

inline
harness_generator::harness_generator(const std::string& file_name)
    : m_main("#include <stdlib.h>\n\n"
             "int main(int argc, char *argv[])\n{\n"
             "    int retval = EXIT_SUCCESS;\n")
{
    if (file_name.empty())
    {
        m_output_p = &std::cout;
    }
    else
    {
        m_file.open(file_name.c_str());
        if (m_file)
        {
            m_output_p = &m_file;
        }
        else
        {
            std::cerr << "Failed to open " << file_name
                << "; redirecting to standard output" << std::endl;
            m_output_p = &std::cout;
        }
    }
}

inline
void 
harness_generator::add_file(const std::string& name)
{
    m_includes += "#include \"";
    m_includes += name;
    m_includes += "\"\n";
}

inline
void 
harness_generator::add_class(const std::string& name)
{
    finish_previous_class();
    m_current_class = name;
    m_current_instance = name + "_instance";
    m_main += "    {\n        ";
    m_main += m_current_class;
    m_main += " ";
    m_main += m_current_instance;
    m_main += ";\n";
}

inline
void 
harness_generator::add_test(const std::string& name)
{
    m_main += "        ";
    m_main += m_current_instance;
    m_main += ".add_test(\"";
    m_main += name;
    m_main += "\", &";
    m_main += m_current_class;
    m_main += "::";
    m_main += name;
    m_main += ");\n";
}

inline
void 
harness_generator::finish_previous_class()
{
    if (!m_current_instance.empty())
    {
        m_main += "        retval = ";
        m_main += m_current_instance;
        m_main += ".run(argc, argv);\n    }\n";
    }
}

inline
test_files::test_files(const std::vector<std::string>& files)
    : m_test_files(files)
{
}

inline 
void
test_files::read_from(const std::string& file_name)
{
    std::ifstream input(file_name.c_str());
    if (input)
    {
        read_from(input);
    }
    else
    {
        std::cerr << "Could not open input file "
            << file_name << std::endl;
    }
}

inline
void
test_files::read_from(std::istream& input)
{
    while (!input.eof())
    {
        std::string test_file;
        input >> test_file;
        if (!test_file.empty())
        {
            m_test_files.push_back(test_file);
        }
    }
}

inline
void
test_files::parse(const std::string& harness_name)
{
    harness_generator generator(harness_name);
    fructose::parse(m_test_files, generator);
}

inline
bool
test_files::empty() const
{
    return m_test_files.empty();
}

}

#endif
