// GriddlerSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>

#include "Grid.h"
#include "HTMLOutput.h"

using namespace Grid;
using namespace HTML;

CTable CreateFromGrid(const CGrid& g)
{
    return {};
}

using namespace std;

int main()
{

    try
    {
        std::ifstream i("in.txt"), HTMLTemplate("temp.html");
        std::ofstream of("out.html");
        CHTMLOutput ho(HTMLTemplate);

        try
        {

            CGrid g = LoadFromStream(i);
            CTable t = CreateFromGrid(g);
            ho.AddTable("Unsolved", t);
        }
        catch (const std::exception &e)
        {
            ho.AddError(e.what());
        }
        ho.OutputToStream(of);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

