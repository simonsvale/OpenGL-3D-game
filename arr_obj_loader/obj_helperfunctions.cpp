#include "obj_helperfunctions.h"

// ONLY FOR PARSING .OBJ WAVEFRONT FILES.
void SplitBySpace(int Index, vector<string> ObjLineVector, vector<string> *SplitObjLineVector)
{
    // Loop through all characters in that vector index.
    for(int Index2 = 0; Index2 < ObjLineVector[Index].size();)
    {   
        // If the space delimiter is found, split the string.
        if(ObjLineVector[Index][Index2] == 0x20)    // Hex code for "space" in ASCII
        {
            // Pushback the up until the delimiter space.
            SplitObjLineVector->push_back(ObjLineVector[Index].substr(0, Index2));
            
            // Create a new substring excluding the split off part of the string and set index 2 to zero.
            ObjLineVector[Index] = ObjLineVector[Index].substr(Index2+1, ObjLineVector.size()-(Index2+1));
            Index2 = 0;
        }
        Index2++;
    }

    // Push the last bit of the vector.
    SplitObjLineVector->push_back(ObjLineVector[Index]);
}

void SplitByDelimiter(string String, vector<string> *SplitStrVecPtr, char Delimiter, int DelimiterAmount)
{
    // Run through the string, and check for the given delimiter.
    for(int Char = 0; Char < String.length();)
    {   
        // Check if the character is the delimiter.
        if(String[Char] == Delimiter)
        {
            // Check if the first character is a delimiter
            if(String[0] != Delimiter)
            {
                // Push the substring until the delimiter, excluding the delimiter, into a vector using a pointer.
                SplitStrVecPtr->push_back(String.substr(0, Char));
            }

            // Create a substring to the right of the delimiter, excluding the delimiter.
            String = String.substr(Char+1, String.size());

            // Found delimiter decrease amount to look for by 1.
            DelimiterAmount -= 1;

            if(DelimiterAmount == 0)
            {
                // Reached last delimiter, push rest of string and return.
                SplitStrVecPtr->push_back(String);
                return;
            }

            /* 
                Run the function with this new substring, since this is recursive function, 
                at some point, the case where no delimiters are found is reached. And the function terminates.
            */
            SplitByDelimiter(String, SplitStrVecPtr, Delimiter, DelimiterAmount);
            
            // Terminate this recursive step of the function.
            return;
        }
        else
        {
            Char++;
        }
    }
    
    // Will only reach when base case
    SplitStrVecPtr->push_back(String.substr(0, String.length()));
}