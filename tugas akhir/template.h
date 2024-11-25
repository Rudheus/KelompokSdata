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
        path = "./Databases/tiket/pesawat.txt";
    } else if (transportasi == "bus") {
        path = "./Databases/tiket/bus.txt";
    } else if (transportasi == "kereta") {
        path = "./Databases/tiket/kereta.txt";
    }

    ifstream output(path.c_str()); // Perbaikan di sini
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
        NodeTiket *newnode = new NodeTiket();
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

void Tiket_perjalanan::simpan_tiket()
{
	ofstream input(path.c_str()); // Perbaikan di sini
	NodeTiket* simpan=head;
    if (!input.is_open()) {
        	cout << "Gagal membuka file!\n";
        	return;
    	}
    while(simpan!=NULL)
    {
    	input<<simpan->tiket.jenis_transportasi << " ";
    	input <<simpan->tiket.asal << " ";
    	input <<simpan->tiket.tujuan << " ";
    	input <<simpan->tiket.harga << " ";
    	input <<simpan->tiket.waktu << " ";
    	input <<simpan->tiket.jumlah_tiket << endl;
    	simpan=simpan->next;
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
            cek->tiket.jumlah_tiket--; // Mengurangi jumlah tiket
            simpan_tiket();
            return cek; // Mengembalikan node yang dipesan
        }
    }
    return NULL; // Jika tidak ditemukan
}


class Keranjang
{
public:
    Keranjang();
    void muat_keranjang(string nama);
    void tampil_keranjang(string nama);
    void tambah_pesan(NodeTiket* x, string nama); // Push tiket
    void simpan_keranjang(string nama);
    NodeTiket* pop();  // Pop tiket
    void bayar_semua(); // Pembayaran dan hapus semua tiket

private:
    bool status;
    NodeTiket* head;
    NodeTiket* tail;
};

Keranjang::Keranjang() {
    status = true;
    head = NULL;
    tail = NULL;
}

// Menambahkan tiket ke keranjang (push)
void Keranjang::tambah_pesan(NodeTiket* x, string nama) {
    if (status) {
        muat_keranjang(nama);
        status = false;
    }
    if (x != NULL) {
        NodeTiket* newnode = new NodeTiket();
        newnode->tiket = x->tiket;
        newnode->tiket.jumlah_tiket = 0;  // Jangan ubah jumlah tiket
        newnode->next = head;  // Menambahkan di depan, mengikuti prinsip stack (LIFO)
        head = newnode;

        // Simpan ke file
        string fileName = "D:/tugas akhir/Databases/user_buy_list/" + nama + "_keranjang.txt";
        ofstream input(fileName.c_str(), ios::app);
        if (!input.is_open()) {
            cout << "Gagal membuka file!\n";
            return;
        }
        input << newnode->tiket.jenis_transportasi << " ";
        input << newnode->tiket.asal << " ";
        input << newnode->tiket.tujuan << " ";
        input << newnode->tiket.harga << " ";
        input << newnode->tiket.waktu << endl;
        input.close();
    }
    else {
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
    temp->next = NULL; // Disconnect the node from the list
    return temp;
}

// Menampilkan tiket dalam keranjang
void Keranjang::tampil_keranjang(string nama) {
    if (status) {
        muat_keranjang(nama);
        status = false;
    }
    NodeTiket* cetak = head;
    while (cetak != NULL) {
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
void Keranjang::bayar_semua() {
    if (head == NULL) {
        cout << "Keranjang kosong, tidak ada tiket untuk dibayar!" << endl;
        return;
    }

    int total = 0;
    NodeTiket* cetak = head;
    while (cetak != NULL) {
        // Menghitung total harga tiket
        // Menggunakan stringstream untuk konversi harga dari string ke integer
        stringstream ss(cetak->tiket.harga);
        int harga;
        ss >> harga;

        total += harga; // Menambahkan harga ke total
        cetak = cetak->next;
    }
    cout << "Total yang harus dibayar: " << total << endl;

    // Menghapus semua tiket setelah pembayaran
    while (head != NULL) {
        NodeTiket* temp = pop();
        delete temp; // Menghapus node setelah pop
    }

    cout << "Pembayaran selesai. Keranjang kosong sekarang.\n";
}

// Fungsi untuk memuat data keranjang dari file
void Keranjang::muat_keranjang(string nama) {
    string fileName = "D:/tugas akhir/Databases/user_buy_list/" + nama + "_keranjang.txt";
    ifstream output(fileName.c_str());
    if (!output.is_open()) {
        cout << "Gagal membuka file!\n";
        return;
    }

    string line;
    while (getline(output, line)) {
        istringstream iss(line);
        Tiket tiket1;
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
}


class Pembayaran
{
	private:
		int total;
		string metode_bayar;
};



class User_ui
{
public:
    User_ui() : tiketbus("bus"), tiketpesawat("pesawat"), tiketkereta("kereta"){}

    void proses(string nama);
    void pesan();

private:
	string nama;
    Tiket_perjalanan tiketbus;
    Tiket_perjalanan tiketpesawat;
    Tiket_perjalanan tiketkereta;
    Keranjang keranjang_user;
};


void User_ui::proses(string nama) {
    bool kondisi = true;
    while (kondisi) {
        int pil, tipe;
        NodeTiket* x;
        cout << "1. Lihat tiket tersedia\n2. Lihat keranjang\n3. Pesan tiket\n4. Hapus tiket terakhir di keranjang\n5. Bayar semua tiket\n6. Logout\n";
        cout << "Masukkan pilihan: ";
        cin >> pil;
        switch (pil) {
            case 1:
                // Pilihan untuk melihat tiket tersedia
                break;
            case 2:
                // Pilihan untuk melihat keranjang
                keranjang_user.tampil_keranjang(nama);
                break;
            case 3:
                // Pilihan untuk memesan tiket
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
                break;
            case 4:
                // Hapus tiket terakhir dari keranjang
                keranjang_user.pop();
                cout << "Tiket terakhir telah dihapus.\n";
                break;
            case 5:
                // Bayar semua tiket di keranjang
                keranjang_user.bayar_semua();
                break;
            case 6:
                kondisi = false;
                cout << "Anda telah logout!\n";
                break;
            default:
                cout << "Pilihan tidak valid.\n";
        }
    }
}


