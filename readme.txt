README - Divide and Conquer

Divide and Conquer is a program made to allow for the quick creation of a wordlist with a first and last name. With the ability to easily create a rule file, you can customize the wordlist to your needs with ease.

REQUIREMENTS

libboost-program-options1.71.0

INSTALL
Run the command below to compile the program.
g++ divideandconquer.cpp -o divideandconquer -l boost_program_options

USING THE PROGRAM

Using the program itself is fairly simple. The only arguments are user file, rule file, and wordlist output file. These are specified by '--user', '--rule', and '--wordlist' followed by the argument. Both the rule and wordlist argument are not required, as their defaults are 'rules.txt', and 'wordlist.txt'.

CREATING RULE FILES

Divide and conquer comes with its own method of easily creating rule files to match usernames. This method involves using '[0]' to represent a first name and using '{0}' to represent the last name. The number the center of these rules can be changed to select the length of the name you want with zero being the entire name. An example of this is shown below.

In user.txt:
firstname lastname

In rules.txt:
[3].{0}

Result:
fir.lastname

Thank you for using Divide and Conquer!
