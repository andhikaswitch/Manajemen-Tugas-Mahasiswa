#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <limits> // Added for numeric_limits
#include <sstream> // Added for string parsing
#include <algorithm>
#include <cctype>

// Add this for cross-platform screen clearing
#ifdef _WIN32
#include <windows.h>
#else
#include <cstdlib>
#endif

using namespace std;

// Function to clear the screen (cross-platform)
void clearScreen() {
#ifdef _WIN32
    // For Windows
    system("cls");
#else
    // For Unix/Linux/MacOS
    system("clear");
#endif
}

// Struktur untuk tanggal
struct Tanggal {
    int hari;
    int bulan;
    int tahun;

    Tanggal(int h = 0, int b = 0, int t = 0) : hari(h), bulan(b), tahun(t) {}

    string toString() const {
        return to_string(hari) + "/" + to_string(bulan) + "/" + to_string(tahun);
    }

    bool operator<(const Tanggal& lain) const {
        if (tahun != lain.tahun) return tahun < lain.tahun;
        if (bulan != lain.bulan) return bulan < lain.bulan;
        return hari < lain.hari;
    }

    bool operator>(const Tanggal& lain) const {
        if (tahun != lain.tahun) return tahun > lain.tahun;
        if (bulan != lain.bulan) return bulan > lain.bulan;
        return hari > lain.hari;
    }

    bool operator==(const Tanggal& lain) const {
        return tahun == lain.tahun && bulan == lain.bulan && hari == lain.hari;
    }
};

// Deklarasi awal
class Tugas;

// Struktur pohon untuk kategori tugas
class Kategori {
public:
    string nama;
    vector<Kategori*> subkategori;
    vector<Tugas*> daftarTugas;
    Kategori* induk;

    Kategori(string nama, Kategori* induk = nullptr) : nama(nama), induk(induk) {}

    void tambahSubkategori(Kategori* subkategori) {
        this->subkategori.push_back(subkategori);
    }

    void tambahTugas(Tugas* tugas) {
        daftarTugas.push_back(tugas);
    }

    void hapusTugas(Tugas* tugas) {
        daftarTugas.erase(
            remove(daftarTugas.begin(), daftarTugas.end(), tugas),
            daftarTugas.end()
        );
    }

    // Tampilkan hierarki kategori - implementasi dipindahkan setelah Tugas didefinisikan
    void tampilkan(int kedalaman = 0);

    // Mendapatkan path lengkap kategori
    string getFullPath() const {
        if (induk == nullptr || induk->induk == nullptr) { // Jika induk adalah root atau tidak ada
            return nama;
        }
        return induk->getFullPath() + "/" + nama;
    }

    ~Kategori() {
        for (auto subkategori : this->subkategori) {
            delete subkategori;
        }
    }
};

// Definisi kelas Tugas
class Tugas {
public:
    int id;
    string judul;
    string deskripsi;
    Tanggal tenggat;
    int prioritas; // 1-5, dimana 1 adalah prioritas tertinggi
    bool selesai;
    Kategori* kategori;

    Tugas(int id, string judul, string deskripsi, Tanggal tenggat, int prioritas, Kategori* kategori = nullptr)
        : id(id), judul(judul), deskripsi(deskripsi), tenggat(tenggat),
          prioritas(prioritas), selesai(false), kategori(kategori) {}

    void tampilkan() const {
        cout << "ID: " << id << endl;
        cout << "Judul: " << judul << endl;
        cout << "Deskripsi: " << deskripsi << endl;
        cout << "Tenggat: " << tenggat.toString() << endl;
        cout << "Prioritas: " << prioritas << endl;
        cout << "Status: " << (selesai ? "Selesai" : "Belum Selesai") << endl;
        cout << "Kategori: " << (kategori ? kategori->getFullPath() : "Tidak Ada") << endl;
    }
};

// Sekarang kita dapat mengimplementasikan Kategori::tampilkan karena Tugas sudah didefinisikan sepenuhnya
void Kategori::tampilkan(int kedalaman) {
    cout << string(kedalaman * 2, ' ') << "- " << nama << endl;

    for (auto tugas : daftarTugas) {
        cout << string((kedalaman + 1) * 2, ' ') << "• Tugas: " << tugas->judul << endl;
    }

    for (auto subkategori : this->subkategori) {
        subkategori->tampilkan(kedalaman + 1);
    }
}

