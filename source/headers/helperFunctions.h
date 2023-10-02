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

        // A function for getting a value from a given key string from an .atris file for uint8_t and uint16_t.
        int GetAtrisKeyValue_uint8_t(string Key, vector<string> StringVector, vector<string> *StringPtr, string AtrisFilePath);
        int GetAtrisKeyValue_uint16_t(string Key, vector<string> StringVector, vector<string> *StringPtr, string AtrisFilePath);

        vector<short> GetAtrisKeyValue_vector(string Key, vector<string> StringVector, vector<string> *StringPtr, string AtrisFilePath);

        // A function for getting a value from a given key string from an .atris file for bool.
        bool GetAtrisKeyValue_bool(string Key, vector<string> StringVector, vector<string> *StringPtr, string AtrisFilePath);

};