#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
#include <fstream>
#include <cctype>
using namespace std;

class GenomeImpl
{
  public:
    GenomeImpl(const string &nm, const string &sequence);
    static bool load(istream &genomeSource, vector<Genome> &genomes);
    int length() const;
    string name() const;
    bool extract(int position, int length, string &fragment) const;

  private:
    string m_name, m_sequence;

};

GenomeImpl::GenomeImpl(const string &nm, const string &sequence)
    : m_name(nm), m_sequence(sequence)
{
}

bool GenomeImpl::load(istream &genomeSource, vector<Genome> &genomes)
{
    string curr;
    string name;
    string sequence;

    getline(genomeSource, curr);
    if (curr[0] != '>' || curr[1] == ' ')
        return false;
    if (curr[0] == '>')
        name = curr.substr(1);

    while (getline(genomeSource, curr)) //read in each line
    {
        if (curr[0] == '>' && name == "")
        {
            if (curr[1] == ' ' || sequence == "") // if the
                return false;

            name = curr.substr(1);
            genomes.push_back(Genome(name, sequence));

            name = "";
            sequence = "";
            continue;
        }
        if (curr[0] != '>' && name != "") // already is a name
        {
            for (char ch : curr) // check if ever char is valid
            {
                toupper(ch);
                if (ch != 'A' && ch != 'T' && ch != 'T' && ch != 'G' && ch != 'N')
                    return false;
            }
            sequence += curr;
        }
    }

    return true;
}

int GenomeImpl::length() const { return m_sequence.size(); }

string GenomeImpl::name() const { return m_name; }

bool GenomeImpl::extract(int position, int length, string &fragment) const
{
    if (position + length > m_sequence.size() || (position < 0 || length < 0))
        return false;

    fragment = m_sequence.substr(position, length);
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string &nm, const string &sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome &other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome &Genome::operator=(const Genome &rhs)
{
    GenomeImpl *newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream &genomeSource, vector<Genome> &genomes)
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string &fragment) const
{
    return m_impl->extract(position, length, fragment);
}
