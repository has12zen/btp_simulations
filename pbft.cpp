#include <bits/stdc++.h>
#include <chrono>
#include <random>

using namespace std;

enum state
{
    BASE,
    PRE_PREPARED,
    PREPARED,
    COMMITED
};

void clearLogs(int idx)
{
    ofstream ofs;
    string filename = "logs/" + to_string(idx) + ".log";
    ofs.open(filename, ofstream::out | ofstream::trunc);
    ofs.close();
}
void clearDecisions(int idx)
{
    ofstream ofs;
    string filename = "decisions/" + to_string(idx) + ".txt";
    ofs.open(filename, ofstream::out | ofstream::trunc);
    ofs.close();
}
bool getRandomDecision()
{
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    bool decision = rng() % 2;
    return decision;
}

void logFile(int idx, string s)
{
    ofstream myfile;
    string filename = "logs/" + to_string(idx) + ".log";
    myfile.open(filename, ios::app);
    myfile << s << endl;
    myfile.close();
}
void logDecision(int idx, string s)
{
    ofstream myfile;
    string filename = "decisions/" + to_string(idx) + ".txt";
    myfile.open(filename, ios::app);
    myfile << s << endl;
    myfile.close();
}

string printDec(int dest, bool dec, enum state s, string request = "")
{
    string str = "";
    str += request;
    str += " sentBy: " + to_string(dest) + " Dec: " + to_string(dec) + " State: ";
    if (s == BASE)
        str += "BASE";
    else if (s == PRE_PREPARED)
        str += "PRE_PREPARED";
    else if (s == PREPARED)
        str += "PREPARED";
    else if (s == COMMITED)
        str += "COMMITED";
    return str;
}
class Peer
{
private:
    int index;
    bool isActive;
    bool isMalicious;
    enum state st;
    bool decision;

public:
    vector<bool> otherDec;
    vector<bool> commits;
    int count_commit;
    int count_updates;
    bool orignalDecision;
    Peer(int index, bool isActive, bool isMalicious)
    {
        this->index = index;
        this->isActive = isActive;
        this->isMalicious = isMalicious;
    }
    void roundReset()
    {
        st = BASE;
        decision = false;
        otherDec.clear();
        commits.clear();
        count_commit = 0;
        count_updates = 0;
    }
    void ResetCommit()
    {
        this->count_commit = 0;
    }
    void ResetCount()
    {
        count_updates = 0;
    }
    void clearOtherDec()
    {
        otherDec.clear();
    }
    void set_decision(bool decision)
    {
        this->decision = decision;
    }
    void set_state(enum state st)
    {
        this->st = st;
    }
    bool get_decision()
    {
        return this->decision;
    }
    enum state get_state()
    {
        return this->st;
    }
    int getIndex()

    {
        return this->index;
    }
    bool getIsActive()
    {
        return this->isActive;
    }
    bool getIsMalicious()
    {
        return this->isMalicious;
    }
    void setIsActive(bool isActive)
    {
        this->isActive = isActive;
    }
    void setIsMalicious(bool isMalicious)
    {
        this->isMalicious = isMalicious;
    }
};

