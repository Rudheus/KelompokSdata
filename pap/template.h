#include <iostream>
#include <fstream>
#include <sstream>
  // untuk std::istringstream
using namespace std;

struct Tiket
{
	string jenis_transportasi;
	string asal;
	string tujuan;
	string harga;
	string waktu;
	int jumlah_tiket;
	int indeks;
};

struct NodeTiket
{
	Tiket tiket;
	NodeTiket* next;
};

class Tiket_perjalanan
{
	public:
		Tiket_perjalanan(string transportasi);
		void tampil();
		void tambah(Tiket tiket);
		NodeTiket* pesan();
		void simpan_tiket();
		
	private:
		string path;
		NodeTiket* head;
		NodeTiket* tail;
};
Tiket_perjalanan::Tiket_perjalanan(string transportasi) {
    head = NULL;
    tail = NULL;

    if (transportasi == "pesawat") {
        path = "Databases/tiket/pesawat.txt";
    } else if (transportasi == "bus") {
        path = "Databases/tiket/bus.txt";
    } else if (transportasi == "kereta") {
        path = "Databases/tiket/kereta.txt";
    }

    ifstream output(path.c_str());
    if (!output.is_open()) {
        cout << "Gagal membuka file!\n";
        return;
    }

    string line;
    while (getline(output, line)) {
        istringstream iss(line);
        Tiket tiket1;

        // Membaca data dari file
        iss >> tiket1.jenis_transportasi;
        iss >> tiket1.asal;
        iss >> tiket1.tujuan;
        iss >> tiket1.harga;
        iss >> tiket1.waktu;
        iss >> tiket1.jumlah_tiket;
        

        // Membuat node baru
        NodeTiket* newnode = new NodeTiket();
        newnode->tiket = tiket1;
        newnode->next = NULL;

        // Menambahkan node ke linked list
        if (head == NULL) {
            head = newnode;
            tail = newnode;
        } else {
            tail->next = newnode;
            tail = newnode;
        }
    }
    output.close();
}


void Tiket_perjalanan::simpan_tiket() {
    ofstream input(path.c_str());
    NodeTiket* simpan = head;
    if (!input.is_open()) {
        cout << "Gagal membuka file!\n";
        return;
    }
    while (simpan != NULL) {
        input << simpan->tiket.jenis_transportasi << " "
              << simpan->tiket.asal << " "
              << simpan->tiket.tujuan << " "
              << simpan->tiket.harga << " "
              << simpan->tiket.waktu << " "
              << simpan->tiket.jumlah_tiket << " "
              << simpan->tiket.indeks << endl; 
        simpan = simpan->next;
    }
}


void Tiket_perjalanan::tampil() {
    NodeTiket* cetak = head;
    while (cetak != NULL) {
        cout << "Jenis Transportasi: " << cetak->tiket.jenis_transportasi << endl;
        cout << "Asal: " << cetak->tiket.asal << endl;
        cout << "Tujuan: " << cetak->tiket.tujuan << endl;
        cout << "Harga: " << cetak->tiket.harga << endl;
        cout << "Waktu: " << cetak->tiket.waktu << endl;
        cout << "Jumlah Tiket: " << cetak->tiket.jumlah_tiket << endl;
        cout << "---------------------------------------" << endl;
        cetak = cetak->next;
    }
}


