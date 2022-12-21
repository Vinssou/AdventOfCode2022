struct Node
{
    int number;
    Node* previous;
    Node* next;
};


int findIndex(vector<Node>& numbers, int number)
{
    for (int i = 0; i < numbers.size(); ++i)
    {
        if (number == numbers[i].number)
            return i;
    }

    return -1;
}

void moveNode(Node* afterNode, Node* node)
{
    // assert(node == afterNode);
    if (node != afterNode)
    {
        // Remove
        node->next->previous = node->previous;
        node->previous->next = node->next;

        // Add
        Node* next = afterNode->next;
        assert(next != node);
        afterNode->next = node;
        assert(next->previous != node);
        next->previous = node;
        node->next = next;
        node->previous = afterNode;
    }
}


void sanityCheck(vector<Node>& numbers)
{
    Node* firstNode = & numbers[0];
    Node* node = firstNode->next;
    int itemCount = 1;
    while (node != firstNode)
    {
        assert(node == node->next->previous);
        assert(node == node->previous->next);
        itemCount++;
        node = node->next;
    }
    assert(itemCount == numbers.size());
}

int puzzle20()
{   
    ifstream myFile("input20.txt");
    string line;

    vector<Node> numbers;
    numbers.reserve(5001);
    Node* previous = nullptr;
    while (getline(myFile, line))
    {
        int theNumber = stoi(line);
        Node node;
        node.number = theNumber;
        node.previous = previous;
        node.next = nullptr;
        numbers.push_back(node);
        Node* current = &numbers[numbers.size() - 1];
        
        if (previous != nullptr)
        {
            previous->next = current;
        }
        previous = current;
    }
    // Here previous is the last node
    numbers[0].previous = previous;
    assert(previous != nullptr);
    previous->next = &numbers[0];

    const int numberCount = numbers.size();
    Node* node = &numbers[0];
    for (int i = 0; i < numberCount; i++)
    {
        cout << node->number << ",  ";
        node = node->next;
    }
    cout << endl;   

    for (auto it = numbers.begin(); it != numbers.end(); ++it)
    {
        Node* currentNode = &(* it);
        int currentNumber = currentNode->number;

        if (currentNumber > 0)
        {
            Node* afterNode = currentNode;
            for (int i = 0; i < currentNumber; i++)
            {
                afterNode = afterNode->next;
                if (currentNode == afterNode)
                {
                    afterNode = afterNode->next;
                }
            }
            moveNode(afterNode, currentNode);
        }
        else if (currentNumber < 0)
        {
            Node* afterNode = currentNode;
            for (int i = 0; i >= currentNumber; i--)
            {
                afterNode = afterNode->previous;
                if (currentNode == afterNode)
                {
                    afterNode = afterNode->previous;
                }
            }
            moveNode(afterNode, currentNode);
        }

        /*Node* node = &numbers[0];
        for (int i = 0; i < numberCount; i++)
        {
            cout << node->number << ",  ";
            node = node->next;
        }*/
        cout << endl;
    }
    
    sanityCheck(numbers);

    int indexZero =  findIndex(numbers, 0);

    Node* zeroNode = &numbers[indexZero];
    Node* thousand = nullptr, *twoThousands = nullptr, *threeThousand = nullptr;
    for (int i = 0; i < 3001; i++)
    {
        if (i == 1000)
        {
            thousand = zeroNode;
        }
        if (i == 2000)
        {
            twoThousands = zeroNode;
        }
        if (i == 3000)
        {
            threeThousand = zeroNode;
        }
        zeroNode = zeroNode->next;
    }

    int firstGroove = thousand->number;
    int secondGroove = twoThousands->number;
    int thirdGroove = threeThousand->number;

    return firstGroove + secondGroove + thirdGroove;
}
