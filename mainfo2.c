#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 1. Syslog verilerini tutacak Çift Yönlü Baðlý Liste Düðüm (Node) Yapýsý
typedef struct SyslogNode {
    char timestamp[64]; // Tarih ve Saat bilgisi (Örn: Mar 12 10:00:01)
    char hostname[64];  // Makine/Sunucu adý
    char process[64];   // Logu üreten süreç/uygulama
    char message[512];  // Logun asýl mesaj içeriði
    
    struct SyslogNode* prev; // Bir önceki log kaydýný gösteren iþaretçi
    struct SyslogNode* next; // Bir sonraki log kaydýný gösteren iþaretçi
} SyslogNode;

// Global baþ (head) ve kuyruk (tail) iþaretçileri
SyslogNode* head = NULL;
SyslogNode* tail = NULL;

// 2. Baðlý Listeye Yeni Log Ekleyen Fonksiyon (Sona Ekleme - Append)
void addLog(const char* timestamp, const char* hostname, const char* process, const char* message) {
    // Yeni düðüm için bellekten yer ayýrma
    SyslogNode* newNode = (SyslogNode*)malloc(sizeof(SyslogNode));
    if (newNode == NULL) {
        printf("Bellek tahsis hatasi!\n");
        return;
    }

    // Verileri düðüme kopyalama
    strncpy(newNode->timestamp, timestamp, sizeof(newNode->timestamp) - 1);
    strncpy(newNode->hostname, hostname, sizeof(newNode->hostname) - 1);
    strncpy(newNode->process, process, sizeof(newNode->process) - 1);
    strncpy(newNode->message, message, sizeof(newNode->message) - 1);

    newNode->next = NULL;
    newNode->prev = tail; // Yeni düðümün öncesi, eski kuyruk olacak

    if (tail != NULL) {
        tail->next = newNode; // Eski kuyruðun sonrasý, yeni düðüm olacak
    } else {
        head = newNode; // Eðer liste boþsa, baþ da bu düðümdür
    }
    tail = newNode; // Yeni kuyruk bu düðüm oldu
}

// 3. Dosyadan Loglarý Okuma ve Ayrýþtýrma (Parsing) Fonksiyonu
void parseLogFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Hata: %s dosyasi acilamadi! Lutfen dosya yolunu kontrol edin.\n", filename);
        return;
    }

    char line[1024];
    // Satýr satýr okuma iþlemi
    while (fgets(line, sizeof(line), file)) {
        char month[10], day[10], time[20], hostname[64], process[64], message[512];
        char timestamp[64];

        // Standart Syslog formatýný ayrýþtýrma (Örn: "Mar 12 12:43:34 mypc kernel: [ 0.000] mesaj...")
        // %[^:]: regex benzeri bir okuma yapar, ':' karakterine kadar olan kýsmý process olarak alýr.
        int parsed = sscanf(line, "%s %s %s %s %[^:]: %[^\n]", month, day, time, hostname, process, message);

        if (parsed >= 6) {
            // Ay, gün ve saat bilgisini tek bir timestamp string'inde birleþtirme
            snprintf(timestamp, sizeof(timestamp), "%s %s %s", month, day, time);
            
            // Ayrýþtýrýlan veriyi baðlý listeye ekle
            addLog(timestamp, hostname, process, message);
        }
    }
    fclose(file);
}

// 4. Baðlý Listeyi Baþtan Sona (Kronolojik) Yazdýrma
void printForward() {
    SyslogNode* current = head;
    printf("\n--- LOG KAYITLARI (BASTAN SONA KRONOLOJIK) ---\n");
    while (current != NULL) {
        printf("[%s] %s | %s: %s\n", current->timestamp, current->hostname, current->process, current->message);
        current = current->next;
    }
}

// 5. Baðlý Listeyi Sondan Baþa (Ters Kronolojik) Yazdýrma - Çift Yönlü Listenin Avantajý
void printBackward() {
    SyslogNode* current = tail;
    printf("\n--- LOG KAYITLARI (SONDAN BASA GUNCELDEN ESKIYE) ---\n");
    while (current != NULL) {
        printf("[%s] %s | %s: %s\n", current->timestamp, current->hostname, current->process, current->message);
        current = current->prev; // Geriye doðru git
    }
}

// 6. Belleði Temizleme (Memory Leak önlemek için)
void freeList() {
    SyslogNode* current = head;
    while (current != NULL) {
        SyslogNode* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    tail = NULL;
}

int main() {
    // Linux'ta gercek log dosyasi genellikle /var/log/syslog'dur. 
    // Ancak erisim izni (sudo) gerekebilecegi icin projenizle ayni dizinde 
    // "syslog.txt" adinda ornek bir text dosyasi olusturup test edebilirsiniz.
    
    const char* log_file_path = "syslog.txt"; 
    
    printf("Syslog verileri isleniyor...\n");
    parseLogFile(log_file_path);

    // Eger liste bos degilse verileri goster
    if (head != NULL) {
        printForward();
        printBackward(); // Cift yonlu bagli listenin neden secildigini kanitlayan kisim
    } else {
        printf("Gosterilecek log kaydi bulunamadi.\n");
    }

    // Program kapanmadan once bellegi temizle
    freeList();

    return 0;
}
