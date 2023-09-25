#include <string>
#include <vector>

using namespace std;

class HelperFunctions
{
    public:
        // A function for removing spaces in a string.
        string RemoveSpaces(string String);

        // Function for splitting a string by a delimiter.
        vector<string> SplitByDelimiter(string String, char Delimiter);

        // A function for getting a value from a given key string from an .atris file.
        int GetAtrisKeyValue(string Key, vector<string> StringVector);

};