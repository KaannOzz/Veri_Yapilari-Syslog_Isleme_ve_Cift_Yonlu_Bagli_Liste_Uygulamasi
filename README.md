# Linux Syslog Kayıtlarının Çift Yönlü Bağlı Liste ile Modellenmesi

## 📖 Proje Hakkında

Bu proje, Linux işletim sistemindeki sistem günlüklerinin (syslog) okunarak, çift yönlü bağlı liste (doubly linked list) veri yapısı üzerinden bellekte dinamik olarak tutulmasını ve kronolojik/ters kronolojik olarak işlenmesini sağlayan bir C uygulamasıdır. Veri yapıları dersi ödevi kapsamında geliştirilmiş olup, sistem günlüklerinin bellekte nasıl verimli bir şekilde tutulabileceğini ve yönetilebileceğini uygulamalı olarak göstermeyi amaçlamaktadır.

## 🎯 Temel Kavramlar ve Proje Sorularının Cevapları

Bu çalışma, aşağıdaki teknik konulara çözüm ve açıklama getirmektedir:

**Syslog Mekanizması Nedir ve Ne İçin Kullanılır?** Linux tabanlı sistemlerde çekirdek (kernel), servisler ve uygulamalar tarafından üretilen sistem mesajlarının, hata bildirimlerinin ve olayların standart bir formatta kaydedilmesini sağlayan protokoldür. Sistem yöneticilerinin sorun giderme (debugging), sistem sağlığını izleme ve güvenlik denetimi yapmasını sağlar.

**Bağlı Liste Veri Yapısı ile Bu Günlükler Nasıl Tutulabilir?** Log dosyasından okunan her bir satır ayrıştırılır (parsing) ve bağlı listenin bir **düğümü (node)** olarak belleğe alınır. Her düğüm; olayın zaman damgası (timestamp), makine adı (hostname), işlemi yapan uygulama/süreç (process) ve mesajın asıl içeriğini barındırır.

**Hangi Bağlı Liste Türü İle Tutulabilir?** Bu projede syslog kayıtlarını tutmak ve yönetmek için **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısı kullanılmıştır.

**Neden Çift Yönlü Bağlı Liste Tercih Edildi? Syslog Kayıtlarıyla İlgisi Nedir?** Syslog kayıtları kronolojik olarak (eskiden yeniye) dosyaya yazılır. Ancak sistem yöneticileri bir hata ararken genellikle **en güncel logdan geçmişe doğru (sondan başa)** inceleme yaparlar. Çift yönlü bağlı liste, `prev` ve `next` işaretçileri sayesinde loglar arasında hem baştan sona (kronolojik) hem de sondan başa (ters kronolojik) performans kaybı yaşamadan ($O(1)$ karmaşıklığı ile) gezinmeyi sağlar. Ayrıca bağlı listelerin dinamik bellek yapısı, sürekli artan log satırları için dizilere (array) kıyasla çok daha esnek ve verimlidir.

## 🚀 Özellikler
- Standart Linux syslog formatını okuma ve veri yapılarına ayrıştırma (Parsing).
- Verileri bellekte dinamik olarak (Çift Yönlü Bağlı Liste ile) tutma.
- Log kayıtlarını kronolojik olarak (baştan sona) listeleme.
- Log kayıtlarını güncelden eskiye doğru (sondan başa) listeleme yeteneği.
- Olası bellek sızıntılarını (memory leak) önlemek için program sonunda RAM temizliği.
  
## 🛠️ Kurulum ve Kullanım (Kali Linux / Ubuntu)
Projeyi çalıştırmak için sisteminizde `gcc` derleyicisinin kurulu olması gerekmektedir. (Kali Linux üzerinde varsayılan olarak gelir.)
**1. Proje dosyalarını indirin veya oluşturun:** Proje dizininde `syslog_list.c` dosyasını oluşturup kaynak kodunu içine dahil edin.
**2. Örnek bir syslog dosyası oluşturun:** Projenin çalışabilmesi için aynı dizinde `syslog.txt` adında bir dosya oluşturun ve içine örnek Linux log satırları ekleyin (`nano syslog.txt`).
**3. Kodu derleyin:** Terminal ekranında `gcc syslog_list.c -o syslog_list` komutunu çalıştırarak C kodunu derleyin.
**4. Programı çalıştırın:** Oluşan çalıştırılabilir dosyayı başlatarak çıktıyı gözlemleyin (`./syslog_list`).

## 📂 Dosya Yapısı
`syslog_list.c`: Çift yönlü bağlı liste ve ana program mantığını içeren C kodu.
`syslog.txt`: Programın okuyacağı örnek log dosyası (Önceden oluşturulmalı).
`README.md`: Proje açıklaması ve kullanım rehberi.
