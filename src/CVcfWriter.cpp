//
//  CVcfWriter.cpp
//  VCFComparison
//
//  Created by Berke.Toptas on 12/16/16.
//  Copyright © 2016 Seven Bridges Genomics. All rights reserved.
//

#include "CVcfWriter.h"
#include <ctime>
#include <iostream>


CVcfWriter::CVcfWriter()
{
    m_HEADER_GUARD = 0;
}

void CVcfWriter::CreateVcf(const char* a_pFileName)
{
    m_pHtsFile = hts_open(a_pFileName,"w");
    m_pRecord  = bcf_init1();
}

void CVcfWriter::CloseVcf()
{
    bcf_destroy1(m_pRecord);
    bcf_hdr_destroy(m_pHeader);
    int ret= hts_close(m_pHtsFile);
    if(ret != 0)
    {
        std::cerr << "A problem occured with saving the VCF file." << std::endl;
    }
}

void CVcfWriter::AddHeaderLine(const std::string& a_rLine)
{
    bcf_hdr_append(m_pHeader, a_rLine.c_str());
}

void CVcfWriter::InitHeader()
{
    if(m_HEADER_GUARD == 0)
    {
        //Initialize header for write
        m_pHeader = bcf_hdr_init("w");
        //Add file date
        bcf_hdr_append(m_pHeader, ("##fileDate=" + GetTime()).c_str());
        m_HEADER_GUARD = 1;
    }
    else
        std::cerr << "Invalid Operation. Header cannot be initialized" << std::endl;

}


void CVcfWriter::WriteHeaderToVcf()
{
    if(m_HEADER_GUARD == 1)
    {
        bcf_hdr_write(m_pHtsFile, m_pHeader);
        m_HEADER_GUARD = -1;
    }
    else
        std::cerr << "Invalid Operation. Header cannot be writtten into file" << std::endl;
    
}

void CVcfWriter::AddSampleName(const std::string &a_rSampleName)
{
    if(m_HEADER_GUARD == 1)
        bcf_hdr_add_sample(m_pHeader, a_rSampleName.c_str());
    else
        std::cerr << "Invalid Operation. Cannot add sample name" << std::endl;
}


void CVcfWriter::AddRecord(const SVcfRecord& a_rVcfRecord)
{
    if(m_HEADER_GUARD != -1)
    {
        std::cerr << "Invalid Operation. Cannot add record before submitting header" << std::endl;
        return;
    }
    
    //Clear previous record
    bcf_clear1(m_pRecord);
    
    //Set chromosome name
    m_pRecord->rid = bcf_hdr_name2id(m_pHeader, a_rVcfRecord.m_chrName.c_str());
    
    //Set start position
    m_pRecord->pos = a_rVcfRecord.m_nPosition;
    
    //Set Quality
    m_pRecord->qual = a_rVcfRecord.m_nQuality;
    
    //Set alleles
    bcf_update_alleles_str(m_pHeader, m_pRecord, a_rVcfRecord.m_alleles.c_str());
    
    //==Set Per Sample Data==

    //1.Genotype Set (GT)
    std::vector<int> genotypes;
    for(int k = 0; k < a_rVcfRecord.m_aSampleData.size(); k++)
    {
        for(int p = 0; p < a_rVcfRecord.m_aSampleData[k].m_nHaplotypeCount; p++)
        {
            if(a_rVcfRecord.m_aSampleData[k].m_bIsPhased)
                genotypes.push_back(bcf_gt_phased(a_rVcfRecord.m_aSampleData[k].m_aGenotype[0]));
            else
                genotypes.push_back(bcf_gt_unphased(a_rVcfRecord.m_aSampleData[k].m_aGenotype[0]));
        }
        
    }
    bcf_update_genotypes(m_pHeader, m_pRecord, static_cast<int*>(&genotypes[0]), static_cast<int>(genotypes.size()));

    //2.Decision Set (BD)
    char* tmpstr[a_rVcfRecord.m_aSampleData.size()];
    for(int k = 0; k < a_rVcfRecord.m_aSampleData.size(); k++)
    {
        tmpstr[k] = new char[a_rVcfRecord.m_aSampleData[k].m_decisionBD.size()];
        strcpy(tmpstr[k], a_rVcfRecord.m_aSampleData[k].m_decisionBD.c_str());
    }
    bcf_update_format_string(m_pHeader, m_pRecord, "BD", (const char**)tmpstr, static_cast<int>(a_rVcfRecord.m_aSampleData.size()));
    

    //3.Match Type Set (BK)
    char* tmpstr2[a_rVcfRecord.m_aSampleData.size()];
    for(int k = 0; k < a_rVcfRecord.m_aSampleData.size(); k++)
    {
        tmpstr2[k] = new char[a_rVcfRecord.m_aSampleData[k].m_matchTypeBK.size()+1];
        strcpy(tmpstr2[k], a_rVcfRecord.m_aSampleData[k].m_matchTypeBK.c_str());
    }
    bcf_update_format_string(m_pHeader, m_pRecord, "BK", (const char**)tmpstr2, static_cast<int>(a_rVcfRecord.m_aSampleData.size()));

 
    //Write record to created VCF File
    bcf_write1(m_pHtsFile, m_pHeader, m_pRecord);
    
    
    //Clean Temporary strings we used
    for(int k = 0; k < a_rVcfRecord.m_aSampleData.size(); k++)
    {
        delete[] tmpstr[k];
        delete[] tmpstr2[k];
    }
    
    
}


std::string CVcfWriter::GetTime()
{
    std::tm* timeInfo;
    char buffer[16];
    
    std::time_t currentTime;
    
    time(&currentTime);
    timeInfo = localtime(&currentTime);

    strftime(buffer,16,"%Y%m%d",timeInfo);
    
    return std::string(buffer);
}































