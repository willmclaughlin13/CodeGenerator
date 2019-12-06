//
// Created by Will on 12/5/2019.
//

#include "Object.h"

#include "Object.h"
#include <sstream>
#include <cstdlib>

#define DEBUG 0
#define debug if (DEBUG) cout

string nameof [] = {"unknown", "integer", "real", "string", "list"};

Object::Object()
{
    type = NONE;
    intval = realval = 0;
}

Object::Object (const int & value)
{
    type = INT;
    intval = value;
    realval = 0;
    MakeName ();
}

Object::Object (const double & value)
{
    type = REAL;
    intval = 0;
    realval = value;
    MakeName ();
}

Object::Object (const string & value)
{
    type = NONE;
    int sgn = 0, num = 0, dp = 0;
    if (value.length() > 1 && (value[0] == '+' || value[0] == '-'))
        sgn++;
    for (int i = sgn; i < value.size(); i++)
        if (isdigit (value[i]))
            num++;
        else if (value[i] == '.')
            dp++;
    if (sgn + num + dp == value.size())
        if (dp == 0)
            *this = Object (atoi (value.c_str()));
        else if (dp == 1)
            *this = Object (atof (value.c_str()));
    if (type != NONE)
        return;
    int i = 0;
    while (isspace(value[i]))
        i++;
    if (value[i] != '(')
    {
        type = STRING;
        strval = value;
        intval = realval = 0;
        MakeName ();
        return;
    }
    debug << "value is " << value << endl;
    string temp;
    for (int i = 0; i < value.length(); i++)
    {
        temp += value[i];
        if (value[i] == '(' || value[i] == ')')
            temp += ' ';
    }
    debug << "temp is " << temp << endl;
    stringstream ss (temp.c_str(), ios_base::in);
    *this = Object (ss);
}

