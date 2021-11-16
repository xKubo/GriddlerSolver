// GriddlerSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "Grid.h"
#include "HTMLOutput.h"
#include "Utils.h"

using namespace Grid;
using namespace HTML;

CTable CreateFromGrid(const CGrid& g)
{
    CExtents e = g.Extents();

    

    int MaxVert = FindMaxNumbersCount(g.Vertical());
    int MaxHoriz = FindMaxNumbersCount(g.Horizontal());

    e.W += MaxHoriz;
    e.H += MaxVert;
    
    int W = e.W;

    CElements es(e.W * e.H);

    const auto& vs = g.Vertical();
    for (int i = 0; i < vs.size(); ++i)
    {
        int Empty = vs[i].size() - MaxVert;
        for (int j = 0; j < vs[i].size(); ++j)
        {
            es[MaxHoriz + i + (j+Empty) * W].Text = std::to_string(vs[i][j]);
        }
    }

    const auto& hs = g.Horizontal();
    for (int i = 0; i < hs.size(); ++i)
    {
        int Empty = vs[i].size() - MaxHoriz;
        for (int j = 0; j < vs[i].size(); ++j)
        {
            es[j + Empty + (MaxVert + i) * W].Text = std::to_string(vs[i][j]);
        }
    }


    CTable t{ g.Extents(),  es };

    return t;
}

using namespace std;
using namespace Utils;

int main()
{

    try
    {
        std::ifstream i("in.txt"), HTMLTemplate("temp.html");
        Check(i.good(), "Input file error");
        Check(HTMLTemplate.good(), "Template file error");
        std::ofstream of("out.html");
        Check(of.good(), "Output file error");
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

