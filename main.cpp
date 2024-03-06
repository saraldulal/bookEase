#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;

class Customer
{
public:
    string name;
    string address;
    string contact;
    int numOfDays;
    void getCustomerDetails()
    {
        cout<<"\nEnter your details."<<endl;
        cout<<"Enter name.";
        cin>>name;
        cout<<"Enter address.";
        cin>>address;
        cout<<"Enter contact number.";
        cin>>contact;
        cout<<"Enter number of days of booking.";
        cin>>numOfDays;
    }
};

class Room
{
    int capacity;
    int price;
    string type;
    bool availability;
    Customer* guest;
public:
    Room() : capacity(0), price(0), type(""), availability(true), guest(nullptr) {}
    Room(int capacity, int price, string type, bool availability) : capacity(capacity), price(price), type(type), availability(availability) {}
    int getCapacity() const
    {
        return capacity;
    }
    int getPrice() const
    {
        return price;
    }
    string getType() const
    {
        return type;
    }
    Customer *getGuest() const
    {
        return guest;
    }
    bool isAvailable() const
    {
        return availability;
    }
    void setAvailability(bool avail) {
        availability = avail;
    }

    void bookRoom(Customer* customer) {
        if (availability) {
            guest = customer;
            availability = false;
            cout << "\nRoom booked successfully!\n";
        }
         else {
            cout << "Room is not available.\n";
            }
    }

};

unordered_map<int, Room> createRoomMap()
{
    unordered_map<int, Room> roomMap;

    roomMap[101]=Room(1,800,"standard",true);
    roomMap[102]=Room(2,1500,"standard",true);
    roomMap[103]=Room(3,2100,"standard",true);
    roomMap[104]=Room(2,2000,"deluxe",true);
    roomMap[105]=Room(2,1600,"standard",true);
    roomMap[106]=Room(1,750,"standard",true);
    roomMap[201]=Room(1,1600,"deluxe",true);
    roomMap[202]=Room(2,1500,"standard",true);
    roomMap[301]=Room(2,5000,"suite",true);
    roomMap[302]=Room(3,7500,"suite",true);
    return roomMap;
}

void displayRooms(unordered_map<int, Room> roomMap, bool displayByPreference = false, int reqCapacity = 0, string reqType = "")
{
    cout<<"\nRoom\tCapacity     Type\tPrice"<<endl<<endl;
    int roomAvail=0;
    vector<pair<int, Room>> sortedRooms;

    for (auto& entry : roomMap) {
        Room& room = entry.second;
       if (room.isAvailable() && ((!displayByPreference) ||
            (reqCapacity == 0 && room.getType() == reqType) ||
            (reqCapacity != 0 && reqCapacity == room.getCapacity() && (reqType.empty() || reqType == room.getType()))))
        {
            sortedRooms.push_back({entry.first, room});
            cout<<entry.first<<"\t   "<<entry.second.getCapacity()<<"\t    "<<entry.second.getType()<<"\t"<<entry.second.getPrice()<<endl;
            roomAvail++;
        }
    }
        if (roomAvail==0)
            cout<<"\nNo available rooms with matching preferences."<<endl;
        else if (roomAvail>1)
        {
            cout<<"\nDo you want to sort by price? Enter yes or no.";
            string ans;
            cin>>ans;
            if (ans=="yes")
            {
                sort(
                    sortedRooms.begin(), sortedRooms.end(), [](const auto& a, const auto& b)
                    {
                        return a.second.getPrice() < b.second.getPrice();
                    }
                );
                cout<<"\nRoom\tCapacity     Type\tPrice"<<endl;
                for (const auto& entry : sortedRooms) {
                cout<<endl;
                cout << entry.first << "\t   " << entry.second.getCapacity() << "\t    " << entry.second.getType() << "\t" << entry.second.getPrice();
                }
            }
        }
}

void listRoomsByPreferences(unordered_map<int, Room>& roomMap) {
    int reqCapacity;
    string reqType= "";
    bool pref = true;
    cout<<"Enter preferred capacity.Press 0 if not sure.";
    cin>>reqCapacity;
    cout<<"Do you have a preferred room type? (Enter 'yes' or 'no'): ";
    string answer;
    cin >> answer;
    if (answer == "yes")
        {
        cout << "Enter required type of room: ";
        cin >> reqType;
        }
    if (answer=="no" && reqCapacity==0)
        pref = false;
    displayRooms(roomMap, pref, reqCapacity, reqType);
}

void menu()
{

    cout<<"\n\t    Welcome to BookEase!"<<endl;
    cout<<"\t   -Hotel Booking System-"<<endl<<endl;
    cout<<"     Options    Operations"<<endl;
    cout<<"\t1.\tView menu."<<endl;
    cout<<"\t2.\tSearch all available rooms."<<endl;
    cout<<"\t3.\tSearch available rooms by preference."<<endl;
    cout<<"\t4.\tBook a room!"<<endl;
    cout<<"\t5.\tView details of your room."<<endl;
    cout<<"\t6.\tEnter 0 to exit!"<<endl;
}

void bookingDetails(int roomNum, Room room, Customer c)
{
    cout<<"\nBOOKING DETAILS:"<<endl<<endl;
    cout<<"Room number:"<<roomNum<<endl;
    cout<<"Capacity:"<<room.getCapacity()<<endl;
    cout<<"Type:"<<room.getType()<<endl;
    cout<<"Price:"<<room.getPrice()<<endl<<endl;
    cout<<"Customer:"<<endl;
    cout<<"Name: "<<c.name<<endl;
    cout<<"Address: "<<c.address<<endl;
    cout<<"Contact number: "<<c.contact<<endl;
    cout<<"Number of days: "<<c.numOfDays<<endl;
    cout<<"Total price: "<<room.getPrice()*c.numOfDays<<endl;
}

void bookRoom(unordered_map<int, Room>& roomMap, Customer& c1) {
    int roomNum;
    c1.getCustomerDetails();
    cout << "Enter room number you want to book: ";
    cin >> roomNum;
    auto it = roomMap.find(roomNum);
    if (it != roomMap.end()) {
        Room& room = it->second;
        bookingDetails(roomNum,room,c1);
        room.bookRoom(&c1);
    } else {
        cout << "Invalid room number.\n";
    }
}

void viewBookedRoomDetails(unordered_map<int, Room> &roomMap)
{
    string customerName;
    cout << "Enter your name: ";
    cin >> customerName;

    bool roomBooked = false;

    for (const auto &entry : roomMap)
    {
    const Room &room = entry.second;
        if (!room.isAvailable())
        {
            Customer *guest = room.getGuest();
            if (guest->name == customerName)
            {
                bookingDetails(entry.first, room, *guest);
                roomBooked = true;
            }
        }
    }
    if (!roomBooked)
    {
        cout << "No booked room found for customer: " << customerName << endl;
    }
}

int main()
{
    int choice=1;
    Customer c1;
    unordered_map<int, Room> roomMap = createRoomMap();
    menu();
    do
    {
        cout<<"\n\nEnter an option!";
        cin>>choice;
        switch (choice)
        {
            case 1:
                menu();
                break;
            case 2:
                cout<<"\nAvailable Rooms:"<<endl;
                displayRooms(roomMap);
                break;
            case 3:
                listRoomsByPreferences(roomMap);
                break;
            case 4:
                bookRoom(roomMap,c1);
                break;
            case 5:
                viewBookedRoomDetails(roomMap);
                break;
            case 0:
                cout<<"Exiting!"<<endl;
                return 0;
            default:
                cout<<"Choose a correct option!"<<endl;
        }
    }while(choice!=0);
    return 0;
}