void Tiket_perjalanan::tambah(Tiket tiket) {
    NodeTiket* newnode = new NodeTiket();
    newnode->tiket = tiket;
    newnode->next = NULL;

    if (head == NULL) {
        head = newnode;
        tail = newnode;
    } else {
        tail->next = newnode;
        tail = newnode;
    }
}
NodeTiket* Tiket_perjalanan::pesan() {
    string asal, tujuan;
    cout << "Masukkan asal kota: ";
    cin >> asal;
    cout << "Masukkan tujuan: ";
    cin >> tujuan;

    NodeTiket* cek = head;
    int maxIndeks = 0;

    // Cari indeks terbesar yang sudah ada
    while (cek != NULL) {
        if (cek->tiket.indeks > maxIndeks) {
            maxIndeks = cek->tiket.indeks;
        }
        cek = cek->next;
    }

    cek = head; // Kembali ke awal untuk pencarian tiket
    while ((cek != NULL) && (cek->tiket.asal != asal || cek->tiket.tujuan != tujuan)) {
        cek = cek->next;
    }

    if (cek != NULL && cek->tiket.asal == asal && cek->tiket.tujuan == tujuan) {
        char pil;
        cout << "Jenis Transportasi: " << cek->tiket.jenis_transportasi << endl;
        cout << "Asal: " << cek->tiket.asal << endl;
        cout << "Tujuan: " << cek->tiket.tujuan << endl;
        cout << "Harga: " << cek->tiket.harga << endl;
        cout << "Waktu: " << cek->tiket.waktu << endl;
        cout << "Jumlah Tiket: " << cek->tiket.jumlah_tiket << endl;
        cout << "---------------------------------------" << endl;
        cout << "Apakah anda ingin memesan tiket ini? [Y/N]: ";
        cin >> pil;

        if (pil == 'Y' || pil == 'y') {
            cek->tiket.jumlah_tiket--;  // Kurangi jumlah tiket
            cek->tiket.indeks = maxIndeks + 1;  // Tetapkan indeks unik baru
            simpan_tiket();  // Simpan perubahan ke file
            return cek;  // Kembalikan node yang dipesan
        }
    }
    return NULL;  // Jika tidak ditemukan
}



class Keranjang {
public:
    Keranjang();
    void muat_keranjang(string nama);
    void tampil_keranjang(string nama);
    void tambah_pesan(NodeTiket* x, string nama);
    void simpan_keranjang(string nama);
    NodeTiket* pop();
   	void bayar_semua(const std::string& nama);
	void bayar_pertama(const std::string& nama);
	void hapus_tiket_keranjang(int indeks);
	void bayar_tiket_keranjang();
	void perbarui_indeks();
private:
    bool status;
    NodeTiket* head;
    NodeTiket* tail;
    string nama;
};
void Keranjang::bayar_tiket_keranjang() {
    if (head == NULL) {
        cout << "Keranjang kosong, tidak ada tiket untuk dibayar!" << endl;
        return;
    }

    bool selesai = false;

    while (!selesai) {
        // Tampilkan isi keranjang
        tampil_keranjang(nama);

        cout << "Masukkan indeks tiket yang ingin dibayar (atau -1 untuk selesai): ";
        int indeks;
        cin >> indeks;

        if (indeks == -1) {
            selesai = true; // Keluar dari perulangan
            cout << "Pembayaran selesai." << endl;
            continue;
        }

        NodeTiket* current = head;
        NodeTiket* previous = NULL;

        // Cari tiket dengan indeks yang sesuai
        while (current != NULL && current->tiket.indeks != indeks) {
            previous = current;
            current = current->next;
        }

        if (current == NULL) {
            cout << "Tiket dengan indeks " << indeks << " tidak ditemukan di keranjang." << endl;
            continue;
        }

        // Hitung total harga tiket
        stringstream ss(current->tiket.harga);
        int harga;
        ss >> harga;
        cout << "Total yang harus dibayar untuk tiket ini: Rp " << harga << endl;

        // Hapus tiket dari linked list
        if (previous == NULL) {
            // Jika node yang dihapus adalah head
            head = current->next;
        } else {
            previous->next = current->next;
        }

        if (current == tail) {
            // Jika node yang dihapus adalah tail
            tail = previous;
        }

        delete current;

        // Perbarui file keranjang setelah penghapusan
        simpan_keranjang(nama);

        cout << "Tiket dengan indeks " << indeks << " berhasil dibayar." << endl;
    }
}

