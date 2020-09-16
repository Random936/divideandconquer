#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <boost/program_options.hpp>
using namespace std;
namespace po = boost::program_options;

class user {
public:
    string firstname;
    string lastname;
    string username;

    user() {
        firstname = "None";
        lastname = "None";
    }

    user(string Firstname, string Lastname) {
        firstname = Firstname;
        lastname = Lastname;
    }

    void printdata() {
        cout << firstname << "\n";
        cout << lastname << "\n";
    }

};

class rule {
public:
    string userrule;
    int firstindex;
    int lastindex;
    int firstsize;
    int lastsize;

    void printdata() {

        cout << "User Rule: " << userrule << "\n";
        cout << "First Index: " << firstindex << "\n";
        cout << "First Size: " << firstsize << "\n";
        cout << "Last Index: " << lastindex << "\n";
        cout << "Last Size: " << lastsize << "\n\n";

    }

    void apply(user username, string filename) {

        string temp;
        string temprule = userrule;
        fstream outfile;
        outfile.open(filename, fstream::app);

        if (firstsize == 0) {
            temp = username.firstname;
        } else {
            temp = username.firstname.substr(0, firstsize);
        } temprule.replace(temprule.find('['), to_string(firstsize).length() + 2, temp);

        if (lastsize == 0) {
            temp = username.lastname;
        } else {
            temp = username.lastname.substr(0, lastsize);
        } temprule.replace(temprule.find('{'), to_string(lastsize).length() + 2, temp);

        outfile << temprule << "\n";
        outfile.close();
    }

};

void loadnames(vector <user> *usernames, string filename) {
    
    user temp;
    string fullname;
    ifstream file;
    file.open(filename);
    while (getline(file, fullname)) {
        temp.firstname = fullname.substr(0, fullname.find(' '));
        temp.lastname = fullname.substr(fullname.find(' ') + 1, fullname.size());
        usernames->push_back(temp);
    }

}

vector <rule> loadrules(vector <rule> *rules, string rulefile) {

    string temp;
    rule temprule;
    ifstream infile;
    infile.open(rulefile);
    while (getline(infile, temprule.userrule)) {

        temprule.firstindex = temprule.userrule.find('[');
        temp = temprule.userrule.substr(temprule.firstindex + 1, temprule.userrule.find(']') - (temprule.firstindex + 1));
        temprule.firstsize = stoi(temp);

        temprule.lastindex = temprule.userrule.find('{');
        temp = temprule.userrule.substr(temprule.lastindex + 1, temprule.userrule.find('}') - (temprule.lastindex + 1));
        temprule.lastsize = stoi(temp);

        if (temprule.firstindex < temprule.lastindex) {
            temprule.lastindex -= 2;
        } else if (temprule.firstindex > temprule.lastindex) {
            temprule.firstindex -= 2;
        }

        rules->push_back(temprule);
    }

    infile.close();
    return *rules;
}

int main(int argc, char **argv) {

    
    
    po::options_description description("Allowed arguments");
    description.add_options() 
    ("help", "Prints the help message.") ("user", po::value<string>(), "Specifies the file containing names. (Required)")
    ("wordlist", po::value<string>()->default_value("wordlist.txt"), "Specifies the output file that contains the wordlist. If an existing file is specified, the wordlist will be appended. (Default is wordlist.txt)") 
    ("rule", po::value<string>()->default_value("rules.txt"), "Specify the rule file to use. (Default is rules.txt)");

    po::variables_map varmap;
    po::store(po::parse_command_line(argc, argv, description), varmap);
    po::notify(varmap);

    if (varmap.count("help")) {
        cout << description << "\n";
        return 1;
    } else if (!varmap.count("user")) {
        cout << description << "\n";
        return 1;
    }


    vector <rule> rules;
    vector <user> usernames;
    loadnames(&usernames, varmap["user"].as<string>());
    rules = loadrules(&rules, varmap["rule"].as<string>());

    for (auto rule : rules) {
        for (auto user : usernames) {
            rule.apply(user, varmap["wordlist"].as<string>());
        }

    }

    return 0;
}