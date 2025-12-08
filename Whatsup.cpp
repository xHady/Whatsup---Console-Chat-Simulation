#include <iostream>
#include <chrono>
#include <thread>

using namespace std;
using namespace this_thread;
using namespace chrono;

class Chat_LinkedList{
    struct ChatNode{
        ChatNode *next;
        int sender;
        string message;
    };
    ChatNode *head;
    ChatNode *tail;
    unsigned int length;
    int myNumber;
    public:

    Chat_LinkedList()
    {
        length = 0;
        head = tail = NULL;
    }
    void initMyNumber(int num)
    {
        myNumber = num;
    }
    bool addMessage(int sndr, string msg)
    {
        if(sndr > 0)
        {
            ChatNode *newNode = new ChatNode();
            newNode->sender = sndr;
            newNode->message = msg;
            if(length == 0)
            {
                head = tail = newNode;
            }
            else
            {
                tail->next = newNode;
                tail = newNode;
            }
            newNode->next = NULL;
            length++;
            return 1;
        }
        cout<<"Error: Sender Invalid, Message { "<<msg<<" } cannot sent\n";
        return 0;
    }
    void drawLine(unsigned short int count)
    {
        for(unsigned short int i = 0;i < count;i++)
        {
            cout<<"-";
        }
        cout<<"\n";
    }
    string getLastMessage()
    {
        return tail->message;
    }
    void printChat()
    {
        ChatNode *Curr = head;
        drawLine(10);
        while(Curr != NULL)
        {
            if(Curr->sender == myNumber)
                cout<<"Sender : Me\n";
            else
                cout<<"Sender : "<<Curr->sender<<"\n";
            cout<<"Message : "<<Curr->message<<"\n";
            drawLine(10);
            Curr = Curr->next;
        }
    }
};
class Contacts_Stack{
    struct ContactNode{
        ContactNode *next;
        int pNumber;
        int sNumber;
        string chatKey;
        Chat_LinkedList chat;
    };

    ContactNode *top;
    int length;
    int myNumber;
    public:

    Contacts_Stack()
    {
        length = 0;
        top = NULL;
    }
    void initMyNumber(int num)
    {
        myNumber = num;
    }
    void changeMyNumber(int num)
    {
        myNumber = num;
        ContactNode *Curr = top;
        while(Curr != NULL)
        {
            Curr->chat.initMyNumber(num);
            Curr = Curr->next;
        }
    }
    
    string makeChatKey(int num1, int num2)
    {
        if(num1 > num2)
            swap(num1, num2);
        return to_string(num1) + "_" + to_string(num2);
    }
    void insertContactNode(int senderNum, int recieverNum, string message)
    {
        ContactNode *newNode = new ContactNode();
        Chat_LinkedList newChat;
        newChat.addMessage(senderNum, message);
        newChat.initMyNumber(myNumber);
        newNode->chatKey = makeChatKey(senderNum, recieverNum);
        newNode->pNumber = senderNum;
        newNode->sNumber = recieverNum;
        newNode->chat = newChat;
        newNode->next = top;
        top = newNode;
        length++;
        cout<<"Contact with chatKey { "<<newNode->chatKey<<" } Created.\n";
    }
    void sendMessage(int senderNum, int recieverNum, string message)
    {
        if(length == 0)
        {
            insertContactNode(senderNum, recieverNum, message);
            return;
        }
        if(top->chatKey == makeChatKey(senderNum, recieverNum))
        {
            top->chat.addMessage(senderNum, message);
            return;
        }
        ContactNode *Curr = top;
        ContactNode *temp = Curr->next;
        while(temp != NULL)
        {
            if(temp->chatKey == makeChatKey(senderNum, recieverNum))
            {
                Curr->next = temp->next;
                temp->next = top;
                top = temp;
                top->chat.addMessage(senderNum, message);
                return;
            }

            temp = temp->next;
            Curr = Curr->next;
        }
        insertContactNode(senderNum, recieverNum, message);
    }
    void viewMyContacts()
    {
        ContactNode *Curr = top;
        int otherNum;
        unsigned short int indx = 1;
        bool empty = 1;
        while(Curr != NULL)
        {
            if(myNumber == Curr->pNumber || myNumber == Curr->sNumber)
            {
                otherNum = ( myNumber == Curr->pNumber ) ? Curr->sNumber : Curr->pNumber;
                if(empty)
                    empty = 0;
                cout<<indx<<" ) ";
                Curr->chat.drawLine(16);
                cout<<"Contact : "<<otherNum<<"\n";
                cout<<"Last Message : "<<Curr->chat.getLastMessage()<<"\n";
            }
            indx++;
            Curr = Curr->next;
        }
        if(empty)
        {
            cout<<"There's no Contacts yet, Make some friends and Come Again.\n";
        }
        Curr->chat.drawLine(20);
    }
    int getChatWithIndex(unsigned short int indx)
    {
        unsigned short int counter = 1;
        ContactNode *Curr = top;
        while(counter < indx)
        {
            counter++;
            Curr = Curr->next;
        }
        if(Curr == NULL)
        {
            cout<<"Invalid Index.\n";
            return -1;
        }
        Curr->chat.printChat();
        return ( myNumber == Curr->pNumber ) ? Curr->sNumber : Curr->pNumber;
    }
};


