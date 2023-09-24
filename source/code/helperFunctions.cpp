#include <string>
#include <vector>

#include "helperFunctions.h"

using namespace std;

// Function for removing spaces in strings
string HelperFunctions::RemoveSpaces(string String)
{
    string ReturnString;

    // Go through each charecter of the string
    for(int CharNum = 0; CharNum < String.length();)
    {   
        // Check if the element is a string.
        if(String[CharNum] != ' ')
        {   
            // If it is not, push it to the the return string.
            ReturnString.push_back(String[CharNum]);
        }

        CharNum++;
    }
    
    return ReturnString;
}

// Splits a string by a given character delimiter in ASCII, and returns the split string in a vector, without the delimiter.
vector<string> HelperFunctions::SplitByDelimiter(string String, char Delimiter)
{
    vector<string> SplitStringVector;

    // Run through all the content of the file, and check for delimiters.
    for(int Char = 0; Char < String.length();)
    {   
        // Check if the character is the delimiter ';'.
        if(String[Char] == Delimiter)
        {
            // Line until the delimiter into a vector.
            SplitStringVector.push_back(String.substr(0, Char));

            // Erase that part of the string
            String.erase(0, Char+1);
            
            // Reset the char counter, since all of the string up till this point have been erased.
            Char = 0;
        }
        else
        {
            Char++;
        }
    }
    
    return SplitStringVector;
}