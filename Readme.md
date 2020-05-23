# *wordindex*: create a linenumber cross-referenced list of words

The *wordindex* program creates an alphabetically sorted list of words present in the input files and it reports the linenumbers where those words occur. In addition it can also report the number of word occurrences and the relative occurrences as a percentage (word frequency). Furthermore *wordindex* can accept a file with keywords --or stopwords-- that it must skip.

I created the *wordindex* project to discover how I would write such a program in [C++](http://en.wikipedia.org/wiki/C%2B%2B), twenty years after its predecessor [xref](http://www.eld.leidenuniv.nl/~moene/Home/museum/software/xref/) which was programmed in  [C](http://en.wikipedia.org/wiki/C_%28programming_language%29). A notable aspect is the treatment of the input files as  [STL](http://en.wikipedia.org/wiki/Standard_Template_Library)-like [containers](http://en.wikipedia.org/wiki/Standard_Template_Library#Containers), or collections as they are called in [[Wilson, 2007](#wilson_2007)].

The *wordindex* program works, but the project is in its alpha stage. *wordindex* compiles with  [Microsoft Visual C++ 8.0](http://en.wikipedia.org/wiki/Visual_C%2B%2B) and with  [GNU C 3.4.2](http://en.wikipedia.org/wiki/GNU_C) on [MS Windows](http://en.wikipedia.org/wiki/Windows), but it's probably not to difficult to support other platforms as well. The [single-session make](http://www.xs4all.nl/~evbergen/nonrecursive-make.html) setup needs further work.

*wordindex* is free software, and is distributed under the [GPL](http://www.gnu.org/copyleft/gpl.html). *wordindex* uses the  [open source](http://opensource.org/) unit test framework  [Fructose](http://fructose.sourceforge.net/) and the [Templatized C++ Command Line Parser Library, TCLAP](http://tclap.sourceforge.net/)

*wordindex*'s help screen.

```Text
Usage: bin\wordindex.exe [option...] [file...]

  -h, --help          display this help and exit
  -a, --author        report authors name and e-mail [no]
      --version       report program and compiler versions [no]
  -v, --verbose       report ... [none]

  -f, --frequency     also report word frequency as d.dd% (n) [no]
  -l, --lowercase     transform words to lowercase [no]
  -r, --reverse       only collect keyword occurrences, see --keywords [no]
  -s, --summary       also report number of (key)words and references [no]

  -i, --input=file    read filenames from given file [standard input or given filenames]
  -o, --output=file   write output to given file [standard output]
  -k, --keywords=file read keywords to skip (stopwords) from given file [none]
```

Long options also may start with a plus, like: `+help`.

*wordindex* creates an alphabetically sorted index of words present in the input files and it reports the lines where those words occur. Words that are marked as keywords are excluded (see option --keywords). Use option --reverse to only show the occurrences of keywords.

Words can be read from standard input, or from files specified on the command line and from files that are specified in another file (see option --input).

A file that specifies input filenames may look as follows:

```Text
# comment that extends to the end of the line ( ; also starts comment line)
file1.txt file2.txt
file3.txt
```

Example run:

```Text
echo hello world | wordindex +summary +frequency
   keywords  0
      words  2
 references  2

      hello  50% (1)  1
      world  50% (1)  1
```

Example:

`wordindex +lowercase file.txt | sort -n -k2 -r`

This creates a list of lowercase words, sorted on frequency of occurrence.


<a name="wilson_2007">Matthew Wilson.</a>
[Extended STL, Volume 1: Collections and Iterators](http://www.extendedstl.com/). Addison-Wesley Professional, 2007. ISBN-10 0-321-30550-7, ISBN-13 978-0-321-30550-3.
