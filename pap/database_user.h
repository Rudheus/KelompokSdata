#include <iostream>
#include <fstream>
#include <sstream>
#include "template.h"

using namespace std;

struct Ktp
{
    string nama;
    string nik;
};

struct user
{
    Ktp ktp;
    string password;
    User_ui user_ui;
    user *next;
};

class database_user
{
	public:
    	database_user();
    	user *current;
    	void tambah(Ktp data, string password);
    	void tampil();
    	bool cek(string usernem, string pass);
    	void proses();
		void pesan();
	private:
    	user *head;
    	user *tail;
    	
};

database_user::database_user()
{
    head = NULL;
    tail = NULL;    
	ifstream output("./Databases/user_list/data.txt", ios::app);    
	if (!output.is_open())
    {
    	cout << "Gagal membuka file!\n";
		return;
	}
	string line;
	string token;
	
	while (getline(output, line))
	{
        istringstream iss(line);
		Ktp ktp;
		string pass;
		iss>>token;
		ktp.nama=token;
        iss>>token;
        ktp.nik=token;
		iss>>token;
        pass=token;
        
        user *newnode = new user();
        newnode->ktp = ktp;
    	newnode->password = pass;
    	if (head == NULL)
    	{
    		head = newnode;
    	    tail = newnode;
			tail->next=NULL;  // Mengatur tail saat list kosong
    	}
    	else
    	{
    		newnode->next = head;
    		head = newnode;
		}
    	
	}
    output.close();
}
    
void database_user::tambah(Ktp data, string password)
{
	user *newnode = new user();
    newnode->ktp = data;
    newnode->password = password;
    newnode->next = head;
    if (head == NULL)
    {
        tail = newnode;  // Mengatur tail saat list kosong
    }
    head = newnode;
    ofstream input("./Databases/user_list/data.txt", ios::app);    
	if (!input.is_open())
    {
    	cout << "Gagal membuka file!\n";
		return;
	}
	input << newnode->ktp.nama << " ";
	input << newnode->ktp.nik << " ";
	input << newnode->password << endl;
	input.close();
	
	 string fileName = "./Databases/user_buy_list/" + newnode->ktp.nama + "_keranjang.txt";
    ofstream keranjangFile(fileName.c_str()); // Menggunakan `c_str()` agar kompatibel dengan compiler lama
    if (!keranjangFile.is_open())
    {
        cout << "Gagal membuat file keranjang untuk pengguna: " << newnode->ktp.nama << "\n";
        return;
    }
    //keranjangFile << "Keranjang belanja untuk: " << newnode->ktp.nama << endl;  // Tambahkan baris awal opsional
    keranjangFile.close();

    cout << "Akun dan file keranjang berhasil dibuat untuk: " << newnode->ktp.nama << "\n";
    system("pause");
}

    
void database_user::tampil()
{
    user *cetak = head;
    while (cetak != NULL)
    {
    	cout << "===================\n";
    	cout << "Nama: " << cetak->ktp.nama << endl;
        cout << "NIK: " << cetak->ktp.nik << endl;
        cout << "Password: " << cetak->password << endl;
        cout << "===================\n";
        cetak = cetak->next;
    }
}

bool database_user::cek(string usernem, string pass)
{
	cout <<"  Username : "<<usernem << endl;
	cout <<"  pasword  : "<<pass << endl;
	user *cek1= head;
	while(cek1!=NULL)
	{
		if(usernem==cek1->ktp.nik && pass==cek1->password)
		{
			cout << "+===================+\n";
			cout << "|  akun ditemukan!  |" << endl;
			cout << "+===================+\n";
			current=cek1;
			cout << "  anda login sebagai " << cek1->ktp.nama << endl;
			system("pause");
			system("cls");
			return true;
		}
		cek1= cek1->next;
	}
	if(cek1==NULL)
	{
		cout << "akun tidak ditemukan!\n";
		return false;
	}
	
}

void database_user::proses()
{
	int pil1;
	string userxx;
	string passxx;
	
	Ktp data;
	
	bool kondisi=true;
	
	while(kondisi)
	{
		
		//tampilan untuk login
		cout << "[---------------------------------------]\n";
		cout << "|\t\t ~ WELCOME ~\t\t|\n";
		cout << "|_______________________________________|\n";
		cout << "|\t\t         \t\t|\n";
		cout << "|\t\t1. login\t\t|\n";
		cout << "|\t\t         \t\t|\n";
		cout << "|\t\t2. daftar\t\t|\n";
		cout << "|\t\t         \t\t|\n";
		cout << "|  -----------------------------------  |\n";
		cout << "|\t\t\t+--------+\t|\n";
		cout << "|\tKLIK 3 untuk \t| keluar |\t|\n";
		cout << "|\t\t\t+--------+\t|\n";
		cout << "|_______________________________________|\n";
		cout<<endl;
		cout << " masukkan pilihan: ";
		cin >> pil1;
		system("cls");
		switch (pil1)
		{
			case 1:
				cout << "_________Silahkan Login________\n";
				cout <<endl;
				cout << "  Masukkan Username : ";
				cin >> userxx;
				cout << "  Masukkan Password : ";
				cin >> passxx;
				if(cek(userxx, passxx))
				{
					current->user_ui.proses(current->ktp.nama);
				}
				break;
			case 2:
				cout << "_________Silahkan Daftar________\n";
				cout <<endl;
				cout << "masukkan nama: ";
				cin.ignore();
				getline(cin, data.nama);
				cout << "masukkan nik (digunakan sebagai username): ";
				cin >> data.nik;
				cout << "masukkan password untuk akun baru: ";
				cin >> passxx;
				tambah(data, passxx);
				system("cls");
				break;
			case 3:
				cout << "\tKeluar aplikasi berhasil. Terima kasih atas penggunaan Anda. ";
				kondisi= false;
				break;
			default:
				cout << "pilihan tidak valid!!\n\n";
				break;
		}
	}
}
