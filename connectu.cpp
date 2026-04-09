/*
 * PROJECT: CONNECT-U (Starter Code)
 * Course: ECE367L Data Structures & Algorithms
 * 
 * SAFETY UPDATE:
 * The saveData() function call in main() is currently COMMENTED OUT.
 * This prevents you from accidentally wiping your 'posts.csv' file
 * if your Lab 1 implementation is incomplete.
 * * ONLY uncomment saveData() after you have verified Lab 1 works!
 */

 /// test 

 ///

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <vector>
#include <set>
#include <queue>
#include <algorithm>

using namespace std;

// ==========================================
// MODELS & DATA STRUCTURES
// ==========================================

struct Post {
    int postId;
    int userId;
    string content;
    int likes;
    long timestamp;
    Post* next; 

    Post(int pid, int uid, string txt, int lk, long time) 
        : postId(pid), userId(uid), content(txt), likes(lk), timestamp(time), next(nullptr) {}
        
    // TODO: LAB 3 - Implement Scoring Logic
    double getScore() {
        double score = 0.0;  // Base score from likes
        double hoursOld = (time(0) - timestamp) / 3600.0;  // Age of post in hours
        score = (likes * 10) + (1000/ (hoursOld + 1));  // Scoring formula
        return score; // Return the calculated score for this post
    }
};

// TODO: LAB 1 - Linked List
class Timeline {
public:
    Post* head;
    Timeline() : head(nullptr) {}

    // Task: Add a new post to the FRONT of the list (O(1))
    void addPost(int pid, int uid, string content, int likes, long time) {
        Post* newPost = new Post(pid, uid, content, likes, time);  // Create new post node  
        newPost->next = head; // Point new post to current head
        head = newPost; // Update head to new post
    }
    // Task: Traverse the linked list and print content
    void printTimeline() {
        Post* current = head;  // Start at head of list
        if (!current) { cout << "  (No posts yet)" << endl; return; }  // If list is empty, print message and return
        
        while(current != nullptr){  // Traverse until end of list
            cout << "  > [ID: " << current->postId << "] @" 
                 << current->userId << ": " << current->content 
                 << " (" << current->likes << " likes) (" << current->timestamp << ")" 
                 << endl;  // Print current post details
            current = current->next;  // Move to next post
        }
    }
};

// Forward Declaration
class User;

// TODO: LAB 4 - Binary Search Tree
struct BSTNode {
    User* user;
    BSTNode* left;
    BSTNode* right;
    BSTNode(User* u) : user(u), left(nullptr), right(nullptr) {}
};

class FriendBST {
public:

    BSTNode* root;
    FriendBST() : root(nullptr) {}

    BSTNode* insert(BSTNode* node, User* u) ;

    void printInOrder(BSTNode* node);

    void addFriend(User* u) { root = insert(root, u); }

    void printFriends() {
        if (root == nullptr) cout << "  (No friends yet)" << endl;
        else printInOrder(root);
    }
};

class User {
public:
    int userId;
    string username;
    int techScore, artScore, sportScore;
    
    Timeline timeline;       // Lab 1
    vector<User*> friends;   // Graph
    FriendBST friendTree;    // Lab 4

    User(int id, string name, int t, int a, int s) 
        : userId(id), username(name), techScore(t), artScore(a), sportScore(s) {}

    void addPost(int pid, string content, int likes, long time) {
        timeline.addPost(pid, userId, content, likes, time);
    }

    void addFriend(User* u) {
        friends.push_back(u);       
        friendTree.addFriend(u);    
    }
    
    vector<User*> getFriendsList() { return friends; }
};

// BST Implementation
BSTNode* FriendBST::insert(BSTNode* node, User* u) {
    if (node == nullptr) {  // If the node is empty
        return new BSTNode(u);  // Create a new node with the user and return it
    }

    if (u->username < node->user->username) {  // If the new user's username is less than the current node's username
        node->left = insert(node->left, u);  // Recursively insert into the left subtree    
    } else if (u->username > node->user->username) {  // If the new user's username is greater than the current node's username
        node->right = insert(node->right, u);  // Recursively insert into the right subtree 
    }

    return node;  // Return the unchanged node pointer after insertion  
}
void FriendBST::printInOrder(BSTNode* node) {
    if (node == nullptr) return;  // If the node is empty, return

    printInOrder(node->left);  // Traverse left subtree
    cout << "  @" << node->user->username << endl;  // Print the username of the current node
    printInOrder(node->right);  // Traverse right subtree
}

// TODO: LAB 3 - Max Heap
class FeedHeap {
private:
    Post* heap[1000]; 
    int size;