Contacts_Stack ContactsList;
int myNumber;
unsigned short int currScreen = 1;

void flushBuffer()
{
    system("cls");
}
void viewChat(unsigned short int index)
{
    unsigned short int choose;
    string message;
    flushBuffer();
    int SenderNumber = ContactsList.getChatWithIndex(index);
    cout<<"1 ) Send Quick Message      2 ) Return\n";
    cout<<"3 ) Exit\nChoose : ";
    cin>>choose;
    switch(choose)
    {
        case 1:
            cout<<"\nEnter Message to Send.\nMessage : ";
            getline(cin >> ws, message);
            ContactsList.sendMessage(myNumber, SenderNumber, message);
            flushBuffer();
            viewChat(index);
            break;
        case 2:
            currScreen = 3;
            break;
        case 3:
            flushBuffer();
            cout<<"Bye, "<<myNumber;
            sleep_for(seconds(3));
            currScreen = 0;
            break;
        default:
            flushBuffer();
            currScreen = 1;
            break;
    }
}
void viewContacts()
{
    unsigned short int choose;
    unsigned short int index;
    flushBuffer();
    ContactsList.viewMyContacts();
    cout<<"\n1 ) Enter to Chat           2 ) Return\n";
    cin>>choose;
    switch(choose)
    {
        case 1:
            cout<<"Enter Chat Index.\n";
            cout<<"Index : ";
            cin>>index;
            viewChat(index);
            break;
        case 2:
            currScreen = 1;
            break;
        default:
            currScreen = 3;
            break;
    }
}
void sendTo()
{
    int sender;
    int reciever;
    unsigned short int choose;
    string message;
    cout<<"Enter Sender Number.\nNumber : ";
    cin>>sender;
    cout<<"\nEnter Reciever Number.\nNumber : ";
    cin>>reciever;
    cout<<"\nEnter Message to Send.\nMessage : ";
    getline(cin >> ws, message);
    ContactsList.sendMessage(sender, reciever, message);
    sleep_for(seconds(1));
    cout<<"Message Sent.\n";
    cout<<"1 ) Return                  2 ) View Contacts\n";
    cout<<"3 ) Exit\n";
    cin>>choose;
    switch(choose)
    {
        case 1:
            currScreen = 1;
            break;
        case 2:
            currScreen = 3;
            break;
        case 3:
            flushBuffer();
            cout<<"Bye, "<<myNumber;
            sleep_for(seconds(3));
            currScreen = 0;
            break;
        default:
            flushBuffer();
            currScreen = 1;
            break;
    }

}
void changeNumber()
{
    int newNum;
    cout<<"Enter New Number : ";
    cin>>newNum;
    myNumber = newNum;
    ContactsList.changeMyNumber(newNum);
    cout<<"\nNumber Changed.";
    sleep_for(seconds(3));
    currScreen = 1;
}
void welcomeScreen()
{
    unsigned short int choose;
    cout<<"Welcome to Whatsup, "<<myNumber<<"\n";
    cout<<"1 ) Send Message                  2 ) View Contacts\n";
    cout<<"3 ) Exit                          4 ) Change My Number\n";
    cout<<"Choose : ";
    cin>>choose;
    switch(choose)
    {
        case 1:
            currScreen = 2;
            break;
        case 2:
            currScreen = 3;
            break;
        case 3:
            flushBuffer();
            cout<<"Bye, "<<myNumber;
            sleep_for(seconds(3));
            currScreen = 0;
            break;
        case 4:
            currScreen = 4;
            break;
        default:
            currScreen = 1;
            break;
    }
}
int main()
{
    cout<<"Welcome to Whatsup, Please enter Your Number.\n";
    cout<<"Your Number : ";
    cin>>myNumber;
    ContactsList.initMyNumber(myNumber);
    while(currScreen != 0)
    {
        flushBuffer();
        if(currScreen == 1)
        {
            welcomeScreen();
        }
        else if(currScreen == 2)
        {
            sendTo();
        }
        else if(currScreen == 3)
        {
            viewContacts();
        }
        else if(currScreen == 4)
        {
            changeNumber();
        }
    }
}