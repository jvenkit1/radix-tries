#include<iostream>
#include<vector>
#include<fstream>
using namespace std;

struct node{
  vector<node*> link;
  bool isLeaf;
  string value;
};

node* createNode(string val){
  node *newNode=new node;
  newNode->isLeaf=true;
  newNode->value=val;
  return newNode;
}

bool sharePrefix(string a, string b){
  return a[0]==b[0]?true:false;
}

string getPrefix(string curr, string input){
  int i=0;
  while(curr[i]==input[i]){
    i+=1;
  }
  return input.substr(0, i);
}

string pruneAndRemovePrefix(string curr, string input){
  int i=0;
  int n=input.size();
  while(curr[i]==input[i]&&(curr[i]!='\0')&&(input[i]!='\0')){
    i+=1;
  }
  return input.substr(i, n-i);
}

void insert(node *root, string val){
  node *traverseNode=root;
  node *childNode=root;  //Auxillary Value
  if(traverseNode->isLeaf){
    node *newNode=createNode(val);
    traverseNode->link.push_back(newNode);
    traverseNode->isLeaf=false;
  }
  else{
    // Amongst all neighbours of root, find if there exists a shared prefix, and recurse to that node
    // iterate through vector list
    bool prefixExists=false;
    string new_val=val;
    string prefix=val;
    for(node *child:traverseNode->link){
      if(sharePrefix(child->value, val)){
        //get prefix, generate new string to be inserted, recurse to the child node
        prefixExists=true;
        childNode=child;
        prefix=getPrefix(child->value, val);
        new_val=pruneAndRemovePrefix(child->value, val);
        break;
      }
    }
    if(!prefixExists){
      node *newNode=createNode(val);
      traverseNode->link.push_back(newNode);
    }
    else{
      // split the current word and create a separate node as well
      // check if child is leaf. If it is leaf, split it
      // prune and remove whatever does not match from temp_child_value;
      string temp_child_value = childNode->value;
      string prunedString=pruneAndRemovePrefix(val, temp_child_value);

      /* for cases where we have to break the current word into 2 node:
         1 node signifying the already existing word and another
         signifying the new word to be inserted
      */
      if(prefix!=temp_child_value){
        childNode->value=prefix;
        node *newNode=createNode(prunedString);

        // Transferring all children of childNode to the newNode
        // which will be child of the current node which is branched out
        for(node* children: childNode->link){
          newNode->link.push_back(children);
        }
        newNode->isLeaf=false;
        childNode->link.clear();
        childNode->link.push_back(newNode);

        // insert data in childNode:
        node* newChildNode=createNode(new_val);
        childNode->link.push_back(newChildNode);
        childNode->isLeaf=false;
      }
      else{
        if(new_val==""){
          cout<<"Value is already inserted\n";
        }
        insert(childNode, new_val);
      }
    }
  }
}

bool search(node *root, string val, string found, string originalString){
  node *traverseNode=root;
  node *childNode=root;
  found+=traverseNode->value;
  if((traverseNode->link.empty())&&(originalString!=found)){
    return false;
  }
  else if((traverseNode->link.empty()) && (originalString==found)){
    return true;
  }
  else{
    // Check if there is a prefix between our val and child nodes
    string new_val;
    string inter_found=found;
    bool prefixExists=false;
    for(node *child:traverseNode->link){
      if(sharePrefix(child->value, val)){
        prefixExists=true;
        childNode=child;
        new_val=pruneAndRemovePrefix(child->value, val);
        inter_found+=child->value;
        break;
      }
    }
    if(!prefixExists){
      return false;
    }
    else if((new_val=="")&&(inter_found==originalString)){
      return true;
    }
    else{
      return search(childNode, new_val, found, originalString);
    }
  }
}

void display(node *root, string prefix){
  string new_prefix=prefix+root->value;
  cout<<new_prefix<<"\n";
  if(!root->isLeaf){
    for(node *child: root->link){
      display(child, new_prefix);
    }
  }
}

int main(){
  int n, q;
  string data, search_term;
  node *root = createNode("");
  root->isLeaf=true;
  // READING DATA FROM A FILE
  ifstream fstream("input.txt");
  if(fstream.is_open()){
    while(getline(fstream, data)){
      data+=".";
      node *auxRoot=root;
      insert(auxRoot, data);
    }
  }

  node *auxRoot=root;
  cout<<"Displaying all nodes now !\n";
  display(auxRoot, "");

  cout<<"\n\nEnter the number of queries\n";
  cin>>q;
  while(q--){
    cin>>search_term;
    search_term+=".";
    cout<<search(root, search_term, "", search_term)<<"\n";
  }
  return 0;
}
