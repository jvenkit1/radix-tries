#include<iostream>
#include<vector>
using namespace std;

struct node;
struct edge;

struct edge{
  node *targetNode;
  string label;
};

struct node{
  vector<edge*> edges;
  bool isLeaf;
};

struct auxillaryDataStore{
  node *targetNode;
  string data;
};

node* createNewNode(){
  node* newNode = new node;
  newNode->isLeaf=false;
  return newNode;
}

edge* createNewEdge(){
  edge* newEdge = new edge;
  newEdge->label="";
  newEdge->targetNode=NULL;
  return newEdge;
}

string getCommonPrefixSubstr(string curr, string input){
  int i=0;
  while(curr[i]==input[i]){
    i+=1;
  }
  return input.substr(0, i);
}

int getCommonPrefixIndex(string curr, string input){
  int i=0;
  while(curr[i]==input[i] && curr[i]!='\0' && input[i]!='\0'){
    i+=1;
  }
  return i;
}

bool hasCommonPrefix(string curr, string input){
  return curr[0]==input[0]?true:false;
}

// COMPLETE
// THIS WILL DISPLAY ALL THE CURRENT NODES OF THE TRIE
void display(node *root){
  node *traverseNode=root;
  while(traverseNode!=NULL){
    node *child=traverseNode;
    for(int i=0;i<child->edges.size();i++){}
  }
}

// ISSUE WAS THAT WE WERE SEARCHING WITH THE SAME EDGE INDEX i. THAT IS EVEN WHEN WE WENT TO THE SECOND NODE,
// WE WERE SEARCHING FOR DATA FROM THE SAME NODE.

bool search(node *root, string data, int elementsFound, int dataLength){
  node *traverseNode=root;
  int size=data.size();

  if(traverseNode==NULL){
    return false;
  }
  if(elementsFound==dataLength){
    return true;
  }
  if(data.size()<=0){
    return true;
  }

  bool prefixExists=false;
  int i;
  //indicates if the prefix string exists in the data structure.
  // at the root of the structure. check if there exists a common prefix with all the edges

  for(i=0;i<traverseNode->edges.size(); i++){
    if(hasCommonPrefix(traverseNode->edges[i]->label, data)){
      prefixExists=true;
      break;
    }
  }

  if(prefixExists){
    // There exists index i with common prefix with data
    string commonString = getCommonPrefixSubstr(traverseNode->edges[i]->label, data);
    int prefixIndex = getCommonPrefixIndex(traverseNode->edges[i]->label, data);
    data = data.substr(prefixIndex, size-prefixIndex+1);
    return search(traverseNode->edges[i]->targetNode, data, elementsFound, dataLength);
  }
  else{
    // Current edges and the search string do not share a common prefix.
    return false;
  }
}

// REACHES THE REQUIRED POSITION
auxillaryDataStore* reachNode(node *root, string data){
  node *traverseNode=root;
  int i;
  bool hasPrefix=false;
  for(i=0;i<traverseNode->edges.size(); i++){
    if(hasCommonPrefix(traverseNode->edges[i]->label, data)){
      hasPrefix=true;
      break;
    }
  }
  if(hasPrefix){
    if(traverseNode->edges[i]->label.size()>=data.size()){
      string intermediateString = traverseNode->edges[i]->label;
      string prefixString = getCommonPrefixSubstr(data, traverseNode->edges[i]->label);
      int prefixIndex = getCommonPrefixIndex(intermediateString, data);
      intermediateString = intermediateString.substr(prefixIndex, intermediateString.size()-prefixIndex+1);
      traverseNode->edges[i]->label=data;
      data=intermediateString;
      traverseNode=traverseNode->edges[i]->targetNode;
      auxillaryDataStore *ads = new auxillaryDataStore;
      ads->targetNode=traverseNode;
      ads->data=data;
      return ads;
    }
    else{
      string commonString = getCommonPrefixSubstr(traverseNode->edges[i]->label, data);
      int prefixIndex = getCommonPrefixIndex(traverseNode->edges[i]->label, data);
      data = data.substr(prefixIndex, data.size()-prefixIndex+1);
      return reachNode(traverseNode->edges[i]->targetNode, data);
    }
  }
  else{
    auxillaryDataStore *ads = new auxillaryDataStore;
    ads->targetNode=traverseNode;
    ads->data=data;
    return ads;
  }
}

/* Insert has 3 cases:
  1. New Node : An entirely new string is added. Hence simply add this element to the root.
  2. String B is added such that it is a suffix of another string A. -> Simply add a new node to the leaf node denoting A,
  with the suffix denoting the value of the edge. Another null edge has to be added to denote string A.
  3. String B is added such that it shares only a few characters with an existing string A. In this case, identify
  the common suffix C of A and B, make C a node with it's parent as the parent of A and make A and B it's Children.
*/
void insert(node *root, string data){
  node *traverseNode=root;
  auxillaryDataStore *ads = new auxillaryDataStore;
  ads=reachNode(traverseNode, data);
  traverseNode=ads->targetNode;
  data=ads->data;
  // simple insert to the root
  edge* newEdge = createNewEdge();
  node* newNode = createNewNode();
  newNode->isLeaf=true;
  if(traverseNode->isLeaf){
    traverseNode->isLeaf=false;
  }

  newEdge->targetNode=newNode;
  newEdge->label=data;
  // Add newEdge to the list of edges of traverseNode
  // @TODO: Inserting to the beginning. Performance can be compared!
  traverseNode->edges.push_back(newEdge);
}

int main(){
  int n, q;
  string data, search_term;
  node *root = createNewNode();
  edge *branch = createNewEdge();
  cout<<"Enter the number of elements to be inserted\n";
  cin>>n;
  for(int i=0;i<n;i++){
    cin>>data;
    insert(root, data);
  }
  cout<<"\nEnter the number of queries\n";
  cin>>q;
  while(q--){
    cin>>search_term;
    cout<<search(root, search_term, 0, search_term.size())<<"\n";
  }
  return 0;
}
