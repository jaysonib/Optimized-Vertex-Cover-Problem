#include <iostream>
  #include <sstream>
  #include <vector>
  #include <ctime>
  #include <stdlib.h>
  #include <fstream>
  #include<math.h>
  #include<unistd.h>
  using namespace std;

  int main(int argc, char** argv) {

      while(true){
      std::ifstream urandom("/dev/urandom");
      if (urandom.fail()) {
          std::cerr << "Error: cannot open /dev/urandom";
          return 1;
      }

      int s = 10;
      int n = 5;
      int l = 5;
      int c = 20;
      for(int i=0;i<argc;i++){try{
              if(argv[i][0] == '-' && argv[i][1] == 's'){
                  s = stoi(argv[++i]);
                  if(s < 2){
                      cerr<<"Error: street numbers are not valid.";
                      exit(0);
                  }
              }
              else if(argv[i][0] == '-' && argv[i][1] == 'n'){
                  n = stoi(argv[++i]);
                  if(n < 1){
                      cerr<<"Error: line segments are not valid.";
                      exit(0);
                  }
              }
              else if(argv[i][0] == '-' && argv[i][1] == 'l'){
                  l = stoi(argv[++i]);
                  if(l < 5){
                      cerr<<"Error: in wait time"<<endl;
                      exit(0);
   }

              }
              else if(argv[i][0] == '-' && argv[i][1] == 'c'){
                  c = stoi(argv[++i]);
              }
          }catch(exception e){
              cerr<<"Error: occured in the stoi() coversion"<<endl;
              exit(0);
          }
      }

      unsigned int num = 42;
      urandom.read((char *)&(num), sizeof(int));
      s = (num%(s-1)+2);
      l = (num%(l-4)+5);
      int count = 0;
      vector<vector<vector<int>>> street;
      bool flag_25 = false;
      for(int i=0;i<s;i++){
          unsigned int num0 = 42;
          urandom.read((char *)&(num0), sizeof(int));
          n = (num0%(n)+1);

          vector<vector<int>> line;

          for(int j=0;j<n+1;j++){
              count = 0;
              while(count<25){
                  bool flag = false;
                  unsigned int num1 = 42;
                  urandom.read((char *)&(num1), sizeof(int));
                  int a = (num1%((2*c)+1)+(-c));
                  unsigned int num2= 42;
                  urandom.read((char *)&(num2), sizeof(int));
                  int b = (num2%((2*c)+1)+(-c));

                  if(line.size()!=0){
                    for(int z = 0;z<line.size();z++){
                          if(a == line[z][0] && b == line[z][1]){
                              flag = true;
                              count++;
                 break;
                          }
                    }for(int z=0;z<line.size()-1;z++){
                          float slope1 = (float(line[z][1]) - float(line[z+1][1]))/(float(line[z][0]) - float(line[z+1][0]));
                          float slope2 =  (float(line[line.size()-1][1]) - float(b))/(float(line[line.size()-1][0]) - float(a));
                        if(slope1 == slope2 || std::isinf(slope1) && std::isinf(slope2)){

                              float AB = sqrt((line[z][0] - line[z+1][0])*(line[z][0] - line[z+1][0]) +
                                              (line[z][1] - line[z+1][1])*(line[z][1] - line[z+1][1]));
                              float AP = sqrt((line[z][0] - a)*(line[z][0] - a) +
                                              (line[z][1] - b)*(line[z][1] - b));
                              float PB = sqrt((line[z+1][0] - a)*(line[z+1][0] - a) +
                                              (line[z+1][1] - b)*(line[z+1][1] - b));
                              if(AB == AP + PB){
                                  count++; flag = true;
                                  break;
                              }

                              AB = sqrt((line[z][0] - line[z+1][0])*(line[z][0] - line[z+1][0]) +
                                        (line[z][1] - line[z+1][1])*(line[z][1] - line[z+1][1]));
                              AP = sqrt((line[z][0] - line[line.size()-1][0])*(line[z][0] - line[line.size()-1][0]) +
                                        (line[z][1] - line[line.size()-1][1])*(line[z][1] - line[line.size()-1][1]));
                              PB = sqrt((line[z+1][0] - line[line.size()-1][0])*(line[z+1][0] - line[line.size()-1][0]) +
                                        (line[z+1][1] - line[line.size()-1][1])*(line[z+1][1] - line[line.size()-1][1]));
                              if(AB == AP + PB){
                                  count++; flag = true;
                                  break;

                              }

                              AB = sqrt((line[line.size()-1][0]-a)*(line[line.size()-1][0]-a) +
                                        (line[line.size()-1][1]-b)*(line[line.size()-1][1]-b));
                              AP = sqrt((line[z][0] - a)*(line[z][0] - a) +
                                        (line[z][1] - b)*(line[z][1] - b));
                              PB = sqrt((line[z][0] - line[line.size()-1][0])*(line[z][0] - line[line.size()-1][0]) +
                                        (line[z][1] - line[line.size()-1][1])*(line[z][1] - line[line.size()-1][1]));
                              if(AB == AP + PB){
                                  count++; flag = true;

                                  break;
                              }
                              AB = sqrt((line[line.size()-1][0]-a)*(line[line.size()-1][0]-a) +
                                        (line[line.size()-1][1]-b)*(line[line.size()-1][1]-b));
                              AP = sqrt((line[z+1][0] - a)*(line[z+1][0] - a) +
                                        (line[z+1][1] - b)*(line[z+1][1] - b));
                              PB = sqrt((line[z+1][0] - line[line.size()-1][0])*(line[z+1][0] - line[line.size()-1][0]) +
                                        (line[z+1][1] - line[line.size()-1][1])*(line[z+1][1] - line[line.size()-1][1]));
                              if(AB == AP + PB){
                                  count++; flag = true;

                                  break;
                              }
                          }



                      }
                  }
                  if(flag == true){ flag = false; continue;}

                  if(line.size() >= 1){
                    for(int st=0;st<street.size();st++){
                        for(int li=0;li<street[st].size()-1;li++){
                              float slope1 = (float(street[st][li][1]) - float(street[st][li+1][1]))/(float(street[st][li][0]) - float(street[st][li+1][0]));
                              float slope2 =  (float(line[line.size()-1][1]) - float(b))/(float(line[line.size()-1][0]) - float(a));

                            if(slope1 == slope2 || std::isinf(slope1) && std::isinf(slope2)){
                                  float AB = sqrt((street[st][li][1] - street[st][li+1][1])*(street[st][li][1] - street[st][li+1][1]) +
                                                  (street[st][li][0] - street[st][li+1][0])*(street[st][li][0] - street[st][li+1][0]));
                                  float AP = sqrt((street[st][li][0] - a)*(street[st][li][0] - a) +
                                                  (street[st][li][1] - b)*(street[st][li][1] - b));
                                  float PB = sqrt((street[st][li+1][0] - a)*(street[st][li+1][0] - a) +
                                                  (street[st][li+1][1] - b)*(street[st][li+1][1] - b));

                                  if(AB == AP + PB){
                                      count++; flag = true;
                                      break;
                                  }
          AB = sqrt((street[st][li][1] - street[st][li+1][1])*(street[st][li][1] - street[st][li+1][1]) +
                                            (street[st][li][0] - street[st][li+1][0])*(street[st][li][0] - street[st][li+1][0]));
                                  AP = sqrt((street[st][li][0] - line[line.size()-1][0])*(street[st][li][0] - line[line.size()-1][0]) +
                                            (street[st][li][1] - line[line.size()-1][1])*(street[st][li][1] - line[line.size()-1][1]));
                                  PB = sqrt((street[st][li+1][0] - line[line.size()-1][0])*(street[st][li+1][0] - line[line.size()-1][0]) +
                                            (street[st][li+1][1] - line[line.size()-1][1])*(street[st][li+1][1] - line[line.size()-1][1]));
                                  if(AB == AP + PB){
                                      count++; flag = true;
                                      break;

                                  }

                                  AB = sqrt((line[line.size()-1][0]-a)*(line[line.size()-1][0]-a) +
                                            (line[line.size()-1][1]-b)*(line[line.size()-1][1]-b));
                                  AP = sqrt((street[st][li][0] - a)*(street[st][li][0] - a) +
                                            (street[st][li][1] - b)*(street[st][li][1] - b));
                                  PB = sqrt((street[st][li][0] - line[line.size()-1][0])*(street[st][li][0] - line[line.size()-1][0]) +
                                            (street[st][li][1] - line[line.size()-1][1])*(street[st][li][1] - line[line.size()-1][1]));

                                  if(AB == AP + PB){
                                      count++; flag = true;
                                      break;
                                  }
                                  AB = sqrt((line[line.size()-1][0]-a)*(line[line.size()-1][0]-a) +
                                            (line[line.size()-1][1]-b)*(line[line.size()-1][1]-b));
                                  AP = sqrt((street[st][li+1][0] - a)*(street[st][li+1][0] - a) +
                                            (street[st][li+1][1] - b)*(street[st][li+1][1] - b));
                                  PB = sqrt((street[st][li+1][0] - line[line.size()-1][0])*(street[st][li+1][0] - line[line.size()-1][0]) +
                                            (street[st][li+1][1] - line[line.size()-1][1])*(street[st][li+1][1] - line[line.size()-1][1]));

                                  if(AB == AP + PB){
                                      count++; flag = true;
                                      break;
                                  }
                              }
                          }
                          if(flag == true) break;
                      }
                  if(flag == true){flag = false; continue;}
  }
                  vector<int> t;
                  t.push_back(a);
                  t.push_back(b);
                  line.push_back(t);
                  break;
              }
          }
          if(count == 25){
              cerr<<"Error: exit due to 25"<<endl;
              cout<<flush;
              flag_25 = true;
              break;
          }
          street.push_back(line);
      }
      if(flag_25 == false){
    for(int i=0;i<street.size();i++){
          cout<<"add \"";
          for(int co=0;co<=(i)/26;co++){
              cout<<char((i%26)+97);
          }
          cout<<"\" ";
        for(int j=0;j<street[i].size();j++){
            if(j==street[i].size()-1)
                  cout<<"("<<street[i][j][0]<<","<<street[i][j][1]<<")";
              else
                  cout<<"("<<street[i][j][0]<<","<<street[i][j][1]<<") ";
          }
          cout<<endl;
      }
      cout<<"gg"<<endl;
    for(int i=0;i<street.size();i++){
          cout<<"rm \"";
          for(int co=0;co<=(i)/26;co++){
              cout<<char((i%26)+97);
          }
          cout<<"\""<<endl;
      }

      }
      sleep(l);
      urandom.close();

      }
      return 0;
  }
