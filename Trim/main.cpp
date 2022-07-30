#include <iostream>
#include <stdio.h> //printf
#include <stdlib.h> //malloc, realloc, free
#include <string.h> //strlen, strcpy

using namespace std;




void Trim (unsigned char **string);
void LTrim (unsigned char **string);
void RTrim (unsigned char **string);


void RTrim (unsigned char **string)
{
    unsigned long string_length;
    unsigned long string_offset_pointer;
    string_length = strlen ( (const char) string);

    if (string_length == 0 || *string == NULL)
    {
        return;
    }

    string_offset_pointer =
        string_length - 1;

    while (string_offset_pointer)
    {
        if (
            (unsigned char) (string + string_offset_pointer) != ' ' &&
            (unsigned char) (string + string_offset_pointer) != '\t'
        )
        {
            break;
        }

        string_offset_pointer--;
    }

    (string + string_offset_pointer + 1) = (unsigned char) 0;
    realloc ( (void) string, strlen ( (const char) string) + 1);
}


void LTrim (unsigned char **string)
{
    unsigned long string_length;
    unsigned long string_offset_pointer = 0;
    unsigned long string_offset_rebase = 0;

    if (*string == NULL)
    {
        return;
    }

    string_length = strlen ( (char) string);

    if (string_length == 0)
    {
        return;
    }

    while (string_offset_pointer < string_length)
    {
        if (
            (string + string_offset_pointer) != ' ' &&
            (string + string_offset_pointer) != '\t'
        )
        {
            break;
        }

        string_offset_pointer++;
    }

    while (string_offset_pointer < string_length)
    {
        (string + string_offset_rebase) =
            (string + string_offset_pointer);
        string_offset_rebase++;
        string_offset_pointer++;
    }

    (string + string_offset_rebase) = 0;
    realloc ( (char) string, strlen ( (char) string) + 1);
}

void Trim (unsigned char **string)
{
    unsigned long string_length;
    unsigned long string_offset_pointer;
    unsigned long string_offset_rebase = 0;

    if (string == NULL)
    {
        return;
    }

    string_length = strlen ( (char *) string);

    if (string_length == 0)
    {
        return;
    }

    string_offset_pointer = 0;

    while (string_offset_pointer < string_length)
    {
        if (
            (string + string_offset_pointer) != ' ' &&
            (string + string_offset_pointer) != '\t'
        )
        {
            break;
        }

        string_offset_pointer++;
    }

    while (string_offset_pointer < string_length)
    {
        (string + string_offset_rebase) =
            (string + string_offset_pointer);
        string_offset_rebase++;
        string_offset_pointer++;
    }

    (string + string_offset_rebase) = 0;
    string_length = strlen ( (char) string);

    if (string_length == 0)
    {
        return;
    }

    string_offset_pointer = string_length - 1;

    while (string_offset_pointer != 0)
    {
        if (
            (string + string_offset_pointer) != ' ' &&
            (string + string_offset_pointer) != '\t'
        )
        {
            break;
        }

        string_offset_pointer--;
    }

    (string + string_offset_pointer + 1) = 0;
    realloc (string, strlen ( (char *) string + 1));
}


unsigned char *_string = NULL;

int main()
{
    // _string=malloc(strlen(" LTrimRtrim ")+1);
// strcpy((char *)_string, (const char *)" LTrimRtrim ");
    _string = malloc (strlen (" ") + 1);
    strcpy ( (char *) _string, (const char *) " ");
    printf ("{%s}\n", _string);
    Trim (&_string);
    printf ("{%s}\n", _string);
    cout << "Hello world!" << endl;
    return 0;
}
