#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <windows.h>
using namespace std;

int extractNum(string &str)
{
    string num;
    for (char c : str)
    {
        if (isdigit(c))
        {
            num += c;
        }
    }
    return stoi(num);
}

class message
{
private:
    int priority;
    string src;
    string dest;
    string payload;
    string trace;

public:
    int message_id;
    message()
    {
        message_id = 0;
        priority = 0;
    }
    message(int id, int p, string src_add, string dest_add, string p_load)
    {
        message_id = id;
        priority = p;
        src = src_add;
        dest = dest_add;
        payload = p_load;
    }
    void display() const
    {
        cout << endl;
        cout << "Message ID: \t\t" << message_id << endl;
        cout << "Priority: \t\t" << priority << endl;
        cout << "Message Source: \t" << src << endl;
        cout << "Message Destination: \t" << dest << endl;
        cout << "Payload: \t\t" << payload << endl;
        cout << "Trace: \t\t\t" << trace << endl
             << endl;
    }
    string getSource()
    {
        return src;
    }
    string getDest()
    {
        return dest;
    }
    void setDest(string str)
    {
        dest = str;
    }
    string getPath()
    {
        return trace;
    }
    void setPath(string str)
    {
        trace = str;
    }
    void setPriority(int p)
    {
        priority = p;
    }
    int getPriority() const
    {
        return priority;
    }
    ~message() {}
};

class node
{
private:
public:
    message msg;
    node *next;
    node()
    {
        next = NULL;
    }
    ~node() {}
};

class queue
{
private:
    node *front;
    node *rear;

public:
    queue()
    {
        front = NULL;
        rear = NULL;
    }
    void enqueue(message m)
    {
        node *newNode = new node();
        newNode->msg = m;
        if (front == NULL)
        {
            front = newNode;
            rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }
    void priorityEnqueue(message m)
    {
        node *newNode = new node();
        newNode->msg = m;

        if (front == NULL || m.getPriority() > front->msg.getPriority())
        {
            newNode->next = front;
            front = newNode;
            if (rear == NULL)
            {
                rear = newNode;
            }
        }
        else
        {
            node *current = front;
            while (current->next != NULL && m.getPriority() <= current->next->msg.getPriority())
            {
                current = current->next;
            }

            newNode->next = current->next;
            current->next = newNode;
            if (current == rear)
            {
                rear = newNode;
            }
        }
    }
    message dequeue()
    {
        node *temp = front;
        message m = temp->msg;
        front = front->next;
        delete temp;
        return m;
    }
    void printQueue()
    {
        node *current = front;
        while (current != NULL)
        {
            cout << current->msg.message_id << endl;
            // current->msg.display();
            current = current->next;
        }
    }
    ~queue() {}
};

class AdjNodeTable
{
private:
public:
    // string dest;
    // string outQ;
    int s;
    message msg;
    AdjNodeTable *next;
    AdjNodeTable()
    {
        next = NULL;
    }
    ~AdjNodeTable() {}
};

class AdjListTable
{
private:
    AdjNodeTable *head;
    AdjNodeTable *tail;
    message msg;

public:
    AdjListTable()
    {
        head = NULL;
        tail = NULL;
    }

    AdjNodeTable *gethead()
    {
        return head;
    }
    void insert(message msg)
    {
        AdjNodeTable *newNode = new AdjNodeTable;
        newNode->msg = msg;
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    void print()
    {
        AdjNodeTable *current = head;
        while (current != NULL)
        {
            current->msg.display();
            current = current->next;
        }
    }
    ~AdjListTable() {}
};

class routingTable
{
private:
    AdjListTable *table;

public:
    routingTable()
    {
        table = new AdjListTable[2];
    }

