//Nabin Shrestha 1001746226
#include <iostream>
#include <map>
#include <vector>
#include <exception>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>
#include <set>
#include <algorithm>
class Neighbours
{
public:
    Neighbours(std::string des, float va) :destination{ des }, value{ va }{}
    std::string destination;
    double value;
};
class Fringe
{
public:
    Fringe(double total, std::vector<std::string> pa):totalcost{total},path{pa}{}
    double totalcost;
    std::vector<std::string> path;
};
std::map<std::string,std::vector<Neighbours>> readfile(std::string fname)
{
    std::ifstream fp(fname);
    std::string line;
    std::map<std::string, std::vector<Neighbours>> graph;
    std::string source, destination;
    float distance;
    int count = 0;
    std::string word;
    std::vector<std::string> words;
    while (std::getline(fp, line))
    {
        std::istringstream ss(line);
        while (ss >> word && count<3)
        {
            words.push_back(word);
            count++;
        }
        if (words.at(0) == "END")
            break;
        count = 0;
        source = words.at(0);
        destination = words.at(1);
        distance = std::stod(words.at(2));
        graph[source].push_back(Neighbours(destination, distance));
        graph[destination].push_back(Neighbours(source, distance));
        words.clear();
        
    }
    return graph;
}
void uniformsearch(std::map<std::string, std::vector<Neighbours>> graph, std::string source, std::string destination)
{
    std::vector<Fringe> _fringe;
    std::vector<std::string> path{ source };
    std::set<std::string> closed_set;
    _fringe.push_back(Fringe(0.0,path));
    std::string current_city = source;
    int node_expanded = 0;
    int node_generated = 1;
    while (_fringe.size() != 0)
    {
        Fringe f = _fringe.back();
        _fringe.pop_back();
        std::string current_city = f.path.back();

        if (current_city == destination)
        {
            node_expanded += 1;
            std::cout << "Nodes expanded: " << node_expanded << "\nNodes Generated: " << node_generated << std::endl;
            std::cout << "Distance: " << f.totalcost <<"\nRoute:"<< std::endl;
            int index = 0;
            while (index < f.path.size() - 1)
            {
                std::vector<Neighbours> neigh = graph[f.path.at(index)];
                for (auto v : neigh)
                {
                    if (v.destination == f.path.at(index+1))
                    {
                        std::cout << f.path.at(index) << " to " << f.path.at(index + 1) << " : " << v.value << " km." << std::endl;
                    }
                }
                index++;
               
            }
            return;
        }

        if (!closed_set.count(current_city))
        {
            std::vector<Neighbours> neigh = graph[current_city];
            for (auto v : neigh)
            {
                std::vector<std::string> newpath = f.path;
                newpath.push_back(v.destination);
                double newpathcost = v.value + f.totalcost;
                _fringe.push_back((Fringe(newpathcost, newpath)));
                node_generated += 1;
            }
            node_expanded += 1;
        }
        else
            node_expanded += 1;
        std::sort(_fringe.begin(), _fringe.end(), [](const Fringe& lhs, const Fringe& rhs) { return lhs.totalcost > rhs.totalcost; });
        closed_set.insert(current_city);
    }
    std::cout << "Nodes expanded: " << node_expanded << "\nNodes Generated: " << node_generated << std::endl;
    std::cout << "Distance: infinity" << "\nRoute: none" << std::endl;
    
}
int main(int argc, char *argv[])
{  
    try {
        std::string fname = argv[1];
        std::string source = argv[2];
        std::string destination = argv[3];
        std::map<std::string, std::vector<Neighbours>> graph = readfile(fname);
        if (argc == 5)
            auto herusticvalue = readfile(argv[4]);
        else
            uniformsearch(graph, source, destination);
    }
    catch (std::exception e)
    {
        std::cout << e.what() << '\n';
    }
}
