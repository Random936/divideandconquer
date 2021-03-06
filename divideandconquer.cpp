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

    rule() {
        firstindex = 0;
        lastindex = 0;
        firstsize = 0;
        lastsize = 0;
    }

    void printdata() {

        cout << "User Rule: " << userrule << "\n";
        cout << "First Index: " << firstindex << "\n";
        cout << "First Size: " << firstsize << "\n";
        cout << "Last Index: " << lastindex << "\n";
        cout << "Last Size: " << lastsize << "\n\n";

    }

    void apply(user username, string filename) {

        string firsttemp, lasttemp;
        string temprule = userrule;
        int tempfirstsize = 0;
        int templastsize = 0;
        fstream outfile;
        outfile.open(filename, fstream::app);

        if (firstsize == 0) {
            firsttemp = username.firstname;
        } else {
            firsttemp = username.firstname.substr(0, firstsize);
        }

        if (lastsize == 0) {
            lasttemp = username.lastname;
        } else {
            lasttemp = username.lastname.substr(0, lastsize);
        } 

        if (firstindex < lastindex) {
            tempfirstsize = firsttemp.size() - 1;
            temprule.replace(firstindex + templastsize, to_string(firstsize).length() + 2, firsttemp);
            temprule.replace(lastindex + tempfirstsize, to_string(lastsize).length() + 2, lasttemp);
        } else if (firstindex > lastindex) {
            templastsize = lasttemp.size() - 1;
            temprule.replace(lastindex + tempfirstsize, to_string(lastsize).length() + 2, lasttemp);
            temprule.replace(firstindex + templastsize, to_string(firstsize).length() + 2, firsttemp);
        }
        
        cout << "Generated username: " << temprule << "\n";
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
        while ((temprule.firstindex != 0) && (temprule.userrule.at(temprule.firstindex - 1) == '\\')) {
            temprule.firstindex++;
            temprule.firstindex = temprule.userrule.find('[', temprule.firstindex);
        }

        temprule.lastindex = temprule.userrule.find('{');
        while ((temprule.lastindex != 0) && (temprule.userrule.at(temprule.lastindex - 1) == '\\')) {
            temprule.lastindex++;
            temprule.lastindex = temprule.userrule.find('{', temprule.lastindex);
        }

        temp = temprule.userrule.substr(temprule.firstindex + 1, temprule.userrule.find(']') - (temprule.firstindex + 1));
        temprule.firstsize = stoi(temp);

        temp = temprule.userrule.substr(temprule.lastindex + 1, temprule.userrule.find('}') - (temprule.lastindex + 1));
        temprule.lastsize = stoi(temp);

        for (int i = 0; i < temprule.userrule.length(); i++) {
            if (temprule.userrule.at(i) == '\\') {

                if ((i < temprule.firstindex) && (i < temprule.lastindex)) {
                    temprule.userrule.erase(i, 1);
                    temprule.firstindex--;
                    temprule.lastindex--;
                } else if (i < temprule.firstindex) {
                    temprule.userrule.erase(i, 1);
                    temprule.firstindex--;
                } else if (i < temprule.lastindex) {
                    temprule.userrule.erase(i, 1);
                    temprule.lastindex--;
                } else {
                    temprule.userrule.erase(i, 1);
                }

            }

        }
        cout << "Loaded rule: " << temprule.userrule << "\n";
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

string createrule(string examplerule) {

    
    string finalrule;
    string firstexample = "firstname";
    string lastexample = "lastname";
    int firstdiff, lastdiff, tempplace = 0;

    while (tempplace + 1 < examplerule.length()) {

        for (int i = 0; i < firstexample.length(); i++) {
            if (examplerule.length() <= (tempplace + i + 1)) {
                tempplace = examplerule.length() - 1;
                firstdiff = i + 1;
                break;
            } else if (firstexample.at(i) != examplerule.at(i + tempplace)) {
                firstdiff = i;
                tempplace += firstdiff;
                break;
            } else if (i + 1 == firstexample.length()) {
                firstdiff = i + 1;
                tempplace += firstdiff;
                break;
            }
        }

        if (firstdiff != 0) {
            if (firstdiff == firstexample.length()) {
                finalrule.append("[0]");
                continue;
            } else {
                finalrule.append('[' + to_string(firstdiff) + ']');
                continue;
            }
        }

        for (int i = 0; i < lastexample.length(); i++) {
            if (examplerule.length() <= (tempplace + i + 1)) {
                tempplace = examplerule.length() - 1;
                lastdiff = i + 1;
                break;
            } else if (lastexample.at(i) != examplerule.at(i + tempplace)) {
                lastdiff = i;
                tempplace += lastdiff;
                break;
            } else if (i + 1 == lastexample.length()) {
                lastdiff = i + 1;
                tempplace += lastdiff;
                break;
            }
        }

        if ((lastdiff != 0) && (lastdiff != firstdiff)) {
            if (lastdiff == lastexample.length()) {
                finalrule.append("{0}");
                continue;
            } else {
                finalrule.append('{' + to_string(lastdiff) + '}');
                continue;
            }
        }
        
        if ((lastdiff == 0) && (firstdiff == 0) && (firstdiff + 1 != firstexample.length())) {
            finalrule.append(examplerule, tempplace, 1);
            tempplace++;
        }
        
    }
    finalrule.append("\n");
    cout << "Created and exported rule: " << finalrule << "\n";
    return finalrule;
}
 
int main(int argc, char **argv) {

    po::options_description description("Allowed arguments");
    description.add_options() 
    ("help", "Prints the help message.") 
    ("user", po::value<string>(), "Specifies the file containing names. (Required)")
    ("wordlist", po::value<string>()->default_value("wordlist.txt"), "Specifies the output file that contains the wordlist. If an existing file is specified, the wordlist will be appended. (Default is wordlist.txt)") 
    ("template", po::value<string>(), "Enter a template of the username policy using the firstname \"firstname\" and the lastname \"lastname\". An example of this is \"fir.lastname\"")
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
    
    if (varmap.count("template")) {
        fstream outfile;
        outfile.open("rules.txt", fstream::app);
        outfile << createrule(varmap["template"].as<string>());
        outfile.close();
    }
    
    loadnames(&usernames, varmap["user"].as<string>());
    rules = loadrules(&rules, varmap["rule"].as<string>());

    for (auto rule : rules) {
        for (auto user : usernames) {
            rule.apply(user, varmap["wordlist"].as<string>());
        }

    }

    return 0;
}