class Network
{
private:
    vector<Peer> peers;
    int n;
    int master = 0;
    int m_limit = 0;

public:
    Network(vector<Peer> peers)
    {
        this->peers = peers;
        this->n = peers.size();
        m_limit = (n - 1) / 3;
    }
    bool doPrePrepare(int ridx, bool dec)
    {
        bool isMalicious = peers[master].getIsMalicious();
        bool isActive = peers[master].getIsActive();
        if (!isActive)
        {
            cout << "Master is inactive" << endl;
            return false;
        }
        peers[master].set_state(PRE_PREPARED);
        for (int i = 0; i < n; i++)
        {
            // normal mode of operation
            bool node_dec = dec;
            peers[i].clearOtherDec();
            peers[i].ResetCount();
            peers[i].count_updates++;
            if (!isMalicious && i != master)
            {
                peers[i].set_state(PRE_PREPARED);
                peers[i].set_decision(dec);
                peers[i].orignalDecision = dec;
                peers[i].otherDec.push_back(dec);
            }
            // malicious mode
            else if (isMalicious && i != master)
            {
                bool random_decision = getRandomDecision();
                bool random_state = getRandomDecision();
                // if (random_state)
                peers[i].set_state(PRE_PREPARED);
                peers[i].set_decision(!random_decision);
                peers[i].orignalDecision = !random_decision;
                peers[i].otherDec.push_back(!random_decision);
                node_dec = !random_decision;
            }
            else
            {
                // node is master
                if (isMalicious)
                {
                    bool random_decision = getRandomDecision();
                    peers[i].set_decision(!random_decision);
                    peers[i].otherDec.push_back(!random_decision);
                    peers[i].orignalDecision = !random_decision;
                    node_dec = !random_decision;
                }
                else
                {
                    peers[i].set_decision(dec);
                    peers[i].orignalDecision = dec;
                    peers[i].otherDec.push_back(dec);
                }
            }
            string s = printDec(master, node_dec, peers[i].get_state(), "PrePrepare");
            logFile(i, s);
        }
        return true;
    }
    void doPrepare()
    {
        for (int i = 0; i < n; i++)
        {
            if (i == master)
            {
                // peers[i].otherDec.push_back(peers[i].get_decision());
                // as master does not send prepare statement to even itself
                continue;
            }
            state st = peers[i].get_state();
            if (st == PRE_PREPARED)
            {
                bool isMalicious = peers[i].getIsMalicious();
                bool isActive = peers[i].getIsActive();
                if (!isActive)
                {
                    continue;
                }
                for (int j = 0; j < n; j++)
                {
                    peers[j].count_updates++;
                    bool node_dec = peers[i].get_decision();
                    if (i != j)
                    {
                        bool random_state = getRandomDecision();
                        bool random_decision = getRandomDecision();
                        if (!isMalicious)
                        {
                            peers[j].otherDec.push_back(peers[i].get_decision());
                        }
                        else
                        {
                            peers[j].otherDec.push_back(!random_decision);
                            node_dec = !random_decision;
                        }
                    }
                    else
                    {
                        // its own decision
                        peers[j].otherDec.push_back(peers[i].get_decision());
                    }
                    string s = printDec(i, node_dec, peers[i].get_state(), "Prepare");
                    logFile(j, s);
                }
            }
        }
        // each node counts the number of times it has received a PRE_PREPARED message
        int cutoff = m_limit * 2 + 1;
        for (int i = 0; i < n; i++)
        {
            int count = 0;
            string s = "";
            bool dec = peers[i].get_decision();
            for (int j = 0; j < peers[i].otherDec.size(); j++)
            {
                s += to_string(peers[i].otherDec[j]) + " ";
                if (peers[i].otherDec[j] == dec)
                {
                    count++;
                }
            }
            logFile(i, "Count: " + to_string(count) + " cutoff: " + to_string(cutoff) + ", " + s + " dec: " + to_string(dec));
            if (count >= cutoff)
            {
                peers[i].set_state(PREPARED);
            }
        }
    }
    void doCommit()
    {
        for (int i = 0; i < n; i++)
        {
            peers[i].ResetCommit();
        }
        for (int i = 0; i < n; i++)
        {
            state st = peers[i].get_state();
            if (st == PREPARED)
            {
                bool isMalicious = peers[i].getIsMalicious();
                bool isActive = peers[i].getIsActive();
                if (!isActive)
                {
                    continue;
                }
                for (int j = 0; j < n; j++)
                {
                    peers[j].count_commit++;
                    bool dec = peers[i].get_decision();
                    bool isMalicious = peers[i].getIsMalicious();
                    if (isMalicious)
                    {
                        dec = getRandomDecision();
                    }
                    peers[j].commits.push_back(dec);
                    string s = printDec(i, dec, peers[i].get_state(), "Commit");
                    logFile(j, s);
                }
            }
        }
        // each node counts the number of times it has received a commit message
        int cutoff = m_limit * 2 + 1;
        int majority = m_limit + 1;
        for (int i = 0; i < n; i++)
        {
            map<bool, int> freq;
            int sum = peers[i].commits.size();
            for (int j = 0; j < peers[i].commits.size(); j++)
            {
                freq[peers[i].commits[j]]++;
            }
            for (auto it : freq)
            {
                if (sum >= cutoff && it.second >= majority)
                {
                    peers[i].set_state(COMMITED);
                    peers[i].set_decision(it.first);
                }
            }
        }
    }
    string violatesG1()
    {
        // check if all nonmalicious nodes have same decision
        // agreement check
        string s = "";
        string t = "";
        set<bool> decsize;
        map<bool, vector<int>> decmap;
        bool iscorrect = true;
        for (int i = 0; i < n; i++)
        {
            if (!peers[i].getIsMalicious())
            {
                if (peers[i].get_state() == COMMITED)
                {
                    decsize.insert(peers[i].get_decision());
                    decmap[peers[i].get_decision()].push_back(i);
                }
                else
                {
                    iscorrect = false;
                    t += "Node i not commited" + to_string(i) + "\n";
                }
            }
        }
        if (!iscorrect)
        {
            s += "G1 violated as complete consensus is not reached\n";
            s += t;
        }
        if (decsize.size() != 1)
        {
            s += "G1 violated decsize:" + to_string(decsize.size()) + " \n";
            for (auto it : decmap)
            {
                s += "dec:" + to_string(it.first) + "\n";
                for (auto it1 : it.second)
                {
                    s += "node:" + to_string(it1) + "\n";
                }
            }
        }
        return s;
    }
    string violateG2()
    {
        // check if all normal nodes follow normal master
        // Validity check
        string s = "";

        if (peers[master].getIsMalicious())
        {
            return s;
        }
        bool dec = peers[master].orignalDecision;
        for (int i = 0; i < n; i++)
        {
            if (!peers[i].getIsMalicious())
            {
                if (peers[i].get_decision() != dec)
                {
                    s += "G2 violated \n";
                    return s;
                }
            }
        }
        return "";
    }
    void finalDecision(int idx = -1, bool givendec = false)
    {
        string str = "";
        str += "Decision by client: " + to_string(givendec) + "\n";
        cout << "\nFinal Decision" << endl;
        cout << "master " << master << endl;
        bool consensus = false;
        for (int i = 0; i < n; i++)
        {
            string s = "Final Decision: " + to_string(peers[i].get_decision()) + " in node " + to_string(i);
            enum state st = peers[i].get_state();
            if (st == BASE)
            {
                s += " BASE";
            }
            else if (st == PRE_PREPARED)
            {
                s += " PRE_PREPARED";
            }
            else if (st == PREPARED)
            {
                s += " PREPARED";
            }
            else if (st == COMMITED)
            {
                s += " COMMITED";
            }
            else
            {
                s += " UNKNOWN";
            }
            str += s + "\n";
            logFile(i, s);
            if (st == COMMITED)
            {
                consensus = true;
            }

            bool isMalicious = peers[i].getIsMalicious();
            bool isActive = peers[i].getIsActive();
            bool isMaster = peers[i].getIndex() == master;
            // cout<<"Node "<<i<<" isMalicious "<<isMalicious<<" isActive "<<isActive<<" isMaster "<<isMaster<<" state "<<peers[i].get_state()<<endl;
            if (!isActive)
            {
                continue;
            }
            if (!isMaster && isMalicious)
                continue;
            if (st == COMMITED)
            {
                cout << "Node " << i << " has decided " << peers[i].get_decision() << endl;
            }
        }
        if (idx != -1)
            logDecision(idx, str);
        if (consensus)
        {
            string str = violatesG1();
            str += violateG2();
            if (str.size() > 0)
            {
                ofstream vio("violations.txt", ios::app);
                vio << idx << " " << str << endl;
            }
        }
    }
    void InitReset()
    {
        for (int i = 0; i < n; i++)
        {
            peers[i].roundReset();
        }
    }
    void logprune(string s)
    {
        for (int i = 0; i < n; i++)
        {
            logFile(i, s);
        }
    }
    void doRequest(int ridx, bool dec)
    {
        string s = "";
        s += "\nRound " + to_string(ridx) + " Decision " + to_string(dec);
        s += "\n---------------------------------------";
        logprune(s);
        InitReset();
        clearDecisions(ridx);
        bool cando = doPrePrepare(ridx, dec);
        if (!cando)
        {
            return;
        }
        doPrepare();
        doCommit();
        cout << s;
        finalDecision(ridx, dec);
    }
};

int main()
{
    vector<Peer> peers;
    int serial = 0;
    int n;
    ofstream vio("violations.txt");
    vio.close();
    // read input from input.txt
    ifstream infile("input.txt");
    cout << "Enter number of peers: \n";
    infile >> n;
    for (int i = 0; i < n; i++)
    {
        bool isActive;
        bool isMalicious;
        clearLogs(i);
        cout << "Is peer active? (1/0) \n";
        infile >> isActive;
        cout << "Is peer malicious? (1/0) \n";
        infile >> isMalicious;
        Peer peer(i, isActive, isMalicious);
        peers.push_back(peer);
    }
    int m_limit = (n - 1) / 3;
    cout << "m_limit: " << m_limit << endl;
    Network network(peers);

    cout << "Enter number of requests: \n";
    int req;
    infile >> req;
    for (int i = 0; i < req; i++)
    {
        cout << "Enter request decision: \n";
        int decision = getRandomDecision();
        network.doRequest(i, decision);
    }
    return 0;
}