    void heapifyDown(int index) { 
        while(2 * index + 1 < size){  // While there is at least a left child
            int left = 2 * index + 1;  // Left child index
            int right = 2 * index + 2;  // Right child index
            int largest = index;  // Assume current index is largest

            if(left < size && heap[left]->getScore() > heap[largest]->getScore()) // If left child exists and is greater than current largest
                largest = left;  // Update largest to left child index
            if(right < size && heap[right]->getScore() > heap[largest]->getScore()) // If right child exists and is greater than current largest
                largest = right;  // Update largest to right child index

            if(largest != index){  // If largest is not current index
                swap(heap[index], heap[largest]);  // Swap current index with largest child
                index = largest;  // Move down to largest index and continue heapifying
            } else {
                break; // Break out of loop if heap property is satisfied
            }
        } 
    }
    void heapifyUp(int index) {
        while (index > 0) {  // While not at root
            int parent = (index - 1) / 2;  // Calculate parent index    
            if (heap[index]->getScore() > heap[parent]->getScore()) {  // If current node is greater than parent
                swap(heap[index], heap[parent]); // Swap current node with parent
                index = parent;  // Move up to parent index and continue heapifying
            } else {
                break; // Break out of loop if heap property is satisfied
            }
        }
          
    }

public:
    FeedHeap() : size(0) {}

    void push(Post* p) { 
        if(size >= 1000) return;  // Safety check to prevent overflow

        heap[size] = p;  // Add new post to end of heap
        size++;  // Increase size of heap
        heapifyUp(size - 1);  // Restore heap property by bubbling new post up to correct position   
    }

    Post* popMax() { 
        if(size == 0) return nullptr;  // If heap is empty, return nullptr

        Post* maxPost = heap[0];  // Store max post
        heap[0] = heap[size - 1];  // Move last post to root
        size--;  // Decrease size of heap

        if(size > 0){
            heapifyDown(0);  // Restore heap property
        }
        
        return maxPost;  // Return the max post that was removed from heap
    }
    bool isEmpty() { return size == 0; }
};

vector<User*> allUsers;

// TODO: LAB 2 - Hash Map
struct HashNode {
    string key;
    User* value;
    HashNode* next;
    HashNode(string k, User* v) : key(k), value(v), next(nullptr) {}
};

class UserMap {
private:
    static const int TABLE_SIZE = 10007; // Size of the has table
    HashNode** table;  // Array of pointers to HashNode

    unsigned long hashFunction(string key) {
        size_t h = 0;  // Initialize hash value
        size_t base = 31;  // A common prime base for string hashing
        for (unsigned char c : key) {  // Iterate over each character in the string
            h = (h * base + c) % TABLE_SIZE;  // Update hash value using polynomial rolling hash method
        }
        return h;  // Simple polynomial rolling hash function
    } 

public:
    UserMap() {
        table = new HashNode*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) table[i] = nullptr;
    }

    void put(string key, User* user) { 
        int index = hashFunction(key);  // Get index from hash function
        HashNode* newNode = new HashNode(key, user);  // Create new hash node with key and user pointer
        newNode->next = table[index];  // Insert at head of linked list for collision resolution (chaining)
        table[index] = newNode;  // Update head of list to new node
    }

    User* get(string key) {
        int index = hashFunction(key);  // Get index from hash function
        HashNode* current = table[index];  // Start at head of linked list at that index
        while (current != nullptr) {  // Traverse linked list to find matching key
            if (current->key == key) return current->value;  // If the key matches, return the current user pointer 
            current = current->next;  // Move to next node in list
        }
        return nullptr;  // If key not found, return nullptr
    }
};

UserMap userMap;

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

vector<string> split(string s) {
    vector<string> tokens;
    string token;
    bool inQuotes = false;
    
    for (char c : s) {
        if (c == '"') {
            inQuotes = !inQuotes; // Toggle quote state
            continue; // Skip the quote character itself
        }
        
        if (c == ',' && !inQuotes) {
            // Found a delimiter outside of quotes -> New Token
            tokens.push_back(token);
            token.clear();
        } else {
            // Regular character or comma inside quotes
            token += c;
        }
    }
    tokens.push_back(token); // Add last token
    return tokens;
}


int GLOBAL_POST_ID_COUNTER = 1;

Post* findPostById(int id) {
    for (User* u : allUsers) {
        Post* curr = u->timeline.head;
        while (curr != nullptr) {
            if (curr->postId == id) return curr;
            curr = curr->next;
        }
    }
    return nullptr;
}

