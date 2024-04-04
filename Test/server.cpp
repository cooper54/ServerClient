#include <iostream>
#include <string>
#include <stack>
using namespace std;

class recognizer
{
public:
    int** matrix;
    int sizemat = 7;

    unsigned char* slovar;

    string helper;
    string st;

    char r[7];
    int fstate = 5;
    int addst[7];
    int secstate = 6;
    int slovarsiz = 3;


    recognizer(string tape);

    string action();
};

recognizer::recognizer(string tt)
{
    this->helper = tt;

    this->matrix = new int* [sizemat];

    this->matrix[0] = new int[3] {1, 2, 5};
    this->matrix[1] = new int[3] {1, 2, 3};
    this->matrix[2] = new int[3] {6, 2, 4};
    this->matrix[3] = new int[3] {6, 3, 6};
    this->matrix[4] = new int[3] {4, 3, 6};
    this->matrix[5] = new int[3] {5, 5, 5};
    this->matrix[6] = new int[3] {6, 6, 6};

    this->slovar = new unsigned char[slovarsiz];

    this->slovar[0] = 'a';
    this->slovar[1] = 'b';
    this->slovar[2] = '0';

    addst[0] = 0;
    addst[1] = 1;
    addst[2] = 1;
    addst[3] = 0;
    addst[4] = 0;
    addst[5] = 0;
    addst[6] = 0;

    r[0] = ' ';
    r[1] = ' ';
    r[2] = ' ';
    r[3] = 'a';
    r[4] = 'b';
    r[5] = ' ';
    r[6] = ' ';
}

string recognizer::action()
{
    string res = "";

    int newst = 0, newres = 0, i = 0, j = 0;

    for (i = 0; i < helper.length(); i++)
    {
        int slovarind = -1;

        for (j = 0; j < slovarsiz; j++)
        {
            if (helper[i] == slovar[j])
            {
                slovarind = j;

                j = slovarsiz;
            }
        }

        if (slovarind != -1)
        {
            newst = matrix[newst][slovarind];

            if (this->addst[newst] && helper[i] != '0')
                st = helper[i] + st;

            else if (this->r[newst] != ' ' && helper[i] != '0')
            {
                if ((st[0] == this->r[newst]) && (st.length() != 0))
                    st = st.substr(1);
                else if (res.length() == 0)
                    res = "No, error in position " + to_string(i + 1) + " = " + helper[i];
            }
        }

        if ((newst == secstate) && (res.length() == 0))
            res = "No, error in position " + to_string(i + 1) + " = " + helper[i];
    }

    if ((st.length() == 0) && (res.length() == 0))
    {
        newres = fstate;

        res = "Yes";
    }
    else if (res.length() == 0)
        res = "No, error in position " + to_string(helper.length() + 1) + " = ";
    else
        newres = secstate;

    return res;
}

bool recognize(const std::string& input, int& errorPos)
{
    std::stack<int> stackA, stackB;
    int state = 0;
    int countA = 0, countB = 0;
    for (int i = 0; i < input.size(); ++i)
    {
        char ch = input[i];
        switch (state)
        {
        case 0:
            if (ch == 'a')
            {
                countA++;
            }
            else if (ch == 'b')
            {
                state = 1;
                stackA.push(countA);
                countA = 0;
            }
            else
            {
                errorPos = i;
                return false;
            }
            break;
        case 1:
            if (ch == 'b')
            {
                countB++;
            }
            else if (ch == '0')
            {
                state = 2;
                stackB.push(countB);
                countB = 0;
            }
            else
            {
                errorPos = i;
                return false;
            }
            break;
        case 2:
            if (ch == 'a')
            {
                countA++;
                if (countA > stackB.top())
                {
                    errorPos = i;
                    return false;
                }
            }
            else if (ch == 'b')
            {
                state = 3;
                stackB.pop();
            }
            else
            {
                errorPos = i;
                return false;
            }
            break;
        case 3:
            if (ch == 'b')
            {
                countB++;
                if (countB > stackA.top())
                {
                    errorPos = i;
                    return false;
                }
            }
            else
            {
                errorPos = i;
                return false;
            }
            break;
        }
    }
    return stackA.empty() && stackB.empty() && state == 3;
}

int main()
{
    string tapeInput;
    cin >> tapeInput;
    recognizer Automaton = recognizer(tapeInput);

    cout << Automaton.action();
}