// Struktur graf untuk merepresentasikan ketergantungan tugas
class GrafTugas {
private:
    map<int, vector<int>> daftarKetergantungan; // ID Tugas ke daftar ID tugas yang bergantung

public:
    void tambahKetergantungan(int idTugas, int idTugasBergantung) {
        daftarKetergantungan[idTugas].push_back(idTugasBergantung);
    }

    void hapusKetergantungan(int idTugas, int idTugasBergantung) {
        auto& ketergantungan = daftarKetergantungan[idTugas];
        ketergantungan.erase(
            remove(ketergantungan.begin(), ketergantungan.end(), idTugasBergantung),
            ketergantungan.end()
        );
    }

    bool adaKetergantungan(int idTugas, int idTugasBergantung) {
        if (daftarKetergantungan.find(idTugas) == daftarKetergantungan.end()) {
            return false;
        }

        auto& ketergantungan = daftarKetergantungan[idTugas];
        return find(ketergantungan.begin(), ketergantungan.end(), idTugasBergantung) != ketergantungan.end();
    }

    vector<int> getKetergantungan(int idTugas) {
        if (daftarKetergantungan.find(idTugas) == daftarKetergantungan.end()) {
            return {};
        }
        return daftarKetergantungan[idTugas];
    }
};

// Node Linked List untuk tugas
struct NodeTugas {
    Tugas* tugas;
    NodeTugas* berikutnya;

    NodeTugas(Tugas* t) : tugas(t), berikutnya(nullptr) {}
};

// Implementasi Linked List
class DaftarTugas {
private:
    NodeTugas* kepala;
    NodeTugas* ekor;
    int ukuran;

public:
    DaftarTugas() : kepala(nullptr), ekor(nullptr), ukuran(0) {}

    void tambahTugas(Tugas* tugas) {
        NodeTugas* nodeBaru = new NodeTugas(tugas);
        if (!kepala) {
            kepala = ekor = nodeBaru;
        } else {
            ekor->berikutnya = nodeBaru;
            ekor = nodeBaru;
        }
        ukuran++;
    }

    bool hapusTugas(int idTugas) {
        if (!kepala) return false;

        if (kepala->tugas->id == idTugas) {
            NodeTugas* temp = kepala;
            kepala = kepala->berikutnya;
            if (!kepala) ekor = nullptr;
            delete temp;
            ukuran--;
            return true;
        }

        NodeTugas* saat_ini = kepala;
        while (saat_ini->berikutnya && saat_ini->berikutnya->tugas->id != idTugas) {
            saat_ini = saat_ini->berikutnya;
        }

        if (saat_ini->berikutnya) {
            NodeTugas* temp = saat_ini->berikutnya;
            saat_ini->berikutnya = temp->berikutnya;
            if (temp == ekor) {
                ekor = saat_ini;
            }
            delete temp;
            ukuran--;
            return true;
        }

        return false;
    }

    Tugas* cariTugas(int idTugas) {
        NodeTugas* saat_ini = kepala;
        while (saat_ini) {
            if (saat_ini->tugas->id == idTugas) {
                return saat_ini->tugas;
            }
            saat_ini = saat_ini->berikutnya;
        }
        return nullptr;
    }

    void tampilkanSemua() {
        if (!kepala) {
            cout << "Tidak ada tugas tersedia." << endl;
            return;
        }

        NodeTugas* saat_ini = kepala;
        while (saat_ini) {
            saat_ini->tugas->tampilkan();
            cout << "------------------------" << endl;
            saat_ini = saat_ini->berikutnya;
        }
    }

    vector<Tugas*> ambilSemuaTugas() {
        vector<Tugas*> daftarTugas;
        NodeTugas* saat_ini = kepala;
        while (saat_ini) {
            daftarTugas.push_back(saat_ini->tugas);
            saat_ini = saat_ini->berikutnya;
        }
        return daftarTugas;
    }

    int getUkuran() const {
        return ukuran;
    }

    ~DaftarTugas() {
        NodeTugas* saat_ini = kepala;
        while (saat_ini) {
            NodeTugas* berikutnya = saat_ini->berikutnya;
            delete saat_ini;
            saat_ini = berikutnya;
        }
    }
};

