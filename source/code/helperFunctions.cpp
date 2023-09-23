#include <string>

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