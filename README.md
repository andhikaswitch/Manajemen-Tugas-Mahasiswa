# Path Kategori di Tambah Tugas Baru
Ketika diminta untuk "Masukkan path kategori", Anda memiliki beberapa opsi:

* **Kategori Default (Utama):**
    * Jika Anda ingin tugas masuk ke kategori **`Utama`** (kategori root), Anda bisa mengetikkan `Utama`.
    * **Penting:** Jika Anda **typo** saat memasukkan nama kategori atau **tidak memasukkan apa pun** (cukup tekan Enter setelah prompt path kategori), sistem akan secara otomatis menetapkan tugas ke kategori **`Utama`** sebagai _default_.

* **Memilih Kategori Level Atas (Akademik atau Pribadi):**
    * Untuk memilih kategori seperti `Akademik`, cukup ketik `Akademik`.
    * Untuk memilih kategori `Pribadi`, cukup ketik `Pribadi`.

* **Memilih Sub-kategori:**
    * Untuk memilih sub-kategori, Anda harus menggunakan **`/` (garis miring)** sebagai pemisah path.
    * Contoh:
        * Untuk menempatkan tugas di kategori `Tugas` yang merupakan sub-kategori dari `Akademik`, Anda harus mengetik: `Akademik/Tugas`
        * Untuk menempatkan tugas di kategori `Kesehatan` yang merupakan sub-kategori dari `Pribadi`, Anda harus mengetik: `Pribadi/Kesehatan`
    * Anda tidak perlu menyertakan `Utama/` di awal path karena `Utama` adalah root implisit untuk semua kategori yang ada. Cukup mulai dari nama kategori level atas seperti `Akademik` atau `Pribadi`.

 # Memahami Fitur Ketergantungan Tugas

Fitur **ketergantungan tugas** memungkinkan Anda untuk mendefinisikan hubungan prasyarat antara tugas-tugas. Ini berarti bahwa **sebuah tugas tidak dapat dimulai atau diselesaikan sebelum tugas lain yang menjadi prasyaratnya selesai.**

**Cara Kerja dan Contoh:**

Misalkan Anda memiliki dua tugas:
* **Tugas ID 1:** "Buat Kerangka Esai"
* **Tugas ID 2:** "Riset Data Pendukung"

Secara logis, Anda mungkin tidak bisa membuat kerangka esai (`Tugas ID 1`) tanpa terlebih dahulu menyelesaikan riset data pendukung (`Tugas ID 2`). Dalam kasus ini, `Tugas ID 1` **bergantung pada** `Tugas ID 2`.

Untuk menetapkan ketergantungan ini, Anda akan diminta untuk memasukkan dua ID tugas:
* **ID Tugas:** ID dari tugas yang **akan memiliki ketergantungan** (yaitu, tugas yang menunggu untuk diselesaikan).
* **ID Tugas yang menjadi Ketergantungan:** ID dari tugas **prasyarat** (yaitu, tugas yang harus diselesaikan terlebih dahulu).
* Jika Anda mencoba menandai **`Tugas ID 1`** sebagai selesai **sebelum `Tugas ID 2` selesai**, sistem akan menolak dan memberikan peringatan bahwa `Tugas ID 2` harus diselesaikan terlebih dahulu.
* Anda harus terlebih dahulu menandai **`Tugas ID 2`** sebagai selesai, atau menghapus `Tugas ID 2`, barulah Anda bisa menandai **`Tugas ID 1`** sebagai selesai.