void Keranjang::simpan_keranjang(string nama) {
    NodeTiket* simpan = head;
    string fileName = "./Databases/user_buy_list/" + nama + "_keranjang.txt";
    ofstream input(fileName.c_str());
    if (!input.is_open()) {
        cout << "Gagal membuka file!\n";
        return;
    }
    while (simpan != NULL) {
        input << simpan->tiket.indeks << " " // Tambahkan indeks
              << simpan->tiket.jenis_transportasi << " "
              << simpan->tiket.asal << " "
              << simpan->tiket.tujuan << " "
              << simpan->tiket.harga << " "
              << simpan->tiket.waktu << endl;
        simpan = simpan->next;
    }
    input.close();
}

void Keranjang::perbarui_indeks() {
    NodeTiket* current = head;
    int indeks = 1; // Mulai dari 1
    while (current != NULL) {
        current->tiket.indeks = indeks;
        current = current->next;
        indeks++;
    }
}

Keranjang::Keranjang() {
    status = true;
    head = NULL;
    tail = NULL;
    nama = ""; // Initialize nama
}

// Menambahkan tiket ke keranjang (push)
void Keranjang::tambah_pesan(NodeTiket* x, string nama) {
    if (status) {
        muat_keranjang(nama); // Memuat keranjang dari file jika belum dimuat
        status = false;
    }
    if (x != NULL) {
        NodeTiket* newnode = new NodeTiket();
        newnode->tiket = x->tiket;

        // Tetapkan indeks baru berdasarkan maksimum yang ada di keranjang
        NodeTiket* temp = head;
        int maxIndeks = 0;
        while (temp != NULL) {
            if (temp->tiket.indeks > maxIndeks) {
                maxIndeks = temp->tiket.indeks;
            }
            temp = temp->next;
        }
        newnode->tiket.indeks = maxIndeks + 1;

        // Tambahkan ke depan sesuai prinsip LIFO
        newnode->next = head;
        head = newnode;

        // Simpan ke file keranjang pengguna
        string fileName = "./Databases/user_buy_list/" + nama +"_keranjang.txt";
        ofstream input(fileName.c_str(), ios::app);
        if (!input.is_open()) {
            cout << "Gagal membuka file pengguna!\n";
            return;
        }
        input << newnode->tiket.indeks << " "
              << newnode->tiket.jenis_transportasi << " "
              << newnode->tiket.asal << " "
              << newnode->tiket.tujuan << " "
              << newnode->tiket.harga << " "
              << newnode->tiket.waktu << endl;
        input.close();

        cout << "Tiket berhasil ditambahkan ke keranjang.\n";
    } else {
        cout << "Tiket tidak ditemukan atau pemesanan batal.\n";
    }
}

// Pop: Menghapus tiket terakhir dari keranjang (LIFO)
NodeTiket* Keranjang::pop() {
    if (head == NULL) {
        cout << "Keranjang kosong, tidak ada tiket untuk dihapus!" << endl;
        return NULL;
    }

    NodeTiket* temp = head;
    head = head->next;
    temp->next = NULL;

    // Simpan perubahan ke file setelah pop
    simpan_keranjang(nama); // tambahkan nama sebagai parameter di fungsi
    return temp;
}

// Menampilkan tiket dalam keranjang
void Keranjang::tampil_keranjang(string nama) {
    this->nama = nama; // Simpan nama untuk digunakan fungsi lain
    if (status) {
        muat_keranjang(nama);
        status = false;
    }
    NodeTiket* cetak = head;
  	cout << "Daftar tiket di keranjang:\n";
    while (cetak != NULL) {
        cout << "Indeks: " << cetak->tiket.indeks << endl; // Tambahkan indeks
        cout << "Jenis Transportasi: " << cetak->tiket.jenis_transportasi << endl;
        cout << "Asal: " << cetak->tiket.asal << endl;
        cout << "Tujuan: " << cetak->tiket.tujuan << endl;
        cout << "Harga: " << cetak->tiket.harga << endl;
        cout << "Waktu: " << cetak->tiket.waktu << endl;
        cout << "---------------------------------------" << endl;
        cetak = cetak->next;
    }
}