// Komparator kustom untuk antrian prioritas
struct KomparatorPrioritasTugas {
    bool operator()(const Tugas* t1, const Tugas* t2) {
        // Angka prioritas lebih rendah berarti prioritas lebih tinggi
        if (t1->prioritas != t2->prioritas) {
            return t1->prioritas > t2->prioritas;
        }
        // Jika prioritas sama, urutkan berdasarkan tenggat waktu
        return t1->tenggat > t2->tenggat;
    }
};

// Kelas Penjadwal Tugas yang menggunakan semua struktur data
class PenjadwalTugas {
private:
    DaftarTugas daftarTugas;
    stack<Tugas*> tugasTerbaruDitambahkan;
    stack<Tugas*> tugasTerbaruDiselesaikan;
    priority_queue<Tugas*, vector<Tugas*>, KomparatorPrioritasTugas> antrianPrioritas;
    Kategori* kategoriUtama;
    GrafTugas grafKetergantungan;
    int idTugasBerikutnya;

    // Array pointer tugas untuk akses cepat
    Tugas** arrayTugas;
    int kapasitasArrayTugas;

public:
    PenjadwalTugas() : kategoriUtama(new Kategori("Utama")), idTugasBerikutnya(1), kapasitasArrayTugas(100) {
        arrayTugas = new Tugas*[kapasitasArrayTugas];
        for (int i = 0; i < kapasitasArrayTugas; i++) {
            arrayTugas[i] = nullptr;
        }
    }

    Kategori* getKategoriUtama() {
        return kategoriUtama;
    }

    Kategori* tambahKategori(string nama, Kategori* induk = nullptr) {
        if (!induk) induk = kategoriUtama;
        Kategori* kategoriBaruPtr = new Kategori(nama, induk);
        induk->tambahSubkategori(kategoriBaruPtr);
        return kategoriBaruPtr;
    }

    // Modify the cariKategoriDenganPath function to make it case-insensitive

