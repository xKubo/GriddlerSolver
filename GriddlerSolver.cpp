// GriddlerSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "Grid.h"
#include "HTMLOutput.h"
#include "Utils.h"
#include "Solver.h"

using namespace Grid;
using namespace HTML;
using Utils::IntSize;

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
    for (int i = 0; i < IntSize(vs); ++i)
    {
        int Empty = MaxVert - vs[i].size();
        for (int j = 0; j < IntSize(vs[i]); ++j)
        {
            es[MaxHoriz + i + (j+Empty) * W].Text = std::to_string(vs[i][j]);
        }
    }
    

    const auto& hs = g.Horizontal();
    for (int i = 0; i < IntSize(hs); ++i)
    {
        int Empty = MaxHoriz - IntSize(hs[i]);
        for (int j = 0; j < IntSize(hs[i]); ++j)
        {
            es[j + Empty + (MaxVert + i) * W].Text = std::to_string(hs[i][j]);
        }
    }

    static const int GridLine = 5;

    for (int i = MaxVert; i < e.H; i += GridLine)
    {
        for (int j = 0; j < e.W; j++)
            AddFlag(es[i*e.W + j].Borders, CBorders::Top);
    }

    for (int i = MaxHoriz; i < e.W; i += GridLine)
    {
        for (int j = 0; j < e.H; j++)
            AddFlag(es[j * e.W + i].Borders, CBorders::Left);
    }

    CTable t{ e,  es };

    return t;
}

using namespace std;
using namespace Utils;

int main()
{

    try
    {
        std::ifstream i("in3.txt"), HTMLTemplate("template.htm");
        Check(i.good(), "Input file error");
        Check(HTMLTemplate.good(), "Template file error");

        std::ofstream of("out.html");
        Check(of.good(), "Output file error");

        std::string sHTMLTemplate = LoadAllFromStream(HTMLTemplate);
        CHTMLOutput ho(sHTMLTemplate);

        try
        {

            CGrid g = LoadFromStream(i);
            CTable t = CreateFromGrid(g);
            ho.AddTable("Unsolved", t);
            CGrid gSolved = Solver::SolveGrid(g);
            CTable tSolved = CreateFromGrid(gSolved);
            ho.AddTable("Solved", tSolved);
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