// Pembayaran semua tiket di keranjang
void Keranjang::bayar_semua(const string& nama) {
    if (head == NULL) {
        cout << "Keranjang kosong, tidak ada tiket untuk dibayar!" << endl;
        return;
    }

    int total = 0;
    NodeTiket* cetak = head;

    // Buka file refund untuk ditambahkan
    string refundPath = "./Databases/refund/" + nama + "_refund.txt";
    ofstream refundFile(refundPath.c_str(), ios::app);
    if (!refundFile.is_open()) {
        cout << "Gagal membuka file refund untuk penyimpanan.\n";
        return;
    }

    while (cetak != NULL) {
        // Hitung total harga tiket
        stringstream ss(cetak->tiket.harga);
        int harga;
        ss >> harga;
        total += harga;

        // Tambahkan tiket ke file refund
        refundFile << cetak->tiket.jenis_transportasi << " "
                   << cetak->tiket.asal << " "
                   << cetak->tiket.tujuan << " "
                   << cetak->tiket.harga << " "
                   << cetak->tiket.waktu << endl;

        cetak = cetak->next;
    }
    refundFile.close();

    // Hapus semua tiket dari keranjang
    while (head != NULL) {
        NodeTiket* temp = pop();
        delete temp;
    }

    cout << "Total yang harus dibayar: " << total << endl;
    cout << "Pembayaran selesai. Tiket telah dipindahkan ke file refund.\n";
}

void Keranjang::bayar_pertama(const string& nama) {
    if (head == NULL) {
        cout << "Keranjang kosong, tidak ada tiket untuk dibayar!" << endl;
        return;
    }

    // Cari tiket pertama (FIFO)
    NodeTiket* current = head;
    NodeTiket* previous = NULL;

    while (current->next != NULL) {
        previous = current;
        current = current->next;
    }

    // Hitung total harga tiket pertama
    stringstream ss(current->tiket.harga);
    int harga;
    ss >> harga;
    cout << "Total yang harus dibayar: " << harga << endl;

    // Tambahkan tiket ke file refund
    string refundPath = "./Databases/refund/" + nama + "_refund.txt";
    ofstream refundFile(refundPath.c_str(), ios::app);
    if (!refundFile.is_open()) {
        cout << "Gagal membuka file refund untuk penyimpanan.\n";
        return;
    }
    refundFile << current->tiket.jenis_transportasi << " "
               << current->tiket.asal << " "
               << current->tiket.tujuan << " "
               << current->tiket.harga << " "
               << current->tiket.waktu << endl;
    refundFile.close();

    // Hapus tiket dari keranjang
    if (previous == NULL) {
        head = head->next;  // Jika hanya satu elemen
    } else {
        previous->next = current->next;
    }
    if (current == tail) {
        tail = previous;  // Perbarui tail jika elemen terakhir
    }

    delete current;
	perbarui_indeks();
    cout << "Pembayaran selesai. Tiket telah dipindahkan ke file refund.\n";
}
void Keranjang::hapus_tiket_keranjang(int indeks) {
    if (head == NULL) {
        cout << "Keranjang kosong, tidak ada tiket untuk dihapus!" << endl;
        return;
    }

    NodeTiket* current = head;
    NodeTiket* previous = NULL;

    // Cari tiket dengan indeks yang sesuai
    while (current != NULL && current->tiket.indeks != indeks) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        cout << "Tiket dengan indeks " << indeks << " tidak ditemukan di keranjang.\n";
        return;
    }

    // Hapus tiket dari linked list
    if (previous == NULL) {
        // Jika node yang dihapus adalah head
        head = current->next;
    } else {
        previous->next = current->next;
    }

    if (current == tail) {
        // Jika node yang dihapus adalah tail
        tail = previous;
    }

    delete current;
	perbarui_indeks();
    // Perbarui file keranjang setelah penghapusan
    simpan_keranjang(nama);
 
    cout << "Tiket dengan indeks " << indeks << " berhasil dihapus dari keranjang.\n";
}
// Fungsi untuk memuat data keranjang dari file
void Keranjang::muat_keranjang(string nama) {
    string fileName = "./Databases/user_buy_list/" + nama + "_keranjang.txt";
    ifstream output(fileName.c_str());
    if (!output.is_open()) {
        cout << "Gagal membuka file keranjang pengguna!\n";
        return;
    }

    string line;
    while (getline(output, line)) {
        istringstream iss(line);
        Tiket tiket1;
        iss >> tiket1.indeks; // Baca indeks
        iss >> tiket1.jenis_transportasi;
        iss >> tiket1.asal;
        iss >> tiket1.tujuan;
        iss >> tiket1.harga;
        iss >> tiket1.waktu;

        NodeTiket* newnode = new NodeTiket();
        newnode->tiket = tiket1;
        newnode->next = head;
        head = newnode;
    }
    output.close();
    cout << "Keranjang pengguna berhasil dimuat.\n";
}

