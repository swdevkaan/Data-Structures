#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val, Node* nxt = nullptr) : data(val), next(nxt) {}
    };

    Node* topNode;

public:
    Stack() : topNode(nullptr) {}

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }

    bool isEmpty() const {
        return topNode == nullptr;
    }

    void push(const T& x) {
        topNode = new Node(x, topNode);
    }

    void pop() {
        if (isEmpty()) {
            throw underflow_error("Hata: Bos yigindan (Stack) cikarma yapilamaz!");
        }
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }

    T top() const {
        if (isEmpty()) {
            throw underflow_error("Hata: Bos yiginin (Stack) tepesine erisilemez!");
        }
        return topNode->data;
    }
};
class Queue {
private:
    string* theArray;
    int front;
    int back;
    int currentSize;
    int capacity;

public:
    // Yapıcı (Constructor)
    Queue(int cap) {
        capacity = cap;
        theArray = new string[capacity];
        front = 0;
        back = -1;
        currentSize = 0;
    }

    // Yıkıcı (Destructor): Dinamik diziyi bellekten temizler
    ~Queue() {
        delete[] theArray;
    }

    // Kuyruk boş mu? - O(1)
    bool isEmpty() const {
        return currentSize == 0;
    }

    // Kuyruk dolu mu? - O(1)
    bool isFull() const {
        return currentSize == capacity;
    }

    // Kuyruğun sonuna eleman ekleme - O(1)
    void enqueue(string expr) {
        if (isFull()) {
            throw overflow_error("Hata: Kuyruk dolu (Queue Overflow)!");
        }
        back = (back + 1) % capacity;
        theArray[back] = expr;
        currentSize++;
    }

    // Kuyruğun başından eleman çıkarma - O(1)
    string dequeue() {
        if (isEmpty()) {
            throw underflow_error("Hata: Bos kuyruktan (Queue) cikarma yapilamaz!");
        }
        string val = theArray[front];
        front = (front + 1) % capacity;
        currentSize--;
        return val;
    }
};
bool isBalanced(string expr) {
    Stack<char> s;
    for (int i = 0; i < expr.length(); i++) {
        char ch = expr[i];

        if (ch == '(' || ch == '[' || ch == '{') {
            s.push(ch);
        }
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (s.isEmpty()) {
                return false; // Açılmamış parantez kapatılmaya çalışılmış
            }
            char topChar = s.top();
            if ((ch == ')' && topChar == '(') ||
                (ch == ']' && topChar == '[') ||
                (ch == '}' && topChar == '{')) {
                s.pop();
                } else {
                    return false; // Parantez türleri uyuşmuyor
                }
        }
    }
    return s.isEmpty(); // Yığında kalan açık parantez yoksa dengelidir
}
int precedence(char op) {
    if (op == '*') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}
