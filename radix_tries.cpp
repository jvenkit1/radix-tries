#include<iostream>
#include<vector>
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
    cout<<"Inserting value: "<<val<<"\n";
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
        cout<<"Common Prefix was: "<<prefix<<"\n";
        new_val=pruneAndRemovePrefix(child->value, val);
        break;
      }
    }
    if(!prefixExists){
      cout<<"Prefix does not exist. Inserting value: "<<val<<"\n";
      node *newNode=createNode(val);
      traverseNode->link.push_back(newNode);
    }
    else{
      // split the current word and create a separate node as well
      // check if child is leaf. If it is leaf, split it
      // if(childNode->isLeaf){
        // prune and remove whatever does not match from temp_child_value;
        string temp_child_value = childNode->value;
        string prunedString=pruneAndRemovePrefix(val, temp_child_value);
        if(prefix!=childNode->value){
          childNode->value=prefix;
          cout<<"Inserting value here: "<<childNode->value<<" : "<<prunedString<<"\n";
          node *newNode=createNode(prunedString);
          childNode->link.push_back(newNode);
        }
      // }
      // cout<<"Inserting here as well: "<<childNode->value<<" : "<<new_val<<"\n";
      insert(childNode, new_val);
    }
  }
}

bool search(node *root, string val, string found, string originalString){
  node *traverseNode=root;
  node *childNode=root;
  found+=traverseNode->value;
  cout<<"Found is "<<found<<"\n";
  if((traverseNode->link.empty())&&(originalString!=found)){
    return false;
  }
  else if((traverseNode->link.empty()) && (originalString==found)){
    cout<<"Returning true in the first if block\n";
    return true;
  }
  else{
    // Check if there is a prefix between our val and child nodes
    cout<<"Moving to Else part. Found till now is : "<<found<<"\n";
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
    cout<<"Value found till now is : "<<inter_found<<"\n";
    cout<<"New Value is : "<<new_val<<" Printed:\n";
    if(!prefixExists){
      return false;
    }
    else if((new_val=="")&&(inter_found==originalString)){
      cout<<"OG string is : "<<originalString<<" Our string found is : "<<inter_found<<"\n";
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
  cout<<"Enter the number of elements to be inserted\n";
  cin>>n;
  for(int i=0;i<n;i++){
    cin>>data;
    node *auxRoot=root;
    insert(auxRoot, data);
  }

  node *auxRoot=root;
  display(auxRoot, "");

  cout<<"\n\nEnter the number of queries\n";
  cin>>q;
  while(q--){
    cin>>search_term;
    cout<<search(root, search_term, "", search_term)<<"\n";
  }
  return 0;
}