class Pembayaran
{
	private:
		int total;
		string metode_bayar;
};
class RefundHandler {
private:
    struct RefundNode {
        Tiket tiket;
        RefundNode* next;
    };
    RefundNode* top;
    string path;

public:
    RefundHandler() : top(NULL) {}
void loadRefundsFromFile() {
    ifstream file(path.c_str());
    if (!file.is_open()) {
        cout << "Gagal membuka file refund untuk membaca.\n";
        return;
    }

    string jenis_transportasi, asal, tujuan, harga, waktu;
    while (file >> jenis_transportasi >> asal >> tujuan >> harga >> waktu) {
        Tiket tiket;
        tiket.jenis_transportasi = jenis_transportasi;
        tiket.asal = asal;
        tiket.tujuan = tujuan;
        tiket.harga = harga;
        tiket.waktu = waktu;

        // Tambahkan ke stack
        RefundNode* newNode = new RefundNode();
        newNode->tiket = tiket;
        newNode->next = top;
        top = newNode;
    }

    file.close();
}

    void setRefundPath(const string& username) {
    path = "./Databases/refund/" + username + "_refund.txt";

    // Buat file jika belum ada
    ofstream file(path.c_str(), ios::app);
    if (file.is_open()) {
        file.close();
    }

    // Muat data refund yang ada ke stack
    loadRefundsFromFile();
}


   void pushRefund(const Tiket& tiket) {
    // Tambahkan tiket ke stack
    cout << "Menambahkan tiket ke stack.\n";
    RefundNode* newNode = new RefundNode();
    newNode->tiket = tiket;
    newNode->next = top;
    top = newNode;

    // Simpan tiket ke file refund
    ofstream file(path.c_str(), ios::app);
    if (file.is_open()) {
        file << tiket.jenis_transportasi << " "
             << tiket.asal << " "
             << tiket.tujuan << " "
             << tiket.harga << " "
             << tiket.waktu << endl;
        cout << "Tiket berhasil disimpan ke file.\n";
        file.close();
    } else {
        cout << "Gagal membuka file refund untuk penyimpanan.\n";
    }
}


    Tiket popRefund() {
        if (top == NULL) {
            cout << "Tidak ada tiket untuk direfund.\n";
            return Tiket(); // Kembalikan tiket kosong
        }

        // Hapus tiket dari stack
        RefundNode* temp = top;
        Tiket tiket = temp->tiket;
        top = top->next;
        delete temp;

        // Perbarui file refund
        updateRefundFile();

        return tiket;
    }

