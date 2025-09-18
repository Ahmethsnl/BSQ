# BSQ (Biggest Square)

## Genel Bakış
BSQ (Biggest Square), bir matris üzerinde en büyük kareyi bulmayı amaçlayan bir algoritmadır. Bu algoritma genellikle, bir harita üzerinde belirli bir alanı kaplayan en büyük kareyi bulmak için kullanılır. Harita, boş ve dolu hücrelerden oluşur ve algoritma, boş hücrelerden oluşan en büyük kareyi tespit eder.

## Algoritma
BSQ algoritması genellikle şu adımları içerir:

1. **Giriş Verisi**:
   - Harita, boş (`.`) ve dolu (`x`) hücrelerden oluşan bir matris olarak temsil edilir.
   - Haritanın boyutları ve içeriği giriş olarak alınır.

2. **Dinamik Programlama Tablosu**:
   - Bir dinamik programlama tablosu (DP tablosu) oluşturulur.
   - Her hücre, o hücreyi içeren en büyük karenin kenar uzunluğunu temsil eder.

3. **Tablonun Güncellenmesi**:
   - Eğer bir hücre boşsa (`.`), o hücre için DP tablosu değeri, sol, üst ve sol-üst hücrelerin minimum değerinin bir fazlası olarak hesaplanır.
   - Eğer hücre doluysa (`x`), DP tablosu değeri sıfır olur.

4. **Sonuç**:
   - DP tablosundaki en büyük değer, en büyük karenin kenar uzunluğunu verir.
   - Bu değere karşılık gelen hücre, karenin sağ-alt köşesini temsil eder.

## Uygulama
Bu depo, BSQ algoritmasının C dilinde bir uygulamasını içermektedir. Kod, `bsq_ex/BSQ/` dizinindeki `bsq_208.c`, `bsq_168.c` ve `bsq_70.c` dosyalarında bulunmaktadır. Her dosya, algoritmanın farklı varyasyonlarını veya optimizasyonlarını içerebilir.

## Nasıl Çalıştırılır
1. `bsq_ex/BSQ/` dizinine gidin.
2. İlgili C dosyasını bir C derleyicisi kullanarak derleyin, örneğin:
   ```bash
   gcc -o bsq bsq_208.c
   ```
3. Derlenmiş programı çalıştırın:
   ```bash
   ./bsq
   ```

## Uygulamalar
BSQ algoritması, aşağıdaki alanlarda kullanılabilir:
- **Harita Analizi**: Coğrafi haritalarda belirli bir alanı kaplayan en büyük kareyi bulmak.
- **Görüntü İşleme**: İkili görüntülerde belirli bir deseni tespit etmek.
- **Oyun Geliştirme**: Oyun haritalarında alan analizi yapmak.

## Katkıda Bulunma
Bu projeye algoritmayı geliştirerek, yeni özellikler ekleyerek veya kodu optimize ederek katkıda bulunabilirsiniz. Depoyu forklayın, değişikliklerinizi yapın ve bir pull request gönderin.

## Lisans
Bu proje açık kaynaklıdır ve MIT Lisansı altında sunulmaktadır.
