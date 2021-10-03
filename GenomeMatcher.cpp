#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
using namespace std;

class GenomeMatcherImpl
{
  public:
    GenomeMatcherImpl(int minSearchLength);
    void addGenome(const Genome &genome);
    int minimumSearchLength() const;
    bool findGenomesWithThisDNA(const string &fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch> &matches) const;
    bool findRelatedGenomes(const Genome &query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch> &results) const;

  private:
    int m_minSearch;
    vector<Genome> m_genomeLib;
    Trie<DNAMatch> m_trie;

    //bool compareDNAMatch(DNAMatch m1, DNAMatch m2);
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength)
    : m_minSearch(minSearchLength)
{
}

void GenomeMatcherImpl::addGenome(const Genome &genome)
{
    string name;
    DNAMatch tempDNA;
    m_genomeLib.push_back(genome);

    for (int i = 0; i < genome.length(); i++)
    {
        if (genome.extract(i, m_minSearch, name))
        {
            tempDNA.genomeName = genome.name();
            tempDNA.position = i;
            tempDNA.length = m_minSearch;

            m_trie.insert(name, tempDNA);
        }
    }
}

int GenomeMatcherImpl::minimumSearchLength() const
{
    return m_minSearch;
}

bool compareDNAMatch(DNAMatch m1, DNAMatch m2)
{
    return m1.length < m2.length;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string &fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch> &matches) const
{
    if (fragment.size() < minimumLength || minimumLength < m_minSearch)
        return false;

    auto vOfDNAMatch = m_trie.find(fragment.substr(0, minimumLength), exactMatchOnly);   

    // if(vOfDNAMatch.empty())
    //     return false;

    for (DNAMatch dna : vOfDNAMatch)
    {
        vector<DNAMatch> tempMatches;
        for (Genome currGenome : m_genomeLib)
        {
            if (currGenome.name() == dna.genomeName)
            {
                string currDNA;
                if (!currGenome.extract(dna.position, fragment.size(), currDNA)) //) // no frags found
                    return false;
                if (!exactMatchOnly)
                {
                    bool oneMissMatchUsed = false;
                    for (int i = 0; i < fragment.size(); i++)
                    {
                        if (fragment[i] != currDNA[i] && !oneMissMatchUsed)
                        {
                            oneMissMatchUsed = true;
                            continue;
                        }
                        else if (fragment[i] == currDNA[i])
                            continue;
                        else
                        {
                            dna.length = i;
                            tempMatches.push_back(dna);
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < fragment.size(); i++)
                    {
                        if (fragment[i] == currDNA[i])
                            continue;
                        else
                        {
                            dna.length = i;
                            tempMatches.push_back(dna);
                        }
                    }
                }
            }
        }
        sort(tempMatches.begin(), tempMatches.end(), compareDNAMatch);
        matches.push_back(tempMatches[tempMatches.size()-1]);
    }

    return true;
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome &query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch> &results) const
{
    return false; // This compiles, but may not be correct
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome &genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string &fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch> &matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome &query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch> &results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
