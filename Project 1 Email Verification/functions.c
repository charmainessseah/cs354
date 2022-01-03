/******************************************************************************
 * @file: functions.c
 *
 * WISC NETID: sseah
 * CANVAS USERNAME: Charmaine Seah
 * WISC ID NUMBER: 9080857817
 * OTHER COMMENTS FOR THE GRADER (OPTIONAL)
 *
 * @creator: sseah@wisc.edu
 * @modified: SUBMISSION DATE
 *****************************************************************************/
#include <stdio.h>
#include "functions.h"

// Some macros that may be useful to you
#define MAX_USERNAME_LEN 32
#define MAX_EMAIL_LEN 32
#define MAX_DOMAIN_LEN 64
#define MIN_PASSWORD_LEN 8
#define MAX_PASSWORD_LEN 16
#define NEW_LINE_CHAR 10

// Set this to 1 to enable dbgprintf statements, make sure to set it back to 0
// before submitting!
#define DEBUG 0
#define dbgprintf(...)       \
    if (DEBUG)               \
    {                        \
        printf(__VA_ARGS__); \
    }

/******************************************************************************
 * Helper function declaration
 *****************************************************************************/

int isLowercase(char character);
int isUppercase(char character);
int isDigit(char character);
int isSpace(char character);
int isUnderscore(char character);

/******************************************************************************
 * Helper function body
 *****************************************************************************/

int isLowercase(char character)
{
    if (character >= 97 && character <= 122)
    {
        return 1;
    }
    return 0;
}

int isUppercase(char character)
{
    if (character >= 65 && character <= 90)
    {
        return 1;
    }
    return 0;
}

int isDigit(char character)
{
    if (character >= 48 && character <= 57)
    {
        return 1;
    }
    return 0;
}

int isSpace(char character)
{
    if (character == 32)
    {
        return 1;
    }
    return 0;
}

int isUnderscore(char character)
{
    if (character == 95)
    {
        return 1;
    }
    return 0;
}
/******************************************************************************
 * Verification functions
 *****************************************************************************/

// can we assume that the length of username >= 0 ?
/*
 * A username must begin with a letter [A-Z, a-z], contain 32 characters
 * or less, and  may only consist of letters, underscores, or 
 * digits [A-Z, a-z, _, 0-9]. An error message is displayed if any of 
 * these conditions are not met. Only print the first applicable error 
 * message. 
 *
 * @param user : The username string
 * @param len : Size of the username input buffer
 * @return 1 if valid, 0 if not
 */
int Verify_Username(char user[], size_t len)
{

    /* BEGIN MODIFYING CODE HERE */
    dbgprintf("This line only prints if DEBUG is set to 1\n");

    // -------------------------------------
    int usernameLength = 0;
    int index = 0;
    while (index < len && user[index] != '\0')
    {
        usernameLength++;
        index++;
    }
    int firstChar = user[0];

    // check if username has valid starting char
    if (!(isUppercase(firstChar) || isLowercase(firstChar)))
    {
        printf(ERROR_01_USER_START_INVALID);
        return 0;
    }

    // check if username length > 0 and <= 32
    if (usernameLength > 32)
    {
        printf(ERROR_02_USER_LEN_INVALID);
        return 0;
    }

    // check if username has invalid char
    int containsInvalidChar = 0;
    for (int i = 0; i < usernameLength; i++)
    {
        if (!(isUppercase(user[i]) || isDigit(user[i]) || isUnderscore(user[i]) || isLowercase(user[i])))
        {
            containsInvalidChar = 1;
            break;
        }
    }
    if (containsInvalidChar)
    {
        printf(ERROR_03_USER_CHARS_INVALID);
        return 0;
    }
    printf(SUCCESS_1_USER);
    return 1;
}

/*
 * An email address has four parts:
 *      name
 *          exists
 *          must start with letter
 *          max 32 characters
 *          may contain only letters and digits
 *      @ symbol
 *          exists
 *      domain name
 *          exists
 *          max of 64 characters
 *          composed of one or more subdomains separated by .
 *          subdomain must begin with a letter
 *          subdomains may contain only letters and digits
 *      top-level domain 
 *          must be [.edu, .com, .net]
 *
 * If the email address contains one or more errors print only the first
 * applicable error from the list.
 *
 * Note this task is based on a real world problem and may not be the best 
 * order to approach writing the code.
 *
 * @param email : The email string
 * @param len : Size of the email input buffer
 * @return 1 if valid, 0 if not
 */
