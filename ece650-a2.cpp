  // Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
  #include <iostream>
  #include <sstream>
  #include <vector>
  using namespace std;

  // parse the vertex string
  int check_V(string line){
      string temp = "";
      int size = line.size();
      if(size<3){
          return 0;
      }else{
          if(line[1] != ' ')
              return 0;
          else{
              int i = 2;
              while(i<size){
                  if((line[i])<'0' || line[i]>'9'){
                      return 0;
                  }
                  temp += line[i];
                  i++;
              }
          }
          if(temp != ""){
              return stoi(temp);
          }
      }
      return 0;
  }
  // parsing the edges string
  vector<vector <int>> check_E(string line, int V){
      vector<vector<int>> vec;
      string temp = "";
      bool flag = false;
      int arr[2] = {0,0};
      int j = 0;
      int i = 0;
      vector<int> t;
      int size = line.size();
 while(i<size){
          if(line[i]>='0' && line[i]<='9')
              temp+=line[i];
          else{
              if(temp!="" && flag == false){
                  arr[0] = stoi(temp);
                  if(arr[0] <= V){
                      flag = true;
                      t.push_back(arr[0]);
                  }
                  else{
                      vec.clear();
                      break;
                  }
              }else if(temp!="" && flag == true){
                  arr[1] = stoi(temp);
                  if(arr[1]<=V && arr[0]!=arr[1] && arr[0]!=0 && arr[1]!=0){
                      flag = false;
                      t.push_back(arr[1]);
                      vec.push_back(t);
                      t.pop_back();
                      t.pop_back();
                      j++;
                  }else{
                      vec.clear();
                      break;
                  }
              }
              temp ="";
          }
      i++;
      }
  return vec;
  }
  // check the node is visited or not
  int nodeVisited(int elem, vector<int>& p)
  {
      int s = p.size();
      for (int i = 0; i < s; i++)
          if (p[i] == elem)
              return 0;
      return 1;
  }
  // print the final path
  string printpath(vector<int> path){
      string temp = "";
      int size = path.size();
      for(int i=0;i<size;i++){
          if(i==size-1){
              temp += to_string(path[i]);
          }else{
              temp+=to_string(path[i])+"-";
          }
      }
      return temp;
  }
  // find the shortest path using bfs
  string shortestpath(vector<vector<int>> vec, int s, int d){
      vector<vector<int>> q;
      vector<int> path;
      string temp = "";
      path.push_back(s);
      q.push_back(path);
      while(q.size()!=0){
          path = q.front();
          q.erase(q.begin());
          int l = path[path.size() - 1];
          if(l == d)
              return printpath(path);
          int size = vec[l].size();
          for (int i = 0; i < size; i++) {
              if (nodeVisited(vec[l][i], path)) {
                  vector<int> newpath(path);
        newpath.push_back(vec[l][i]);
                  q.push_back(newpath);
              }
          }
      }
      return "Error: path does not exists.";
  }
  // construct a graph using vector list
  vector<vector<int>> constructGraph(int V, vector<vector<int>> vector1){
      vector<vector<int>> vec;
      vec.resize(V+1);
      int size = vector1.size();
      for(int i=0;i<size;i++){
          vec[vector1[i][0]].push_back(vector1[i][1]);
          vec[vector1[i][1]].push_back(vector1[i][0]);
      }
      return vec;
  }
  // parse the source to destination string
  string check_s(string line, int V, vector<vector<int>> vector1){
      int v_size = vector1.size();
      int l_size = line.size();
      if(V<=1 || v_size == 0){
          return "Error: path does not exists.";
      }else if(l_size <=2){
          return "Error: please enter valid input.";
      }else{
          int i = 2;
          string temp = "";
          int s=0,d=0;
          while(i<l_size){
              if(line[i]>='0' && line[i]<='9'){
                  temp+=line[i];
              }else{
                  if(temp!="" && s==0){
                      s = stoi(temp);
                      temp = "";
                  }
              }
              i++;
          }
  if(temp!=""){
              d = stoi(temp);
          }
          if(s==d && s != 0){
              return "Error: source and destination are the same.";
          }else if(s==0 || d == 0 || s>V || d>V){
              return "Error: invalid input for source or destination.";
          }
          vector1 = constructGraph(V, vector1);
          return shortestpath(vector1,s , d);
      }
      return "Error: path does not exists.";
  }

int main(int argc, char** argv) {
      int V = 0;
      vector<vector<int>> vector1;
      // read from stdin until EOF
      while (!cin.eof()) {
          string line;
          getline(cin, line);
          if(line==""){return 0;}
          if(line[0] == 'V'){
              V = check_V(line);
              vector1.clear();
          }else if(line[0] == 'E' && line[1]!='r'){
              int size = vector1.size();
              if(size > 0){
                  vector1.clear();
                  V = 0;
              }
              vector1 = check_E(line,V);
              cout<<"V "<<V<<endl;
              cout<<"E {";
            for(int i=0;i<vector1.size();i++){
                if(i == vector1.size()-1)
                      cout<<"<"<<vector1[i][0]<<","<<vector1[i][1]<<">";
                  else
                      cout<<"<"<<vector1[i][0]<<","<<vector1[i][1]<<">,";
              }
              cout<<"}"<<endl;
              if(V == 0)
              cout<<"Error: error in V and E."<<endl;
          }else if(line[0] == 's'){
              string x = check_s(line, V,vector1);
              cout<<x<<endl;
          }
          cout<<flush;

      }
      cout<<flush;
      return 0;
  }