void createNewPost(User* author, string content) {
    int postId = GLOBAL_POST_ID_COUNTER++;
    long timestamp = time(0); 
    author->addPost(postId, content, 0, timestamp);
    cout << "\n[SUCCESS] Post saved to timeline." << endl;
}

void registerNewUser(string username, int tech, int art, int sport) {
    int newId = allUsers.size() + 1; 
    User* newUser = new User(newId, username, tech, art, sport);
    allUsers.push_back(newUser);
    userMap.put(username, newUser);
    cout << "\n[SUCCESS] Account created." << endl;
}

void addFriendship(User* requester, User* target) {
    requester->addFriend(target);
    target->addFriend(requester);
    cout << "\n[SUCCESS] You are now friends with @" << target->username << endl;
}

// TODO: LAB 5 - Breadth First Search
void recommendFriends(User* startUser) {
    cout << "\n[GRAPH ANALYSIS] Finding friends of friends..." << endl;
    
    std::queue<User*> toVisit;  // BFS Queue
    std::set<int> visited;  // Track visited user IDs to avoid cycles 

    visited.insert(startUser->userId); // Mark starting user as visited

    for (User* friendPtr : startUser->friends) {   // Iterate direct friends
        visited.insert(friendPtr->userId);  // Mark friends as visited
        toVisit.push(friendPtr); // Add friends to queue for BFS
    }

    bool found = false;  // Mark as no recommendations found yet
    while (!toVisit.empty()) {  // BFS Loop
        User* current = toVisit.front(); // Get next user to explore
        toVisit.pop(); // Remove from queue

        for (User* connection : current->friends) {  // Check friends of current friend
            if (visited.find(connection->userId) == visited.end()) {  //If not visited
                cout << " > Recommendation: " << connection->username << " (Connection through " << current->username << ")" << endl;  // Print recommendation
                visited.insert(connection->userId);  // Mark as visited to prevent duplicates
                found = true;  // Mark as found
            }
        }
    }
    if (!found) {  // No recommendations found
        cout << "No new recommendations found at this time." << endl;
    }
}

// ==========================================
// FILE I/O 
// ==========================================

void loadData() {
    cout << "Loading data from CSV files..." << endl;
    ifstream userFile("users.csv");
    string line;
    if (userFile.is_open()) {
        getline(userFile, line); 
        while (getline(userFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 5) continue;
            User* newUser = new User(stoi(row[0]), row[1], stoi(row[2]), stoi(row[3]), stoi(row[4]));
            allUsers.push_back(newUser);
            userMap.put(row[1], newUser); 
        }
        userFile.close();
    }
    ifstream relFile("relations.csv");
    if (relFile.is_open()) {
        getline(relFile, line); 
        while (getline(relFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 2) continue;
            int u1 = stoi(row[0]); int u2 = stoi(row[1]);
            if (u1 <= allUsers.size() && u2 <= allUsers.size()) {
                allUsers[u1-1]->addFriend(allUsers[u2-1]);
                allUsers[u2-1]->addFriend(allUsers[u1-1]);
            }
        }
        relFile.close();
    }
    ifstream postFile("posts.csv");
    if (postFile.is_open()) {
        getline(postFile, line); 
        while (getline(postFile, line)) {
            vector<string> row = split(line);
            if (row.size() < 5) continue;
            int pid = stoi(row[0]); int uid = stoi(row[1]);
            if (uid <= allUsers.size()) {
                allUsers[uid-1]->addPost(pid, row[2], stoi(row[3]), stol(row[4])); 
                if (pid >= GLOBAL_POST_ID_COUNTER) GLOBAL_POST_ID_COUNTER = pid + 1;
            }
        }
        postFile.close();
    }
}

void saveData() {
    // SAFETY CHECK: If no users exist, do not overwrite files!
    if (allUsers.empty()) {
        cout << "[SAFETY] No data in memory. Skipping save to prevent file wipe." << endl;
        return;
    }

    cout << "Saving data..." << endl;
    ofstream userFile("users.csv");
    userFile << "user_id,username,tech_score,art_score,sport_score\n";
    for (User* u : allUsers) {
        userFile << u->userId << "," << u->username << "," << u->techScore << "," << u->artScore << "," << u->sportScore << "\n";
    }
    userFile.close();

    ofstream relFile("relations.csv");
    relFile << "user_id_1,user_id_2\n";
    for (User* u : allUsers) {
        for (User* f : u->friends) {
            if (u->userId < f->userId) relFile << u->userId << "," << f->userId << "\n";
        }
    }
    relFile.close();

    ofstream postFile("posts.csv");
    postFile << "post_id,user_id,content,likes,timestamp\n";
    for (User* u : allUsers) {
        vector<Post*> temp;
        Post* curr = u->timeline.head;
        while (curr) { temp.push_back(curr); curr = curr->next; }
        for (int i = temp.size() - 1; i >= 0; i--) {
            Post* p = temp[i];
            string safeContent = p->content;
            if (safeContent.find(',') != string::npos) {
                safeContent = "\"" + safeContent + "\"";
            } 
            postFile << p->postId << "," << p->userId << "," << safeContent << "," << p->likes << "," << p->timestamp << "\n";
        }
    }
    postFile.close();
    cout << "Done." << endl;
}