    AdjListTable *gettable()
    {
        return table;
    }
    void datainsertion(string destination, string next)
    {
        message msg;
        msg.setDest(destination);
        table->insert(msg);
    }
    string update(string destination)
    {
        AdjNodeTable *current = table->gethead();
        while (current != nullptr)
        {
            if (current->msg.getDest() == destination)
            {
                return current->next->msg.getDest();
            }
            current = current->next;
        }
    }
    void print()
    {
        cout << "Printing ROuting table Data: " << endl;
        AdjNodeTable *current = table->gethead();
        while (current != nullptr)
        {
            cout << "Destination: " << current->msg.getDest() << "\t Outgoing Queue: " << current->next->msg.getDest() << endl;
            current = current->next;
        }
    }

    ~routingTable() {}
};

class router : public message
{
private:
    queue incomingQ;
    queue outgoingQ;

public:
    routingTable table;
    string title;
    router() {}
    void insert(string d)
    {
        title = d;
    }
    void incomingMessage(message msg)
    {
        incomingQ.priorityEnqueue(msg);
        message tmp = incomingQ.dequeue();
        outgoingQ.priorityEnqueue(tmp);
        cout << "\nPrinting Incoming queue of " << title << endl;
        incomingQ.printQueue();
        Sleep(1000);
        cout << "\nPrinting Outgoing queue of " << title << endl;
        outgoingQ.printQueue();
    }
    void hopRouter()
    {
    }
    void print()
    {
        cout << "Printing Router " << title << " data: " << endl;
        // cout << "Incoming Queue: " << endl;
        // incomingQ.printQueue();
        // cout << "Outgoing Queue: " << endl;
        // outgoingQ.printQueue();
    }
    ~router() {}
};

class adjNode
{
private:
public:
    router rtr;
    adjNode *next;
    adjNode()
    {
        // rtr.title =
        next = NULL;
    }
    ~adjNode() {}
};

class adjList
{
private:
    adjNode *head;
    adjNode *tail;

public:
    adjList()
    {
        head = NULL;
        tail = NULL;
    }
    void insert(string d)
    {
        adjNode *newNode = new adjNode;
        newNode->rtr.insert(d);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    void sendToRouter(message m, string s, string d)
    {
        adjNode *tmp = head;
        while (tmp != NULL && tmp->rtr.title != s)
        {
            tmp = tmp->next;
        }
        if (tmp == NULL)
        {
            return;
        }
        else
        {
            tmp->rtr.incomingMessage(m);
            m.display();
        }
    }
    string getHead() const
    {
        return head->rtr.title;
    }
    adjNode *Head()
    {
        return head;
    }
    void print()
    {
        adjNode *current = head;
        while (current != NULL)
        {
            cout << current->rtr.title << " ";
            current = current->next;
        }
    }
    void printTable()
    {
        adjNode *current = head;
        while (current != NULL)
        {
            // cout << current->rtr << " ";
            current = current->next;
        }
    }
    ~adjList() {}
};

class graph
{
private:
    adjList *arr;
    int vertices;

public:
    graph(int v)
    {
        arr = new adjList[v];
        vertices = v;
        for (int i = 0; i < v; i++)
        {
            string label = "M" + to_string(i + 1);
            arr[i].insert(label);
        }
    }
    void insertEdge(string src, string dest)
    {
        int source = extractNum(src);
        int destination = extractNum(dest);
        arr[source - 1].insert(dest);
        arr[destination - 1].insert(src);
    }
    void sendMessage(message msg)
    {
        string src = msg.getSource();
        string dest = msg.getDest();
        cout << "\n\n\t\tSending message " << msg.message_id << " from " << src << " to " << dest << endl
             << endl;
        DFS(msg);
        cout << "\nMessage Trace: " << msg.getPath() << endl;
        Sleep(2000);
        // arr[extractNum(src) - 1].sendToRouter(msg, src, dest);
    }
    bool DFS(message &msg)
    {
        string src = msg.getSource();
        string dest = msg.getDest();
        string path;
        // cout << "\nPrinting graph traversal from " << src << " to " << dest << endl;
        bool *visited = new bool[vertices];
        for (int i = 0; i < vertices; i++)
        {
            visited[i] = false;
        }
        bool found = DFSRec(msg, src, dest, path, visited);
        path = path.substr(0, path.length() - 4);
        delete[] visited;
        msg.setPath(path);
        return found;
    }
    bool DFSRec(message msg, string src, string dest, string &path, bool visited[])
    {
        int index = extractNum(src);
        visited[index - 1] = true;
        adjNode *current = arr[index - 1].Head();
        while (current != NULL)
        {
            int destIndex = extractNum(dest);
            int ind = extractNum(current->rtr.title);
            if (!visited[ind - 1])
            {
                if (ind == destIndex)
                {
                    path = current->rtr.title + " -> " + path;
                    current->rtr.incomingMessage(msg);
                    return true;
                }
                else if (DFSRec(msg, current->rtr.title, dest, path, visited))
                {
                    path = current->rtr.title + " -> " + path;
                    current->rtr.incomingMessage(msg);
                    return true;
                }
            }
            current = current->next;
        }
        return false;
    }
    void display()
    {
        for (int i = 0; i < vertices; i++)
        {
            cout << "Printing Adj List for " << arr[i].getHead() << ": ";
            arr[i].print();
            cout << endl;
        }
    }
    void printTrack(message msg)
    {
        // DFS(msg);
        msg.display();
    }
    ~graph() {}
};

void printEndPoints()
{
    for (int i = 0; i < 10; i++)
    {
        cout << "M" << i + 1 << "\t";
    }
}

int main()
{
    // message m1(1, 2, "M2", "M3", "Hello World");
    // message m2(2, 3, "M2", "M4", "Test Payload");
    // message m3(3, 1, "M5", "M6", "Important Data");
    // message m4(4, 5, "M7", "M8", "Emergency Alert");
    // message m5(5, 4, "M6", "M10", "Urgent Message");

    graph network(10);
    network.insertEdge("M1", "M3");
    network.insertEdge("M2", "M4");
    network.insertEdge("M1", "M4");
    network.insertEdge("M3", "M6");
    network.insertEdge("M2", "M5");
    network.insertEdge("M5", "M7");
    network.insertEdge("M2", "M8");
    network.insertEdge("M8", "M9");
    network.insertEdge("M7", "M10");
    network.insertEdge("M5", "M9");
    network.display();

    // // File handling to read edges from CSV file
    // ifstream file("network.csv");  // Adjust the path as necessary
    // string src, dest;
    // while (file.good())
    // {
    //     getline(file, src, ','); // Read the source node from file
    //     getline(file, dest, '\n'); // Read the destination node from file
    //     if (!src.empty() && !dest.empty()) {
    //         network.insertEdge(src, dest);
    //     }
    // }
    // file.close();

    // network.sendMessage(m4);
    // network.sendMessage(m3);
    // network.sendMessage(m5);
    // network.sendMessage(m1);
    // network.sendMessage(m2);
    // network.display();

    string m;
    int count;
    int priority;
    string source, destination;
    message *m0;
    int choice = 0;

    cout << "\t\t\t****NETWORK EMULATOR****\n\n"
         << endl;
    do
    {
        cout << "Choose an option: " << endl;
        cout << "1 -> Send message" << endl;
        // cout << "2 -> Change Routing Table" << endl;
        cout << "3 -> Print Path" << endl;
        cout << "0 -> Exit" << endl;
        cin >> choice;
        if (choice == 1)
        {
            cout << "Enter the message you want to send: \n>>> ";
            cin.ignore();
            getline(cin, m);
            cout << "Give a priority (0-10) to your message [10 is the highest priority]: \n>>> ";
            cin >> priority;
            cout << "What is the source of your message: \n"
                 << endl;
            printEndPoints();
            cout << "\n\n>>> ";
            cin >> source;
            cout << "Give a destination to your message: \n"
                 << endl;
            printEndPoints();
            cout << "\n\n>>> ";
            cin >> destination;
            message msg(count, priority, source, destination, m);
            network.sendMessage(msg);
            cout << "\n\n";
            m0 = &msg;
        }
        else if (choice == 3)
        {
            network.printTrack(*m0);
        }
    } while (choice != 0);

    return 0;
}