int Verify_Email(char email[], size_t len)
{
    // separate the email into 4 parts
    int index = 0;
    int atSymbolIndex = 0;
    int containsAtSymbol = 0;
    int emailLength = 0;

    // check if email contains @ symbol
    while (index < len && email[index] != '\0')
    {
        if (email[index] == '@')
        {
            atSymbolIndex = index;
            containsAtSymbol = 1;
        }
        emailLength++;
        index++;
    }

    // check if top level domain is missing
    int topLevelDomainIndexStart = emailLength - 4;
    char topLevelDomain[] = {email[topLevelDomainIndexStart], email[topLevelDomainIndexStart + 1], email[topLevelDomainIndexStart + 2], email[topLevelDomainIndexStart + 3]};
    char validTopLevelDomains[3][4] = {{'.', 'e', 'd', 'u'}, {'.', 'c', 'o', 'm'}, {'.', 'n', 'e', 't'}};
    int containsValidTopDomain = 0;
    for (int i = 0; i < 3; i++)
    {
        int charMatchCount = 0;
        for (int j = 0; j < 4; j++)
        {
            if (topLevelDomain[j] == validTopLevelDomains[i][j])
            {
                charMatchCount++;
            }
            else
            {
                break;
            }
        }
        if (charMatchCount == 4)
        {
            containsValidTopDomain = 1;
            break;
        }
    }

    // 1. check if name is empty
    if ((containsAtSymbol && atSymbolIndex == 0) || email[0] == '\0')
    {
        printf(ERROR_04_EMAIL_MISSING_NAME);
        return 0;
    }

    // 2. check that name starting char is valid
    // valid chars: (capital letters) 65 to 90, (small letters) 97 to 122
    if (!(isLowercase(email[0]) || isUppercase(email[0])))
    {
        printf(ERROR_05_EMAIL_START_INVALID);
        return 0;
    }

    // 3. check that name length <= 32
    int nameLength = 0;
    if (containsAtSymbol)
    {
        nameLength = atSymbolIndex;
    }
    else if (containsValidTopDomain)
    {
        nameLength = emailLength - 1 - 4;
    }
    else
    { // no @ symbol or top domain
        nameLength = emailLength;
    }

    if (nameLength > 32)
    {
        printf(ERROR_06_EMAIL_NAME_LEN_INVALID);
        return 0;
    }

    // 4. check that chars in name are valid
    // valid chars: (capital letters) 65 to 90, (digits) 48 to 57, (small letters) 97 to 122
    for (int i = 0; i < nameLength; i++)
    {
        if (!((isLowercase(email[i])) || isUppercase(email[i]) || isDigit(email[i])))
        {
            printf(ERROR_07_EMAIL_NAME_CHARS_INVALID);
            return 0;
        }
    }

    // 5. check if @ symbol is missing
    // in what scenario does this error print out?
    if (!containsAtSymbol)
    {
        printf(ERROR_08_EMAIL_MISSING_SYMBOL);
        return 0;
    }

    // 6. check if domain is missing
    int domainLength = 0;
    if (containsValidTopDomain)
    {
        domainLength = emailLength - nameLength - 1 - 4;
    }
    else
    {
        domainLength = emailLength - nameLength - 1;
    }

    if (domainLength == 0)
    {
        printf(ERROR_09_EMAIL_MISSING_DOMAIN);
        return 0;
    }
    else if (domainLength > 64)
    { // 7. check that domain length <= 64
        printf(ERROR_10_EMAIL_DOMAIN_LEN_INVALID);
        return 0;
    }

    // 8. check that domain or subdomain begins with a letter
    // valid chars: (capital letters) 65 to 90, (small letters) 97 to 122
    int subdomainLastIndex;
    if (containsValidTopDomain)
    {
        subdomainLastIndex = emailLength - 1 - 4;
    }
    else
    {
        subdomainLastIndex = emailLength - 1;
    }

    for (int i = atSymbolIndex + 1; i <= subdomainLastIndex - 1; i++)
    {
        // check first letter after @ symbol
        if (i == (atSymbolIndex + 1))
        {
            if (!(isUppercase(email[i]) || isLowercase(email[i])))
            {
                printf(ERROR_11_EMAIL_DOMAIN_START_INVALID);
                return 0;
            }
        }

        // check every following subdomain first char
        if (email[i] == '.')
        {
            if (!(isUppercase(email[i + 1]) || isLowercase(email[i + 1])))
            {
                printf(ERROR_11_EMAIL_DOMAIN_START_INVALID);
                return 0;
            }
        }
    }

    // 9. check that domain or subdomain are valids chars
    // valid chars: (capital letters) 65 to 90, (digits) 48 to 57, (small letters) 97 to 122
    for (int i = atSymbolIndex + 1; i <= subdomainLastIndex; i++)
    {
        if (email[i] != '.')
        {
            if (!((isLowercase(email[i])) || isUppercase(email[i]) || isDigit(email[i])))
            {
                printf(ERROR_12_EMAIL_DOMAIN_CHARS_INVALID);
                return 0;
            }
        }
    }

    // 10. Check for valid top level domain
    if (!containsValidTopDomain)
    {
        printf(ERROR_13_TOP_LEVEL_DOMAIN_INVALID);
        return 0;
    }

    /* END MODIFYING CODE HERE */

    printf(SUCCESS_2_EMAIL);
    return 1;
}

