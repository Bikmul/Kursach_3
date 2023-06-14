#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>
 
using namespace std;
class Edge
{
public:
    // вес ребра и тд  
    int weight;
    int dest;
    int src;
    Edge* next;
 
    Edge(int weight, int src, int dest)
    {
        this->weight = weight;
        this->dest = dest;
        this->src = src;
        this->next = NULL;
    }
};
    
{
public: int parent;
      int rank;
      State(int parent, int rank)
      {
          this->parent = parent;
          this->rank = rank;
      }
};
class Graph
{
public: int vertices;
      vector < vector < Edge*> > graphEdge;
      Graph(int vertices)
      {
          this->vertices = vertices;
 
          for (int i = 0; i < this->vertices; ++i)
          {
              this->graphEdge.push_back(vector < Edge*>());
          }
      }
 
      void addEdge(int src, int dest, int w)
      {
          if (dest < 0 || dest >= this->vertices ||
              src < 0 || src >= this->vertices)
          {
              return;
          }
          // добавим ребро узла 
          this->graphEdge.at(src).push_back(new Edge(w, src, dest));
          if (dest == src)
          {
              return;
          }
          this->graphEdge.at(dest).push_back(new Edge(w, dest, src));
      }
      void printGraph()
      {
          cout << "\Spisok smezhnosti grafa ";
          for (int i = 0; i < this->vertices; ++i)
          {
              cout << " \n [" << i << "] :";
              // итерировать ребра i узла
              for (int j = 0; j < this->graphEdge.at(i).size(); ++j)
              {
                  cout << "  " << this->graphEdge.at(i).at(j)->dest;
              }
          }
      }
      int find(State** subsets, int i)
      {
          if (subsets[i]->parent != i)
          {
              subsets[i]->parent = this->find(subsets, subsets[i]->parent);
          }
          return subsets[i]->parent;
      }
      void findUnion(State** subsets, int x, int y)
      {
          int a = this->find(subsets, x);
          int b = this->find(subsets, y);
          if (subsets[a]->rank < subsets[b]->rank)
          {
              subsets[a]->parent = b;
          }
          else if (subsets[a]->rank > subsets[b]->rank)
          {
              subsets[b]->parent = a;
          }
          else
          {
              subsets[b]->parent = a;
              subsets[a]->rank++;
          }
      }
      void boruvkaMST()
      {
          // Содержит сумму веса в пути mst
          int result = 0;
          int selector = this->vertices;
          State** subsets = new State * [this->vertices];
          Edge** cheapest = new Edge * [this->vertices];
          for (int v = 0; v < this->vertices; ++v)
          {
              subsets[v] = new State(v, 0);
          }
          while (selector > 1)
          {
              for (int v = 0; v < this->vertices; ++v)
              {
                  cheapest[v] = NULL;
              }
              for (int k = 0; k < this->vertices; k++)
              {
                  for (int i = 0; i < this->graphEdge.at(k).size(); ++i)
                  {
                      int set1 = this->find(subsets,
                          this->graphEdge.at(k).at(i)->src);
                      int set2 = this->find(subsets,
                          this->graphEdge.at(k).at(i)->dest);
                      if (set1 != set2)
                      {
                          if (cheapest[k] == NULL)
                          {
                              cheapest[k] = this->graphEdge.at(k).at(i);
                          }
                          else if (cheapest[k]->weight >
                              this->graphEdge.at(k).at(i)->weight)
                          {
                              cheapest[k] = this->graphEdge.at(k).at(i);
                          }
                      }
                  }
              }
              for (int i = 0; i < this->vertices; i++)
              {
                  if (cheapest[i] != NULL)
                  {
                      int set1 = this->find(subsets, cheapest[i]->src);
                      int set2 = this->find(subsets, cheapest[i]->dest);
                      if (set1 != set2)
                      {
                          // уменьшить ребро
                          selector--;
                          this->findUnion(subsets, set1, set2);
                          // Отображение соединений ребер
                          cout << "\n Vklychaem rebro ("
                              << cheapest[i]->src
                              << " - "
                              << cheapest[i]->dest
                              << ") ves "
                              << cheapest[i]->weight;
                          // добавим вес
                          result += cheapest[i]->weight;
                      }
                  }
              }
          }
          cout << "\n Ves poluchennogo dereva "
              << result << endl;
      }
};
int main()
{
    string s;
    int i = 0;
    ifstream file("C:/Users/Админ/OneDrive/Рабочий стол/графы/4.txt");
    if (file.is_open())
    {
        getline(file, s);
        i = static_cast<int>(s[0]) - 48;
    }
    Graph* g = new Graph(i);
    if (file.is_open())
    {
        while (getline(file, s))
        {
            int i1 = static_cast<int>(s[0]) - 48;
            int i2 = static_cast<int>(s[1]) - 48;
            int i3 = static_cast<int>(s[2]) - 48;
            g->addEdge(i1, i2, i3);
        }
    }
    file.close();
        g->printGraph();
        g->boruvkaMST();
    return 0;
}