    void displayRefunds() {
    if (top == NULL) {
        cout << "Tidak ada tiket yang tersedia untuk refund.\n";
        return;
    }

    RefundNode* current = top;
    while (current != NULL) {
        cout << "Jenis Transportasi: " << current->tiket.jenis_transportasi << endl
             << "Asal: " << current->tiket.asal << endl
             << "Tujuan: " << current->tiket.tujuan << endl
             << "Harga: " << current->tiket.harga << endl
             << "Waktu: " << current->tiket.waktu << endl
             << "---------------------------------------" << endl;
        current = current->next;
    }
}


private:
    void updateRefundFile() {
        ofstream file(path.c_str());
        if (!file.is_open()) {
            cout << "Gagal membuka file refund untuk pembaruan.\n";
            return;
        }

        RefundNode* current = top;
        while (current != NULL) {
            file << current->tiket.jenis_transportasi << " "
                 << current->tiket.asal << " "
                 << current->tiket.tujuan << " "
                 << current->tiket.harga << " "
                 << current->tiket.waktu << endl;
            current = current->next;
        }

        file.close();
    }
};


class User_ui {
private:
    string nama;
    Tiket_perjalanan tiketbus;
    Tiket_perjalanan tiketpesawat;
    Tiket_perjalanan tiketkereta;
    Keranjang keranjang_user;
    Keranjang tiket_user;
	RefundHandler refundHandler;
public:
    User_ui() : tiketbus("bus"), tiketpesawat("pesawat"), tiketkereta("kereta") {}
    
    void setRefundPath(string username) {
        refundHandler.setRefundPath(username);
        // Buat file refund jika belum ada
        string refundPath = "./Databases/refund/" + username + "_refund.txt";
        ofstream file(refundPath.c_str(), ios::app);
        if (file.is_open()) {
            file.close();
        }
    }
    