/*
 * The following password requirements must be verified:
 *  - May use any character except spaces (i.e., "my password" is invalid)
 *  - Must contain at least 8 characters (i.e., "Password" has 8 characters 
 *    and is valid)
 *  - May have at most 16 characters (i.e., "1234567890Abcdef" has 16 
 *    characters and is valid)
 *  - Must contain at least one upper case character [A-Z]
 *  - Must contain at least one lower case character [a-z]
 *
 * @param pwd : The original password string
 * @param len : Size of the original password input buffer
 * @return 1 if valid, 0 if not
 */
int Verify_Password(char pwd[], size_t len)
{

    /* BEGIN MODIFYING CODE HERE */

    // 1. check if pw contains spaces
    int index = 0;
    while (index < len && pwd[index] != '\0')
    {
        if (isSpace(pwd[index]))
        {
            printf(ERROR_14_PWD_SPACES_INVALID);
            return 0;
        }
        index++;
    }

    // 2. check for valid pw length (8 to 16)
    index = 0;
    int passwordLength = 0;
    while (index < len && pwd[index] != '\0')
    {
        passwordLength++;
        index++;
    }

    if (passwordLength < 8)
    {
        printf(ERROR_15_PWD_MIN_LEN_INVALID);
        return 0;
    }
    else if (passwordLength > 16)
    {
        printf(ERROR_16_PWD_MAX_LEN_INVALID);
        return 0;
    }

    // 3. check that pw contains at least one uppercase letter
    int containsUppercase = 0;
    index = 0;
    while (index < len && pwd[index] != '\0')
    {
        if (isUppercase(pwd[index]))
        {
            containsUppercase = 1;
            break;
        }
        index++;
    }

    if (!containsUppercase)
    {
        printf(ERROR_17_PWD_MIN_UPPER_INVALID);
        return 0;
    }

    // 4. check that pw contains at least one lowercase letter
    int containsLowercase = 0;
    index = 0;
    while (index < len && pwd[index] != '\0')
    {
        if (isLowercase(pwd[index]))
        {
            containsLowercase = 1;
            break;
        }
        index++;
    }
    if (!containsLowercase)
    {
        printf(ERROR_18_PWD_MIN_LOWER_INVALID);
        return 0;
    }

    /* END MODIFYING CODE HERE */
    return 1;
}
/*
 * Original Password and the Reentered Password must match
 *
 * @param pwd1 : The original password string
 * @param len1 : Size of the original password input buffer
 * @param pwd2 : The reentered password string
 * @param len2 : Size of the renetered password input buffer
 * @return 1 if valid, 0 if not
 */
int Verify_Passwords_Match(char pwd1[], size_t len1, char pwd2[], size_t len2)
{

    /* BEGIN MODIFYING CODE HERE */

    int index = 0;

    while (index < len1 && index < len2 && pwd1[index] != '\0' && pwd2[index] != '\0')
    {
        if (pwd1[index] != pwd2[index])
        {
            printf(ERROR_19_PWD_MATCH_INVALID);
            return 0;
        }
        index++;
    }

    if (pwd1[index] != '\0')
    {
        printf(ERROR_19_PWD_MATCH_INVALID);
        return 0;
    }

    if (pwd2[index] != '\0')
    {
        printf(ERROR_19_PWD_MATCH_INVALID);
        return 0;
    }
    /* END MODIFYING CODE HERE */

    printf(SUCCESS_3_PASSWORDS);
    return 1;
}

/******************************************************************************
 * I/O functions
 *****************************************************************************/

/*
 * Prompts user with an input and reads response from stdin
 *
 * @param message : Prompt displayed to the user
 * @param data : char array to hold user repsonse
 * @param MAX_LENGTH : Size of user response input buffer
 */
void Get_User_Data(char *message, char *data, const int MAX_LENGTH)
{
    printf("%s", message);
    fgets(data, MAX_LENGTH, stdin);
    /* BEGIN MODIFYING CODE HERE */
    // replace all newline char with null char
    int index = 0;
    while (data[index] != '\0' && index < MAX_LENGTH)
    {
        if (data[index] == '\n')
        {
            data[index] = '\0';
        }
        else
        {
            index++;
        }
    }
    /* END MODIFYING CODE HERE */
    return;
}
