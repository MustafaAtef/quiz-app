#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct User {
    string name;
    string pass;
    string email;
    string phone;
    User() {
        name = "";
        pass = "";
        email = "";
        phone = "";
    }
};

struct Question {
    string q;
    vector<string> answers;
    int correct;
    Question() {
        q  = "";
        correct = 0;
    }

    bool valid_answer(int answer) {
        return answer == correct;
    }  
};

User loggedin_user;

User resolve_user_data(string user) {
    user += "|";
    User user_data;
    string user_segment = "";
    int segment = 1;
    for (int i = 0; i < user.length(); i++) {
        if (user[i] == '|') {
            if (segment == 1) user_data.name = user_segment;
            if (segment == 2) user_data.pass = user_segment;
            if (segment == 3) user_data.email = user_segment;
            if (segment == 4) user_data.phone = user_segment;
            user_segment = "", segment++;
        }
        else
            user_segment += user[i];
    }
    return user_data;
}
bool user_exist(string username) {
    fstream users;
    users.open("users.txt", ios::in);
    if (users.is_open()) {
        string user;
        while (getline(users, user)) {
            User resolved_data = resolve_user_data(user);
            if (resolved_data.name == username) {
                users.close();
                return true; 
            }
        }
        users.close();
    }
    return false;
}
bool save_user(User user) {
    fstream users;
    users.open("users.txt", ios::app);
    if (users.is_open()) {
        users << user.name << "|" << user.pass << "|" << user.email << "|" << user.phone << "\n";
        users.close();
        return true;
    }
    return false;
}
void login() {
    bool found = false;
    while (!found) {
        cout << "Enter your username: ";
        string entered_user; cin >> entered_user;
        cout << "Enter your password: ";
        string entered_pass; cin >> entered_pass;
        fstream users;
        users.open("users.txt", ios::in);
        if (users.is_open()) {
            string user;
            while (getline(users, user)) {
                User resolved_data = resolve_user_data(user);
                if (resolved_data.name == entered_user && resolved_data.pass == entered_pass) {
                    found = true;
                    loggedin_user = resolved_data;
                    break;
                }
            }
            users.close();
            if (found) {
                cout << "Logged in successfully ! \n";
                break;
            }
            else {
                cout << "There is no username or password matches.\n";
                cout << "1- Login again\n";
                cout << "2- Return to the start menu\n";
                int choise{ 0 };
                while (!(choise > 0 && choise < 3)) {
                    cout << "Please choose from the menu: ";
                    cin >> choise;
                }
                if (choise == 1) continue;
                else break;
            }
        }
        else {
            cout << "Can't logging you in, Please try again later! :(\n";
        }
    }
};
void signup() {
    User registered_user;
    while (true) {
        cout << "Enter a username: ";
        cin >> registered_user.name;
        cout << "Enter a password: ";
        cin >> registered_user.pass;
        cout << "Enter an email: ";
        cin >> registered_user.email;
        cout << "Enter an phone: ";
        cin >> registered_user.phone;

        if (user_exist(registered_user.name)) {
            cout << "This username is already exists\n";
            cout << "1- signup again\n";
            cout << "2- Return to the start menu\n";
            int choise{ 0 };
            while (!(choise > 0 && choise < 3)) {
                cout << "Please choose from the menu: ";
                cin >> choise;
            }
            if (choise == 1) continue;
            else break;
        }
        else {
            if (save_user(registered_user)) {
                cout << "Signed up successfully! \n";
                loggedin_user = registered_user;
                break;
            }
            else {
                cout << "Saving the user failed, try again later !\n";
                cout << "1- signup again\n";
                cout << "2- Return to the start menu\n";
                int choise{ 0 };
                while (!(choise > 0 && choise < 3)) {
                    cout << "Please choose from the menu: ";
                    cin >> choise;
                }
                if (choise == 1) continue;
                else break;
            }
        }
    }
};

int start_menu() {
    while (true) {
        cout << "Welcome to our Q/A application :) \n";
        cout << "1- Login \n";
        cout << "2- Signup \n";
        cout << "3- Exit \n";
        int choise{ 0 };
        while (!(choise < 4 && choise > 0)) {
            cout << "Please choose from the menu: ";
            cin >> choise;
        }
        if (choise == 1) {
            login();
            if (loggedin_user.name.length()) {
                cout << "Hello, " << loggedin_user.name << endl;
                return 1;
            }
            else continue;
        }
        else if (choise == 2) {
            signup();
            if (loggedin_user.name.length()) {
                cout << "Hello, " << loggedin_user.name << endl;
                return 1;
            }
            else continue;
        } 
        else return 0;
    }
}
int admin_menu() {
    int choise{ 0 };
    while (!(choise > 0 && choise < 5)) {
        cout << "1- Add new question.\n";
        cout << "2- View all questions.\n";
        cout << "3- Logout.\n";
        cout << "4- Exit.\n";
        cout << "Please choose from the menu: ";
        cin >> choise;
    }
    return choise;
}
int user_menu() {
    int choise{ 0 };
    while (!(choise > 0 && choise < 4)) {
        cout << "1- Do a quiz \n";
        cout << "2- Logout.\n";
        cout << "3- Exit.\n";
        cout << "Please choose from the menu: ";
        cin >> choise;
    }
    return choise;
}

