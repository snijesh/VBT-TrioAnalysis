#include "CFastaReader.h"
#include <iostream>


CFastaReader::CFastaReader()
{
    m_bIsOpen = false;
    m_fastaFile = 0;
}

CFastaReader::CFastaReader(const char * a_pFileName)
{
    m_bIsOpen = false;   
    m_fastaFile = 0;
    Open(a_pFileName); 
}

CFastaReader::~CFastaReader()
{
    //Close();
}
        
bool CFastaReader::Open(const char *a_pFileName)
{

    m_fileName = std::string(a_pFileName);

    m_fastaFile = gzopen(m_fileName.c_str(), "r"); 
    if(m_fastaFile == 0) 
    {
        std::cout<<"Unable to Open FASTA file"<<std::endl;
        return false;
    }
    m_bIsOpen = true;

    m_pContig = kseq_init(m_fastaFile); 

    return true;
}

void CFastaReader::ReadContig()
{
    kseq_read(m_pContig);
}

void CFastaReader::PrintContig()
{
    //printing id
    std::cout << m_pContig->name.s << std::endl;
    //printing seq
    std::cout << m_pContig->seq.s << std::endl;
}

char* CFastaReader::GetRefSeq()
{
    return m_pContig->seq.s;
}

int CFastaReader::GetRefSeqSize()
{
    return (int)strlen(m_pContig->seq.s);
}

bool CFastaReader::Close()
{
    kseq_destroy(m_pContig);
    gzclose(m_fastaFile);
    m_bIsOpen = false;
    return true;
}


