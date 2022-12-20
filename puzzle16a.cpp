struct Valve
{
    bool open;
    string name;
    int rate;
    vector<Valve*> valves;
};


int lookForValve(const vector<Valve>& valves, string name)
{
    int index = 0;
    for (auto it = valves.begin(); it != valves.end(); ++it)
    {
        if (name == it->name)
            return index;
        index++;
    }
    return -1;
}

void closeAllValves(vector<Valve>& valves)
{
    for (auto it = valves.begin(); it != valves.end(); ++it)
    {
        it->open = false;
    }
}

void loadValves(vector<Valve>& valves)
{
    ifstream myFile("input16.txt");
    string line;

    vector<string> lines;

    while (getline(myFile, line))
    {
        if (line != "")
        {
            Valve valve;
            valve.name = line.substr(6, 2);
            size_t firstPart = line.find(";");
            valve.rate = stoi(line.substr(23, firstPart - 23));

            valves.push_back(valve);
            line = line.substr(firstPart);
            lines.push_back(line);
        }
    }

    int index = 0;
    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
        line = *it;
        line = line.substr(25);
        size_t endName = line.find(",");
        while (endName != string::npos)
        {
            string name = line.substr(0, endName);
            int refIndex = lookForValve(valves, name);
            valves[index].valves.push_back(&valves[refIndex]);
            line = line.substr(endName + 2);
            endName = line.find(",");
        }
        string name = line;
        int refIndex = lookForValve(valves, name);
        valves[index].valves.push_back(&valves[refIndex]);
        index++;
    }
}

int puzzle16aRL()
{
    vector<Valve> valves;
    loadValves(valves);

    int startIndex = lookForValve(valves, string("AA"));

    // close / move 1 / move 2.. move 5
    vector<pair<vector<int>, int>> valveMovesScore;

    double exploration = 1.0;
    double explorationDecay = 0.0000001;
    double exploration_min = 0.05;
    
    int computingIterationCount = 5000000;
    int prevMax = 0;

    for (int i = 0; i < computingIterationCount; ++i)
    {
        closeAllValves(valves);
        Valve* currentValve = &valves[startIndex];
        int valveRate = 0;
        int valveRateTotal = 0;
        vector<int> valveMove;
        for (int iter = 0; iter < 30; ++iter)
        {
            valveRateTotal += valveRate;

            double randomExplo = double(rand() % 1000) / 1000.0;
            
            // Exploration
            if (exploration > randomExplo)
            {
                // if close : 50% open 50% move
                if (!currentValve->open && rand() % 2 == 0)
                {
                    currentValve->open = true;
                    valveRate += currentValve->rate;
                    valveMove.push_back(0);
                }
                else
                {
                    int choiceCount = currentValve->valves.size();
                    int choice = rand() % choiceCount;
                    valveMove.push_back(choice + 1);
                    // Move to next valve 
                    currentValve = currentValve->valves[choice];
                }
            }
            // Exploitation
            else
            {
                auto& bestMove = valveMovesScore[min(rand() % 1000, int(valveMovesScore.size() - 1))];
                int choice = bestMove.first[iter];
                if (choice == 0 && !currentValve->open)
                {
                    currentValve->open = true;
                    valveRate += currentValve->rate;
                    valveMove.push_back(0);
                }
                else
                {
                    choice = max(1, min(choice, int(currentValve->valves.size())));
                    currentValve = currentValve->valves[choice-1];
                    valveMove.push_back(choice);
                }
            }
            exploration -= explorationDecay;
            exploration = max(exploration, exploration_min);
        }

        bool hasInserted = false;
        for (auto it = valveMovesScore.begin(); it != valveMovesScore.end(); ++it)
        {
            if (valveRateTotal > it->second)
            {
                valveMovesScore.insert(it, make_pair(valveMove, valveRateTotal));
                hasInserted = true;
                break;
            }
        }
        if (!hasInserted)
        {
            valveMovesScore.push_back(make_pair(valveMove, valveRateTotal));
        }

        if (valveMovesScore.size() > 20000)
        {
            if (prevMax != valveMovesScore[0].second)
            {
                // cout << valveMovesScore[0].second << " Explo: " << exploration << "   ...";
                prevMax = valveMovesScore[0].second;
            }
            
            valveMovesScore.erase(valveMovesScore.begin() + 20000, valveMovesScore.end());
        }
    }

    return valveMovesScore[0].second;
}