void add_question() {

    // Getting the question data
    Question new_q;
    cout << "Adding a question.\n";
    cout << "Enter the question: ";
    cin.ignore(256, '\n');
    getline(cin, new_q.q);
    cout << "Enter The Answers : \n";
    vector<char> answers_labels = { 'A', 'B', 'C', 'D' };
    for (char label : answers_labels) {
        cout << label << ") ";
        string entered_answer;
        getline(cin, entered_answer);
        new_q.answers.push_back(entered_answer);
    }
    while (true) {
        cout << "Correct answer is (1,2,3,4): ";
        cin >> new_q.correct;
        if (new_q.correct > 0 && new_q.correct < 5) break;
        else {
            cout << "Invalid answer\n";
            continue;
        }
    }

    // Save the question in the file
    fstream questions;
    questions.open("questions.txt", ios::app);
    if (questions.is_open()) {
        questions << new_q.q << "|";
        for (string a : new_q.answers) {
            questions << a << "|";
        }
        questions << new_q.correct << "\n";
        questions.close();
        cout << "Question added successfully! \n";
        return;
    }
    else {
        cout << "Something went wrong \n";
        return;
    }
}
Question resolve_question(string q) {
    Question resolved;
    string q_segment;
    int segment{1};
    q += "|";
    for (int i = 0; i < q.size(); i++) {
        if (q[i] == '|') {
            if (segment == 1) resolved.q = q_segment;
            if (segment == 2) resolved.answers.push_back(q_segment);
            if (segment == 3) resolved.answers.push_back(q_segment);
            if (segment == 4) resolved.answers.push_back(q_segment);
            if (segment == 5) resolved.answers.push_back(q_segment);
            if (segment == 6) resolved.correct = int(q_segment[0]) - 48;
            q_segment = "";
            segment++;
        }
        else {
            q_segment += q[i];
        }
    }
    return resolved;

}
vector<Question> load_questions() {
    fstream questions;
    questions.open("questions.txt", ios::in);
    if (questions.is_open()) {
        vector<Question> loaded_questions;
        string q;
        while (getline(questions, q)) {
            loaded_questions.push_back(resolve_question(q));
        }
        questions.close();
        return loaded_questions;
    }
    else {
        return {};
    }
}
void view_questions() {
    vector<Question> all_questions = load_questions();
    for (Question q : all_questions) {
        cout << "---------------------------------------------------\n";
        cout << q.q << "\n";
        for (int i = 0; i < 4; i++) {
            cout << char(65 + i) << ") " << q.answers[i] << "\n";
        }
        cout << "Correct Answer is : " << q.correct << "\n";
    }
    cout << "\n";
}
void do_quiz() {
    vector<Question> all_questions = load_questions();
    int score{ 0 };
    for (Question q : all_questions) {
        cout << "---------------------------------------------------\n";
        cout << q.q << "\n";
        for (int i = 0; i < 4; i++) {
            cout << char(65 + i) << ") " << q.answers[i] << "\n";
        }
        cout << "Choose Your Answer : ";
        int answer;
        cin >> answer;
        if (q.valid_answer(answer)) {
            cout << "Correct Answer ! \n";
            score++;
        }
        else {
            cout << "Wrong Answer! \n";
        }
    }
    cout << "You answer " << score << " questions correctly and " << all_questions.size() - score << " Wrong !\n";
    cout << "\n";
}

int main()
{
    while (true) {
        if (start_menu()) {
            if (loggedin_user.name == "admin") {
                while (true) {
                    int menu_choise = admin_menu();
                    if (menu_choise == 4) {
                        cout << "Good bye!\n";
                        return 0;
                    }
                    else if (menu_choise == 1) {
                        add_question();
                    }
                    else if (menu_choise == 2) {
                        view_questions();
                    }
                    else if (menu_choise == 3) {
                        loggedin_user.name = "";
                        loggedin_user.email = "";
                        loggedin_user.pass = "";
                        loggedin_user.phone = "";
                        break;
                    }
                }
            }
            else {
                while (true) {
                    int menu_choise = user_menu();
                    if (menu_choise == 3) {
                        cout << "Good bye!\n";
                        return 0;
                    }
                    else if (menu_choise == 2) {
                        loggedin_user.name = "";
                        loggedin_user.email = "";
                        loggedin_user.pass = "";
                        loggedin_user.phone = "";
                        break;
                    }
                    else {
                        do_quiz();
                    }
                }
            }
        }
        else {
            cout << "Good bye!\n";
            return 0;
        };
    }   
}