    void proses(string nama) {
        this->nama = nama;
        RefundHandler refundHandler;
        refundHandler.setRefundPath(nama);
        
        bool kondisi = true;
        while (kondisi) {
        	char pil2;
            int pil, tipe;
            NodeTiket* x;
            cout << " _______________________________________________ \n";
            cout << "[\t\t  ~ MENU ~\t\t\t]\n";
            cout << "|\t\t    ---- \t\t\t|\n";
            cout << "|\t1. Lihat tiket tersedia\t\t\t|\n";
            cout << "|\t\t         \t\t\t|\n";
			cout << "|\t2. Lihat keranjang\t\t\t|\n";
			cout << "|\t\t         \t\t\t|\n";
			cout << "|\t3. Pesan tiket\t\t\t\t|\n";
			cout << "|\t\t         \t\t\t|\n";
			cout << "|\t4. Hapus tiket terakhir di keranjang\t|\n";
			cout << "|\t\t         \t\t\t|\n";
			cout << "|\t5. Bayar seluruh tiket di keranjang\t|\n";
			cout << "|\t\t         \t\t\t|\n";
			cout << "|\t6. Hapus tiket berdasarkan indeks\t|\n";
			cout << "|\t\t         \t\t\t|\n";
			cout << "|\t7. Bayar tiket\t\t\t\t|\n";
            cout << "|\t\t         \t\t\t|\n";
            cout << "|\t8. Bayar tiket yang masuk pertama\t|\n";
            cout << "|\t\t         \t\t\t|\n";
			cout << "|\t9. Lihat tiket dari telah dibayar\t|\n";
			cout << "|\t\t         \t\t\t|\n";
			cout << "|\t10. Refund Tiket\t\t\t|\n";
			cout << "|\t\t         \t\t\t|\n";
			cout << "|\t11. Logout\t\t\t\t|\n";
			cout << "[_______________________________________________]\n\n";
            cout << "Masukkan pilihan: ";
            cin >> pil;
            
        switch (pil) {
        	
            case 1:
            	cout << "1. Tiket Bus\n2. Tiket Pesawat\n3. Tiket Kereta\n";
                cout << "Masukkan jenis tiket yang ingin dipesan: ";
                cin >> tipe;
                if (tipe == 1) {
                    tiketbus.tampil();
                } else if (tipe == 2) {
                    tiketpesawat.tampil();
                } else if (tipe == 3) {
                    tiketkereta.tampil();
                } else {
                    cout << "Pilihan tidak valid.\n";
                }
                
                system("pause");
                system("cls");
                // Pilihan untuk melihat tiket tersedia
                break;
            case 2:
                // Pilihan untuk melihat keranjang
                keranjang_user.tampil_keranjang(nama);
                break;
            case 3:
			    do {
			        cout << "1. Tiket Bus\n2. Tiket Pesawat\n3. Tiket Kereta\n";
			        cout << "Masukkan jenis tiket yang ingin dipesan: ";
			        cin >> tipe;
			
			        if (tipe == 1) {
			            tiketbus.tampil();
			            x = tiketbus.pesan();
			            keranjang_user.tambah_pesan(x, nama);
			        } else if (tipe == 2) {
			            tiketpesawat.tampil();
			            x = tiketpesawat.pesan();
			            keranjang_user.tambah_pesan(x, nama);
			        } else if (tipe == 3) {
			            tiketkereta.tampil();
			            x = tiketkereta.pesan();
			            keranjang_user.tambah_pesan(x, nama);
			        } else {
			            cout << "Pilihan tidak valid.\n";
			        }
			
			        cout << "Ingin pesan lagi? [y/n]: ";
			        cin >> pil2;
			    } while (pil2 != 'n' && pil2 != 'N');
			    break;

            case 4:
                // Hapus tiket terakhir dari keranjang
                keranjang_user.pop();
                //keranjang_user.simpan_keranjang(nama);
                cout << "Tiket terakhir telah dihapus.\n";
                break;
           case 5:
			    // Bayar semua tiket di keranjang
			    keranjang_user.bayar_semua(nama);
			    break;
			case 6: {
				keranjang_user.tampil_keranjang(nama); 
			    cout << "Masukkan indeks tiket yang ingin dihapus: ";
			    int indeks;
			    cin >> indeks;
			    keranjang_user.hapus_tiket_keranjang(indeks);
			
			    
			    break;
			}

			case 7: {
			    keranjang_user.bayar_tiket_keranjang();
			    break;
				}

			case 8:
			    // Bayar tiket pertama di keranjang
			    keranjang_user.bayar_pertama(nama);
			    break;

            case 9:
            	tiket_user.tampil_keranjang(nama);
            	break;
            case 10: 
				{
    				    cout << "Menu Refund:\n";
    				    cout << "1. Tambah Refund\n2. Lihat Refund\n3. Proses Refund\n";
    				    int refundChoice;
    				    cin >> refundChoice;
    				
    				    switch (refundChoice) {
    				    case 1: {
    				        // Ambil tiket terakhir dari keranjang untuk refund
    				        NodeTiket* ticket = keranjang_user.pop();
    				        if (ticket != NULL) {
    				            refundHandler.pushRefund(ticket->tiket);
    				            cout << "Refund berhasil ditambahkan ke daftar refund.\n";
    				        }
    				        break;
    				    }
    				    case 2:
    				        // Tampilkan tiket yang ada di daftar refund
    				        refundHandler.displayRefunds();
    				        break;
    				    case 3: {
    				        // Proses refund tiket
    				        Tiket refundedTicket = refundHandler.popRefund();
    				        if (!refundedTicket.jenis_transportasi.empty()) {
    				            cout << "Refund berhasil diproses untuk tiket:\n";
    				            cout << "Dari: " << refundedTicket.asal << " Ke: " << refundedTicket.tujuan << endl;
    				        }
    				        break;
    				    }
    				    default:
    				        cout << "Pilihan tidak valid.\n";
    				    }
    				    break;
    			}
            case 11:
                kondisi = false;
                cout << "Anda telah logout!\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
        
    	}
	}
};	