bool Object::operator == (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return intval == O.intval;
            if (O.type == REAL)
                return intval == O.realval;
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return realval == O.intval;
            if (O.type == REAL)
                return realval == O.realval;
            throw "";
        }
        if (type == STRING)
        {
            if (O.type == STRING)
                return strval == O.strval;
            throw "";
        }
        if (type == LIST)
        {
            if (O.type == LIST)
            {
                if (listval.size() != O.listval.size())
                    return false;
                for (int i = 0; i < listval.size(); i++)
                    if (listval[i] != O.listval[i])
                        return false;
                return true;
            }
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for == operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

bool Object::operator != (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return intval != O.intval;
            if (O.type == REAL)
                return intval != O.realval;
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return realval != O.intval;
            if (O.type == REAL)
                return realval != O.realval;
            throw "";
        }
        if (type == STRING)
        {
            if (O.type == STRING)
                return strval != O.strval;
            throw "";
        }
        if (type == LIST)
        {
            if (O.type == LIST)
            {
                if (listval.size() != O.listval.size())
                    return true;
                for (int i = 0; i < listval.size(); i++)
                    if (listval[i] != O.listval[i])
                        return true;
                return false;
            }
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for != operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

bool Object::operator < (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return intval < O.intval;
            if (O.type == REAL)
                return intval < O.realval;
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return realval < O.intval;
            if (O.type == REAL)
                return realval < O.realval;
            throw "";
        }
        if (type == STRING)
        {
            if (O.type == STRING)
                return strval < O.strval;
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for < operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

bool Object::operator <= (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return intval <= O.intval;
            if (O.type == REAL)
                return intval <= O.realval;
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return realval <= O.intval;
            if (O.type == REAL)
                return realval <= O.realval;
            throw "";
        }
        if (type == STRING)
        {
            if (O.type == STRING)
                return strval <= O.strval;
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for <= operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

bool Object::operator > (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return intval > O.intval;
            if (O.type == REAL)
                return intval > O.realval;
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return realval > O.intval;
            if (O.type == REAL)
                return realval > O.realval;
            throw "";
        }
        if (type == STRING)
        {
            if (O.type == STRING)
                return strval > O.strval;
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for > operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

bool Object::operator >= (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return intval >= O.intval;
            if (O.type == REAL)
                return intval >= O.realval;
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return realval >= O.intval;
            if (O.type == REAL)
                return realval >= O.realval;
            throw "";
        }
        if (type == STRING)
        {
            if (O.type == STRING)
                return strval >= O.strval;
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for >= operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

Object Object::operator + (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return Object (intval + O.intval);
            if (O.type == REAL)
                return Object (intval + O.realval);
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return Object (realval + O.intval);
            if (O.type == REAL)
                return Object (realval + O.realval);
            throw "";
        }
        if (type == STRING)
        {
            if (O.type == STRING)
                return Object (strval + O.strval);
            throw "";
        }
        if (type == LIST)
        {
            if (O.type == LIST)
            {
                string n1 = name;
                n1[n1.size() - 1] = ' ';
                string n2 = O.name;
                n2[0] = ' ';
                return Object (n1 + n2);
            }
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for + operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

Object Object::operator - (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return Object (intval - O.intval);
            if (O.type == REAL)
                return Object (intval - O.realval);
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return Object (realval - O.intval);
            if (O.type == REAL)
                return Object (realval - O.realval);
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for - operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

Object Object::operator * (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
                return Object (intval * O.intval);
            if (O.type == REAL)
                return Object (intval * O.realval);
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
                return Object (realval * O.intval);
            if (O.type == REAL)
                return Object (realval * O.realval);
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for * operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

Object Object::operator / (const Object & O) const
{
    try
    {
        if (type == INT)
        {
            if (O.type == INT)
            {
                if (O.intval == 0)
                    throw "division by 0";
                return Object (intval / O.intval);
            }
            if (O.type == REAL)
            {
                if (O.realval == 0)
                    throw "division by 0";
                return Object (intval / O.realval);
            }
            throw "";
        }
        if (type == REAL)
        {
            if (O.type == INT)
            {
                if (O.intval == 0)
                    throw "division by 0";
                return Object (realval / O.intval);
            }
            if (O.type == REAL)
            {
                if (O.realval == 0)
                    throw "division by 0";
                return Object (realval / O.realval);
            }
            throw "";
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for / operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

Object Object::operator % (const Object & O) const
{
    try
    {
        if (type == INT && O.type == INT)
        {
            if (O.intval == 0)
                throw "modulo by 0";
            return Object (intval % O.intval);
        }
        throw "";
    }
    catch (const char * message)
    {
        cerr << "Wrong types for % operator: " << name << " and " << O.name
             << " (" << nameof[type] << " and " << nameof[O.type] << ")\n";
        exit (1);
    }
}

bool numberp (const Object & O)
{
    if (O.type == INT || O.type == REAL)
        return true;
    return false;
}

bool symbolp (const Object & O)
{
    if (O.type == STRING)
        return true;
    return false;
}

bool listp (const Object & O)
{
    if (O.type == LIST)
        return true;
    return false;
}

bool zerop (const Object & O)
{
    try
    {
        if (O.type != INT && O.type != REAL)
            throw "type";
        if (O.type == INT && O.intval == 0)
            return true;
        if (O.type == REAL && O.realval == 0)
            return true;
        return false;
    }
    catch (const char * message)
    {
        cerr << "Wrong " << message << " for zero? predicate\n";
        exit (1);
    }
}

bool nullp (const Object & O)
{
    if (O.type == LIST && O.listval.size() == 0)
        return true;
    return false;
}

bool stringp (const Object & O)
{
    if (O.type == STRING)
        return true;
    return false;
}

Object listop (const string & S, const Object & O)
{// will handle list operator as well as car, cdr, etc.
    try
    {
        debug << "listop name : " << S << endl;
        if (S == "list")
        {
            Object T;
            T.type = LIST;
            T.listval.push_back (O);
            T.MakeName ();
            return T;
        }
        if (O.type != LIST)
            throw "type";
        if (O.listval.size() < 1)
            throw "size";
        if (S.length() < 3 || S[0] != 'c' || S[S.length()-1] != 'r')
            throw "name";
        Object T = O;
        int i = S.length()-2;
        while (S[i] == 'd' || S[i] == 'a')
        {
            if (S[i] == 'd')
                T.listval.erase (T.listval.begin());
            else if (S[i] == 'a')
                T = Object (T.listval[0]);
            i--;
        }
        if (S[i] == 'c')
        {
            T.MakeName ();
            return T;
        }
        debug << S << " : " << S[i] << endl;
        throw "name";
    }
    catch (const char * message)
    {
        cerr << "Wrong " << message << " for list operation function: " << S;
        if (message[0] == 't')
            cerr << " (" << nameof[O.type]<< ")";
        else if (message[0] == 's')
            cerr << " (" << O.listval.size() << ")";
        else if (message[0] == 'n')
            cerr << " (list operator may need 2 parameters)";
        cerr << endl;
        exit (1);
    }
}

Object listop (const string & S, const Object & O1, const Object O2)
{// will handle cons and append operators
    try
    {
        if (S == "cons")
        {
            if (O2.type != LIST)
                throw "type";
            Object T = O2;
            T.listval.insert (T.listval.begin(), O1);
            T.MakeName();
            return T;
        }
        if (S == "append")
        {
            if (O1.type != LIST || O2.type != LIST)
                throw "type";
            Object T = O1;
            for (int i = 0; i < O2.listval.size(); i++)
                T.listval.push_back(O2.listval[i]);
            T.MakeName();
            return T;
        }
        debug << S << endl;
        throw "name";
    }
    catch (const char * message)
    {
        cerr << "Wrong " << message << " for list operation function: " << S;
        if (message[0] == 't')
            cerr << " (" << nameof[O1.type]<< " or " << nameof[O2.type]<< ")";
        else if (message[0] == 's')
            cerr << " (" << O1.listval.size() << " or " << O2.listval.size() << ")";
        else if (message[0] == 'n')
            cerr << " (list operator may accept only 1 parameter)";
        cerr << endl;
        exit (1);
    }
}

Object round (const Object & O)
{
    try
    {
        int r = 0;
        if (O.type == INT)
            r = O.intval;
        else if (O.type == REAL)
            r = O.realval + 0.5;
        else
            throw "type";
        return Object(r);
    }
    catch (const char * message)
    {
        cerr << "Wrong " << message << " for round function parameter: " << O.name;
        if (message[0] == 't')
            cerr << " (" << nameof[O.type]<< ")";
        cerr << endl;
        exit (1);
    }
}

ostream & operator << (ostream & outs, const Object & O)
{
    outs << O.name;
    return outs;
}

Object::Object (stringstream & ss)
{
    try
    {
        type = LIST;
        intval = realval = 0;
        char achar;
        ss >> achar;
        if (achar != '(')
            throw "(";
        bool endfound = false;
        while (!endfound)
        {
            Object temp;
            while (isspace (achar = ss.peek()))
                ss.get (achar);
            if (achar == '(')
                temp = Object (ss);
            else if (achar == ')')
            {
                endfound = true;
                ss.get (achar);
            }
            else
            {
                string astring;
                ss >> astring;
                debug << "astring is " << astring << endl;
                if (astring[astring.size()-1] == ')')
                {
                    endfound = true;
                    astring = astring.substr (0, astring.size()-1);
                }
                if (astring.size() > 0)
                    temp = Object (astring);
            }
            if (temp.type != NONE)
            {
                debug << "Adding " << temp << " to LIST Object\n";
                listval.push_back (temp);
            }
        }
        MakeName ();
        debug << "name is " << name << endl;
    }
    catch (const char * message)
    {
        cerr << "message" << endl;
        exit (1);
    }
}

void Object::MakeName ()
{
    stringstream ss;
    switch (type)
    {
        case INT:
            ss << intval;
            break;
        case REAL:
            ss << realval;
            break;
        case STRING:
            ss << strval;
            break;
        case LIST:
            ss << '(';
            if (listval.size() > 0)
                ss << listval[0].name;
            for (int i = 1; i < listval.size(); i++)
                ss << ' ' << listval[i].name;
            ss << ')';
            break;
        default:
            ss << "";
    }
    name = ss.str();
}
