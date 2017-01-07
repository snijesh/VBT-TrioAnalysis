//
//  CResultLog.cpp
//  VCFComparison
//
//  Created by Berke.Toptas on 12/29/16.
//  Copyright © 2016 Seven Bridges Genomics. All rights reserved.
//

#include "CResultLog.h"
#include <fstream>


void CResultLog::SetLogPath(const std::string& a_rLogFolder)
{
    m_aLogPath = a_rLogFolder + "/log.txt";
}


//Records the result for given chromosome
void CResultLog::LogStatistic(int a_nChromosomeId, int a_nTpCalled, int a_nTpBaseline, int a_nHalfTPCalled, int a_nHalfTPBaseline, int a_nFalsePositive, int a_nFalseNegative)
{
    m_aResultEntries[a_nChromosomeId].m_nTpCalled = a_nTpCalled;
    m_aResultEntries[a_nChromosomeId].m_nTpBase = a_nTpBaseline;
    m_aResultEntries[a_nChromosomeId].m_nHalfTpCalled = a_nHalfTPCalled;
    m_aResultEntries[a_nChromosomeId].m_nHalfTpBase = a_nHalfTPBaseline;
    m_aResultEntries[a_nChromosomeId].m_nFp = a_nFalsePositive;
    m_aResultEntries[a_nChromosomeId].m_nFn = a_nFalseNegative;
    m_aResultEntries[a_nChromosomeId].m_bIsNull = false;
}

//Write the results in log.txt file
void CResultLog::WriteStatistics()
{
    
    std::ofstream outputLog;
    outputLog.open(m_aLogPath);
    
    outputLog << "ChromosomeID" << "\t" << "True-Pos-Called" << "\t" << "True-Pos-Baseline" << "\t" << "False-Pos" << "\t" << "False-Neg" << "\t" << "Precision" << "\t" << "Recall";
    outputLog << "\t" << "F-measure" << std::endl;

    for(int k = 0; k < 25; k++)
    {
        if(m_aResultEntries[k].m_bIsNull)
            continue;
        
        if(k > 22)
            outputLog << "Chr" << char(120 + k - 22);
        else
            outputLog << "Chr" << k;
        
        outputLog << "\t" << m_aResultEntries[k].m_nTpCalled << "\t" << m_aResultEntries[k].m_nTpBase << "\t" << m_aResultEntries[k].m_nFp + m_aResultEntries[k].m_nHalfTpCalled;
        outputLog << "\t" << m_aResultEntries[k].m_nHalfTpBase + m_aResultEntries[k].m_nFn;
        
        double Precision = static_cast<double>(m_aResultEntries[k].m_nTpBase) / static_cast<double>(m_aResultEntries[k].m_nTpBase + m_aResultEntries[k].m_nFp);
        double Recall = static_cast<double>(m_aResultEntries[k].m_nTpBase) / static_cast<double>(m_aResultEntries[k].m_nTpBase + m_aResultEntries[k].m_nFn);
        double Fmeasure = Precision + Recall == 0.0 ? 0.0 : (2.0 * Precision * Recall) / (Precision + Recall);
    
        outputLog.precision(4);
        outputLog << "\t" << Precision << "\t" << Fmeasure << std::endl;
    }
}