// ==========================================
// MAIN MENU (UI)
// ==========================================

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void showUserDashboard(User* currentUser) {
    int choice = 0;
    while (choice != 7) {
        cout << "\n--- Welcome, @" << currentUser->username << " ---" << endl;
        cout << "1. View My Post (Lab 1)" << endl;
        cout << "2. Create New Post (Lab 1)" << endl;
        cout << "3. Add Friend (Lab 2)" << endl;
        cout << "4. Algorithmic Feed (Lab 3)" << endl;
        cout << "5. View Friends Sorted (Lab 4)" << endl;
        cout << "6. Get Friend Recommendations (Lab 5)" << endl;
        cout << "7. Logout" << endl;
        cout << "Select >> ";
        cin >> choice;

        if (choice == 1) {
            cout << "\n[MY POST]" << endl;
            currentUser->timeline.printTimeline();
        }
        else if (choice == 2) {
            cout << "\nEnter post content: ";
            cin.ignore(); 
            string content;
            getline(cin, content);
            createNewPost(currentUser, content);
        }
        else if (choice == 3) {
            string friendName;
            cout << "Enter username to add: "; cin >> friendName;
            User* target = userMap.get(friendName);
            if(target && target != currentUser) addFriendship(currentUser, target);
            else cout << "Invalid user (or Hash Map not implemented)." << endl;
        }
        else if (choice == 4) {
            cout << "\n[ALGORITHMIC FEED]" << endl;
            FeedHeap feed;
            vector<User*> friends = currentUser->getFriendsList();
            for(User* f : friends) {
                Post* p = f->timeline.head;
                int limit = 0;
                while(p != nullptr && limit < 5) {
                    feed.push(p);
                    p = p->next;
                    limit++;
                }
            }
            int count = 0;
            while(!feed.isEmpty() && count < 10) {
                Post* top = feed.popMax();
                if(top)
                    cout << "  > [ID: " << top->postId << "] [Score: " << (int)top->getScore() << "] @" 
                         << allUsers[top->userId - 1]->username << ": " << top->content 
                         << " (" << top->likes << " likes)" << endl;
                count++;
            }
            if(count == 0) cout << "  No posts found." << endl;
            else {
                cout << "\nDo you want to like a post? (y/n): ";
                char resp; cin >> resp;
                if (resp == 'y' || resp == 'Y') {
                    int pid; cout << "Enter Post ID: "; cin >> pid;
                    Post* p = findPostById(pid);
                    if (p) { p->likes++; cout << "Liked!" << endl; }
                }
            }
        }
        else if (choice == 5) {
            cout << "\n[MY FRIENDS]" << endl;
            currentUser->friendTree.printFriends();
        }
        else if (choice == 6) {
             recommendFriends(currentUser);
        }
        else if (choice == 7) {
            cout << "Logging out..." << endl;
        }
    }
}

void showMainMenu() {
    int choice = 0;
    while (choice != 3) {
        cout << "\n=== CONNECT-U ===" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit & Save" << endl;
        cout << "Select >> ";
        cin >> choice;

        if (choice == 1) {
            string username;
            cout << "Username: "; cin >> username;
            User* user = userMap.get(username);
            if (user) showUserDashboard(user);
            else cout << "User not found." << endl;
        } 
        else if (choice == 2) {
            string username;
            int t, a, s;
            cout << "Username: "; cin >> username;
            cout << "Tech/Art/Sport (1-10): "; cin >> t >> a >> s;
            registerNewUser(username, t, a, s);
        }
        else if (choice == 3) {
            // SAFETY: Commented out to prevent data loss on initial run.
            // Students must uncomment this ONLY when Lab 1 is complete.
            // saveData(); 
            cout << "Goodbye! " << endl;
        }
    }
}

int main() {
    loadData(); 
    clearScreen();
    showMainMenu();
    return 0;
}