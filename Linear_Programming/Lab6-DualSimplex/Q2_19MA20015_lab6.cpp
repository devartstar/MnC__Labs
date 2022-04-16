/*
    Name: Gaddam Yogesh
    Roll No: 19MA20015
    Lab No: 6
    Question: 2
*/
// Menu driven program to solve a LPP using Dual simplex method
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <string.h>
#define nline "\n"

using namespace std;

typedef vector<int> VI;
typedef vector<float> VF;
typedef vector<VF> VVF;
typedef vector<string> VS;
typedef vector<bool> VB;

int n, m; // Global variables to store the dimensions of the simplex table
long long INT_MAX = 1e9;
long long INT_MIN = -1e9;

// Function to convert an integer to a sstring
string convert_to_string(int num);
// Function to print the simplex table
void print_table(VVF &coeff_mat, VS &basic, VS &nbasic);
// Function to perform an iteration on the simplex table
int simplex_method(VVF &coeff_mat, VS &basic, VS &nbasic);
// Function to take input fron the user and add it to the table
int take_input(VVF &coeff_mat, VS &basic, VS &nbasic);
// Function to check if the Dual simplex method can be used to find an ooptimal solution
bool checkDualFeasibility(string &type, VF &ObjFn);
// Function to convert all >= & = type into <= type
void modify_inequalitites(VVF &coeff_mat, VS &ineq, VF &RHS);
// Function to handle all the cases for the menu driven program
int solve(VVF coeff_mat, VS basic, VS nbasic);

