#include<iostream>
#include<string>
using namespace std;
// çift yönlü dairesel bağlı liste
class Node {
    public:
    string title;
    string artist;
    Node* next;
    Node* prev;

    Node(string title, string artist) {
        this->title = title;
        this->artist = artist;
        next = nullptr;
        prev = nullptr;

    }
};
class Playlist {
    public:
        Node* head;
        Node* tail;
        Node* currentSong;
        bool isRepeat;
        //  contructor: liste ilk oluştuğunda her şey boş
        Playlist() {
            head = nullptr;
            tail = nullptr;
            currentSong = nullptr;
            isRepeat = false;
        }
        //destructor: liste yok edilirken düğümlerin tamamı siliniyor
        ~Playlist() {
            if (head = nullptr) return;//liste kontrolü boş ise bişey yapma
            tail->next = nullptr;// dairesel yapıdan kurtulmak için son bağı koparma
            Node* temp = head;//kopya yollayarak tek tek bellekten silebilme
            //döngü geçici değişkenin kopyalarını tek tek siliyor
            while (temp!= nullptr) {
                Node* nextNode = temp->next;
                delete temp;
                temp=nextNode;
            }
            head = nullptr;
            tail = nullptr;
            currentSong = nullptr;
        }




        void addSong(string artist,string title) {

            Node* newNode=new Node(artist,title);
            if (head == nullptr) {
                head = newNode;
                tail = newNode;
                currentSong = newNode;

                // dairsel bağ kurma
                head->next = newNode;
                head->prev = newNode;
            }
            //listede eleeman varsa sona ekleme
            else {

                tail->next = newNode; //eski sonuncunun nextine yeni düğümü bağla
                newNode->prev = tail; // yeni düğümün previne eski sonuncuyu bağla

                newNode->next = head; //yeni sonuncunun nexti head gösterir
                head->prev = newNode; // başın previ yeni yeeni taili gösteriyor
            }
            tail=newNode; // yeni tail newNode
        }
        // o an seçili olan current song pointerının gösterdiği şarkının adı ve şarkıyı verir boş ise uyarı verir
        void playCurrent() {
            if (currentSong == nullptr) {
                cout<<"Liste bos"<<endl;
                return;
            }
            cout << "Su an caliyor: " << currentSong->title << " - " << currentSong->artist << endl;}

    // bool olan isRepeat değişkenini tersine çevirir(true ise false , false ise true
    void toggleRepeat() {
        isRepeat = !isRepeat;
        cout << "tekrar modu: " << (isRepeat ? "acik" : "kapali") << endl;
    }

        void nextSong() {
            if (currentSong == nullptr) {
                cout << "Liste Bos!" << endl;
                return;
            }

            // teekrar modu kapalıysa ve sson şarkıdaysak en başa dönme
            if (!isRepeat && currentSong == tail) {
                cout << "Listenin sonuna geldiniz. (Tekrar modu kapali)" << endl;
                return;
            }

            // tekrar modu açıka veya son şarkıda değilse sonraki şarkıya geçiş
            currentSong = currentSong->next;
            cout << "Sonraki sarki: " << currentSong->title << " - " << currentSong->artist << endl;
        }


        void prevSong() {
            if (currentSong == nullptr) {
                cout << "Liste Bos!" << endl;
                return;
            }

            // Tekrar modu kapalıysa VE ilk şarkıdaysak geriye gitme
            if (!isRepeat && currentSong == head) {
                cout << "Listenin basindasiniz. (Tekrar modu kapali)" << endl;
                return;
            }

            // Aksi halde önceki şarkıya geç
            currentSong = currentSong->prev;
            cout << "Onceki sarki: " << currentSong->title << " - " << currentSong->artist << endl;
        }

        // dairesel listede null olmayacağı için sonsuz döngüyü headden başlatıp head e geelene kadar yapıyoruz


        void printPlaylist() {
            if (head == nullptr) {
                cout << "Oynatma listesi bos" << endl;
                return;
            }

            cout << "\n--- OYNATMA LISTESI ---" << endl;


            Node* temp = head;
            int index = 1;

            cout << index << ". " << temp->title << " - " << temp->artist;
            if (temp == currentSong) cout << " <== calan sarki";
            cout << endl;


            temp = temp->next;
            index++;


            while (temp != head) {
                cout << index << ". " << temp->title << " - " << temp->artist;
                if (temp == currentSong) cout << " <== calan sarki";
                cout << endl;

                temp = temp->next;
                index++;
            }

            cout << "-----------------------\n" << endl;
        }
    bool removeSong(string title) {
            if (head == nullptr) return false; // Liste boşsa direkt false dön

            Node* temp = head;
            bool found = false;
            bool firstStep = true; // Dairesel döngüyü başlatan bayrak

            // 1. ŞARKIYI ARAMA (Düz while ile)
            while (temp != head || firstStep) {
                firstStep = false; // İlk adımı geçtik

                if (temp->title == title) {
                    found = true; // Şarkı bulundu!
                    break;
                }
                temp = temp->next;
            }

            // Şarkı listede yoksa silmeden çık
            if (!found) return false;

            // 2. SİLME İŞLEMİ VE BAĞLANTILARI GÜNCELLEME

            // DURUM 1: Listede tek bir şarkı varsa ve o siliniyorsa
            if (head == tail && temp == head) {
                head = nullptr;
                tail = nullptr;
                currentSong = nullptr;
            }
            // DURUM 2: Listede birden fazla şarkı varsa
            else {
                // temp'in solundaki ve sağındaki elemanları birbirine bağlayıp temp'i aradan çıkarıyoruz
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;

                // Özel işaretçileri (pointer) kaydırma kontrolleri:
                if (temp == head) head = temp->next;         // Silinen ilk şarkıysa head'i kaydır
                if (temp == tail) tail = temp->prev;         // Silinen son şarkıysa tail'ı kaydır
                if (temp == currentSong) currentSong = temp->next; // Silinen çalan şarkıysa sonrakine geç
            }

            // 3. BELLEKTEN SİLME (Memory Leak Önleme)
            delete temp;
            cout << "'" << title << "' listeye veda etti ve silindi." << endl;
            return true;
        }




    };


int main() {


    Playlist myPlaylist;

    // Şarkı ekleme testleri
    myPlaylist.addSong("Neset Ertas", "Daglar dagladi beni");
    myPlaylist.addSong("Metallica", "Nothing else matters");
    myPlaylist.addSong("Rammstein", "Ich will");

    myPlaylist.printPlaylist();

    // Çalma ve Gezinme Testleri
    myPlaylist.playCurrent();
    myPlaylist.nextSong();
    myPlaylist.nextSong();

    // Tekrar kapalıyken son şarkıdan ileri gitme testi
    myPlaylist.nextSong();

    // Tekrar modunu açma ve döngü testi
    myPlaylist.toggleRepeat();
    myPlaylist.nextSong(); //

    // Silme Testi
    myPlaylist.removeSong("Ich will");
    myPlaylist.printPlaylist();
    return 0;
};