    // Helper function to convert string to lowercase
    string toLower(string str) {
        string result = str;
        transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c){ return tolower(c); });
        return result;
    }

    // Fungsi untuk mencari kategori berdasarkan path
    Kategori* cariKategoriDenganPath(string path) {
        if (path.empty()) return kategoriUtama;

        // Parse path
        vector<string> pathSegments;
        stringstream ss(path);
        string segment;

        while (getline(ss, segment, '/')) {
            if (!segment.empty()) {
                pathSegments.push_back(segment);
            }
        }

        if (pathSegments.empty()) return kategoriUtama;

        // Mulai pencarian dari kategori utama
        Kategori* current = kategoriUtama;

        // Jika segment pertama adalah "Utama", lewati
        size_t startIdx = (toLower(pathSegments[0]) == "utama") ? 1 : 0;

        // Telusuri path
        for (size_t i = startIdx; i < pathSegments.size(); i++) {
            bool found = false;
            for (auto& subkat : current->subkategori) {
                // Case-insensitive comparison
                if (toLower(subkat->nama) == toLower(pathSegments[i])) {
                    current = subkat;
                    found = true;
                    break;
                }
            }

            if (!found) {
                // Kategori tidak ditemukan
                return nullptr;
            }
        }

        return current;
    }

    Tugas* tambahTugas(string judul, string deskripsi, Tanggal tenggat, int prioritas, Kategori* kategori = nullptr) {
        if (!kategori) kategori = kategoriUtama;

        Tugas* tugasBaru = new Tugas(idTugasBerikutnya++, judul, deskripsi, tenggat, prioritas, kategori);

        // Tambahkan ke linked list
        daftarTugas.tambahTugas(tugasBaru);

        // Tambahkan ke antrian prioritas
        antrianPrioritas.push(tugasBaru);

        // Tambahkan ke stack tugas terbaru
        tugasTerbaruDitambahkan.push(tugasBaru);

        // Tambahkan ke kategori
        kategori->tambahTugas(tugasBaru);

        // Tambahkan ke array untuk akses cepat
        if (tugasBaru->id < kapasitasArrayTugas) {
            arrayTugas[tugasBaru->id] = tugasBaru;
        } else {
            // Ubah ukuran array jika diperlukan
            int kapasitasBaru = kapasitasArrayTugas * 2;
            Tugas** arrayBaru = new Tugas*[kapasitasBaru];
            for (int i = 0; i < kapasitasArrayTugas; i++) {
                arrayBaru[i] = arrayTugas[i];
            }
            for (int i = kapasitasArrayTugas; i < kapasitasBaru; i++) {
                arrayBaru[i] = nullptr;
            }
            delete[] arrayTugas;
            arrayTugas = arrayBaru;
            kapasitasArrayTugas = kapasitasBaru;
            arrayTugas[tugasBaru->id] = tugasBaru;
        }

        return tugasBaru;
    }

    bool hapusTugas(int idTugas) {
        Tugas* tugas = getTugasById(idTugas);
        if (!tugas) return false;

        // Hapus dari kategori
        if (tugas->kategori) {
            tugas->kategori->hapusTugas(tugas);
        }

        // Hapus dari linked list
        bool dihapus = daftarTugas.hapusTugas(idTugas);

        // FIX: Hapus dari antrian prioritas dengan membuat antrian baru tanpa tugas yang dihapus
        priority_queue<Tugas*, vector<Tugas*>, KomparatorPrioritasTugas> antrianBaru;
        priority_queue<Tugas*, vector<Tugas*>, KomparatorPrioritasTugas> antrianSementara = antrianPrioritas;

        while (!antrianSementara.empty()) {
            Tugas* t = antrianSementara.top();
            antrianSementara.pop();
            if (t->id != idTugas) {
                antrianBaru.push(t);
            }
        }
        antrianPrioritas = antrianBaru;

        // FIX: Hapus dari stack tugas terbaru ditambahkan
        stack<Tugas*> stackBaru;
        stack<Tugas*> stackSementara = tugasTerbaruDitambahkan;

        while (!stackSementara.empty()) {
            Tugas* t = stackSementara.top();
            stackSementara.pop();
            if (t->id != idTugas) {
                stackBaru.push(t);
            }
        }

        // Kembalikan ke urutan asli
        tugasTerbaruDitambahkan = stack<Tugas*>();
        while (!stackBaru.empty()) {
            Tugas* t = stackBaru.top();
            stackBaru.pop();
            tugasTerbaruDitambahkan.push(t);
        }

        // FIX: Hapus dari stack tugas terbaru diselesaikan jika ada
        stackBaru = stack<Tugas*>();
        stackSementara = tugasTerbaruDiselesaikan;

        while (!stackSementara.empty()) {
            Tugas* t = stackSementara.top();
            stackSementara.pop();
            if (t->id != idTugas) {
                stackBaru.push(t);
            }
        }

        // Kembalikan ke urutan asli
        tugasTerbaruDiselesaikan = stack<Tugas*>();
        while (!stackBaru.empty()) {
            Tugas* t = stackBaru.top();
            stackBaru.pop();
            tugasTerbaruDiselesaikan.push(t);
        }

        // Hapus dari array
        if (idTugas < kapasitasArrayTugas) {
            arrayTugas[idTugas] = nullptr;
        }

        return dihapus;
    }

    void tambahKetergantungan(int idTugas, int idTugasBergantung) {
        grafKetergantungan.tambahKetergantungan(idTugas, idTugasBergantung);
    }

    void hapusKetergantungan(int idTugas, int idTugasBergantung) {
        grafKetergantungan.hapusKetergantungan(idTugas, idTugasBergantung);
    }

    vector<int> getKetergantunganTugas(int idTugas) {
        return grafKetergantungan.getKetergantungan(idTugas);
    }

    bool tandaiTugasSelesai(int idTugas) {
        Tugas* tugas = getTugasById(idTugas);
        if (!tugas) return false;

        // Periksa apakah semua ketergantungan telah selesai
        vector<int> ketergantungan = grafKetergantungan.getKetergantungan(idTugas);
        for (int depId : ketergantungan) {
            Tugas* tugasBergantung = getTugasById(depId);
            if (tugasBergantung && !tugasBergantung->selesai) {
                cout << "Tidak dapat menyelesaikan tugas ini sampai ketergantungannya '"
                     << tugasBergantung->judul << "' selesai." << endl;
                return false;
            }
        }

        tugas->selesai = true;
        tugasTerbaruDiselesaikan.push(tugas);
        return true;
    }

    Tugas* getTugasById(int idTugas) {
        if (idTugas < kapasitasArrayTugas && arrayTugas[idTugas] != nullptr) {
            return arrayTugas[idTugas];
        }
        return daftarTugas.cariTugas(idTugas);
    }

    void tampilkanSemuaTugas() {
        cout << "Semua Tugas:" << endl;
        cout << "------------------------" << endl;
        daftarTugas.tampilkanSemua();
    }

    void tampilkanTugasBerdasarkanPrioritas() {
        cout << "Tugas berdasarkan Prioritas:" << endl;
        cout << "------------------------" << endl;

        // Buat salinan antrian prioritas
        priority_queue<Tugas*, vector<Tugas*>, KomparatorPrioritasTugas> antrianSementara = antrianPrioritas;

        while (!antrianSementara.empty()) {
            Tugas* tugas = antrianSementara.top();
            antrianSementara.pop();

            // FIX: Lewati tugas yang sudah selesai atau yang sudah dihapus
            if (tugas->selesai || getTugasById(tugas->id) == nullptr) continue;

            tugas->tampilkan();
            cout << "------------------------" << endl;
        }
    }

    void tampilkanTugasBerdasarkanTenggat() {
        cout << "Tugas berdasarkan Tenggat:" << endl;
        cout << "------------------------" << endl;

        vector<Tugas*> daftarTugas = this->daftarTugas.ambilSemuaTugas();

        // Urutkan tugas berdasarkan tenggat
        sort(daftarTugas.begin(), daftarTugas.end(), [](Tugas* a, Tugas* b) {
            return a->tenggat < b->tenggat;
        });

        for (Tugas* tugas : daftarTugas) {
            if (!tugas->selesai) {
                tugas->tampilkan();
                cout << "------------------------" << endl;
            }
        }
    }

    void tampilkanHierarkiKategori() {
        cout << "Hierarki Kategori:" << endl;
        kategoriUtama->tampilkan();
    }

    void tampilkanTugasTerbaruDitambahkan(int jumlah = 5) {
        cout << "Tugas Terbaru Ditambahkan:" << endl;
        cout << "------------------------" << endl;

        stack<Tugas*> stackSementara = tugasTerbaruDitambahkan;
        int ditampilkan = 0;

        while (!stackSementara.empty() && ditampilkan < jumlah) {
            Tugas* tugas = stackSementara.top();
            stackSementara.pop();

            // FIX: Verifikasi bahwa tugas masih ada dalam sistem
            if (getTugasById(tugas->id) != nullptr) {
                tugas->tampilkan();
                cout << "------------------------" << endl;
                ditampilkan++;
            }
        }
    }

    void tampilkanTugasTerbaruDiselesaikan(int jumlah = 5) {
        cout << "Tugas Terbaru Diselesaikan:" << endl;
        cout << "------------------------" << endl;

        stack<Tugas*> stackSementara = tugasTerbaruDiselesaikan;
        int ditampilkan = 0;

        while (!stackSementara.empty() && ditampilkan < jumlah) {
            Tugas* tugas = stackSementara.top();
            stackSementara.pop();

            // FIX: Verifikasi bahwa tugas masih ada dalam sistem
            if (getTugasById(tugas->id) != nullptr) {
                tugas->tampilkan();
                cout << "------------------------" << endl;
                ditampilkan++;
            }
        }
    }

    ~PenjadwalTugas() {
        delete kategoriUtama;
        delete[] arrayTugas;
    }
};