// Function to convert an integer to a sstring
string convert_to_string(int num)
{
    string res = "";
    bool flag = false;
    if (num < 0)
    {
        num *= -1;
        flag = true;
    }
    while (num != 0)
    {
        res = char((num % 10) + '0') + res;
        num /= 10;
    }
    if (flag)
        res = '-' + res;
    return res;
}
// Function to print the simplex table
void print_table(VVF &coeff_mat, VS &basic, VS &nbasic)
{

    for (int i = 0; i < m - 1; i++)
    {
        cout << setw(10);
        cout << "-" + nbasic[i];
    }
    cout << setw(10) << "1";
    cout << setw(10) << "NBV/BV" << nline;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            cout << setw(10) << coeff_mat[i][j];
        cout << setw(10) << basic[i] << nline;
    }
}
// Function to perform an iteration on the simplex table
// returns 1 if iteration is successful
// returns 0 if optimal solution is reached
// returns -1 if infeasible
int simplex_method(VVF &coeff_mat, VS &basic, VS &nbasic)
{
    cout << "\nObjective value: ";
    if (basic[n - 1][0] == '-')
        cout << basic[n - 1].substr(1) << " = " << -1 * coeff_mat[n - 1][m - 1];
    else
        cout << basic[n - 1] << " = " << coeff_mat[n - 1][m - 1];
    cout << nline << nline;
    // checking for the largest negative number to get the required row
    float min = INT_MAX;
    int row = 0;
    for (int i = 0; i < n - 1; i++)
    {
        if (coeff_mat[i][m - 1] < min) /////////////////////////////////////
        {
            min = coeff_mat[i][m - 1];
            row = i;
        }
    }
    if (coeff_mat[row][m - 1] >= 0)
    {
        cout << "Since all XBi >= 0, an optimal solution is reached.\n";
        return 0;
    }
    else
    {
        cout << "Minimum negative XB is " << coeff_mat[row][m - 1] << " and its row index is " << row + 1 << ".\n";
        cout << "So, the leaving variable is " << basic[row] << ".\n\n";
        // checking for the row with the pivot element
        VF ratios;
        float max = INT_MIN;
        int col = -1;
        for (int i = 0; i < m - 1; i++)
        {
            if (coeff_mat[row][i] < 0)
            {
                float rat = coeff_mat[n - 1][i] / coeff_mat[row][i];
                if (rat > 0)
                    continue;
                ratios.push_back(rat);
                if (rat > max)
                {
                    max = rat;
                    col = i;
                }
            }
        }
        // if no negative ratios found => unbounded
        if (ratios.size() == 0)
        {
            cout << "Since all ratios are positive, the solution is unbouded.\n";
            return -1;
        }
        else
        {
            cout << "Maximum negative ratio is " << coeff_mat[n - 1][col] / coeff_mat[row][col] << " and its column index is " << col + 1 << ".\n";
            cout << "So, the entering variable is " << nbasic[col] << ".\n\n";
            // swapping the non basic and basic variables
            swap(nbasic[col], basic[row]);
            float pivot_val = coeff_mat[row][col];
            coeff_mat[row][col] = 1 / coeff_mat[row][col];

            // s* = (ps-qr)/p
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < m; j++)
                {
                    if (i != row && j != col)
                    {
                        coeff_mat[i][j] = (coeff_mat[i][j] * pivot_val - coeff_mat[i][col] * coeff_mat[row][j]) / pivot_val;
                    }
                }
            }

            // modifying the column elements
            for (int i = 0; i < n; i++)
            {
                if (i != row)
                    coeff_mat[i][col] = -1 * coeff_mat[i][col] / pivot_val;
            }

            // modifying the row elements
            for (int i = 0; i < m; i++)
            {
                if (i != col)
                    coeff_mat[row][i] = coeff_mat[row][i] / pivot_val;
            }
        }
    }
    print_table(coeff_mat, basic, nbasic);

    return 1;
}
// Function to take input fron the user and add it to the table
int take_input(VVF &coeff_mat, VS &basic, VS &nbasic)
{
    // number of variables and equations
    cout << "Enter the number of variables: ";
    cin >> m;
    cout << "Enter the number of inequalities: ";
    cin >> n;
    coeff_mat.resize(n, VF(m + 1));
    // Taking the objective of the problem, minimize or maximize
    string type;
    cout << "Provide the objective (min/max): ";
    cin >> type;
    if (type != "min" && type != "max")
    {
        cout << "INVALID type!!";
        return -1;
    }

    // taking input for objective function
    float temp;
    VF ObjFn(m);
    float SOL = 0;
    cout << "Enter the coefficients of the objective function:\n";
    for (int i = 0; i < m; i++)
    {
        cout << "coefficient of x" << i + 1 << " : ";
        cin >> ObjFn[i];
    }

    // taking inputs for the inequalitites
    VS ineq(n);
    VF RHS(n);
    cout << "\nProvide the conditions :\n\n";
    for (int i = 0; i < n; i++)
    {
        cout << "Condition #" << i + 1 << ":\n";
        for (int j = 0; j < m; j++)
        {
            cout << "coefficient of x" << j + 1 << " : ";
            cin >> coeff_mat[i][j];
        }
        cout << "inequality (>=, = , <=): ";
        cin >> ineq[i];
        cout << "RHS: ";
        cin >> RHS[i];
        cout << nline;
    }

    // Printing the given input
    // printing the objective function
    cout << "Given Objective function:\n"
         << type << ": Z = ";
    if (ObjFn[0] != 0)
        cout << ObjFn[0] << "(x1"
             << ") ";
    for (int i = 1; i < m; i++)
    {
        if (ObjFn[i] != 0)
        {
            if (ObjFn[i] < 0)
                cout << "- " << -1 * ObjFn[i] << "(x" << i + 1 << ") ";
            else
                cout << "+ " << ObjFn[i] << "(x" << i + 1 << ") ";
        }
    }

    // printing the conditions
    cout << "Given conditions are:\n";
    for (int i = 0; i < n; i++)
    {
        cout << i + 1 << ") ";
        if (coeff_mat[i][0] != 0)
            cout << coeff_mat[i][0] << "(x" << 1 << ") ";
        for (int j = 1; j < m; j++)
            if (coeff_mat[i][j] != 0)
            {
                if (coeff_mat[i][j] < 0)
                    cout << "- " << -1 * coeff_mat[i][j] << "(x" << j + 1 << ") ";
                else
                    cout << "+ " << coeff_mat[i][j] << "(x" << j + 1 << ") ";
            }
        cout << ineq[i] << " " << RHS[i] << nline;
    }

    // checking if the method works for the given problem
    if (!checkDualFeasibility(type, ObjFn))
    {
        cout << "Dual simplex method fails to get the optimal solution.\n";
        return -1;
    }

    // converting all >= inequalities into <= by multiplying them by -1
    modify_inequalitites(coeff_mat, ineq, RHS);
    coeff_mat.resize(n + 1, VF(m + 1));

    // storing the variable names
    for (int i = 0; i < m; i++)
        nbasic.push_back("x" + convert_to_string(i + 1));

    // combining all the components to make the final table
    for (int i = 0; i < n; i++)
    {
        coeff_mat[i][m] = RHS[i];
        basic.push_back("s" + convert_to_string(i + 1));
    }
    if (type == "max")
        basic.push_back("Z");
    else
        basic.push_back("-Z");

    ObjFn.push_back(SOL);
    m++;
    for (int i = 0; i < m; i++)
    {
        if (type == "max")
            coeff_mat[n][i] = -1 * ObjFn[i];
        else
            coeff_mat[n][i] = ObjFn[i];
    }
    n++;
    RHS.clear();
    ObjFn.clear();
    ineq.clear();
    return 0;
}
// Function to check if the Dual simplex method can be used to find an ooptimal solution
bool checkDualFeasibility(string &type, VF &ObjFn)
{
    if (type == "max")
    {
        for (int i = 0; i < m; i++)
            if (ObjFn[i] > 0)
                return false;
    }
    else
    {
        for (int i = 0; i < m; i++)
            if (ObjFn[i] < 0)
                return false;
    }
    return true;
}
// Function to convert all >= & = type into <= type
void modify_inequalitites(VVF &coeff_mat, VS &ineq, VF &RHS)
{
    for (int i = 0; i < n; i++)
    {
        if (ineq[i] == ">=")
        {
            for (int j = 0; j < m; j++)
                coeff_mat[i][j] *= -1;
            RHS[i] *= -1;
            ineq[i] = "<=";
        }
        else if (ineq[i] == "=")
        {
            ineq[i] = "<=";
            coeff_mat.resize(n + 1, VF(m + 1));
            for (int j = 0; j < m; j++)
            {
                coeff_mat[n][j] = -1 * coeff_mat[i][j];
            }
            RHS.push_back(-1 * RHS[i]);
            ineq.push_back("<=");
            n++;
        }
    }
}
// Function to handle all the cases for the menu driven program
int solve(VVF coeff_mat, VS basic, VS nbasic)
{
    cout << "------------------------------------------------------------\n";
    cout << "\nI - Initial table: \n";
    print_table(coeff_mat, basic, nbasic);
    cout << "------------------------------------------------------------\n\n";
    cout << "II : \n\n";
    // For querys not related to the iterations
    int res = 1;
    int iter = 1;
    while (res != 0)
    {
        cout << "\nIteration #" << iter << nline;
        iter++;
        print_table(coeff_mat, basic, nbasic);
        res = simplex_method(coeff_mat, basic, nbasic);
        cout << "\n------------------------------------------------------------\n";
        if (res == -1)
            return -1;
    }
    cout << "III - Final solution, Optimal value of the problem:\n";
    if (basic[n - 1][0] == '-')
        cout << basic[n - 1].substr(1) << " = " << -1 * coeff_mat[n - 1][m - 1];
    else
        cout << basic[n - 1] << " = " << coeff_mat[n - 1][m - 1];
    cout << nline;
    for (int i = 0; i < n - 1; i++)
    {
        if (basic[i][0] == 'x')
            cout << basic[i] << "=" << coeff_mat[i][m - 1] << " ";
    }
    for (auto e : nbasic)
    {
        if (e[0] == 'x')
            cout << e << "=0 ";
    }
    cout << "\n------------------------------------------------------------\n";
    return 0;
}

int main()
{
    // taking user input
    VVF coeff_mat;
    VS basic, nbasic;
    if (take_input(coeff_mat, basic, nbasic) == -1)
        return -1;

    solve(coeff_mat, basic, nbasic);
}