string infixToPostfix(string expr) {
    Stack<char> s;
    string postfix = "";

    for (int i = 0; i < expr.length(); i++) {
        char ch = expr[i];

        if (ch == ' ') continue; // Boşlukları atla

        // 1. Rakam ise doğrudan çıktıya ekle
        if (ch >= '0' && ch <= '9') {
            if (!postfix.empty()) postfix += " ";
            postfix += ch;
        }
        // 2. Açılış parantezi ise yığına at
        else if (ch == '(' || ch == '[' || ch == '{') {
            s.push('('); // Hepsini standart '(' olarak tutmak kontrolü kolaylaştırır
        }
        // 3. Kapanış parantezi ise '(' görene kadar yığındakileri çıktıya aktar
        else if (ch == ')' || ch == ']' || ch == '}') {
            while (!s.isEmpty() && s.top() != '(') {
                if (!postfix.empty()) postfix += " ";
                postfix += s.top();
                s.pop();
            }
            if (!s.isEmpty()) s.pop(); // Açılış parantezini yığından at
        }
        // 4. Operatör (+, -, *) ise öncelik kontrolü yap
        else if (ch == '+' || ch == '-' || ch == '*') {
            while (!s.isEmpty() && s.top() != '(' && precedence(s.top()) >= precedence(ch)) {
                if (!postfix.empty()) postfix += " ";
                postfix += s.top();
                s.pop();
            }
            s.push(ch);
        }
    }

    // Yığında kalan tüm operatörleri çıktıya ekle
    while (!s.isEmpty()) {
        if (!postfix.empty()) postfix += " ";
        postfix += s.top();
        s.pop();
    }

    return postfix;
}
int evaluatePostfix(string postfix) {
    Stack<int> s;

    for (int i = 0; i < postfix.length(); i++) {
        char ch = postfix[i];

        if (ch == ' ') continue;

        // Rakam ise int'e çevirip yığına at
        if (ch >= '0' && ch <= '9') {
            s.push(ch - '0');
        }
        // Operatör ise iki sayı çekip hesapla
        else if (ch == '+' || ch == '-' || ch == '*') {
            int val2 = s.top(); s.pop();
            int val1 = s.top(); s.pop();

            if (ch == '+') s.push(val1 + val2);
            else if (ch == '-') s.push(val1 - val2);
            else if (ch == '*') s.push(val1 * val2);
        }
    }

    return s.top(); // Yığında kalan son değer sonuçtur
}
int main() {
    // Öğrenci No: ...1020 -> d = 0 -> k = 3 + (0 mod 3) = 3
    int k = 3;
    Queue jobQueue(k);

    string expressions[6] = {
        "( 1 + 0 ) * 2 - 0 * ( 0 + 3 ) - 1", // 1. Kişisel ifade (E)
        "5 - 3 - 1",                         // 2. İfade
        "{ 2 * [ 3 + 4 ] }",                 // 3. İfade
        "( 1 + 2 ] * 3",                     // 4. İfade
        "( ( 4 - 1 ) * 2",                   // 5. İfade
        "4 + 1 ) * 2"                        // 6. İfade
    };

    // 1. İfadeleri sırayla k kapasiteli kuyruğa ekle; doluysa yakala, 1 iş işle ve tekrar ekle
    for (int i = 0; i < 6; i++) {
        try {
            jobQueue.enqueue(expressions[i]);
            cout << "Kuyruga eklendi: " << expressions[i] << endl;
        }
        catch (const overflow_error& e) {
            cout << "\n[Hata Yakalandi]: " << e.what() << endl;
            cout << "Yer acmak icin bir is isleniyor...\n" << endl;

            // Kuyruktan bir işi çek (dequeue) ve işle
            string expr = jobQueue.dequeue();
            cout << "Ifade   : " << expr << endl;
            if (isBalanced(expr)) {
                string postfix = infixToPostfix(expr);
                int result = evaluatePostfix(postfix);
                cout << "Durum   : Dengeli" << endl;
                cout << "Postfix : " << postfix << endl;
                cout << "Sonuc   : " << result << endl;
            } else {
                cout << "Durum   : Dengesiz" << endl;
            }
            cout << "------------------------------------" << endl;

            // Aynı ifadeyi yeniden kuyruğa ekle
            jobQueue.enqueue(expressions[i]);
            cout << "Tekrar kuyruga eklendi: " << expressions[i] << endl;
        }
    }

    cout << "\n--- Tum ifadeler eklendi, kalan isler isleniyor ---\n\n";

    // 2. Kuyruk boşalana kadar kalan işleri işle
    while (!jobQueue.isEmpty()) {
        string expr = jobQueue.dequeue();
        cout << "Ifade   : " << expr << endl;
        if (isBalanced(expr)) {
            string postfix = infixToPostfix(expr);
            int result = evaluatePostfix(postfix);
            cout << "Durum   : Dengeli" << endl;
            cout << "Postfix : " << postfix << endl;
            cout << "Sonuc   : " << result << endl;
        } else {
            cout << "Durum   : Dengesiz" << endl;
        }
        cout << "------------------------------------" << endl;
    }

    // 3. Boş kuyruktan dequeue ve boş yığından pop yapıldığında hatanın yakalandığını göster
    cout << "\n--- Bos Yapi Hata Testleri ---\n";

    try {
        jobQueue.dequeue();
    }
    catch (const exception& e) {
        cout << "Bos kuyruk hatasi yakalandi: " << e.what() << endl;
    }

    try {
        Stack<int> emptyStack;
        emptyStack.pop();
    }
    catch (const exception& e) {
        cout << "Bos yigin hatasi yakalandi: " << e.what() << endl;
    }

    return 0;
}