// Fungsi pembantu untuk mendapatkan input tanggal dengan validasi
Tanggal getInputTanggal() {
    Tanggal tanggal;

    // Validasi input hari
    cout << "Masukkan hari: ";
    while (!(cin >> tanggal.hari)) {
        cout << "Error: Masukkan harus berupa angka!" << endl;
        cin.clear(); // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Masukkan hari: ";
    }

    // Validasi input bulan
    cout << "Masukkan bulan: ";
    while (!(cin >> tanggal.bulan)) {
        cout << "Error: Masukkan harus berupa angka!" << endl;
        cin.clear(); // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Masukkan bulan: ";
    }

    // Validasi input tahun
    cout << "Masukkan tahun: ";
    while (!(cin >> tanggal.tahun)) {
        cout << "Error: Masukkan harus berupa angka!" << endl;
        cin.clear(); // Clear error flag
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
        cout << "Masukkan tahun: ";
    }

    return tanggal;
}

// Fungsi untuk menampilkan pesan dan menunggu pengguna menekan Enter
void pauseScreen() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Fungsi utama dengan antarmuka berbasis menu
int main() {
    PenjadwalTugas penjadwal;

    // Buat beberapa kategori default
    Kategori* kategoriAkademik = penjadwal.tambahKategori("Akademik");
    Kategori* kategoriPribadi = penjadwal.tambahKategori("Pribadi");

    penjadwal.tambahKategori("Tugas", kategoriAkademik);
    penjadwal.tambahKategori("Ujian", kategoriAkademik);
    penjadwal.tambahKategori("Proyek", kategoriAkademik);

    penjadwal.tambahKategori("Kesehatan", kategoriPribadi);
    penjadwal.tambahKategori("Hobi", kategoriPribadi);

    int pilihan;

    do {
        // Clear screen at the beginning of each menu display
        clearScreen();

        cout << "\n===== SISTEM MANAJEMEN TUGAS MAHASISWA =====\n";
        cout << "1. Tambah tugas baru\n";
        cout << "2. Tandai tugas sebagai selesai\n";
        cout << "3. Hapus tugas\n";
        cout << "4. Tambah ketergantungan tugas\n";
        cout << "5. Tampilkan semua tugas\n";
        cout << "6. Tampilkan tugas berdasarkan prioritas\n";
        cout << "7. Tampilkan tugas berdasarkan tenggat\n";
        cout << "8. Tampilkan hierarki kategori\n";
        cout << "9. Tampilkan tugas terbaru ditambahkan\n";
        cout << "10. Tampilkan tugas terbaru diselesaikan\n";
        cout << "0. Keluar\n";
        cout << "Masukkan pilihan Anda: ";

        // Validasi input pilihan menu
        while (!(cin >> pilihan)) {
            cout << "Error: Masukkan harus berupa angka!" << endl;
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Masukkan pilihan Anda: ";
        }

        // Clear screen after selection before showing the result
        clearScreen();

        switch (pilihan) {
            case 1: {
                string judul, deskripsi;
                int prioritas;

                cout << "\n===== TAMBAH TUGAS BARU =====\n";

                cin.ignore();
                cout << "Masukkan judul tugas: ";
                getline(cin, judul);

                cout << "Masukkan deskripsi tugas: ";
                getline(cin, deskripsi);

                cout << "Masukkan tenggat waktu:\n";
                Tanggal tenggat = getInputTanggal();

                cout << "Masukkan prioritas (1-5, dimana 1 adalah tertinggi): ";
                // Validasi input prioritas
                while (!(cin >> prioritas) || prioritas < 1 || prioritas > 5) {
                    if (cin.fail()) {
                        cout << "Error: Masukkan harus berupa angka!" << endl;
                        cin.clear(); // Clear error flag
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    } else {
                        cout << "Error: Prioritas harus antara 1-5!" << endl;
                    }
                    cout << "Masukkan prioritas (1-5, dimana 1 adalah tertinggi): ";
                }

                // Tampilkan kategori untuk dipilih
                cout << "Pilih kategori:\n";
                penjadwal.tampilkanHierarkiKategori();

                cout << "bila tidak memilih kategori/typo maka Utama akan menjadi default"<< endl;
                cout<< "Masukkan path kategori (contoh: Utama, Akademik/Proyek, Pribadi/Hobi): ";
                string pathKategori;
                cin.ignore();
                getline(cin, pathKategori);

                Kategori* kategoriTerpilih = nullptr;
                if (pathKategori != "tidak ada") {
                    // Cari kategori berdasarkan path
                    kategoriTerpilih = penjadwal.cariKategoriDenganPath(pathKategori);

                    // Jika kategori tidak ditemukan, gunakan kategori utama
                    if (!kategoriTerpilih) {
                        cout << "Kategori dengan path '" << pathKategori << "' tidak ditemukan. Menggunakan kategori Utama." << endl;
                        kategoriTerpilih = penjadwal.getKategoriUtama();
                    }
                } else {
                    kategoriTerpilih = penjadwal.getKategoriUtama();
                }

                Tugas* tugasBaru = penjadwal.tambahTugas(judul, deskripsi, tenggat, prioritas, kategoriTerpilih);
                cout << "Tugas berhasil ditambahkan dengan ID: " << tugasBaru->id << endl;
                cout << "Kategori: " << tugasBaru->kategori->getFullPath() << endl;

                pauseScreen();
                break;
            }
            case 2: {
                int idTugas;
                cout << "\n===== TANDAI TUGAS SELESAI =====\n";

                cout << "Masukkan ID tugas yang akan ditandai sebagai selesai: ";
                // Validasi input ID tugas
                while (!(cin >> idTugas)) {
                    cout << "Error: Masukkan harus berupa angka!" << endl;
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Masukkan ID tugas yang akan ditandai sebagai selesai: ";
                }

                if (penjadwal.tandaiTugasSelesai(idTugas)) {
                    cout << "Tugas berhasil ditandai sebagai selesai." << endl;
                } else {
                    cout << "Gagal menandai tugas sebagai selesai." << endl;
                }

                pauseScreen();
                break;
            }
            case 3: {
                int idTugas;
                cout << "\n===== HAPUS TUGAS =====\n";

                cout << "Masukkan ID tugas yang akan dihapus: ";
                // Validasi input ID tugas
                while (!(cin >> idTugas)) {
                    cout << "Error: Masukkan harus berupa angka!" << endl;
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Masukkan ID tugas yang akan dihapus: ";
                }

                if (penjadwal.hapusTugas(idTugas)) {
                    cout << "Tugas berhasil dihapus." << endl;
                } else {
                    cout << "Gagal menghapus tugas." << endl;
                }

                pauseScreen();
                break;
            }
            case 4: {
                int idTugas, idTugasBergantung;
                cout << "\n===== TAMBAH KETERGANTUNGAN TUGAS =====\n";

                cout << "Masukkan ID tugas: ";
                // Validasi input ID tugas
                while (!(cin >> idTugas)) {
                    cout << "Error: Masukkan harus berupa angka!" << endl;
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Masukkan ID tugas: ";
                }

                cout << "Masukkan ID tugas yang menjadi ketergantungan: ";
                // Validasi input ID tugas bergantung
                while (!(cin >> idTugasBergantung)) {
                    cout << "Error: Masukkan harus berupa angka!" << endl;
                    cin.clear(); // Clear error flag
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
                    cout << "Masukkan ID tugas yang menjadi ketergantungan: ";
                }

                penjadwal.tambahKetergantungan(idTugas, idTugasBergantung);
                cout << "Ketergantungan berhasil ditambahkan." << endl;

                pauseScreen();
                break;
            }
            case 5:
                cout << "\n===== SEMUA TUGAS =====\n";
                penjadwal.tampilkanSemuaTugas();
                pauseScreen();
                break;
            case 6:
                cout << "\n===== TUGAS BERDASARKAN PRIORITAS =====\n";
                penjadwal.tampilkanTugasBerdasarkanPrioritas();
                pauseScreen();
                break;
            case 7:
                cout << "\n===== TUGAS BERDASARKAN TENGGAT =====\n";
                penjadwal.tampilkanTugasBerdasarkanTenggat();
                pauseScreen();
                break;
            case 8:
                cout << "\n===== HIERARKI KATEGORI =====\n";
                penjadwal.tampilkanHierarkiKategori();
                pauseScreen();
                break;
            case 9:
                cout << "\n===== TUGAS TERBARU DITAMBAHKAN =====\n";
                penjadwal.tampilkanTugasTerbaruDitambahkan();
                pauseScreen();
                break;
            case 10:
                cout << "\n===== TUGAS TERBARU DISELESAIKAN =====\n";
                penjadwal.tampilkanTugasTerbaruDiselesaikan();
                pauseScreen();
                break;
            case 0:
                cout << "Keluar dari program. Sampai jumpa!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                pauseScreen();
        }

    } while (pilihan != 0);

    return 0;
}
